/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <liImageRegistrationConsole.h>
#include <Fl/fl_file_chooser.H>
#include <liCommandUpdateProgress.h>
#include <liCommandUpdateLightButton.h>
#include <itkFileIOMetaImage.h>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
liImageRegistrationConsole
::liImageRegistrationConsole()
{
 
  FileIOMetaImageFactory * factory = new FileIOMetaImageFactory;
  ObjectFactoryBase * factoryBase = static_cast<ObjectFactoryBase *>( factory );
  ObjectFactoryBase::RegisterFactory( factoryBase );
  
  typedef li::CommandUpdateLightButton  CommandButton;

  this->m_InputViewer           = new InputImageViewerType;
  this->m_ReferenceViewer       = new ImageViewerType;
  this->m_MappedReferenceViewer = new ImageViewerType;

  this->m_InputViewer->SetLabel( "Target Image" );
  this->m_ReferenceViewer->SetLabel( "Reference Image" );
  this->m_MappedReferenceViewer->SetLabel( "Mapped Reference Image" );

  li::CommandUpdateProgress::Pointer commandUpdateProgress =
                              li::CommandUpdateProgress::New();

  commandUpdateProgress->SetConsole( this );

  m_Reader->AddObserver( itk::Command::StartEvent, targetButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::EndEvent, targetButton->GetCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, targetButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, targetButton->GetCommand().GetPointer() );
  
  ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liImageRegistrationConsole
::~liImageRegistrationConsole()
{

  if( this->m_InputViewer ) 
  {
    delete this->m_InputViewer;
    this->m_InputViewer = 0;
  }

  if( this->m_ReferenceViewer ) 
  {
    delete this->m_ReferenceViewer;
    this->m_ReferenceViewer = 0;
  }

  if( this->m_MappedReferenceViewer ) 
  {
    delete this->m_MappedReferenceViewer;
    this->m_MappedReferenceViewer = 0;
  }


}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liImageRegistrationConsole
::Load( void )
{

  const char * filename = fl_file_chooser("Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Loading image file...");
  
  try 
  {
    liImageRegistrationConsoleBase::Load( filename );
  }
  catch( ... ) 
  {
    ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  ShowStatus("File Loaded");

  controlsGroup->activate();

}

   
/************************************
 *
 *  Show
 *
 ***********************************/
void
liImageRegistrationConsole
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
liImageRegistrationConsole
::Hide( void )
{

  consoleWindow->hide();
  this->m_InputViewer->Hide();
  this->m_ReferenceViewer->Hide();
  this->m_MappedReferenceViewer->Hide();
  aboutWindow->hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
liImageRegistrationConsole
::Quit( void )
{
  Hide();
}





 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowProgress( float fraction )
{
  liImageRegistrationConsoleBase::ShowProgress( fraction );
  progressSlider->value( fraction );
  Fl::check();
}




 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowStatus( const char * message )
{
  liImageRegistrationConsoleBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Target Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowTarget( void )
{

  if( !(this->m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  this->m_InputViewer->SetImage( this->m_Reader->GetOutput() );  
  this->m_InputViewer->Show();

}


 
/************************************
 *
 *  Show Reference Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowReference( void )
{

  if( !(this->m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  this->m_ReferenceViewer->SetImage( this->m_ReferenceImage );  
  this->m_ReferenceViewer->Show();

}



 
/************************************
 *
 *  Show Mapped Reference Image
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowMappedReference( void )
{

  if( !(this->m_ImageLoaded ) )
  {
    ShowStatus("Please load an image first");
    return;
  }

  this->m_MappedReferenceViewer->SetImage( this->m_MappedReferenceImage );
  this->m_MappedReferenceViewer->Show();

}




 
/************************************
 *
 *  Show About Window
 *
 ***********************************/
void
liImageRegistrationConsole
::ShowAbout( void )
{

  aboutWindow->show();

}






 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
liImageRegistrationConsole
::Execute( void )
{

  if( ! (this->m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }


  ShowStatus("Registering Reference against Target ...");

  liImageRegistrationConsoleBase::Execute();


  ShowStatus("Registration done ");
  
}




 
/************************************
 *
 *  Update the parameters of the 
 *  Transformation
 *
 ***********************************/
void
liImageRegistrationConsole
::UpdateTransformationParameters( void )
{

  ParametersType transformationParameters;

  transformationParameters[0] = xTranslation->value();
  transformationParameters[1] = yTranslation->value();
  transformationParameters[2] = zTranslation->value();

  this->m_ImageMapper->GetTransformation()->SetParameters(
                                              transformationParameters);


}




 
/************************************
 *
 *  Generate Reference Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateReference( void )
{
  
  referenceButton->value( 0 );
  referenceButton->redraw();
  
  liImageRegistrationConsoleBase::GenerateReference();
  
  referenceButton->value( 1 );
  referenceButton->redraw();

}




 
/************************************
 *
 *  Generate Mapped Reference Image
 *  Modify button colors and then
 *  delegate to base class
 *
 ***********************************/
void
liImageRegistrationConsole
::GenerateMappedReference( void )
{
  
  mappedReferenceButton->value( 0 );
  mappedReferenceButton->redraw();
  
  liImageRegistrationConsoleBase::GenerateMappedReference();
  
  mappedReferenceButton->value( 1 );
  mappedReferenceButton->redraw();

}

