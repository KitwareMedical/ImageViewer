/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateSpineModelGUI.cxx
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
#include "liCommandUpdateSpineModelGUI.h"
#include <FL/Fl.H>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandUpdateSpineModelGUI::CommandUpdateSpineModelGUI(void)  
{
  m_SpineModelGUI  = 0;
}


//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandUpdateSpineModelGUI::SetSpineModelGUI( 
    SpineModelGUI * gui )  
{
  m_SpineModelGUI = gui;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateSpineModelGUI::Execute( 
                    itk::Object * unit, 
                    const itk::EventObject & event )
{

  if( typeid( event ) == typeid( li::SpineChangedEvent ) )
  {

    if( m_SpineModelGUI )
    {
      m_SpineModelGUI->UpdateGUI();
    }
    
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateSpineModelGUI::Execute(
                  const itk::Object * unit,
                  const itk::EventObject & event )
{

  if( typeid( event ) == typeid( FluoroscopyUnitMovedEvent ) )
  {
    if( m_SpineModelGUI )
    {
      m_SpineModelGUI->UpdateGUI();
    }
  }
}




} // end namespace li


