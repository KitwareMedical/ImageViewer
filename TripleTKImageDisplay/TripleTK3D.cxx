#include "TripleTK3D.h"


/*
*	  Constructor:  Calls FLVTKVolRenWin's constructor with fluid produced
*				          parameters that specify the location, boundaries
*				          and label of the 3D window.
*/
TripleTK3D::TripleTK3D(int x, int y, int w, int h, const char *l) 
: FLVTKVolRenWin<short>(x,y,w,h,l)
  {
  }


/*
*	  Deconstuctor
*/
TripleTK3D::~TripleTK3D()
  {
  }


/*
*   Centers the planes in the vtk window.
*/
void TripleTK3D::centerPlanes(void)
  {
  mRenderer->ResetCamera();
  }

