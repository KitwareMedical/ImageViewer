/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMesh.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "FEMMesh.h"

FEMMesh
::FEMMesh()
{
 
}




FEMMesh
::~FEMMesh()
{

}



void
FEMMesh
::CreateSphere(void)
{
  this->FEMMeshBase::CreateSphere();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}





void
FEMMesh
::CreateTriangle(void)
{
  this->FEMMeshBase::CreateTriangle();
  m_FlRenderWindowInteractor->redraw();
  Fl::check();
}





