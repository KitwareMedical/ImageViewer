/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractiveFlat.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef fltkGLWINDOWINTERFLATCLASS
#define fltkGLWINDOWINTERFLATCLASS

#include <fltkGlWindowInteractive.h>

namespace fltk {

class GlWindowInteractiveFlat: public GlWindowInteractive {
public:
  GlWindowInteractiveFlat(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindowInteractiveFlat();
  virtual void SetViewPoint(void);
  };


} // end namespace fltk
 
#endif   
