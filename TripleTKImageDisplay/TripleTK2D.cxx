#include "TripleTK2D.h"


//----------------------------------------------------------------------
TripleTK2D::TripleTK2D(int x, int y, int w, int h, const char *l)
: GLSliceView<short,unsigned char>(x,y,w,h,l)
  {
  }

//----------------------------------------------------------------------
TripleTK2D::~TripleTK2D()
  {
  }


//----------------------------------------------------------------------
float TripleTK2D::getOriginalLevel(void)
  {
  return originalLevelValue;
  }


//----------------------------------------------------------------------
float TripleTK2D::getOriginalWindow(void)
  {
  return originalWindowValue;
  }


//----------------------------------------------------------------------
void TripleTK2D::setOriginalLevel(float level)
  {
  originalLevelValue = level;  
  }


//----------------------------------------------------------------------
void TripleTK2D::setOriginalWindow(float window)
  {
  originalWindowValue = window;  
  }


//----------------------------------------------------------------------
bool TripleTK2D::getOverlayBoolean(void)
  {
  return cViewOverlayData;
  }


