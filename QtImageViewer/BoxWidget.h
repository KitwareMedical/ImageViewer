/*=========================================================================

Library:   TubeTK

Copyright Kitware Inc.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __BoxWidget_h
#define __BoxWidget_h

#include <vector>
#include <memory>
#include <functional>
#include "itkImage.h"
#include "itkPoint.h"
#include <QOpenGLWidget>
#include "QtGlSliceView.h"

class QtGlSliceView; // cross-referencing due to the callbacks for the OpenGL painting


enum class BoxToolState {
    standing = 0, /// not drawing anything,
    drawing = 1 /// one side of the box has been set, also covers repositioning a box
};


/**
* BoxToolMetaData stores color, name, and a sorting id for a Box.  It allows for different workflows where defined
* Boxes need to be specified.
*/
struct BoxToolMetaData {
    /**
    * Monotonically increasing ID.  Used to reset the workflow if a box or more is deleted.  I.e., if the ONSD
    * requires a 3mm measurement, then the width, we don't want the user deleting the ONSD and measuring another
    * 3mm.
    */
    int sortId;

    /**
    * Non-unique identifier for this box.
    */
    std::string name;

    /**
    * Color of box on-screen.
    */
    QColor color;
};

/**
* Defined backwards so that this works with priority queue
*/
bool operator< (std::unique_ptr< BoxToolMetaData > const& lhs, std::unique_ptr< BoxToolMetaData > const& rhs);

/**
* Functor for generating new meta data for newly created boxes.  Allows for rudimentary workflow.
*/
class BoxMetaDataGenerator {
public:
    virtual std::unique_ptr< BoxToolMetaData > operator()(void) = 0;
};

/**
* A constant set of colors and increasing integer names.
*/
class ConstantBoxMetaDataGenerator : public BoxMetaDataGenerator {
public:
    ConstantBoxMetaDataGenerator() : ConstantBoxMetaDataGenerator("", 1) {}
    ConstantBoxMetaDataGenerator(std::string name, QColor color) : mName(name), mColor(color) {}
    std::unique_ptr< BoxToolMetaData > operator()(void);
protected:
    std::string mName;
    QColor mColor;
    int curId = 0;
};

/**
* Factory class for meta data for new boxes.  Uses MetaDataGenerators.  Used to "refund" deleted boxes, i.e. if a user deletes a box
* the next box they draw has the deleted meta data.
*
* Suppose a user has drawn ONSD measurements (4 boxes) on two slices.  Now they have deleted 3 of the 4.  We want the next box they place
* to be an ONSD from the still existing R1 measurement.  This isn't fool-proof, but it works.
*/
class BoxToolMetaDataFactory {
public:
    BoxToolMetaDataFactory(std::unique_ptr< BoxMetaDataGenerator > generator);

    /**
    * Get a the appropriate meta data for the next box.  Recycles deleted meta data.
    */
    std::unique_ptr< BoxToolMetaData > getNext();

    /**
    * Return a deleted meta data for reuse.
    */
    void refund(std::unique_ptr< BoxToolMetaData > box_meta);

protected:
    std::unique_ptr< BoxMetaDataGenerator > generator;
    std::vector< std::unique_ptr< BoxToolMetaData > > refunds;
};

/**
* Stores and renders a box widget.  Note, it isn't a Qt widget as Qt doesn't like overlapping widgets.  Therefore,
* this relies on the Qt Painter widgets rendering on the parent QOpenGLWidget.
*/
class BoxTool {
public:

    using PointType3D = itk::Point<double, 3>;
    using PointType2D = itk::Point<double, 2>;

    BoxTool() = delete;

    /**
    * \param parent QtGlSliceView
    * \param index The image index location of first point of the box
    * \param metaData metaData for the box
    */
    BoxTool(QtGlSliceView* parent, PointType3D index, std::unique_ptr< BoxToolMetaData > metaData);
    virtual ~BoxTool();

    /**
    * Whether the given image index is within the bounding box of one of the box ends
    *
    * \param index
    * \return -1 for no, 0 or 1 identifies which end of the box was clicked
    */
    int isOver(double index[]) const;

    /**
    * The floating index is which end of the box (0 or 1) that is moving with the cursor.
    *
    * \param id 0 or 1
    */
    void setFloatingIndex(int id);

    /**
    * The floating index is which end of the box (0 or 1) that is moving with the cursor.
    */
    int getFloatingIndex() const;

    /**
    * Update the current floating index with the image index position.
    *
    * \param index the image floating position
    */
    void updateFloatingIndex(double index[]);

    /**
    * Returns the area of the box.
    * An axis (X=0, Y=1, Z=2) must be specified because
    * box coordinates are in 3D.
    */
    double area(int axis);

    void paint();

    std::unique_ptr< BoxToolMetaData > metaData;

    /**
    * Constructs JSON { name: "blargh", indices : [ [ x1, y1, z1 ], [ x2, y2, z2] ], points : [ [ x1, y1, z1 ], [ x2, y2, z2] ], distance : 0.3 } where distance is length()
    */
    std::string toJson();
protected:
    QtGlSliceView* parent;
    int floatingIndex; // if we are drawing, what index is being moved
    int crossStart; // defines how big the cross is in pixels
    int crossEnd; // defines how big the empty middle of the cross is
    int lineWidth; // how wide the cross and box lines

    PointType3D points[2];
    PointType3D indices[2];

    float clickRadius; // how close to get to the cross points for the mouse handover
    BoxToolState state;
};


/**
* Represents a collection of Boxes drawn on a particular slice of an Image.  Currently set up to have a BoxToolCollection per z-slice.  An alternative would have been
* one per entire image, but this makes the event handling a little easier.
*
* TODO : I don't really handle multiple generators well.  Weird things happen if you would toggle while drawing.
*
*/
class BoxToolCollection {
public:
    /**
    * \param parent the widget we're rendering in
    * \param metaDataFactory factory to generate box color, name, etc
    * \param axis which axis of the image slice corresponds to
    * \param slice integer slice these boxes are draw on
    */
    BoxToolCollection(QtGlSliceView* parent, std::shared_ptr< BoxToolMetaDataFactory > metaDataFactory, unsigned short axis, unsigned int slice);
    virtual ~BoxToolCollection();

    /**
    * Note, QtGlSliceView does all the computation to determine screen coordinate to image index space.  So we do a lot of image index space back
    * to screen coordinates.
    *
    * \param eventType
    * \param index image index of mouse event
    */
    void handleMouseEvent(QMouseEvent* eventType, double index[]);
    void paint();
    BoxTool* getActive();
    std::vector< std::unique_ptr< BoxTool > > boxes;

    /**
    * Constructs JSON { axis : axis_num, slice : slice_num, boxes : [ see boxes ] }
    */
    std::string toJson();

    void setMetaDataFactory(std::shared_ptr< BoxToolMetaDataFactory > factory);

protected:
    QtGlSliceView* parent;

    /**
    * Is the image index coordinate over any of the boxes in this collection.
    *
    * \return -1 for no, otherwise, the index 0 - N of the box that contains index
    */
    int isOver(double index[]);

    int currentId;
    BoxToolState state;
    unsigned short axis;
    unsigned int slice;

    std::shared_ptr< BoxToolMetaDataFactory > metaDataFactory;
};


#endif
