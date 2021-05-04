/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

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
#ifndef __RulerWidget_h
#define __RulerWidget_h

#include <vector>
#include <memory>
#include <functional>
#include "itkImage.h"
#include "itkPoint.h"
#include <QOpenGLWidget>
#include "QtGlSliceView.h"

class QtGlSliceView; // cross-referencing due to the callbacks for the OpenGL painting


enum class RulerToolState { 
    standing = 0, /// not drawing anything, 
    drawing = 1 /// one side of the ruler has been set, also covers repositioning a ruler
};


/**
* RulerToolMetaData stores color, name, and a sorting id for a Ruler.  It allows for different workflows where defined
* Rulers need to be specified.
*/
struct RulerToolMetaData {
    /**
    * Monotonically increasing ID.  Used to reset the workflow if a ruler or more is deleted.  I.e., if the ONSD
    * requires a 3mm measurement, then the width, we don't want the user deleting the ONSD and measuring another
    * 3mm.
    */
    int sortId;

    /**
    * Non-unique identifier for this ruler.
    */
    std::string name;

    /**
    * Color of ruler on-screen.
    */
    QColor color;
};

/**
* Defined backwards so that this works with priority queue
*/
bool operator< (std::unique_ptr< RulerToolMetaData > const& lhs, std::unique_ptr< RulerToolMetaData > const& rhs); 

/**
* Functor for generating new meta data for newly created rulers.  Allows for rudimentary workflow.
*/
class MetaDataGenerator {
public:
    virtual std::unique_ptr< RulerToolMetaData > operator()(void) = 0;
};

/**
* A looping set of 7 colors (ggplot color scheme) and increasing integer names.  Good default for generic workflows.
*/
class RainbowMetaDataGenerator : public MetaDataGenerator {
public:
    RainbowMetaDataGenerator();

    std::unique_ptr< RulerToolMetaData > operator()(void);
protected:
    std::vector< std::string > colors = { "#F8766D", "#BB9D00", "#00B81F", "#00C0B8", "#00A5FF", "#E76BF3", "#FF6C90" };
    std::vector< std::string >::iterator curColor;
    int curId = 0;
};

/**
* Two colors.  First ruler, "R1" should be the 3mm from ocular orb.  Second ruler, "ONSD" is the ONSD (optic nerve sheathe) measurment.  Names
* are not unique, so rely on the positions and slices of the ruler to distinguish.
*/
class ONSDMetaDataGenerator : public MetaDataGenerator {
public:
    ONSDMetaDataGenerator() { };

    std::unique_ptr< RulerToolMetaData > operator()(void);
protected:
    std::vector< std::string > colors = { "#F8766D", "#BB9D00" };
    bool flipper = true;
    int curId = 0;
};


/**
* Factory class for meta data for new rulers.  Uses MetaDataGenerators.  Used to "refund" deleted rulers, i.e. if a user deletes a ruler
* the next ruler they draw has the deleted meta data.
* 
* Suppose a user has drawn ONSD measurements (4 rulers) on two slices.  Now they have deleted 3 of the 4.  We want the next ruler they place
* to be an ONSD from the still existing R1 measurement.  This isn't full-proof, but it works.
*/
class RulerToolMetaDataFactory {
public:
    RulerToolMetaDataFactory(std::unique_ptr< MetaDataGenerator > generator);

    /**
    * Get a the appropriate meta data for the next ruler.  Recycles deleted meta data.
    */
    std::unique_ptr< RulerToolMetaData > getNext();

    /**
    * Return a deleted meta data for reuse.
    */
    void refund(std::unique_ptr< RulerToolMetaData > ruler_meta);

protected:
    std::unique_ptr< MetaDataGenerator > generator;
    std::vector< std::unique_ptr< RulerToolMetaData > > refunds;
};

/**
* Stores and renders a ruler widget.  Note, it isn't a Qt widget as Qt doesn't like overlapping widgets.  Therefore,
* this relies on the Qt Painter widgets rendering on the parent QOpenGLWidget.
*/
class RulerTool {
public:

    using PointType3D = itk::Point<double, 3>;
    using PointType2D = itk::Point<double, 2>;

    RulerTool() = delete;

    /**
    * \param parent QtGlSliceView
    * \param index The image index location of first point of the ruler
    * \param metaData metaData for the ruler
    */
    RulerTool(QtGlSliceView* parent, PointType3D& index, std::unique_ptr< RulerToolMetaData > metaData);
    virtual ~RulerTool();

    /**
    * Whether the given image index is within the bounding box of one of the ruler ends
    *
    * \param index
    * \return -1 for no, 0 or 1 identifies which end of the ruler was clicked
    */
    int isOver(double index[]) const;

    /**
    * The floating index is which end of the ruler (0 or 1) that is moving with the cursor.
    * 
    * \param id 0 or 1
    */
    void setFloatingIndex(int id);

    /**
    * The floating index is which end of the ruler (0 or 1) that is moving with the cursor.
    */
    int getFloatingIndex() const;
    
    /**
    * Update the current floating index with the image index position.
    * 
    * \param index the image floating position
    */
    void updateFloatingIndex(double index[]);

    /**
    * Returns the Euclidean distance between the image physical points.
    */
    double length();

    void paint();

    std::unique_ptr< RulerToolMetaData > metaData;

    /**
    * Constructs JSON { name: "blargh", indices : [ [ x1, y1, z1 ], [ x2, y2, z2] ], points : [ [ x1, y1, z1 ], [ x2, y2, z2] ], distance : 0.3 } where distance is length()
    */
    std::string toJson();
protected:
    QtGlSliceView* parent;
    int floatingIndex; // if we are drawing, what index is being moved
    int crossStart; // defines how big the cross is in pixels
    int crossEnd; // defines how big the empty middle of the cross is
    int lineWidth; // how wide the cross and ruler lines

    PointType3D points[2];
    PointType3D indices[2];

    float clickRadius; // how close to get to the cross points for the mouse handover
    RulerToolState state;
};


/**
* Represents a collection of Rulers drawn on a particular slice of an Image.  Currently set up to have a RulerToolCollection per z-slice.  An alternative would have been
* one per entire image, but this makes the event handling a little easier.
*
* TODO : I don't really handle multiple generators well.  Weird things happen if you would toggle while drawing.
*
*/
class RulerToolCollection {
public:
    /**
    * \param parent the widget we're rendering in
    * \param metaDataFactory factory to generate ruler color, name, etc
    * \param axis which axis of the image slice corresponds to
    * \param slice integer slice these rulers are draw on
    */
    RulerToolCollection(QtGlSliceView* parent, std::shared_ptr< RulerToolMetaDataFactory > metaDataFactory, unsigned short axis, unsigned int slice);
    virtual ~RulerToolCollection();

    /**
    * Note, QtGlSliceView does all the computation to determine screen coordinate to image index space.  So we do a lot of image index space back
    * to screen coordinates.
    * 
    * \param eventType
    * \param index image index of mouse event
    */
    void handleMouseEvent(QMouseEvent* eventType, double index[]);
    void paint();
    RulerTool* getActive();
    std::vector< std::unique_ptr< RulerTool > > rulers;

    /**
    * Constructs JSON { axis : axis_num, slice : slice_num, rulers : [ see rulers ] }
    */
    std::string toJson();

    void setMetaDataFactory(std::shared_ptr< RulerToolMetaDataFactory > factory);

protected:
    QtGlSliceView* parent;
   
    /**
    * Is the image index coordinate over any of the rulers in this collection.
    * 
    * \return -1 for no, otherwise, the index 0 - N of the ruler that contains index
    */
    int isOver(double index[]);

    int currentId;
    RulerToolState state;
    unsigned short axis;
    unsigned int slice;

    std::shared_ptr< RulerToolMetaDataFactory > metaDataFactory;
};


#endif