/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkProgressBarRedrawCommand.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <fltkProgressBarRedrawCommand.h>
#include <itkProcessObject.h>


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
ProgressBarRedrawCommand::ProgressBarRedrawCommand(void)  
{
}



//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
ProgressBarRedrawCommand
::Execute( itk::LightObject * caller, unsigned long event )
{

  if( event == ProgressEvent )
  {

    Fl_Slider * progressBar = GetWidget();
    
    if( progressBar )
    {

      itk::ProcessObject::Pointer  process = 
                        dynamic_cast< itk::ProcessObject *>( caller );

      progressBar->value( process->GetProgress() );
      Fl::check();
    }

  }

}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
ProgressBarRedrawCommand
::Execute( const itk::LightObject * caller, unsigned long event )
{

  if( event == ProgressEvent )
  {

    Fl_Slider * progressBar = GetWidget();
    
    if( progressBar )
    {

      itk::ProcessObject::ConstPointer  process = 
                        dynamic_cast<const itk::ProcessObject *>( caller );

      progressBar->value( process->GetProgress() );
      Fl::check();
    }

  }

}





} // end namespace li


