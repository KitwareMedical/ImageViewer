/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractiveFlat.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
