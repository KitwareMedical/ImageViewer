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


#include <liCommandUpdateRegistratorFieldOfView.h>
#include <liShape3D.h>
#include <FL/Fl.h>

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
CommandUpdateRegistratorFieldOfView::Execute( itk::LightObject * unit, unsigned long event )
{

  if( event == FieldOfViewChangedEvent )
  {
    if( m_Registrator )
    {

      FluoroscopyCArmMobile::Pointer  fluoroscopyCArmMobile = 
                        static_cast< FluoroscopyCArmMobile *>( unit );

      m_Registrator->GetRegistrator()->GetMetric()
        ->GetMapper()->GetTransform()->GetTransform().SetFocalDistance(  
                    fluoroscopyCArmMobile->GetSourceToFilmDistance() );

      m_Registrator->GetRegistrator()->GetMetric()
        ->GetMapper()->GetTransform()->GetTransform().SetHeight(  
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0 );

    }
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateRegistratorFieldOfView::Execute(const itk::LightObject * unit, unsigned long event )
{

  if( event == FieldOfViewChangedEvent )
  {
    if( m_Registrator )
    {

      FluoroscopyCArmMobile::ConstPointer  fluoroscopyCArmMobile = 
                        static_cast<const FluoroscopyCArmMobile *>( unit );

      m_Registrator->GetRegistrator()->GetMetric()
        ->GetMapper()->GetTransform()->GetTransform().SetFocalDistance(  
                    fluoroscopyCArmMobile->GetSourceToFilmDistance() );

      m_Registrator->GetRegistrator()->GetMetric()
        ->GetMapper()->GetTransform()->GetTransform().SetHeight(  
                    fluoroscopyCArmMobile->GetDetectorRadius() * 2.0 );

    }
  }
}




} // end namespace li


