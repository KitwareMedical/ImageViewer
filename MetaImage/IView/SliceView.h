#ifndef _SLICEVIEW_H
#define _SLICEVIEW_H

#include<itkPhysicalImage.h>
#include<FL/fl_draw.H>
#include<list>

using namespace itk;

namespace itk {

/*! Clicking in a window will cause different events
 *  NOP = nothing
 *  PAN = shift the center of the ROI
 *  SELECT = report pixel info
 *  USER = call a user function
 */
typedef enum {CM_NOP, CM_PAN, CM_SELECT, CM_USER, CM_BOX} ClickModeType;

/*! Handling of values outside intensity window range - values above and below
 *      can be handled separately
 *  IW_MIN = set values outside range to min value
 *  IW_MAX = set values outside range to max value
 *  IW_FLIP = rescale values to be within range by flipping
 */

typedef enum {IMG_VAL, IMG_LOG, IMG_DX, IMG_DY, IMG_DZ, IMG_H, IMG_MIP} ImageModeType;

typedef enum {IW_MIN, IW_MAX, IW_FLIP} IWModeType;

/*! Structure clickPoint to store the x,y,z and intensity value of a point
 *  in the image
 */
struct ClickPoint {
   float x, y, z;
   double value;

   ClickPoint(float _x,float _y,float _z,double v):x(_x),y(_y),z(_z),value(v){}
};


/*! Multifunction slice-by-slice viewer
 *  This is a virtual base class, which provides most of the data members and
 *  methods slice viewers would need.
 *  To get a concrete slice-viewer, inherit this class along with a windowing
 *  class (example: Fl_window or Fl_Gl_Window).
 *  /author Stephen R. Aylward
 *  /date 11/22/99
 */
template <class imType>
class SliceView {
protected:
  void   (* cSliceNumCallBack)(void);
  void    * cSliceNumArg;
  void   (* cSliceNumArgCallBack)(void * sliceNumArg);

  bool                     cValidImData;
  bool                     cViewImData;
  PhysicalImage<imType,3>::Pointer cImData;
  void                    (* cViewImDataCallBack)(void);
  void                     * cViewImDataArg;
  void                    (* cViewImDataArgCallBack)(void *viewImDataArg);

  ClickModeType cClickMode;
  float         cClickSelectX, cClickSelectY, cClickSelectZ;
  void          (* cClickSelectCallBack)(float x,float y,float z,float v);
  void           * cClickSelectArg;
  void          (* cClickSelectArgCallBack)(float x, float y, float z, float v, void *clickSelectArg);

  float       cClickUserX, cClickUserY, cClickUserZ;
  void        (* cClickUserCallBack)(float x,float y,float z,float v);
  void         * cClickUserArg;
  void        (* cClickUserArgCallBack)(float x, float y, float z, float v, void * clickUserArg);

  float       cBoxMinX, cBoxMinY, cBoxMinZ;
  float       cBoxMaxX, cBoxMaxY, cBoxMaxZ;
  void        (* cClickBoxCallBack)(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
  void         * cClickBoxArg;
  void        (* cClickBoxArgCallBack)(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, void * clickBoxArg);

  float       cIWMin;
  float       cIWMax;
  IWModeType  cIWModeMin;
  IWModeType  cIWModeMax;
  void        (* cIWCallBack)(void);
  void         * cIWArg;
  void        (* cIWArgCallBack)(void * iwArg);

  ImageModeType cImageMode;
  
  bool        cFlipX;
  bool        cFlipY;
  bool        cFlipZ;

  float               cWinZoom;
  unsigned int        cWinOrder[3];
  unsigned int        cWinOrientation;
  void                (* cWinOrientationCallBack)(void);
  void                 * cWinOrientationArg;
  void                (* cWinOrientationArgCallBack)(void * winOrientationArg);

  unsigned int        cWinCenter[3];
  void                (* cWinCenterCallBack)(void);
  void                 * cWinCenterArg;
  void                (* cWinCenterArgCallBack)(void * winCenterArg);

  bool        cViewAxisLabel;
  char        cAxisLabelX[3][80];
  char        cAxisLabelY[3][80];

  unsigned int     cWinMinX;
  unsigned int     cWinMaxX;
  unsigned int     cWinSizeX;
  unsigned int     cWinMinY;
  unsigned int     cWinMaxY;
  unsigned int     cWinSizeY;
  unsigned int     cWinDataSizeX;
  unsigned int     cWinDataSizeY;
  unsigned int	 inDataSizeX;
  unsigned int	 inDataSizeY;
  unsigned char    *cWinImData;
  unsigned short   *cWinZBuffer;

	unsigned long *cDimSize;
	imType cDataMax, cDataMin;

	    /* list of points clicked and maximum no. of points to be stored*/
        std::list< ClickPoint * > cClickedPoints;
	    unsigned int maxClickPoints;

        int cX, cY, cW, cH;

public:
  /*! SliceView Constructor */
  SliceView(int x, int y, int w, int h, const char *l=0);
  /*! Standard destructor */
  virtual ~SliceView(void);

  /*! Specify the 3D image to view in slices - pure virtual function*/
  virtual void  SetInputImage(const PhysicalImage<imType,3>::Pointer newImData) = 0;

  /*! Return a pointer to the image data */
  
  const PhysicalImage<imType,3>::Pointer GetInputImage(void) const;
  
  /*! Dammit, give me a pointer to the image that's not const! */
  PhysicalImage<imType,3>::Pointer GetInputImage(void);
  
  /*! Return a pointer to the pixels being displayed */
  unsigned char *         winImData(void);

  /*! Return the window x-dim */

  unsigned int    winDataSizeX(void);

  /*! Return the window y-dim */

  unsigned int    winDataSizeY(void);
  /*! Turn on/off the viewing of the image */
  void	        viewImData(bool newViewImData);
  /*! Status of the image - viewed / not viewed */
  bool            viewImData(void);
  /*! Called when viewing of the image is toggled */
  void            viewImDataCallBack(void (* newViewImDataCallBack)(void));
  void            viewImDataCallBack(void (* newViewImDataArgCallBack)(void *), void * viewImDataArg);

  /*! Flip the image about the x-axis */
  virtual void    flipX(bool newFlipX);
  /*! Is the image flipped? */
  bool            flipX(void);
  /*! Flip the image about the y-axis */
  virtual void    flipY(bool newFlipY);
  /*! Is the image flipped? */
  bool            flipY(void);
  /*! Flip the image about the z-axis */
  virtual void    flipZ(bool newFlipZ);
  /*! Is the image flipped? */
  bool            flipZ(void);

  /*! Specify a zoom factor */
  void    winZoom(float newWinZoom);
  /*! How much is the window zoomed */
  float   winZoom(void);
  /*! Specify the coordinates of the center of the region of interest to view */
  void    winCenter(unsigned int newWinCenterX, unsigned int newWinCenterY);
  /*! Default centering, center at the middle of the image */
  void    winCenter(void);
  /*! Get the coordinates of the center of the region of interest being viewed */
  unsigned int    winCenterX(void);
  /*! Get the coordinates of the center of the region of interest being viewed */
  unsigned int    winCenterY(void);
  /*! Shift the region of interest being viewed */
  void    winShift(int newWinShiftUp, int newWinShiftRight);
  /*! Called when center is changed */
  void    winCenterCallBack(void (* newWinCenterCallBack)(void));
  void    winCenterCallBack(void (* newWinCenterArgCallBack)(void *), void * newWinCetnerArg);

  /*! Return the total number of slices */
  unsigned int    numSlices(void);
  /*! Specify the slice to view */
  void 	        sliceNum(unsigned int newSliceNum);
  /*! What slice is being viewed */
  unsigned int    sliceNum(void);
  /*! Called when new slice is viewed */
  void            sliceNumCallBack(void (* newSliceNumCallBack)(void));
  void            sliceNumCallBack(void (* newSliceNumCallBack)(void *), void * newSliceNumArg);

  unsigned int orientation(void);
  void         orientation(unsigned int newOrientation);
  void         orientationCallBack(void (*newOrientationCallBack)(void));
  void         orientationCallBack(void (*newOrientationArgCallBack)(void *), void * newOrientationArg);

  /*! Specify the clickMode - a user click in a window will cause various events */
  void            clickMode(ClickModeType newClickMode);
  ClickModeType   clickMode(void);

  virtual void clickSelect(float newX, float newY, float newZ);
  float        clickSelectX(void);
  float        clickSelectY(void);
  float        clickSelectZ(void);
  void         clickSelectCallBack(void (*newClickSelectCallBack)(float newX, float newY, float newZ, float newV));
  void         clickSelectCallBack(void (*newClickSelectArgCallBack)(float, float, float, float, void *), void * newClickSelectArg);

  /*! Returns the last ith clicked point - ie. to get the last point
  * i = 0, to get the 2nd last point i = 1, and so on
  */
  ClickPoint*  getClickedPoint(int i=0);
  float*       getClickedPointCoordinates(int i=0);
  int numClickedPointsStored(){ return cClickedPoints.size();}

  void         maxClickedPointsStored(unsigned int i);
  unsigned int maxClickedPointsStored();

  void	clearClickedPointsStored(){cClickedPoints.clear();}

  void        clickUser(float newX, float newY, float newZ);
  float       clickUserX(void);
  float       clickUserY(void);
  float       clickUserZ(void);
  void        clickUserCallBack(void (*newClickUserCallBack)(float newX, float newY, float newZ, float newV));
  void        clickUserCallBack(void (*newClickUserArgCallBack)(float, float, float, float, void *), void * newClickUserArg);

  void            boxMin(float minX, float minY, float minZ);
  virtual void    boxMax(float maxX, float maxY, float maxZ);
  void            clickBoxCallBack(void (*newClickBoxCallBack)(float minX, float minY, float minZ, float maxX, float maxY, float maxZ));
  void            clickBoxCallBack(void (*newClickBoxArgCallBack)(float, float, float, float, float, float, void *), void * newClickBoxArg);

  void        iwMin(float newIWMin);
  float       iwMin(void);
  void        iwMax(float newIWMax);
  float       iwMax(void);
  void        iwModeMin(IWModeType newIWModeMin);
  IWModeType  iwModeMin(void);
  void        iwModeMax(IWModeType newIWModeMin);
  IWModeType  iwModeMax(void);
  void        iwCallBack(void (*newIWCallBack)(void));
  void        iwCallBack(void (*newIWArgCallBack)(void *), void * newIWArg);

  void            imageMode(ImageModeType newImageMode);
  ImageModeType   imageMode(void);

  virtual void    size(int w, int h);
  virtual void    resize(int x, int y, int w, int h);

  virtual int  handle(int event);

  virtual void update() = 0;
  virtual void draw() = 0;

  unsigned int WinMinX(){return cWinMinX;}
  unsigned int WinMinY(){return cWinMinY;}
  unsigned int WinSizeX(){return cWinSizeX;}
  unsigned int WinSizeY(){return cWinSizeY;}
};



template<class imType>
SliceView<imType>::SliceView(int x, int y, int w, int h, const char * l)
{
    cX = x;
    cY = y;
    cW = w;
    cH = h;

    cValidImData = false;
    cViewImData = false;
    cViewImDataCallBack = NULL;
    cViewImDataArg = NULL;
    cViewImDataArgCallBack = NULL;

    cClickMode = CM_SELECT;
    cClickSelectCallBack = NULL;
    cClickSelectArg = NULL;
    cClickSelectArgCallBack = NULL;
    cClickUserCallBack = NULL;
    cClickUserArg = NULL;
    cClickUserArgCallBack = NULL;
    cClickBoxCallBack = NULL;
    cClickBoxArg = NULL;
    cClickBoxArgCallBack = NULL;

    cIWMin = 0;
    cIWMax = 0;
    cIWModeMin = IW_MIN;
    cIWModeMax = IW_MAX;    
    cImageMode = IMG_VAL;
    cIWCallBack = NULL;
    cIWArg = NULL;
    cIWArgCallBack = NULL;
    
    cFlipX = false;
    cFlipY = false;
    cFlipZ = false;

    cWinZoom = 1;
    cWinOrder[0] = 0;
    cWinOrder[1] = 1;
    cWinOrder[2] = 2;
    cWinOrientation = 2;
    cWinOrientationCallBack = NULL;
    cWinOrientationArg = NULL;
    cWinOrientationArgCallBack = NULL;

    cWinCenter[0] = 0;
    cWinCenter[1] = 0;
    cWinCenter[2] = 0;
    cWinCenterCallBack = NULL;
    cWinCenterArg = NULL;
    cWinCenterArgCallBack = NULL;

    cSliceNumCallBack = NULL;
    cSliceNumArg = NULL;
    cSliceNumArgCallBack = NULL;

    cViewAxisLabel = true;
    sprintf(cAxisLabelX[0], "S");
    sprintf(cAxisLabelX[1], "L");
    sprintf(cAxisLabelX[2], "L");
    sprintf(cAxisLabelY[0], "P");
    sprintf(cAxisLabelY[1], "S");
    sprintf(cAxisLabelY[2], "P");

    cWinMinX = 0;
    cWinMaxX = 0;
    cWinSizeX = 0;
    cWinMinY = 0;
    cWinMaxY = 0;
    cWinSizeY = 0;
    cWinDataSizeX = 0;
    cWinDataSizeY = 0;
    cWinImData = NULL;
    cWinZBuffer = NULL;

    maxClickPoints = 0;

}

//
//
//
template <class imType>
SliceView<imType>::~SliceView()
{
    if(cWinImData != NULL)
        delete [] cWinImData;
    cWinImData = NULL;
    if(cWinZBuffer != NULL)
        delete [] cWinZBuffer;
    cWinZBuffer = NULL;
}




//
//
template <class imType>
const PhysicalImage<imType,3>::Pointer SliceView<imType>::GetInputImage(void) const
{
    return cImData;
}




template <class imType>
PhysicalImage<imType,3>::Pointer SliceView<imType>::GetInputImage(void)
{
	return (PhysicalImage<imType,3>::Pointer)cImData;
}




//
//
template <class imType>
unsigned char* SliceView<imType>::winImData(void)
{
	return cWinImData;
}


//
//

template <class imType>
unsigned int SliceView<imType>::winDataSizeX(void)
{

	return cWinDataSizeX;

}


//
//
template <class imType>
unsigned int SliceView<imType>::winDataSizeY(void)

{

	return cWinDataSizeY;

}



//
//
template <class imType>
void SliceView<imType>::viewImData(bool newViewImData)
{
    cViewImData = newViewImData;
    if(cViewImDataCallBack != NULL)
        cViewImDataCallBack();
    if(cViewImDataArgCallBack != NULL)
        cViewImDataArgCallBack(cViewImDataArg);
}




template <class imType>
bool SliceView<imType>::viewImData(void)
{
    return cViewImData;
}




template <class imType>
void SliceView<imType>::viewImDataCallBack(void (* newViewImDataCallBack)(void))
{
    cViewImDataCallBack = newViewImDataCallBack;
}


  
  
template <class imType>
void SliceView<imType>::viewImDataCallBack(void (* newViewImDataArgCallBack)(void *), void * newViewImDataArg)
{
    cViewImDataArg = newViewImDataArg;
    cViewImDataArgCallBack = newViewImDataArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::flipX(bool newFlipX)
{
    cFlipX = newFlipX;
}




template <class imType>
bool SliceView<imType>::flipX()
{
    return cFlipX;
}




//
//
template <class imType>
void SliceView<imType>::flipY(bool newFlipY)
{
    cFlipY = newFlipY;
}




template <class imType>
bool SliceView<imType>::flipY()
{
    return cFlipY;
}




//
//
template <class imType>
void SliceView<imType>::flipZ(bool newFlipZ)
{
    cFlipZ = newFlipZ;
}




template <class imType>
bool SliceView<imType>::flipZ()
{
    return cFlipZ;
}




//
//
template <class imType>
unsigned int SliceView<imType>::numSlices(void)
{
PhysicalImage<imType,3>::RegionType cregion = cImData->GetLargestPossibleRegion();
PhysicalImage<imType,3>::SizeType   size   = cregion.GetSize();
unsigned long *s=new unsigned long[3];
s[0]=size[0];
s[1]=size[1];
s[2]=size[2];

    return s[cWinOrder[2]];
}



//
//
template <class imType>
void SliceView<imType>::sliceNum(unsigned int newSliceNum)
{
    if(newSliceNum>=cDimSize[cWinOrder[2]])
        newSliceNum = cDimSize[cWinOrder[2]]-1;
    cWinCenter[cWinOrder[2]] = newSliceNum;

    if(cSliceNumCallBack != NULL)
        cSliceNumCallBack();
    if(cSliceNumArgCallBack != NULL)
        cSliceNumArgCallBack(cSliceNumArg);
}




template <class imType>
unsigned int SliceView<imType>::sliceNum(void)
{
    return cWinCenter[cWinOrder[2]];
}




template <class imType>
void SliceView<imType>::sliceNumCallBack(void (* newSliceNumCallBack)(void))
{
    cSliceNumCallBack = newSliceNumCallBack;
}




template <class imType>
void SliceView<imType>::sliceNumCallBack(void (* newSliceNumArgCallBack)(void *), void * newSliceNumArg)
{
    cSliceNumArg = newSliceNumArg;
    cSliceNumArgCallBack = newSliceNumArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::clickMode(ClickModeType newClickMode)
{
    cClickMode = newClickMode;
}




template <class imType>
ClickModeType SliceView<imType>::clickMode(void)
{
    return cClickMode;
}




template <class imType>
void SliceView<imType>::clickSelectCallBack(void (*newClickSelectCallBack)(float _x, float _y, float _z, float _v))
{
    cClickSelectCallBack = newClickSelectCallBack;
}




template <class imType>
void SliceView<imType>::clickSelectCallBack(void (*newClickSelectArgCallBack)(float _x, float _y, float _z, float _v, void *clickSelectArg), void * newClickSelectArg)
{
    cClickSelectArg = newClickSelectArg;
    cClickSelectArgCallBack = newClickSelectArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::clickSelect(float newX, float newY, float newZ)
{    
    cClickSelectX = newX;
    if(cClickSelectX<0)
        cClickSelectX = 0;
    if(cClickSelectX >= cDimSize[0])
        cClickSelectX = cDimSize[0]-1;

    cClickSelectY = newY;
    if(cClickSelectY<0)
        cClickSelectY = 0;
    if(cClickSelectY >= cDimSize[1])
        cClickSelectY = cDimSize[1]-1;

    cClickSelectZ = newZ;
    if(cClickSelectZ<0)
        cClickSelectZ = 0;
    if(cClickSelectZ >= cDimSize[2])
        cClickSelectZ = cDimSize[2]-1;

	itk::PhysicalImage<imType,3>::IndexType ind;
    ind[0] = (unsigned long)cClickSelectX;
    ind[1] = (unsigned long)cClickSelectY;
    ind[2] = (unsigned long)cClickSelectZ;
    imType tf = cImData->GetPixel(ind);
        
    /*if length of list is equal to max, remove the earliest point stored */
    if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
	      cClickedPoints.pop_back();

    cClickedPoints.push_front(new ClickPoint(cClickSelectX,
				cClickSelectY, cClickSelectZ, tf));

    if(cClickSelectCallBack != NULL)
        cClickSelectCallBack(cClickSelectX, cClickSelectY, cClickSelectZ, tf);
    if(cClickSelectArgCallBack != NULL)
        cClickSelectArgCallBack(cClickSelectX, cClickSelectY, cClickSelectZ, tf, cClickSelectArg);
}




template <class imType>
void SliceView<imType>::maxClickedPointsStored(unsigned int i)
{
 maxClickPoints = i;
}




template <class imType>
unsigned int SliceView<imType>::maxClickedPointsStored()
{
 return maxClickPoints;
}




template <class imType>
float SliceView<imType>::clickSelectX(void)
{
    return cClickSelectX;
}




template <class imType>
float SliceView<imType>::clickSelectY(void)
{
    return cClickSelectY;
}




template <class imType>
float SliceView<imType>::clickSelectZ(void)
{
    return cClickSelectZ;
}




template <class imType>
ClickPoint* SliceView<imType>::getClickedPoint(int index)
{
    if(index >= cClickedPoints.size())
	return ((ClickPoint*)0);
    std::list<ClickPoint*>::const_iterator j = cClickedPoints.begin();
    for(int i=0;i<index;i++,j++);
    return(*j);
}




template <class imType>
float* SliceView<imType>::getClickedPointCoordinates(int index)
{
    if(index>= cClickedPoints.size())
	return (float*)0;
    float *p = new float[3];
    std::list<ClickPoint*>::const_iterator j = cClickedPoints.begin();
    for(int i=0;i<index;i++,j++);
    p[0] = (*j)->x;
    p[1] = (*j)->y;
    p[2] = (*j)->z;
    return p;
}




template <class imType>
void SliceView<imType>::clickUserCallBack(void (*newClickUserCallBack)(float _x, float _y, float _z, float _v))
{
    cClickUserCallBack = newClickUserCallBack;
}




template <class imType>
void SliceView<imType>::clickUserCallBack(void (*newClickUserArgCallBack)(float _x, float _y, float _z, float _v, void * clickUserArg), void * newClickUserArg)
{
    cClickUserArg = newClickUserArg;
    cClickUserArgCallBack = newClickUserArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::clickUser(float newX, float newY, float newZ)
{    
    cClickUserX = newX;
    if(cClickUserX<0)
        cClickUserX = 0;
    if(cClickUserX>=cDimSize[0])
        cClickUserX = cDimSize[0]-1;

    cClickUserY = newY;
    if(cClickUserY<0)
        cClickUserY = 0;
    if(cClickUserY>=cDimSize[1])
        cClickUserY = cDimSize[1]-1;

    cClickUserZ = newZ;
    if(cClickUserZ<0)
        cClickUserZ = 0;
    if(cClickUserZ>=cDimSize[2])
        cClickUserZ = cDimSize[2]-1;

    PhysicalImage<imType,3>::IndexType ind;
    ind[0] = (unsigned long)cClickUserX;
    ind[1] = (unsigned long)cClickUserY;
    ind[2] = (unsigned long)cClickUserZ;
    imType tf = cImData->GetPixel(ind);
 
    if(cClickUserCallBack != NULL)
        cClickUserCallBack(cClickUserX, cClickUserY, cClickUserZ, tf);
    if(cClickUserArgCallBack != NULL)
        cClickUserArgCallBack(cClickUserX, cClickUserY, cClickUserZ, tf, cClickUserArg);

}




template <class imType>
float SliceView<imType>::clickUserX(void)
{    
    return cClickUserX;
}




template <class imType>
float SliceView<imType>::clickUserY(void)
{    
    return cClickUserY;
}




template <class imType>
float SliceView<imType>::clickUserZ(void)
{    
    return cClickUserZ;
}




//
//
//
template <class imType>
void SliceView<imType>::clickBoxCallBack(void (*newClickBoxCallBack)(float minX, float minY, float minZ, float maxX, float maxY, float maxZ))
{
    cClickBoxCallBack = newClickBoxCallBack;
}




template <class imType>
void SliceView<imType>::clickBoxCallBack(void (*newClickBoxArgCallBack)(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, void * clickBoxArg), void * newClickBoxArg)
{
    cClickBoxArg = newClickBoxArg;
    cClickBoxArgCallBack = newClickBoxArgCallBack;
}




template <class imType>
void SliceView<imType>::boxMin(float minX, float minY, float minZ)
{
    cBoxMinX = minX;
    cBoxMinY = minY;
    cBoxMinZ = minZ;
}




template <class imType>
void SliceView<imType>::boxMax(float x, float y, float z)
{
    float x0, y0, z0;
    float x1, y1, z1;
    
    x0 = (cBoxMinX<x) ? cBoxMinX : x;
    y0 = (cBoxMinY<y) ? cBoxMinY : y;
    z0 = (cBoxMinZ<z) ? cBoxMinZ : z;

    x1 = (cBoxMinX<x) ? x : cBoxMinX;
    y1 = (cBoxMinY<y) ? y : cBoxMinY;
    z1 = (cBoxMinZ<z) ? z : cBoxMinZ;

    cBoxMinX = x0;
    cBoxMinY = y0;
    cBoxMinZ = z0;

    cBoxMaxX = x1;
    cBoxMaxY = y1;
    cBoxMaxZ = z1;

    if(cClickBoxCallBack != NULL)
        cClickBoxCallBack(cBoxMinX, cBoxMinY, cBoxMinZ, cBoxMaxX, cBoxMaxY, cBoxMaxZ);
    if(cClickBoxArgCallBack != NULL)
        cClickBoxArgCallBack(cBoxMinX, cBoxMinY, cBoxMinZ, cBoxMaxX, cBoxMaxY, cBoxMaxZ, cClickBoxArg);
}

        
//
//
//
template <class imType>
void SliceView<imType>::iwMin(float newIWMin)
{
    cIWMin = newIWMin;
    if(cIWCallBack != NULL)
        cIWCallBack();
    if(cIWArgCallBack != NULL)
        cIWArgCallBack(cIWArg);
}




template <class imType>
float SliceView<imType>::iwMin(void)
{
    return cIWMin;
}




//
//
//
template <class imType>
void SliceView<imType>::iwMax(float newIWMax)
{
    cIWMax = newIWMax;
    if(cIWCallBack != NULL)
        cIWCallBack();
    if(cIWArgCallBack != NULL)
        cIWArgCallBack(cIWArg);
}




template <class imType>
float SliceView<imType>::iwMax(void)
{
    return cIWMax;
}




//
//
//
template <class imType>
void SliceView<imType>::iwModeMin(IWModeType newIWModeMin)
{
    cIWModeMin = newIWModeMin;
    if(cIWCallBack != NULL)
        cIWCallBack();
    if(cIWArgCallBack != NULL)
        cIWArgCallBack(cIWArg);
}




template <class imType>
IWModeType SliceView<imType>::iwModeMin(void)
{
    return cIWModeMin;
}




//
//
//
template <class imType>
void SliceView<imType>::iwModeMax(IWModeType newIWModeMax)
{
    cIWModeMax = newIWModeMax;
    if(cIWCallBack != NULL)
        cIWCallBack();
    if(cIWArgCallBack != NULL)
        cIWArgCallBack(cIWArg);
}




template <class imType>
IWModeType SliceView<imType>::iwModeMax(void)
{
    return cIWModeMax;
}




//
//
//
template <class imType>
void SliceView<imType>::imageMode(ImageModeType newImageMode)
{
    cImageMode = newImageMode;
    if(cIWCallBack != NULL)
        cIWCallBack();
    if(cIWArgCallBack != NULL)
        cIWArgCallBack(cIWArg);
}




template <class imType>
ImageModeType SliceView<imType>::imageMode(void)
{
    return cImageMode;

}




//
//
template <class imType>
void SliceView<imType>::iwCallBack(void (* newIWCallBack)(void))
{
    cIWCallBack = newIWCallBack;
}




template <class imType>
void SliceView<imType>::iwCallBack(void (* newIWArgCallBack)(void *), void * newIWArg)
{
    cIWArg = newIWArg;
    cIWArgCallBack = newIWArgCallBack;
}




//
//
//
template <class imType>
void SliceView<imType>::winZoom(float newWinZoom)
{
    cWinZoom = newWinZoom;

    if(cWinZoom<1)
        cWinZoom = 1;

    if(cWinZoom>cDimSize[cWinOrder[0]])
        cWinZoom = (float)cDimSize[cWinOrder[0]]/2;

    if(cWinZoom>cDimSize[cWinOrder[1]])
        cWinZoom = (float)cDimSize[cWinOrder[1]]/2;
}




template <class imType>
float SliceView<imType>::winZoom(void)
{
    return cWinZoom;
}




//
//
template <class imType>
void SliceView<imType>::winCenter(void)
{
    cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
    cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;

    if(cWinCenterCallBack != NULL)
        cWinCenterCallBack();
    if(cWinCenterArgCallBack != NULL)
        cWinCenterArgCallBack(cWinCenterArg);
}





//
//
template <class imType>
void SliceView<imType>::winCenter(unsigned int newWinCenterX, unsigned int newWinCenterY)
{
    cWinCenter[cWinOrder[0]] = newWinCenterX;
    if(cWinCenter[cWinOrder[0]]>=cDimSize[cWinOrder[0]])
        cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]-1;
    cWinCenter[cWinOrder[1]] = newWinCenterY;
    if(cWinCenter[cWinOrder[1]]>=cDimSize[cWinOrder[1]])
        cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]-1;

    if(cWinCenterCallBack != NULL)
        cWinCenterCallBack();
    if(cWinCenterArgCallBack != NULL)
        cWinCenterArgCallBack(cWinCenterArg);
}




template <class imType>
unsigned int SliceView<imType>::winCenterX(void)
{
    return cWinCenter[cWinOrder[0]];
}




template <class imType>
unsigned int SliceView<imType>::winCenterY(void)
{
    return cWinCenter[cWinOrder[1]];
}




template <class imType>
void SliceView<imType>::winCenterCallBack(void (* newWinCenterCallBack)(void))
{
    cWinCenterCallBack = newWinCenterCallBack;
}




template <class imType>
void SliceView<imType>::winCenterCallBack(void (* newWinCenterArgCallBack)(void *), void * newWinCenterArg)
{
    cWinCenterArg = newWinCenterArg;
    cWinCenterArgCallBack = newWinCenterArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::winShift(int newWinShiftUp, int newWinShiftRight)
{
    winCenter(cWinCenter[cWinOrder[0]]+newWinShiftRight,
	      cWinCenter[cWinOrder[1]]+newWinShiftUp);
}




//
//
//
template <class imType>
unsigned int SliceView<imType>::orientation(void)
{
    return cWinOrientation;
}




template <class imType>
void SliceView<imType>::orientation(unsigned int newOrientation)
{
    cWinOrientation = newOrientation;
    switch(cWinOrientation) {
        case 0 :
            cWinOrder[0] = 2;
            cWinOrder[1] = 1;
            cWinOrder[2] = 0;
            break;
        case 1 :
            cWinOrder[0] = 0;
            cWinOrder[1] = 2;
            cWinOrder[2] = 1;
            break;
        default:
        case 2 :
            cWinOrientation = 2;
            cWinOrder[0] = 0;
            cWinOrder[1] = 1;
            cWinOrder[2] = 2;
            break;
    }

    if(cWinOrientationCallBack != NULL)
        cWinOrientationCallBack();
    if(cWinOrientationArgCallBack != NULL)
        cWinOrientationArgCallBack(cWinOrientationArg);
}




template <class imType>
void SliceView<imType>::orientationCallBack(void (* newOrientationCallBack)(void))
{
    cWinOrientationCallBack = newOrientationCallBack;
}




template <class imType>
void SliceView<imType>::orientationCallBack(void (* newOrientationArgCallBack)(void *), void * newOrientationArg)
{
    cWinOrientationArg = newOrientationArg;
    cWinOrientationArgCallBack = newOrientationArgCallBack;
}




//
//
template <class imType>
void SliceView<imType>::size(int w, int h)
{
    cW = w;
    cH = h;
}




template <class imType>
void SliceView<imType>::resize(int x, int y, int w, int h)
{
    cX = x;
    cY = y;
    cW = w;
    cH = h;
}




template <class imType>
int SliceView<imType>::handle(int event)
{
  if( !cImData ) 
  {
    return 1;
  }

  double * spacing = (double*)cImData->GetSpacing();

  int x = Fl::event_x();
  int y = Fl::event_y();
  int button;
  int key;

  int imgShiftSize = (int)(cWinSizeX/10/cWinZoom);
  if(imgShiftSize<1)
  {
    imgShiftSize = 1;
  }

  float scale0 = (cW/(float)cDimSize[0]*cWinZoom)*spacing[cWinOrder[0]]/spacing[0];
  float scale1 = (cW/(float)cDimSize[0]*cWinZoom)*spacing[cWinOrder[1]]/spacing[0];

    switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
        button = Fl::event_button()-1;
        
        if(button <= 0)
            if(cClickMode == CM_PAN) 
                {
                int c0 = (int)(cWinMinX + (cFlipX*(cW-x) + (1-cFlipX)*x) / scale0);
                if(c0<(int)cWinMinX) c0 = cWinMinX;
                if(c0>(int)cWinMaxX) c0 = cWinMaxX;
                int c1 = (int)(cWinMinY + (cFlipY*y + (1-cFlipY)*(cH-y)) / scale1);
                if(c1<(int)cWinMinY) c1 = cWinMinY;
                if(c1>(int)cWinMaxY) c1 = cWinMaxY;
                winCenter((3*cWinCenter[cWinOrder[0]]+c0)/4, (3*cWinCenter[cWinOrder[1]]+c1)/4);
                return 1;
                }
            else if(cClickMode == CM_SELECT || cClickMode == CM_USER || cClickMode == CM_BOX) 
                {
                float p[3];
                p[cWinOrder[0]] = cWinMinX + (cFlipX*(cW-x) + (1-cFlipX)*x) / scale0;
                if(p[cWinOrder[0]]<cWinMinX) p[cWinOrder[0]] = cWinMinX;
                if(p[cWinOrder[0]]>cWinMaxX) p[cWinOrder[0]] = cWinMaxX;
                p[cWinOrder[1]] = cWinMinY + (cFlipY*y + (1-cFlipY)*(cH-y)) / scale1;
                if(p[cWinOrder[1]]<cWinMinY) p[cWinOrder[1]] = cWinMinY;
                if(p[cWinOrder[1]]>cWinMaxY) p[cWinOrder[1]] = cWinMaxY;
                if(cImageMode != IMG_MIP)
                    p[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
                else
                    p[cWinOrder[2]] = cWinZBuffer[(int)p[cWinOrder[0]]-cWinMinX + ((int)p[cWinOrder[1]]-cWinMinY)*cWinDataSizeX];
                if(cClickMode == CM_SELECT)
                     clickSelect(p[0], p[1], p[2]);
                else if(cClickMode == CM_USER)
                    clickUser(p[0], p[1], p[2]);
                else if(cClickMode == CM_BOX)
                    if(event == FL_PUSH)
                        boxMin(p[0], p[1], p[2]);
                return 1;
                }
            return 0;
            break;
    case FL_RELEASE:
            if(cClickMode == CM_BOX)
                {
                float p[3];
                p[cWinOrder[0]] = cWinMinX + (cFlipX*(cW-x) + (1-cFlipX)*x) / scale0;
                if(p[cWinOrder[0]]<cWinMinX) p[cWinOrder[0]] = cWinMinX;
                if(p[cWinOrder[0]]>cWinMaxX) p[cWinOrder[0]] = cWinMaxX;
                p[cWinOrder[1]] = cWinMinY + (cFlipY*y + (1-cFlipY)*(cH-y)) / scale1;
                if(p[cWinOrder[1]]<cWinMinY) p[cWinOrder[1]] = cWinMinY;
                if(p[cWinOrder[1]]>cWinMaxY) p[cWinOrder[1]] = cWinMaxY;
                if(cImageMode != IMG_MIP)
                    p[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
                else
                    p[cWinOrder[2]] = cWinZBuffer[(int)p[cWinOrder[0]]-cWinMinX + ((int)p[cWinOrder[1]]-cWinMinY)*cWinDataSizeX];
                boxMax(p[0], p[1], p[2]);
                return 1;
                }
            return 0;
            break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
        key = Fl::event_text()[0];
        switch(key)
        {
        case '0':
            orientation(0);
            update();
            return 1;
            break;
        case '1':
            orientation(1);
            update();
            return 1;
            break;
        case '2':
            orientation(2);
            update();
            return 1;
            break;
        case '<':
        case ',':
            if((int)cWinCenter[cWinOrder[2]]-1<0)
                return 1;
            sliceNum((int)cWinCenter[cWinOrder[2]]-1);
            update();
            return 1;
            break;
        case '>':
        case '.':
            sliceNum(cWinCenter[cWinOrder[2]]+1);
            update();
            return 1;
            break;
        case 'r':
            if( !cImData ) 
            {
              return false;
            }
            winZoom(1);
            winCenter(cDimSize[cWinOrder[0]]/2, cDimSize[cWinOrder[1]]/2);
            imageMode(IMG_VAL);
            iwMax(cDataMax);
            iwMin(cDataMin);
            update();
            return 1;
            break;
        case '+':
        case '=':
            winZoom(cWinZoom*2);
            update();
            return 1;
            break;
        case '-':
        case '_':
            winZoom(cWinZoom/2);
            update();
            return 1;
            break;
        case 'x':
            flipX(!cFlipX);
            update();
            return 1;
            break;
        case 'y':
            flipY(!cFlipY);
            update();
            return 1;
            break;
        case 'z':
            flipZ(!cFlipZ);
            update();
            return 1;
            break;
        case 'p':
            if(clickMode() == CM_PAN) {
                clickMode(CM_SELECT);
            }
            else
                if(clickMode() == CM_SELECT) {
                    clickMode(CM_USER);
                }
                else {
                    clickMode(CM_PAN);
                }
            return 1;
            break;
        case 'C':
            if(iwModeMax() == IW_FLIP) {
                iwModeMax(IW_MAX);
                update();
            }
            else {
                iwModeMax(IW_FLIP);
                update();
            }
            return 1;
            break;
        case 'c':
            if(iwModeMin() == IW_FLIP) {
                iwModeMin(IW_MIN);
                update();
            }
            else {
                iwModeMin(IW_FLIP);
                update();
            }
            return 1;
            break;
        case 'l':
			switch(cImageMode) {
				default:
				case IMG_VAL:
					imageMode(IMG_LOG);
                    update();
					break;
				case IMG_LOG:
					imageMode(IMG_DX);
                    update();
					break;
				case IMG_DX:
					imageMode(IMG_DY);
                    update();
					break;
				case IMG_DY:
					imageMode(IMG_DZ);
                    update();
					break;
				case IMG_DZ:
					imageMode(IMG_H);
                    update();
					break;
				case IMG_H:
					imageMode(IMG_MIP);
                    update();
					break;
				case IMG_MIP:
					imageMode(IMG_VAL);
                    update();
					break;
			}
            return 1;
            break;
        case 'q':
            if( !cImData )
            {
              return false;
            }
            iwMax(cIWMax-(float)0.02*(cDataMax-cDataMin));
            update();
            return 1;
            break;
        case 'w':
            if( !cImData )
            {
              return false;
            }
            iwMax(cIWMax+(float)0.02*(cDataMax-cDataMin));
            update();
            return 1;
            break;
        case 'a':
            if( !cImData )
            {
              return false;
            }
            iwMin(cIWMin-(float)0.02*(cDataMax-cDataMin));
            update();
            return 1;
            break;
        case 's':
            if( !cImData )
            {
              return false;
            }
            iwMin(cIWMin+(float)0.02*(cDataMax-cDataMin));
            update();
            return 1;
            break;
        case 'i':
            if(cFlipY)
                winCenter(cWinCenter[cWinOrder[0]], cWinCenter[cWinOrder[1]]-imgShiftSize);
            else
                winCenter(cWinCenter[cWinOrder[0]], cWinCenter[cWinOrder[1]]+imgShiftSize);
            update();
            return 1;
            break;
        case 'm':
            if(cFlipY)
                winCenter(cWinCenter[cWinOrder[0]], cWinCenter[cWinOrder[1]]+imgShiftSize);
            else
                winCenter(cWinCenter[cWinOrder[0]], cWinCenter[cWinOrder[1]]-imgShiftSize);
            update();
            return 1;
            break;
        case 'j':
            if(cFlipX)
                winCenter(cWinCenter[cWinOrder[0]]+imgShiftSize, cWinCenter[cWinOrder[1]]);
            else
                winCenter(cWinCenter[cWinOrder[0]]-imgShiftSize, cWinCenter[cWinOrder[1]]);
            update();
            return 1;
            break;
        case 'k':
            if(cFlipX)
                winCenter(cWinCenter[cWinOrder[0]]-imgShiftSize, cWinCenter[cWinOrder[1]]);
            else
                winCenter(cWinCenter[cWinOrder[0]]+imgShiftSize, cWinCenter[cWinOrder[1]]);
            update();
            return 1;
            break;
        default:
            break;
        }
    default:
        break;
    }
    
    return 0;
}




}; //namespace
#endif
