/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlDrawer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef fltkGLDRAWERCLASS
#define fltkGLDRAWERCLASS


namespace fltk {

class GlDrawer 
{

public:
  GlDrawer() {};
  virtual ~GlDrawer() {};
  virtual void glDraw(void) const =0;
};

typedef GlDrawer * GlDrawerPtr;

}


#endif
