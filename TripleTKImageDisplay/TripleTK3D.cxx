/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTK3D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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

