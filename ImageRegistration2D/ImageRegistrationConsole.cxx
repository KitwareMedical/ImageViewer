/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistrationConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <ImageRegistrationConsole.h>
#include <FL/fl_file_chooser.H>
 



/************************************
 *
 *  Constructor
 *
 ***********************************/
ImageRegistrationConsole
::ImageRegistrationConsole()
{
 
  m_FixedImageViewer  = ImageViewerType::New();
  m_MovingImageViewer = ImageViewerType::New();

  m_RegisteredMovingImageViewer = ImageViewerType::New();
 
  m_MovingImageViewer->SetLabel( "Moving Image" );
  m_FixedImageViewer->SetLabel( "Fixed Image" );
  m_RegisteredMovingImageViewer->SetLabel( "Registered Moving Image" );

  progressSlider->Observe( m_ResampleMovingImageFilter.GetPointer() );

  fixedImageButton->Observe(        m_FixedImageReader.GetPointer()  );
  loadFixedImageButton->Observe(    m_FixedImageReader.GetPointer()  );
  movingImageButton->Observe(       m_MovingImageReader.GetPointer() );
  loadMovingImageButton->Observe(   m_MovingImageReader.GetPointer() );

  this->ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
ImageRegistrationConsole
::~ImageRegistrationConsole()
{

}



 
/************************************
 *
 *  Load Moving Image
 *
 ***********************************/
void
ImageRegistrationConsole
::LoadMovingImage( void )
{

  const char * filename = fl_file_chooser("Moving Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading moving image file...");
  
  try 
  {
    ImageRegistrationConsoleBase::LoadMovingImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("Moving Image Loaded");

  controlsGroup->activate();

}


 
/************************************
 *
 *  Load Fixed Image
 *
 ***********************************/
void
ImageRegistrationConsole
::LoadFixedImage( void )
{

  const char * filename = fl_file_chooser("Fixed Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading fixed image file...");
  
  try 
  {
    ImageRegistrationConsoleBase::LoadFixedImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("Fixed Image Loaded");

  controlsGroup->activate();

}

   


 
/************************************
 *
 *  Save Moving Image
 *
 ***********************************/
void
ImageRegistrationConsole
::SaveRegisteredMovingImage( void )
{

  const char * filename = fl_file_chooser("Moving Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving moving image file...");
  
  try 
  {
    ImageRegistrationConsoleBase::SaveRegisteredMovingImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems saving file format");
    return;
  }

  this->ShowStatus("Moving Image Saved");

}





/************************************
 *
 *  Show
 *
 ***********************************/
void
ImageRegistrationConsole
::Show( void )
{
  consoleWindow->show();
}





/************************************
 *
 *  Hide
 *
 ***********************************/
void
ImageRegistrationConsole
::Hide( void )
{
  consoleWindow->hide();
  m_FixedImageViewer->Hide();
  m_MovingImageViewer->Hide();
  m_RegisteredMovingImageViewer->Hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
ImageRegistrationConsole
::Quit( void )
{
  this->Hide();
}






 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
ImageRegistrationConsole
::ShowStatus( const char * message )
{
  ImageRegistrationConsoleBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Fixed Image
 *
 ***********************************/
void
ImageRegistrationConsole
::ShowFixedImage( void )
{

  if( !m_FixedImageIsLoaded )
    {
    return;
    }

  m_FixedImageViewer->SetImage( m_FixedImageReader->GetOutput() );  
  m_FixedImageViewer->Show();
}



 
/************************************
 *
 *  Show Moving Image
 *
 ***********************************/
void
ImageRegistrationConsole
::ShowMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  m_MovingImageViewer->SetImage( m_MovingImageReader->GetOutput() );  
  m_MovingImageViewer->Show();

}



 
/************************************
 *
 *  Show Registered Moving Image
 *
 ***********************************/
void
ImageRegistrationConsole
::ShowRegisteredMovingImage( void )
{

  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  m_RegisteredMovingImageViewer->SetImage( m_ResampleMovingImageFilter->GetOutput() );
  m_RegisteredMovingImageViewer->Show();
}







 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
ImageRegistrationConsole
::Execute( void )
{

  this->ShowStatus("Registering Moving Image against Fixed Image ...");

  ImageRegistrationConsoleBase::Execute();

  this->ShowStatus("Registration done ");
  
}




/************************************
 *
 *  Generate Registered Moving Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
ImageRegistrationConsole
::GenerateRegisteredMovingImage( void )
{
  
  if( !m_MovingImageIsLoaded )
    {
    return;
    }

  registeredMovingImageButton->selection_color( FL_RED );
  registeredMovingImageButton->value( 1 );
  registeredMovingImageButton->redraw();
  
  ImageRegistrationConsoleBase::GenerateRegisteredMovingImage();
  
  registeredMovingImageButton->selection_color( FL_GREEN );
  registeredMovingImageButton->value( 1 );
  registeredMovingImageButton->redraw();

}

