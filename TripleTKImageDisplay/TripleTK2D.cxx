/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTK2D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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


