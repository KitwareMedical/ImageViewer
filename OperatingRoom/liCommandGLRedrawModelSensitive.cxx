/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandGLRedrawModelSensitive.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project:   Operating Room
//
//     Author:    Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include "fltkShape3D.h"
#include "liCommandGLRedrawModelSensitive.h"
#include <FL/Fl.H>

namespace li {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandGLRedrawModelSensitive::CommandGLRedrawModelSensitive(void)  
{
}


//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandGLRedrawModelSensitive::SetModelShape( fltk::Shape3D * model )  
{
  m_ModelShape  = model;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModelSensitive::Execute( itk::Object *, const itk::EventObject & )
{
  if( m_ModelShape )
  {
    m_ModelShape->SetAutoSensing( true );
    m_ModelShape->SetRestoreTransform( false );
    m_ModelShape->glDraw();
    m_ModelShape->SetAutoSensing( false );
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModelSensitive::Execute(const itk::Object * , const itk::EventObject & )
{
  if( m_ModelShape )
  {
    m_ModelShape->SetAutoSensing( true );
    m_ModelShape->SetRestoreTransform( false );
    m_ModelShape->glDraw();
    m_ModelShape->SetAutoSensing( false );
  }
}

 


} // end namespace li


