/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateRegistratorFieldOfView.cxx
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
#include "liCommandUpdateRegistratorFieldOfView.h"
#include <FL/Fl.H>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandUpdateRegistratorFieldOfView::CommandUpdateRegistratorFieldOfView(void)  
{
  m_Registrator  = 0;
}


//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandUpdateRegistratorFieldOfView::SetRegistrator( RegistratorType * unit )
{
  m_Registrator = unit;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateRegistratorFieldOfView::Execute( 
                            itk::Object * unit, 
                            const itk::EventObject & event )
{

  if( typeid( event ) == typeid( FieldOfViewChangedEvent ) )
  {
    if( m_Registrator )
    {

      FluoroscopyCArmMobile::Pointer  fluoroscopyCArmMobile = 
                        static_cast< FluoroscopyCArmMobile *>( unit );

      const double focalDistance = 
                    fluoroscopyCArmMobile->GetSourceToFilmDistance();

      const double height = 
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0;

      const double width = 
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0;
      
      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
              ->GetTransform()->SetFocalDistance( focalDistance ); 

      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
                      ->GetTransform()->SetHeight( height ); 

      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
                      ->GetTransform()->SetWidth( width ); 



    }
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateRegistratorFieldOfView::Execute(
                            const itk::Object * unit, 
                            const itk::EventObject &  event )
{

  if( typeid( event ) == typeid( FieldOfViewChangedEvent ) )
  {
    if( m_Registrator )
    {

      FluoroscopyCArmMobile::ConstPointer  fluoroscopyCArmMobile = 
                        static_cast<const FluoroscopyCArmMobile *>( unit );


      const double focalDistance = 
                    fluoroscopyCArmMobile->GetSourceToFilmDistance();

      const double height = 
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0;

      const double width = 
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0;

      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
              ->GetTransform()->SetFocalDistance( focalDistance ); 

      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
                      ->GetTransform()->SetHeight( height ); 

      m_Registrator->GetRegistrator()->GetMetric()->GetMapper()
                      ->GetTransform()->SetWidth( width ); 

    }
  }
}




} // end namespace li


