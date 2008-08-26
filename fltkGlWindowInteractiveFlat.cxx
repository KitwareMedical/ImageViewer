/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractiveFlat.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <fltkGlWindowInteractiveFlat.h>


namespace fltk {

//------------------------------------------
//
//    Creator
//
//------------------------------------------
GlWindowInteractiveFlat
::GlWindowInteractiveFlat(int lx,int ly,int lw,int lh, const char * llabel)
  :GlWindowInteractive(lx,ly,lw,lh,llabel) 
{

}






//------------------------------------------
//
//    Destructor
//
//------------------------------------------
GlWindowInteractiveFlat
::~GlWindowInteractiveFlat() 
{

}




//------------------------------------------
//
//    Set viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractiveFlat
::SetViewPoint( void ) 
{
  glScaled( m_Zoom, m_Zoom, m_Zoom );
  glTranslated( m_Center[0], m_Center[1], m_Center[2] );
}





} // end namespace fltk
