/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandGLRedrawModelFixed.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "fltkShape3D.h"
#include "liCommandGLRedrawModelFixed.h"
#include <FL/Fl.H>

namespace li {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandGLRedrawModelFixed::CommandGLRedrawModelFixed(void)  
{
}


//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandGLRedrawModelFixed::SetModelShape( fltk::Shape3D * model )  
{
  m_ModelShape  = model;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModelFixed::Execute( itk::Object *, const itk::EventObject & )
{
  if( m_ModelShape )
  {
    m_ModelShape->SetAutoSensing( false );
    m_ModelShape->SetRestoreTransform( true );
    m_ModelShape->glDraw();
    m_ModelShape->SetRestoreTransform( false );
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModelFixed::Execute(const itk::Object * , const itk::EventObject & )
{
  if( m_ModelShape )
  {
    m_ModelShape->SetAutoSensing( false );
    m_ModelShape->SetRestoreTransform( true );
    m_ModelShape->glDraw();
    m_ModelShape->SetRestoreTransform( false );
  }
}

 


} // end namespace li


