/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractiveFlat.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <fltkGlWindowInteractiveFlat.h>


namespace fltk {

//------------------------------------------
//
//    Creator
//
//------------------------------------------
GlWindowInteractiveFlat
::GlWindowInteractiveFlat(int x,int y,int w,int h, const char * label)
  :GlWindowInteractive(x,y,w,h,label) 
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
