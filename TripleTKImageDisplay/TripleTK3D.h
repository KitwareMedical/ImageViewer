/*===============================================================
Summer 2001

  This class is an abstraction of the class FLVTKVolRenWin.  It is
  derived of this class for the purposes of being used with 
  the TripleTKGUI structure.  This class will allow the viewing of
  image data in 3D through its FLVTKVolRenWin inheritance and allows
  for any necessary additions to FLVTKVolRenWin without actually 
  having to manipulate FLVTKVolRenWin.

  Author: Andrew MacKelfresh
  CADDLab
  UNC-CH
===============================================================*/
#ifndef _TRIPLETK3D_H
#define _TRIPLETK3D_H

#include "3D/FLVTKVolRenWin.h"


/*
*	  Class Pan3D derived from class FLVTKVolRenWin
*/
class TripleTK3D : public FLVTKVolRenWin<short>
  {

  public:
        
	  TripleTK3D(int x, int y, int w, int h, const char *l);
	  ~TripleTK3D(void);
    void centerPlanes(void);

  };


#endif

