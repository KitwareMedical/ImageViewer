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


#include <liCommandGLRedrawModel.h>
#include <FL/Fl.h>
#include <liShape3D.h>

namespace li {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandGLRedrawModel::CommandGLRedrawModel(void)  
{
  m_Model  = 0;
}



//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandGLRedrawModel::SetModel( fltk::GlDrawer * model )  
{
  m_Model  = model;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModel::Execute( itk::LightObject *, unsigned long )
{
  if( m_Model )
  {
    m_Model->glDraw();
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandGLRedrawModel::Execute(const itk::LightObject * , unsigned long )
{
  if( m_Model )
  {
    m_Model->glDraw();
  }
}

 


} // end namespace li


