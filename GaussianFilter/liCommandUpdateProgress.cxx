//--------------------------------------------
//
//     Project:   Gaussian Filter
//
//     Author:    Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liCommandUpdateProgress.h>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandUpdateProgress::CommandUpdateProgress(void)  
{
  m_Console  = 0;
}


//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
CommandUpdateProgress::Pointer 
CommandUpdateProgress::New( void ) 
{
  Self * ret = ::itk::ObjectFactory< Self >::Create();
  if( ret )
  {
    return ret;
  }
  return new Self;

}



//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandUpdateProgress::SetConsole( liFilterConsoleBase * console )  
{
  m_Console = console;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateProgress::Execute( itk::LightObject * caller, unsigned long event )
{

  if( event == ProgressEvent )
  {
    if( m_Console )
    {

      itk::ProcessObject::Pointer  process = 
                        dynamic_cast< itk::ProcessObject *>( caller );

      m_Console->ShowProgress( process->GetProgress() );

    }
  }
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateProgress::Execute( const itk::LightObject * caller, unsigned long event )
{

  if( event == ProgressEvent )
  {
    if( m_Console )
    {

      itk::ProcessObject::ConstPointer  process = 
                        dynamic_cast< const itk::ProcessObject *>( caller );

      m_Console->ShowProgress( process->GetProgress() );

    }
  }
}





} // end namespace li


