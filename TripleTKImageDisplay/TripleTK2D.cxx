#include "TripleTK2D.h"


/*
*	  Constructor:  Calls GLSliceView's constructor with fluid produced
*				          parameters that specify the location, boundaries
*				          and label of the 2D window.
*/
TripleTK2D::TripleTK2D(int x, int y, int w, int h, const char *l)
: GLSliceView<short,unsigned char>(x,y,w,h,l)
  {
  }


/*
*	  Destructor
*/
TripleTK2D::~TripleTK2D()
  {
  }


/*
*   Return the original level value;
*/
float TripleTK2D::getOriginalLevel(void)
  {
  return originalLevelValue;
  }


/*
*   Return the original window value;
*/
float TripleTK2D::getOriginalWindow(void)
  {
  return originalWindowValue;
  }


/*
*   Set the original level value;
*/
void TripleTK2D::setOriginalLevel(float level)
  {
  originalLevelValue = level;  
  }


/*
*   Set the original window value;
*/
void TripleTK2D::setOriginalWindow(float window)
  {
  originalWindowValue = window;  
  }


/*
*   Return the view overlay data boolean from GLSliceView
*/
bool TripleTK2D::getOverlayBoolean(void)
  {
  return cViewOverlayData;
  }


