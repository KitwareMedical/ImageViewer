/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <liFilterConsole.h>
#include <Fl/fl_file_chooser.H>
#include <liCommandUpdateProgress.h>
#include <liCommandUpdateLightButton.h>
#include <itkFileIOMetaImage.h>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
liFilterConsole
::liFilterConsole()
{
 
  FileIOMetaImageFactory * factory = new FileIOMetaImageFactory;
  ObjectFactoryBase * factoryBase = static_cast<ObjectFactoryBase *>( factory );
  ObjectFactoryBase::RegisterFactory( factoryBase );
  
  typedef li::CommandUpdateLightButton  CommandButton;

  this->m_InputViewer = new InputImageViewerType;

  this->m_Viewer_H1x  = new ImageViewerType;
  this->m_Viewer_H1y  = new ImageViewerType;
  this->m_Viewer_H1z  = new ImageViewerType;

  this->m_Viewer_H2x  = new ImageViewerType;
  this->m_Viewer_H2y  = new ImageViewerType;
  this->m_Viewer_H2z  = new ImageViewerType;

  this->m_Viewer_Laplacian = new ImageViewerType;

  this->m_Viewer_Gradient_Modulus = new ImageViewerType;

  this->m_InputViewer->SetLabel( "Input Image" );

  this->m_Viewer_H1x->SetLabel( "Gradient X" );
  this->m_Viewer_H1y->SetLabel( "Gradient Y" );
  this->m_Viewer_H1z->SetLabel( "Gradient Z" );

  this->m_Viewer_H2x->SetLabel( "Second Derivative X" );
  this->m_Viewer_H2y->SetLabel( "Second Derivative Y" );
  this->m_Viewer_H2z->SetLabel( "Second Derivative Z" );

  this->m_Viewer_Laplacian->SetLabel( "Laplacian" );
  
  this->m_Viewer_Gradient_Modulus->SetLabel( "Gradient Modulus" );

  li::CommandUpdateProgress::Pointer commandUpdateProgress =
                              li::CommandUpdateProgress::New();

  commandUpdateProgress->SetConsole( this );


  m_H1x->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_H1y->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_H1z->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_H2x->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_H2y->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_H2z->AddObserver( itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hx->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hy->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hz->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hxy->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hyz->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Hzx->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Add->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
  m_Modulus->AddObserver(  itk::Command::ProgressEvent, commandUpdateProgress.GetPointer() );
                              
  m_Reader->AddObserver( itk::Command::StartEvent, inputButton->GetCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::StartEvent, HxButton->GetCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::StartEvent, HyButton->GetCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::StartEvent, HzButton->GetCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::StartEvent, HxyButton->GetCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::StartEvent, HyzButton->GetCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::StartEvent, HzxButton->GetCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::StartEvent, H1xButton->GetCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::StartEvent, H1yButton->GetCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::StartEvent, H1zButton->GetCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::StartEvent, H2xButton->GetCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::StartEvent, H2yButton->GetCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::StartEvent, H2zButton->GetCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::StartEvent, laplacianButton->GetCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::StartEvent, modulusButton->GetCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::EndEvent, inputButton->GetCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::EndEvent, HxButton->GetCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::EndEvent, HyButton->GetCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::EndEvent, HzButton->GetCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::EndEvent, HxyButton->GetCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::EndEvent, HyzButton->GetCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::EndEvent, HzxButton->GetCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::EndEvent, H1xButton->GetCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::EndEvent, H1yButton->GetCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::EndEvent, H1zButton->GetCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::EndEvent, H2xButton->GetCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::EndEvent, H2yButton->GetCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::EndEvent, H2zButton->GetCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::EndEvent, laplacianButton->GetCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::EndEvent, modulusButton->GetCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, inputButton->GetCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::ModifiedEvent, HxButton->GetCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::ModifiedEvent, HyButton->GetCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::ModifiedEvent, HzButton->GetCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::ModifiedEvent, HyzButton->GetCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::ModifiedEvent, HzxButton->GetCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::ModifiedEvent, H1zButton->GetCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::ModifiedEvent, H2zButton->GetCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, inputButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HyButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HzButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HyzButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HzxButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1zButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2zButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetCommand().GetPointer() );

  ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsole
::~liFilterConsole()
{

  if( this->m_Viewer_H1x ) 
  {
    delete this->m_Viewer_H1x;
    this->m_Viewer_H1x = 0;
  }

  if( this->m_Viewer_H1y ) 
  {
    delete this->m_Viewer_H1y;
    this->m_Viewer_H1y = 0;
  }

  if( this->m_Viewer_H1z ) 
  {
    delete this->m_Viewer_H1z;
    this->m_Viewer_H1z = 0;
  }

  if( this->m_Viewer_H2x ) 
  {
    delete this->m_Viewer_H2x;
    this->m_Viewer_H2x = 0;
  }

  if( this->m_Viewer_H2y ) 
  {
    delete this->m_Viewer_H2y;
    this->m_Viewer_H2y = 0;
  }

  if( this->m_Viewer_H2z ) 
  {
    delete this->m_Viewer_H2z;
    this->m_Viewer_H2z = 0;
  }


  if( this->m_InputViewer ) 
  {
    delete this->m_InputViewer;
    this->m_InputViewer = 0;
  }

  if( this->m_Viewer_Laplacian ) 
  {
    delete this->m_Viewer_Laplacian;
    this->m_Viewer_Laplacian = 0;
  }

  if( this->m_Viewer_Gradient_Modulus ) 
  {
    delete this->m_Viewer_Gradient_Modulus;
    this->m_Viewer_Gradient_Modulus = 0;
  }


}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liFilterConsole
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
    liFilterConsoleBase::Load( filename );
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
liFilterConsole
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
liFilterConsole
::Hide( void )
{

  consoleWindow->hide();
  this->m_Viewer_H1x->Hide();
  this->m_Viewer_H1y->Hide();
  this->m_Viewer_H1z->Hide();
  this->m_Viewer_H2x->Hide();
  this->m_Viewer_H2y->Hide();
  this->m_Viewer_H2z->Hide();
  this->m_InputViewer->Hide();
  this->m_Viewer_Laplacian->Hide();
  this->m_Viewer_Gradient_Modulus->Hide();
  aboutWindow->hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
liFilterConsole
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
liFilterConsole
::ShowProgress( float fraction )
{
  liFilterConsoleBase::ShowProgress( fraction );
  progressSlider->value( fraction );
  Fl::check();
}




 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liFilterConsole
::ShowStatus( const char * message )
{
  liFilterConsoleBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Input Image
 *
 ***********************************/
void
liFilterConsole
::ShowInput( void )
{

  if( ! (this->m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }

  this->m_InputViewer->SetImage( this->m_Reader->GetOutput() );  
  this->m_InputViewer->Show();

}



 
/************************************
 *
 *  Show Filtered Image
 *
 ***********************************/
void
liFilterConsole
::ShowFilteredX( void )
{

  this->m_H1x->Update();
  this->m_Viewer_H1x->SetImage( this->m_H1x->GetOutput() );  
  this->m_Viewer_H1x->Show();

}


 
/************************************
 *
 *  Show Filtered Image
 *
 ***********************************/
void
liFilterConsole
::ShowFilteredY( void )
{

  this->m_H1y->Update(); 
  this->m_Viewer_H1y->SetImage( this->m_H1y->GetOutput() );  
  this->m_Viewer_H1y->Show();

}



 
/************************************
 *
 *  Show Filtered Image
 *
 ***********************************/
void
liFilterConsole
::ShowFilteredZ( void )
{

  this->m_H1z->Update();
  this->m_Viewer_H1z->SetImage( this->m_H1z->GetOutput() );  
  this->m_Viewer_H1z->Show();

}


 
/************************************
 *
 *  Show Second Derivative X
 *
 ***********************************/
void
liFilterConsole
::ShowSecondDerivativeX( void )
{

  this->m_H2x->Update();
  this->m_Viewer_H2x->SetImage( this->m_H2x->GetOutput() );  
  this->m_Viewer_H2x->Show();

}


 
/************************************
 *
 *  Show Second Derivative Y
 *
 ***********************************/
void
liFilterConsole
::ShowSecondDerivativeY( void )
{

  this->m_H2y->Update();
  this->m_Viewer_H2y->SetImage( this->m_H2y->GetOutput() );  
  this->m_Viewer_H2y->Show();

}




 
/************************************
 *
 *  Show Second Derivative Z
 *
 ***********************************/
void
liFilterConsole
::ShowSecondDerivativeZ( void )
{

  this->m_H2z->Update();
  this->m_Viewer_H2z->SetImage( this->m_H2z->GetOutput() );  
  this->m_Viewer_H2z->Show();

}


 
/************************************
 *
 *  Show Laplacian
 *
 ***********************************/
void
liFilterConsole
::ShowLaplacian( void )
{

  this->m_Add->Update();
  this->m_Viewer_Laplacian->SetImage( this->m_Add->GetOutput() );  
  this->m_Viewer_Laplacian->Show();

}



 
/************************************
 *
 *  Show Gradient Modulus
 *
 ***********************************/
void
liFilterConsole
::ShowGradientModulus( void )
{

  this->m_Modulus->Update();
  this->m_Viewer_Gradient_Modulus->SetImage( this->m_Modulus->GetOutput() );  
  this->m_Viewer_Gradient_Modulus->Show();

}



 
/************************************
 *
 *  Show About Window
 *
 ***********************************/
void
liFilterConsole
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
liFilterConsole
::Execute( void )
{

  if( ! (this->m_ImageLoaded) ) 
  {
    ShowStatus("Please load an image first");
    return;
  }


  ShowStatus("Filtering Image with a Gaussian...");

  liFilterConsoleBase::Execute();


  ShowStatus("Filtering done ");
  
}






/************************************
 *
 *  Write Laplacian
 *
 ***********************************/
void
liFilterConsole
::WriteLaplacian(void)
{

  const char * filename = fl_file_chooser("Laplacian filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Writing Laplacian ...");
  liFilterConsoleBase::WriteLaplacian( filename );
  ShowStatus("Laplacian written");

}




/************************************
 *
 *  Write Gradient X
 *
 ***********************************/
void
liFilterConsole
::WriteGradientX(void)
{

  const char * filename = fl_file_chooser("Gradient X filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Writing Gradient X ...");
  liFilterConsoleBase::WriteGradientX( filename );
  ShowStatus("Gradient X written");

}





/************************************
 *
 *  Write Gradient Y
 *
 ***********************************/
void
liFilterConsole
::WriteGradientY(void)
{

  const char * filename = fl_file_chooser("Gradient Y filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Writing Gradient Y ...");
  liFilterConsoleBase::WriteGradientY( filename );
  ShowStatus("Gradient Y written");

}








/************************************
 *
 *  Write Gradient Z
 *
 ***********************************/
void
liFilterConsole
::WriteGradientZ(void)
{

  const char * filename = fl_file_chooser("Gradient Z filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Writing Gradient Z ...");
  liFilterConsoleBase::WriteGradientZ( filename );
  ShowStatus("Gradient Z written");

}








/************************************
 *
 *  Write Gradient Modulus
 *
 ***********************************/
void
liFilterConsole
::WriteGradientModulus(void)
{

  const char * filename = fl_file_chooser("Gradient Modulus filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  ShowStatus("Writing Gradient Modulus ...");
  liFilterConsoleBase::WriteGradientModulus( filename );
  ShowStatus("Gradient Modulus written");

}























