/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole2D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <liFilterConsole2D.h>
#include <FL/fl_file_chooser.H>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
liFilterConsole2D
::liFilterConsole2D()
{
 

  m_InputViewer = InputImageViewerType::New();

  m_Viewer_H1x  = ImageViewerType::New();
  m_Viewer_H1y  = ImageViewerType::New();

  m_Viewer_H2x  = ImageViewerType::New();
  m_Viewer_H2y  = ImageViewerType::New();

  m_Viewer_Laplacian =        ImageViewerType::New();
  m_Viewer_Smoothed  =        ImageViewerType::New();
  m_Viewer_Gradient_Modulus = ImageViewerType::New();

  m_InputViewer->SetLabel( "Input Image" );

  m_Viewer_H1x->SetLabel( "Gradient X" );
  m_Viewer_H1y->SetLabel( "Gradient Y" );

  m_Viewer_H2x->SetLabel( "Second Derivative X" );
  m_Viewer_H2y->SetLabel( "Second Derivative Y" );

  m_Viewer_Laplacian->SetLabel( "Laplacian" );
  m_Viewer_Smoothed->SetLabel( "Smoothed" );
  m_Viewer_Gradient_Modulus->SetLabel( "Gradient Modulus" );


  progressSlider->Observe( m_Hx.GetPointer() );
  progressSlider->Observe( m_Hy.GetPointer() );
  progressSlider->Observe( m_H1x.GetPointer() );
  progressSlider->Observe( m_H1y.GetPointer() );
  progressSlider->Observe( m_H2x.GetPointer() );
  progressSlider->Observe( m_H2y.GetPointer() );
  progressSlider->Observe( m_Laplacian.GetPointer() );
  progressSlider->Observe( m_Smoothed.GetPointer() );
  progressSlider->Observe( m_Modulus.GetPointer() );
  progressSlider->Observe( m_RescaleIntensitySmoothed.GetPointer() );
  progressSlider->Observe( m_RescaleIntensityLaplacian.GetPointer() );
  progressSlider->Observe( m_RescaleIntensityModulus.GetPointer() );
  progressSlider->Observe( m_WriterSmoothed.GetPointer() );
  progressSlider->Observe( m_WriterLaplacian.GetPointer() );
  progressSlider->Observe( m_WriterModulus.GetPointer() );
                              
  loadButton->Observe( m_Reader.GetPointer() );
  inputButton->Observe( m_Reader.GetPointer() );
  HxButton->Observe( m_Hx.GetPointer() );
  HyButton->Observe( m_Hy.GetPointer() );
  H1xButton->Observe( m_H1x.GetPointer() );
  H1yButton->Observe( m_H1y.GetPointer() );
  H2xButton->Observe( m_H2x.GetPointer() );
  H2yButton->Observe( m_H2y.GetPointer() );
  laplacianButton->Observe( m_Laplacian.GetPointer() );
  smoothedButton->Observe( m_Smoothed.GetPointer() );
  modulusButton->Observe( m_Modulus.GetPointer() );

  m_Reader->AddObserver( itk::ModifiedEvent(), HxButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), HyButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), H1xButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), H1yButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), H2xButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), H2yButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), laplacianButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), smoothedButton->GetRedrawCommand());
  m_Reader->AddObserver( itk::ModifiedEvent(), modulusButton->GetRedrawCommand());

  this->ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsole2D
::~liFilterConsole2D()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liFilterConsole2D
::Load( void )
{

  const char * filename = fl_file_chooser("Image filename","*.png","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading image file...");
  
  try 
  {
    liFilterConsole2DBase::Load( filename );
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
liFilterConsole2D
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
liFilterConsole2D
::Hide( void )
{

  consoleWindow->hide();
  m_Viewer_H1x->Hide();
  m_Viewer_H1y->Hide();
  m_Viewer_H2x->Hide();
  m_Viewer_H2y->Hide();
  m_InputViewer->Hide();
  m_Viewer_Laplacian->Hide();
  m_Viewer_Smoothed->Hide();
  m_Viewer_Gradient_Modulus->Hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
liFilterConsole2D
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
liFilterConsole2D
::ShowStatus( const char * message )
{
  liFilterConsole2DBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Input Image
 *
 ***********************************/
void
liFilterConsole2D
::ShowInput( void )
{

  if( ! (m_ImageFileNameAvailable) ) 
  {
    this->ShowStatus("Please provide a image filename first");
    return;
  }

  m_Reader->Update();
  m_InputViewer->SetImage( m_Reader->GetOutput() );  
  m_InputViewer->Show();

}



 
/************************************
 *
 *  Show Filtered Image
 *
 ***********************************/
void
liFilterConsole2D
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
liFilterConsole2D
::ShowFilteredY( void )
{

  m_H1y->Update(); 
  m_Viewer_H1y->SetImage( m_H1y->GetOutput() );  
  m_Viewer_H1y->Show();

}




 
/************************************
 *
 *  Show Second Derivative X
 *
 ***********************************/
void
liFilterConsole2D
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
liFilterConsole2D
::ShowSecondDerivativeY( void )
{

  m_H2y->Update();
  m_Viewer_H2y->SetImage( m_H2y->GetOutput() );  
  m_Viewer_H2y->Show();

}




 
/************************************
 *
 *  Show Smoothed
 *
 ***********************************/
void
liFilterConsole2D
::ShowSmoothed( void )
{

  m_Smoothed->Update();
  m_Viewer_Smoothed->SetImage( m_Smoothed->GetOutput() );  
  m_Viewer_Smoothed->Show();

}




 
/************************************
 *
 *  Show Laplacian
 *
 ***********************************/
void
liFilterConsole2D
::ShowLaplacian( void )
{

  m_Laplacian->Update();
  m_Viewer_Laplacian->SetImage( m_Laplacian->GetOutput() );  
  m_Viewer_Laplacian->Show();

}



 
/************************************
 *
 *  Show Gradient Modulus
 *
 ***********************************/
void
liFilterConsole2D
::ShowGradientModulus( void )
{

  m_Modulus->Update();
  m_Viewer_Gradient_Modulus->SetImage( m_Modulus->GetOutput() );  
  m_Viewer_Gradient_Modulus->Show();

}




 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
liFilterConsole2D
::Execute( void )
{

  if( ! (m_ImageFileNameAvailable) ) 
  {
    this->ShowStatus("Please provide a filename for the image first");
    return;
  }


  this->ShowStatus("Filtering Image with a Gaussian...");

  liFilterConsole2DBase::Execute();


  this->ShowStatus("Filtering done ");
  
}





   
/************************************
 *
 *  SaveLaplacian
 *
 ***********************************/
void
liFilterConsole2D
::SaveLaplacian( void )
{

  const char * filename = fl_file_chooser("Image filename","*.png","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving image file...");
  
  try 
  {
    liFilterConsole2DBase::SaveLaplacian( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems writing file format");
    return;
  }

  this->ShowStatus("File Saved");

}


/************************************
 *
 *  SaveModulus
 *
 ***********************************/
void
liFilterConsole2D
::SaveModulus( void )
{

  const char * filename = fl_file_chooser("Image filename","*.png","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving image file...");
  
  try 
  {
    liFilterConsole2DBase::SaveModulus( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems writing file format");
    return;
  }

  this->ShowStatus("File Saved");

}


/************************************
 *
 *  SaveSmoothed
 *
 ***********************************/
void
liFilterConsole2D
::SaveSmoothed( void )
{

  const char * filename = fl_file_chooser("Image filename","*.png","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving image file...");
  
  try 
  {
    liFilterConsole2DBase::SaveSmoothed( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems writing file format");
    return;
  }

  this->ShowStatus("File Saved");

}



