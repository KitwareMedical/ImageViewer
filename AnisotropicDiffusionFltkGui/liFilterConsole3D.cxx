/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole3D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <liFilterConsole3D.h>
#include <itkMetaImageIO.h>
#include <FL/fl_file_chooser.H>
#include <stdio.h>

/************************************
 *
 *  Constructor
 *
 ***********************************/
liFilterConsole3D
::liFilterConsole3D()
{
 

  m_InputViewer = new InputImageViewerType;

  m_ViewerG     = new ImageViewerType;

  m_ViewerC     = new ImageViewerType;

  m_InputViewer->SetLabel( "Input Image" );

  m_ViewerG->SetLabel( "Gradient Image" );

  m_ViewerC->SetLabel( "Curvature Image" );

  progressSlider->Observe( m_FilterG.GetPointer() );

  progressSlider->Observe( m_FilterC.GetPointer() );

  progressSlider->Observe( m_Rescaler.GetPointer() );

  progressSlider->Observe( m_Writer.GetPointer() );
                              
  loadButton->Observe( m_Reader.GetPointer() );
  inputButton->Observe( m_Reader.GetPointer() );

  GradientButton->Observe( m_FilterG.GetPointer() );

  CurvatureButton->Observe( m_FilterC.GetPointer() );

  m_Reader->AddObserver( itk::ModifiedEvent(), GradientButton->GetRedrawCommand());

  m_Reader->AddObserver( itk::ModifiedEvent(), CurvatureButton->GetRedrawCommand());

  this->ShowStatus("Let's start by loading an image...");

  instructTextOutput->value( "This module performs smoothing by an\nedge-preserving anisotropic diffusion.\nInput data should be a MetaImage.\nOnce you have successfully loaded the\nsource, the \"Noise reduction\" filters\nbecome available.  At this stage, the\ngoal is to minimize the presence of\nuninteresting image features.  What \n\"uninteresting\" means in this context\ndepends entirely on your data and the\nresult you want to achieve.  Some\nrecommended parameter settings to start\nwith are 10 iterations and a conductance\nof 1.0.  The gradient-based filter\npreserves areas of high gradient magnitude\nin the image.  The curvature-based filter\npreserves areas of high curvature.\nExperiment with different parameter\nsettings until you get the results you want." );

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsole3D
::~liFilterConsole3D()
{
  if( m_InputViewer ) 
    {
      delete m_InputViewer;
      m_InputViewer = 0;
    }
  if( m_ViewerG ) 
    {
      delete m_ViewerG;
      m_ViewerG = 0;
    }
  if( m_ViewerC ) 
    {
      delete m_ViewerC;
      m_ViewerC = 0;
    }

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liFilterConsole3D
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
    liFilterConsole3DBase::Load( filename );
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
liFilterConsole3D
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
liFilterConsole3D
::Hide( void )
{

  consoleWindow->hide();

  m_InputViewer->Hide();

  m_ViewerG->Hide();

  m_ViewerC->Hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
liFilterConsole3D
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
liFilterConsole3D
::ShowStatus( const char * message )
{
  liFilterConsole3DBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Input Image
 *
 ***********************************/
void
liFilterConsole3D
::ShowInputImage( void )
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
 *  Show Gradient Image
 *
 ***********************************/
void
liFilterConsole3D
::ShowGradientImage( void )
{

  m_FilterG->Update();
  m_ViewerG->SetImage( m_FilterG->GetOutput() );  
  m_ViewerG->Show();

}

/************************************
 *
 *  Show Curvature Image
 *
 ***********************************/
void
liFilterConsole3D
::ShowCurvatureImage( void )
{

  m_FilterC->Update();
  m_ViewerC->SetImage( m_FilterC->GetOutput() );  
  m_ViewerC->Show();

}

/************************************
 *
 *  Save Gradient Image
 *
 ***********************************/
void
liFilterConsole3D
::SaveGradientImage( void )
{

  const char * filename = fl_file_chooser("Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving image file...");
  
  try 
  {
    liFilterConsole3DBase::SaveGradientImage( filename );
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
 *  Save Curvature Image
 *
 ***********************************/
void
liFilterConsole3D
::SaveCurvatureImage( void )
{

  const char * filename = fl_file_chooser("Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Saving image file...");
  
  try 
  {
    liFilterConsole3DBase::SaveCurvatureImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems writing file format");
    return;
  }

  this->ShowStatus("File Saved");

}


