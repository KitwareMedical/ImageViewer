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
#ifndef __QtGlSliceView_h
#define __QtGlSliceView_h

// Qt includes
#include <QOpenGLWidget>
#include <QtOpenGL/qgl.h>

// ITK includes
#include "itkImage.h"
#include "itkColorTable.h"

// ImageViewer includes
#include "QtImageViewer_Export.h"

using namespace itk;

/*! Clicking in a window will cause different events
*  NOP = nothing
*  SELECT = report pixel info
*/
const int NUM_ClickModeTypes = 3;
typedef enum {CM_NOP, CM_SELECT, CM_PAINT} ClickModeType;
const char ClickModeTypeName[3][7] =
  {{'N', 'O', 'P', '\0', ' ', ' ', ' '},
  {'S', 'e', 'l', 'e', 'c', 't', '\0'},
  {'B', 'o', 'x', '\0', ' ', ' ', ' '}};

  /*! Handling of values outside intensity window range - values above 
  *    and below can be handled separately
  *  IW_MIN = set values outside range to min value
  *  IW_MAX = set values outside range to max value
  *  IW_FLIP = rescale values to be within range by flipping
*/
const int NUM_ImageModeTypes = 8;
typedef enum {IMG_VAL, IMG_INV, IMG_LOG, IMG_DX, IMG_DY, IMG_DZ,
  IMG_BLEND, IMG_MIP} ImageModeType;
const char ImageModeTypeName[8][8] =
  {{'V', 'a', 'l', 'u', 'e', '\0', ' ', ' '},
  {'I', 'n', 'v', 'e', 'r', 's', 'e', '\0'},
  {'L', 'o', 'g', '\0', ' ', ' ', ' ', ' '},
  {'D', 'e', 'r', 'i', 'v', '-', 'X', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Y', '\0'},
  {'D', 'e', 'r', 'i', 'v', '-', 'Z', '\0'},
  {'B', 'l', 'e', 'n', 'd', '\0', ' ', ' '},
  {'M', 'I', 'P', '\0', ' ', ' ', ' ', ' '}};

const int NUM_IWModeTypes = 3;
typedef enum {IW_MIN, IW_MAX, IW_FLIP} IWModeType;
const char IWModeTypeName[3][5] =
  {{'M', 'i', 'n', '\0', ' '},
  {'M', 'a', 'x', '\0', ' '},
  {'F', 'l', 'i', 'p', '\0'}};

const int NUM_cWinOrientation = 3;
enum OrientationType{
  X_AXIS=0,
  Y_AXIS,
  Z_AXIS
};

  /*! Structure clickPoint to store the x,y,z and intensity value of a
  * point in the image
*/
struct ClickPoint 
  {
  double x, y, z;
  double value;
  
  ClickPoint(double _x,double _y,double _z,double v)
    : x(_x),y(_y),z(_z),value(v){}
  //double operator[](int index){return }
  };


/**
* QtGlSliceView : Derived from abstract class SliceView and Fl_Gl_Window
* See SliceView.h for details...
  **/
//  

class QtImageViewer_EXPORT QtGlSliceView :
    public QOpenGLWidget
{
  Q_OBJECT

  Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged);
  Q_PROPERTY(IWModeType iwModeMin READ iwModeMin WRITE setIWModeMin);
  Q_PROPERTY(IWModeType iwModeMax READ iwModeMax WRITE setIWModeMax);
  Q_PROPERTY(bool transpose READ isTransposed WRITE transpose);
  Q_PROPERTY(bool zFlipped READ isZFlipped WRITE flipZ);
  Q_PROPERTY(bool yFlipped READ isYFlipped WRITE flipY);
  Q_PROPERTY(bool xFlipped READ isXFlipped WRITE flipX);
  Q_PROPERTY(ImageModeType imageMode READ imageMode WRITE setImageMode);
  Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged);
  Q_PROPERTY(double iwMin READ iwMin WRITE setIWMin NOTIFY iwMinChanged);
  Q_PROPERTY(double iwMax READ iwMax WRITE setIWMax NOTIFY iwMaxChanged);
  Q_PROPERTY(double overlayOpacity READ overlayOpacity WRITE setOverlayOpacity NOTIFY overlayOpacityChanged);
  Q_PROPERTY(int sliceNum READ sliceNum WRITE setSliceNum NOTIFY sliceNumChanged);
  Q_PROPERTY(bool viewCrosshairs READ viewCrosshairs WRITE setViewCrosshairs);
  Q_PROPERTY(bool viewValue READ viewValue WRITE setViewValue);
  Q_PROPERTY(bool viewOverlayData READ viewOverlayData WRITE setViewOverlayData);
  Q_PROPERTY(bool viewAxisLabel READ viewAxisLabel WRITE setViewAxisLabel);
  Q_PROPERTY(bool viewClickedPoints READ viewClickedPoints WRITE setViewClickedPoints);
  Q_PROPERTY(bool viewValuePhysicalUnits READ viewValuePhysicalUnits WRITE setViewValuePhysicalUnits);
  Q_PROPERTY(int fastMoveValue READ fastMoveValue WRITE setFastMoveValue);
  Q_PROPERTY(double fastIWValue READ fastIWValue WRITE setFastIWValue);
  Q_PROPERTY(bool validOverlayData READ validOverlayData WRITE setValidOverlayData NOTIFY validOverlayDataChanged);
  Q_PROPERTY(int maxClickedPointsStored READ maxClickedPointsStored WRITE setMaxClickedPointsStored
             NOTIFY maxClickedPointsStoredChanged);
  /// This property controls how the image annotations are displayed. 0 means
  /// OFF, 1 means ON, >1 can be used by the application. "D" switches between each power of 2 state.
  /// The number of states is controlled by maxDisplayStates.
  /// 0x01 by default.
  /// \sa displayState(), setDisplayState(), displayStateChanged(),
  /// maxDisplayStates, nextDisplayState()
  Q_PROPERTY(int displayState READ displayState WRITE setDisplayState NOTIFY displayStateChanged);
  /// This property controls how many annotation states are controlled by the view.
  /// 2 by default: Off(0x00) and On(0x01).
  /// \sa maxDisplayStates, setMaxDisplayStates(),
  /// displayState
  Q_PROPERTY(int maxDisplayStates READ maxDisplayStates WRITE setMaxDisplayStates);

public:
  typedef QOpenGLWidget                        Superclass;
  typedef double                           ImagePixelType;
  typedef unsigned char                    OverlayPixelType;
  typedef itk::Image<ImagePixelType,3>     ImageType;
  typedef itk::Image<OverlayPixelType,3>   OverlayType;
  typedef ImageType::Pointer      ImagePointer;
  typedef OverlayType::Pointer    OverlayPointer;
  typedef ImageType::RegionType   RegionType;
  typedef ImageType::SizeType     SizeType;
  typedef ImageType::IndexType    IndexType;
  typedef itk::ColorTable<double> ColorTableType;
  typedef ColorTableType::Pointer ColorTablePointer;

public:

  QtGlSliceView(QWidget *parent = 0);

  virtual const ImagePointer & inputImage(void) const;

  /*! Return a pointer to the overlay data */
  const OverlayPointer &inputOverlay(void) const;

  void setClickSelectCallBack( void(*cb)(double,double,double,double) )
    { cClickSelectCallBack = cb; };

  void setClickSelectArgCallBack( void(*cb)(double,double,double,double,void*) )
    { cClickSelectArgCallBack = cb; };
  void setClickSelectArg( void *v )
    { cClickSelectArg = v; };

  void setClickMode( ClickModeType m )
    { cClickMode = m; };

  ClickModeType clickMode( void )
    { return cClickMode; };

  /*! Get the opacity of the overlay */
  double overlayOpacity(void) const;

  ColorTableType *colorTable(void) const;

  virtual QSize minimumSizeHint()const;
  virtual QSize sizeHint()const;

  virtual bool hasHeightForWidth()const;
  virtual int heightForWidth(int width)const;

  virtual void update();

  /*! What slice is being viewed */
  int sliceNum(void) const;

  virtual void mousePressEvent(QMouseEvent *event);

  virtual void mouseMoveEvent(QMouseEvent *event) ;

  virtual void keyPressEvent(QKeyEvent* event);

  /// Return the minimum intensity of the image
  /// \sa maxIntensity(), intensityRange()
  double minIntensity() const;

  /// Return the maximum intensity of the image
  /// \sa minIntensity(), intensityRange()
  double maxIntensity() const;

  /// Return teh range of intensity of the image.
  /// \sa minIntensity(), maxIntensity()
  double intensityRange() const;

  /// Return the lower intensity of the window.
  /// \sa iwMin
  double iwMin() const;

  /// Return the upper intensity of the window.
  /// \sa iwMax
  double iwMax() const;

  double zoom() const;

  ///Get the coordinates of the center of the region of interest being viewed
  ///\sa windowCenterY() windowCenterZ() centerWindow()
  int windowCenterX() const;

  ///Get the coordinates of the center of the region of interest being viewed
  ///\sa windowCenterX() windowCenterZ() centerWindow()
  int windowCenterY() const;

  ///Get the coordinates of the center of the region of interest being viewed
  ///\sa windowCenterX() windowCenterY() centerWindow()
  int windowCenterZ() const;

  int orientation() const;

  ///Returns the ImageModeType
  ImageModeType imageMode() const;

  ///Return if the image is flipped about the x-axis
  ///\sa flipX(bool newFlipX), flipY(bool newFlipY), yFlipped(), flipZ(bool newFlipZ), zFlipped()
  bool isXFlipped() const;

  ///Return if the image is flipped about the y-axis
  ///\sa flipX(bool newFlipX), flipY(bool newFlipY), xFlipped(), flipZ(bool newFlipZ), zFlipped()
  bool isYFlipped() const;

  ///Return if the image is flipped about the z-axis
  ///\sa flipX(bool newFlipX), flipY(bool newFlipY), yFlipped(), flipZ(bool newFlipZ), xFlipped()
  bool isZFlipped() const;

  ///Is the image Transpose?
  bool isTransposed() const;

  IWModeType iwModeMin() const;

  IWModeType iwModeMax() const;

  void saveClickedPointsStored();

  bool viewOverlayData(void) const;

  bool viewCrosshairs() const;

  bool viewValue() const;

  int fastMoveValue() const;

  double fastIWValue() const;

  bool viewAxisLabel() const;

  bool viewClickedPoints() const;

  bool viewValuePhysicalUnits() const;

  bool clickedPoint(int index, ClickPoint & point);

  int clickedPointsStored()const;

  int maxClickedPointsStored() const;

  int maxSliceNum() const;

  bool validOverlayData() const;
  QDialog* helpWindow() const;

  int imageSize(int axis) const;

  /// Return the displayState property value.
  /// \sa displayState, setDisplayState()
  int displayState() const;

  /// Return the maxDisplayStates property value.
  /// \sa maxDisplayStates, setMaxDisplayStates()
  int maxDisplayStates() const;

  /// Set the maxDisplayStates property value.
  /// \sa maxDisplayStates, maxDisplayStates()
  void setMaxDisplayStates(int stateNumber);

public slots:
  /// Set the displayState property value.
  /// \sa displayState, displayState()
  void setDisplayState(int state);

  void setValidOverlayData(bool validOverlayData);

  void clearClickedPointsStored();

  void deleteLastClickedPointsStored();

  void setMaxClickedPointsStored(int i);

  void setViewValuePhysicalUnits(bool physicalValueUnits);

  void setViewClickedPoints(bool pointsClicked);
  /*! Turn on/off the viewing of the overlay */
  void setViewOverlayData(bool newViewOverlayData);

  void setViewValue(bool value);

  void setViewCrosshairs(bool crosshairs);
  /*! Specify the slice to view */
  void setSliceNum(int newSliceNum);

  /*! Specify the opacity of the overlay */
  void  setOverlayOpacity(double newOverlayOpacity);

  /*! Specify the 3D image to view slice by slice */
  virtual void setInputImage(ImageType * newImData);

  /*! Specify the 3D image to view as an overlay */
  void setInputOverlay(OverlayType * newOverlayData);

  void setOverlay(bool newOverlay);
  void createOverlay( void );
  void saveOverlay( void );
  void paintOverlayPoint( double x, double y, double z );

  void setIWModeMin(IWModeType newIWModeMin);
  void setIWModeMin(const char* mode);
  void setIWModeMax(IWModeType newIWModeMax);
  void setIWModeMax(const char* mode);

  ///Transpose the image in the xy-plane
  void transpose(bool newTranspose);

  ///Flip the image about the z-axis
  ///\sa zFlipped(), flipY(bool newFlipY), yFlipped(), flipX(bool newFlipX), xFlipped()
  void flipZ(bool newFlipZ);

  ///Flip the image about the y-axis
  ///\sa xFlipped(), flipX(bool newFlipX), yFlipped(), flipZ(bool newFlipZ), zFlipped()
  void flipY(bool newFlipY);

  ///Flip the image about the x-axis
  ///\sa xFlipped(), flipY(bool newFlipY), yFlipped(), flipZ(bool newFlipZ), zFlipped()
  void flipX(bool newFlipX);

  ///Set the ImageModeType
  void setImageMode(ImageModeType newImageMode);
  void setImageMode(const char* newImageMode);

  void setViewAxisLabel(bool axisLabel);

  ///This method fixes cBoxMin[].
  ///\sa setBoxMax(double x, double y, double z)
  void setBoxMin(double minX, double minY, double minZ);

  ///This method fixes cBoxMax[].
  ///\sa setBoxMin(double minX, double minY, double minZ)
  void setBoxMax(double x, double y, double z);

  ///Set the orientation
  void setOrientation(int newOrientation);

  ///Default centering, center at the middle of the image
  ///\sa centerWindow(int newWinCenterX, int newWinCenterY, int newwindowCenterZ)
  void centerWindow();

  ///Specify the coordinates of the center of the region of interest to view
  ///\sa centerWindow()
  void centerWindow(int newWinCenterX, int newWinCenterY, int newWinCenterZ);

  /// This method allows to zoom in or zoom out.
  /// \sa zoom()
  void setZoom(double newWinZoom);

  ///Fix the new Slice
  void changeSlice(int value);

  /// Fix the lower limit of the intensity widowing
  /// \sa iwMin
  void setIWMin(double value);

  /// Fix the upper limit of the intensity widowing
  /// \sa iwMax
  void setIWMax(double value);

  void setFastMoveValue(int movVal);

  void setFastIWValue(double iwVal);

  void zoomIn();
  void zoomOut();
  void showHelp();

  void selectPoint(double newX, double newY, double newZ);

  void renderText(double x, double y, const QString & str,
    const QFont & font = QFont() );


signals:

  void imageChanged();
  void positionChanged(double newX, double newY, double newZ, double click);
  void iwMinChanged(double minimum);
  void iwMaxChanged(double maximum);
  void sliceNumChanged(int value);
  void zoomChanged(double zoom);
  void detailsChanged(QString s);
  void orientationChanged(int maximum);
  void overlayOpacityChanged(double opacity);
  void validOverlayDataChanged(bool valid);
  void maxClickedPointsStoredChanged(int max);
  void displayStateChanged(int state);

protected:

  void initializeGL();
  void paintGL();

  /// Return the next display state.
  /// \sa displayState
  virtual int nextDisplayState(int state)const;

  int cDisplayState;
  int cMaxDisplayStates;
  bool cValidOverlayData;
  double cOverlayOpacity;
  int cOverlayPaintRadius;
  int cOverlayPaintColor;

  OverlayPointer cOverlayData;

  unsigned char *cWinOverlayData;
  QDialog* cHelpDialog;

  ColorTablePointer cColorTable;

  void (*cSliceNumCallBack)(void);
  void *cSliceNumArg;
  void (*cSliceNumArgCallBack)(void *sliceNumArg);

  bool cValidImData;
  bool cViewImData;
  bool cViewClickedPoints;
  ImagePointer cImData;
  unsigned long cDimSize[3];
  double cSpanMax;
  double cSpacing[3];

  ClickModeType cClickMode;
  double cClickSelect[3];
  double cClickSelectV;
  void (*cClickSelectCallBack)(double x,double y,double z,
                               double v);
  void *cClickSelectArg;
  void (*cClickSelectArgCallBack)(double x, double y, double z,
                                  double v, void *clickSelectArg);

  double cBoxMin[3];
  double cBoxMax[3];
  void (*cClickBoxCallBack)(double minX, double minY, double minZ,
                            double maxX, double maxY, double maxZ);
  void *cClickBoxArg;
  void (*cClickBoxArgCallBack)(double minX, double minY, double minZ,
                               double maxX, double maxY, double maxZ,
                               void * clickBoxArg);

  double cIWMin;
  double cIWMax;
  IWModeType cIWModeMin;
  IWModeType cIWModeMax;
  void (*cIWCallBack)(void);
  void *cIWArg;
  void (*cIWArgCallBack)(void *iwArg);

  ImageModeType cImageMode;

  bool cFlipX[3];
  bool cFlipY[3];
  bool cFlipZ[3];
  bool cTranspose[3];

  double cWinZoom;
  int cWinOrder[3];
  int cWinOrientation;
  void (*cWinOrientationCallBack)(void);
  void *cWinOrientationArg;
  void (*cWinOrientationArgCallBack)(void *winOrientationArg);

  int cWinCenter[3];
  void (*cWinCenterCallBack)(void);
  void *cWinCenterArg;
  void (*cWinCenterArgCallBack)(void *winCenterArg);

  bool cViewAxisLabel;
  char cAxisLabelX[3][80];
  char cAxisLabelY[3][80];

  bool cViewOverlayData;
  bool cViewCrosshairs;
  bool cViewValue;
  bool cViewValuePhysicalUnits;
  const char * cPhysicalUnitsName;

  int cWinMinX;
  int cWinMaxX;
  unsigned int cWinSizeX;
  int cWinMinY;
  int cWinMaxY;
  unsigned int cWinSizeY;
  int cWinDataSizeX;
  int cWinDataSizeY;
  int inDataSizeX;
  int inDataSizeY;
  unsigned char *cWinImData;
  unsigned short *cWinZBuffer;

  double cDataMax;
  double cDataMin;

  /* list of points clicked and maximum no. of points to be stored*/
  typedef QList<ClickPoint> ClickPointListType;
  ClickPointListType cClickedPoints;
  int cMaxClickPoints;

  int cFastPace;
  int cFastMoveValue[3]; //fast moving pace
  double cFastIWValue[3]; //fast IW pace
};
  
#endif
