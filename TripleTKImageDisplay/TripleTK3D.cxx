#include "TripleTK3D.h"


//----------------------------------------------------------------------
TripleTK3D::TripleTK3D(int x, int y, int w, int h, const char *l) 
: FLVTKVolRenWin<short>(x,y,w,h,l)
  {
  }


//----------------------------------------------------------------------
TripleTK3D::~TripleTK3D()
  {
  }


//----------------------------------------------------------------------
void TripleTK3D::centerPlanes(void)
  {
  mRenderer->ResetCamera();
  }

