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


#include <liCommandUpdateSpineModelGUI.h>
#include <liShape3D.h>
#include <FL/Fl.h>

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
CommandUpdateSpineModelGUI::Execute( itk::LightObject * unit, unsigned long event )
{

  if( event == li::SpineChangedEvent )
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
CommandUpdateSpineModelGUI::Execute(const itk::LightObject * unit, unsigned long event )
{

  if( event == FluoroscopyUnitMovedEvent )
  {
    if( m_SpineModelGUI )
    {
      m_SpineModelGUI->UpdateGUI();
    }
  }
}




} // end namespace li


