/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateImageIntensifier.cxx
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
#include "liCommandUpdateImageIntensifier.h"
#include <FL/Fl.H>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandUpdateImageIntensifier::CommandUpdateImageIntensifier(void)  
{
  m_FluoroscopyUnitView  = 0;
}


//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandUpdateImageIntensifier::SetFluoroscopyUnitView( 
    FluoroscopyUnitView * unit )  
{
  m_FluoroscopyUnitView = unit;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateImageIntensifier::Execute( 
                         itk::Object * unit, 
                         const itk::EventObject & event )
{
  if( typeid( event ) == typeid( FluoroscopyUnitMovedEvent ) )
  {
    if( m_FluoroscopyUnitView )
    {

      FluoroscopyCArmMobile::Pointer  fluoroscopyCArmMobile = 
                        static_cast< FluoroscopyCArmMobile *>( unit );

      m_FluoroscopyUnitView->SetSourcePosition( 
                    fluoroscopyCArmMobile->GetSourcePosition() );

      m_FluoroscopyUnitView->SetDetectorPosition( 
                    fluoroscopyCArmMobile->GetDetectorPosition() );

      m_FluoroscopyUnitView->SetDetectorRadius( 
                    fluoroscopyCArmMobile->GetDetectorRadius() );

      m_FluoroscopyUnitView->SetDetectorVerticalDirection( 
                    fluoroscopyCArmMobile->GetDetectorVerticalDirection() );

    }
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateImageIntensifier::Execute(
                      const itk::Object * unit, 
                      const itk::EventObject & event )
{

  if( typeid( event ) == typeid( FluoroscopyUnitMovedEvent ) )
  {
    if( m_FluoroscopyUnitView )
    {

      FluoroscopyCArmMobile::ConstPointer  fluoroscopyCArmMobile = 
                        static_cast<const FluoroscopyCArmMobile *>( unit );

      m_FluoroscopyUnitView->SetSourcePosition( 
                    fluoroscopyCArmMobile->GetSourcePosition() );

      m_FluoroscopyUnitView->SetDetectorPosition( 
                    fluoroscopyCArmMobile->GetDetectorPosition() );
      
      m_FluoroscopyUnitView->SetDetectorRadius( 
                    fluoroscopyCArmMobile->GetDetectorRadius() );

      m_FluoroscopyUnitView->SetDetectorVerticalDirection( 
                    fluoroscopyCArmMobile->GetDetectorVerticalDirection() );

    }
  }
}




} // end namespace li


