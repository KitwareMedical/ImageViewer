/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/


#include <liFilterConsole.h>
#include <FL/fl_file_chooser.H>
#include <fltkLightButtonRedrawCommand.h>
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
  
  typedef fltk::LightButtonRedrawCommand  CommandButton;

  m_InputViewer = new InputImageViewerType;

  m_Viewer_H1x  = new ImageViewerType;
  m_Viewer_H1y  = new ImageViewerType;
  m_Viewer_H1z  = new ImageViewerType;

  m_Viewer_H2x  = new ImageViewerType;
  m_Viewer_H2y  = new ImageViewerType;
  m_Viewer_H2z  = new ImageViewerType;

  m_Viewer_Laplacian = new ImageViewerType;

  m_Viewer_Gradient_Modulus = new ImageViewerType;

  m_InputViewer->SetLabel( "Input Image" );

  m_Viewer_H1x->SetLabel( "Gradient X" );
  m_Viewer_H1y->SetLabel( "Gradient Y" );
  m_Viewer_H1z->SetLabel( "Gradient Z" );

  m_Viewer_H2x->SetLabel( "Second Derivative X" );
  m_Viewer_H2y->SetLabel( "Second Derivative Y" );
  m_Viewer_H2z->SetLabel( "Second Derivative Z" );

  m_Viewer_Laplacian->SetLabel( "Laplacian" );
  
  m_Viewer_Gradient_Modulus->SetLabel( "Gradient Modulus" );

  fltk::ProgressBarRedrawCommand * progressUpdateCommand = 
                            progressSlider->GetRedrawCommand().GetPointer();

  m_H1x->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H1y->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H1z->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H2x->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H2y->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H2z->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hx->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hy->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hz->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hxy->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hyz->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hzx->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Add->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Modulus->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
                              
  m_Reader->AddObserver( itk::Command::StartEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::StartEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::StartEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::StartEvent, HzButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::StartEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::StartEvent, HyzButton->GetRedrawCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::StartEvent, HzxButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::StartEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::StartEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::StartEvent, H1zButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::StartEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::StartEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::StartEvent, H2zButton->GetRedrawCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::StartEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::StartEvent, modulusButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::EndEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::EndEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::EndEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::EndEvent, HzButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::EndEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::EndEvent, HyzButton->GetRedrawCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::EndEvent, HzxButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::EndEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::EndEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::EndEvent, H1zButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::EndEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::EndEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::EndEvent, H2zButton->GetRedrawCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::EndEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::EndEvent, modulusButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::ModifiedEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::ModifiedEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hz->AddObserver(  itk::Command::ModifiedEvent, HzButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_Hyz->AddObserver( itk::Command::ModifiedEvent, HyzButton->GetRedrawCommand().GetPointer() );
  m_Hzx->AddObserver( itk::Command::ModifiedEvent, HzxButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::ModifiedEvent, H1zButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::ModifiedEvent, H2zButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_H1z->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_H2z->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HzButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HyzButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HzxButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1zButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2zButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );

  this->ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsole
::~liFilterConsole()
{

  if( m_Viewer_H1x ) 
  {
    delete m_Viewer_H1x;
    m_Viewer_H1x = 0;
  }

  if( m_Viewer_H1y ) 
  {
    delete m_Viewer_H1y;
    m_Viewer_H1y = 0;
  }

  if( m_Viewer_H1z ) 
  {
    delete m_Viewer_H1z;
    m_Viewer_H1z = 0;
  }

  if( m_Viewer_H2x ) 
  {
    delete m_Viewer_H2x;
    m_Viewer_H2x = 0;
  }

  if( m_Viewer_H2y ) 
  {
    delete m_Viewer_H2y;
    m_Viewer_H2y = 0;
  }

  if( m_Viewer_H2z ) 
  {
    delete m_Viewer_H2z;
    m_Viewer_H2z = 0;
  }


  if( m_InputViewer ) 
  {
    delete m_InputViewer;
    m_InputViewer = 0;
  }

  if( m_Viewer_Laplacian ) 
  {
    delete m_Viewer_Laplacian;
    m_Viewer_Laplacian = 0;
  }

  if( m_Viewer_Gradient_Modulus ) 
  {
    delete m_Viewer_Gradient_Modulus;
    m_Viewer_Gradient_Modulus = 0;
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

  this->ShowStatus("Loading image file...");
  
  try 
  {
    liFilterConsoleBase::Load( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("File Loaded");

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
  m_Viewer_H1x->Hide();
  m_Viewer_H1y->Hide();
  m_Viewer_H1z->Hide();
  m_Viewer_H2x->Hide();
  m_Viewer_H2y->Hide();
  m_Viewer_H2z->Hide();
  m_InputViewer->Hide();
  m_Viewer_Laplacian->Hide();
  m_Viewer_Gradient_Modulus->Hide();
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

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }

  m_InputViewer->SetImage( m_Reader->GetOutput() );  
  m_InputViewer->Show();

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

  m_H1x->Update();
  m_Viewer_H1x->SetImage( m_H1x->GetOutput() );  
  m_Viewer_H1x->Show();

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

  m_H1y->Update(); 
  m_Viewer_H1y->SetImage( m_H1y->GetOutput() );  
  m_Viewer_H1y->Show();

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

  m_H1z->Update();
  m_Viewer_H1z->SetImage( m_H1z->GetOutput() );  
  m_Viewer_H1z->Show();

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

  m_H2x->Update();
  m_Viewer_H2x->SetImage( m_H2x->GetOutput() );  
  m_Viewer_H2x->Show();

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

  m_H2y->Update();
  m_Viewer_H2y->SetImage( m_H2y->GetOutput() );  
  m_Viewer_H2y->Show();

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

  m_H2z->Update();
  m_Viewer_H2z->SetImage( m_H2z->GetOutput() );  
  m_Viewer_H2z->Show();

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

  m_Add->Update();
  m_Viewer_Laplacian->SetImage( m_Add->GetOutput() );  
  m_Viewer_Laplacian->Show();

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

  m_Modulus->Update();
  m_Viewer_Gradient_Modulus->SetImage( m_Modulus->GetOutput() );  
  m_Viewer_Gradient_Modulus->Show();

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

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }


  this->ShowStatus("Filtering Image with a Gaussian...");

  liFilterConsoleBase::Execute();


  this->ShowStatus("Filtering done ");
  
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

  this->ShowStatus("Writing Laplacian ...");
  liFilterConsoleBase::WriteLaplacian( filename );
  this->ShowStatus("Laplacian written");

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

  this->ShowStatus("Writing Gradient X ...");
  liFilterConsoleBase::WriteGradientX( filename );
  this->ShowStatus("Gradient X written");

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

  this->ShowStatus("Writing Gradient Y ...");
  liFilterConsoleBase::WriteGradientY( filename );
  this->ShowStatus("Gradient Y written");

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

  this->ShowStatus("Writing Gradient Z ...");
  liFilterConsoleBase::WriteGradientZ( filename );
  this->ShowStatus("Gradient Z written");

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

  this->ShowStatus("Writing Gradient Modulus ...");
  liFilterConsoleBase::WriteGradientModulus( filename );
  this->ShowStatus("Gradient Modulus written");

}























