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


