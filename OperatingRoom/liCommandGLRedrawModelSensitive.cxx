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


#include <liShape3D.h>
#include <liCommandGLRedrawModelSensitive.h>
#include <FL/Fl.h>

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
CommandGLRedrawModelSensitive::SetModelShape( Shape3D * model )  
{
  m_ModelShape  = model;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModelSensitive::Execute( itk::LightObject *, unsigned long )
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
CommandGLRedrawModelSensitive::Execute(const itk::LightObject * , unsigned long )
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


