/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshApplication.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "FEMMeshApplication.h"

FEMMeshApplication
::FEMMeshApplication()
{
 
}




FEMMeshApplication
::~FEMMeshApplication()
{

}



void
FEMMeshApplication
::CreateSphere(void)
{
  this->FEMMeshApplicationBase::CreateSphere();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}





void
FEMMeshApplication
::CreateTriangle(void)
{
  this->FEMMeshApplicationBase::CreateTriangle();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}





