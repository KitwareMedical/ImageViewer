/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateProgress.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


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


