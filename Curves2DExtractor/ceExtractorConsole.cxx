/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsole.cxx
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


#include "ceExtractorConsole.h"
#include "fltkLightButtonRedrawCommand.h"
#include "itkMetaImageIOFactory.h"
#include <FL/fl_file_chooser.H>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
ceExtractorConsole
::ceExtractorConsole()
{
 
  itk::MetaImageIOFactory::RegisterOneFactory();

  
  typedef fltk::LightButtonRedrawCommand  CommandButton;

  m_InputViewer = InputImageViewerType::New();

  m_Viewer_H1x  = ImageViewerType::New();
  m_Viewer_H1y  = ImageViewerType::New();

  m_Viewer_H2x  = ImageViewerType::New();
  m_Viewer_H2y  = ImageViewerType::New();

  m_Viewer_Hxy   = ImageViewerType::New();
  m_Viewer_H1xy  = ImageViewerType::New();

  m_Viewer_Laplacian = ImageViewerType::New();

  m_Viewer_Gradient_Modulus = ImageViewerType::New();

  m_Viewer_Min_EigenValue = ImageViewerType::New();
  m_Viewer_Max_EigenValue = ImageViewerType::New();

  m_Viewer_Gradient_On_EigenVector = ImageViewerType::New();

  m_InputViewer->SetLabel( "Input Image" );

  m_Viewer_H1x->SetLabel( "Gradient X" );
  m_Viewer_H1y->SetLabel( "Gradient Y" );

  m_Viewer_H2x->SetLabel( "Second Derivative X" );
  m_Viewer_H2y->SetLabel( "Second Derivative Y" );

  m_Viewer_H1xy->SetLabel( "Cross Derivative XY" );

  m_Viewer_Laplacian->SetLabel( "Laplacian" );

  m_Viewer_Hxy->SetLabel( "Smoothed" );
  
  m_Viewer_Gradient_Modulus->SetLabel( "Gradient Modulus" );

  m_Viewer_Max_EigenValue->SetLabel( "Max Eigen Value" );
  m_Viewer_Min_EigenValue->SetLabel( "Min Eigen Value" );

  m_Viewer_Gradient_On_EigenVector->SetLabel( "Gradient Projected on EigenVector" );

  m_ParametricSpaceSamplesShape = PointSetShapeType::New();

  m_ParametricSpaceSamplesShape->SetPointSet( m_ParametricSpace->GetOutput() );
  
  // OpenGL display list mode
  m_ParametricSpaceSamplesShape->SetCompileMode( fltk::Shape3D::compileExecute ); 

  m_ParametricSpaceViewer.SetLabel("Parametric Space");


  fltk::ProgressBarRedrawCommand * progressUpdateCommand = 
                            progressSlider->GetRedrawCommand().GetPointer();

  m_Reader->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_H1x->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H1y->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H2x->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_H2y->AddObserver( itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hx->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hy->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Hxy->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_H1xy->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Add->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Modulus->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Eigen->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_Gradient->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_ScalarProduct->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
  m_ParametricSpace->AddObserver(  itk::Command::ProgressEvent, progressUpdateCommand );
                              
  m_Reader->AddObserver( itk::Command::StartEvent, loadButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::StartEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::StartEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::StartEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::StartEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::StartEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::StartEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::StartEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::StartEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::StartEvent, H1xyButton->GetRedrawCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::StartEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::StartEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::StartEvent, maxEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::StartEvent, minEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::StartEvent, maxEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_ScalarProduct->AddObserver( itk::Command::StartEvent, 
                                gradientOnEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_ParametricSpace->AddObserver(  itk::Command::StartEvent, 
                                   parametricSpaceButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::EndEvent, loadButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::EndEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::EndEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::EndEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::EndEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::EndEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::EndEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::EndEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::EndEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::EndEvent, H1xyButton->GetRedrawCommand().GetPointer() );
  m_Add->AddObserver( itk::Command::EndEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Modulus->AddObserver( itk::Command::EndEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::EndEvent, maxEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::EndEvent, minEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Eigen->AddObserver( itk::Command::EndEvent, maxEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_ScalarProduct->AddObserver( itk::Command::EndEvent, 
                                gradientOnEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_ParametricSpace->AddObserver(  itk::Command::EndEvent, 
                         parametricSpaceButton->GetRedrawCommand().GetPointer() );
  m_ParametricSpace->AddObserver(  itk::Command::EndEvent, 
                         m_ParametricSpaceSamplesShape->GetDisplayListUpdateCommand().GetPointer() );
  m_ParametricSpace->AddObserver(  itk::Command::EndEvent, 
                         m_ParametricSpaceViewer.GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, loadButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, inputButton->GetRedrawCommand().GetPointer() );
  m_Hx->AddObserver(  itk::Command::ModifiedEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Hy->AddObserver(  itk::Command::ModifiedEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Hxy->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_H1x->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_H1y->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_H2x->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_H2y->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::ModifiedEvent, H1xyButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::ModifiedEvent, maxEigenValueButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::ModifiedEvent, minEigenValueButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::ModifiedEvent, maxEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_H1xy->AddObserver( itk::Command::ModifiedEvent, 
                                gradientOnEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_ParametricSpace->AddObserver(  itk::Command::ModifiedEvent, 
                                   parametricSpaceButton->GetRedrawCommand().GetPointer() );

  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, HxyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H2yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, H1xyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, laplacianButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, modulusButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, maxEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, minEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, maxEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, gradientOnEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::Command::ModifiedEvent, parametricSpaceButton->GetRedrawCommand().GetPointer() );


  // Register the PointSet as a Drawer in the OpenGL window
  m_ParametricSpaceViewer.GetNotifier()->AddObserver( 
                                                fltk::GlDrawEvent, 
                                                m_ParametricSpaceSamplesShape->GetDrawCommand() );

  // Notify the OpenGL window when the set of points change
  m_ParametricSpace->AddObserver( itk::Command::EndEvent,
                                  m_ParametricSpaceViewer.GetRedrawCommand() );


  fltk::Shape3D::ColorType  parametricSpacePointsColor;
  parametricSpacePointsColor.SetRed(   0.0 );
  parametricSpacePointsColor.SetGreen( 0.0 );
  parametricSpacePointsColor.SetBlue(  0.0 );
  m_ParametricSpaceSamplesShape->SetColor( parametricSpacePointsColor );

  fltk::GlWindowInteractive::ColorType  parametricSpaceBackgroundColor;
  parametricSpaceBackgroundColor.SetRed(   1.0 );
  parametricSpaceBackgroundColor.SetGreen( 1.0 );
  parametricSpaceBackgroundColor.SetBlue(  1.0 );
  m_ParametricSpaceViewer.GetGlWindow()->SetBackground( 
                                           parametricSpaceBackgroundColor );

  this->ShowStatus("Let's start by loading an image...");

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
ceExtractorConsole
::~ceExtractorConsole()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
ceExtractorConsole
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
    ceExtractorConsoleBase::Load( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("File Loaded");

  controlsGroup->activate();

  m_InputViewer->Update();

}

   
/************************************
 *
 *  Show
 *
 ***********************************/
void
ceExtractorConsole
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
ceExtractorConsole
::Hide( void )
{

  consoleWindow->hide();
  m_Viewer_H1x->Hide();
  m_Viewer_H1y->Hide();
  m_Viewer_H2x->Hide();
  m_Viewer_H2y->Hide();
  m_Viewer_H1xy->Hide();
  m_Viewer_Hxy->Hide();
  m_InputViewer->Hide();
  m_Viewer_Laplacian->Hide();
  m_Viewer_Gradient_Modulus->Hide();
  m_Viewer_Max_EigenValue->Hide();
  m_Viewer_Min_EigenValue->Hide();
  m_Viewer_Gradient_On_EigenVector->Hide();
  m_ParametricSpaceViewer.Hide();
}





/************************************
 *
 *  Quit
 *
 ***********************************/
void
ceExtractorConsole
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
ceExtractorConsole
::ShowStatus( const char * message )
{
  ceExtractorConsoleBase::ShowStatus( message );
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Input Image
 *
 ***********************************/
void
ceExtractorConsole
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
ceExtractorConsole
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
ceExtractorConsole
::ShowFilteredY( void )
{

  m_H1y->Update(); 
  m_Viewer_H1y->SetImage( m_H1y->GetOutput() );  
  m_Viewer_H1y->Show();

}



 
/************************************
 *
 *  Show Cross Derivative XY
 *
 ***********************************/
void
ceExtractorConsole
::ShowCrossDerivativeXY( void )
{

  m_H1xy->Update();
  m_Viewer_H1xy->SetImage( m_H1xy->GetOutput() );  
  m_Viewer_H1xy->Show();

}



 
/************************************
 *
 *  Show Smoothed XY
 *
 ***********************************/
void
ceExtractorConsole
::ShowSmoothed( void )
{

  m_Hxy->Update();
  m_Viewer_Hxy->SetImage( m_Hxy->GetOutput() );  
  m_Viewer_Hxy->Show();

}




 
/************************************
 *
 *  Show Second Derivative X
 *
 ***********************************/
void
ceExtractorConsole
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
ceExtractorConsole
::ShowSecondDerivativeY( void )
{

  m_H2y->Update();
  m_Viewer_H2y->SetImage( m_H2y->GetOutput() );  
  m_Viewer_H2y->Show();

}





 
/************************************
 *
 *  Show Laplacian
 *
 ***********************************/
void
ceExtractorConsole
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
ceExtractorConsole
::ShowGradientModulus( void )
{

  m_Modulus->Update();
  m_Viewer_Gradient_Modulus->SetImage( m_Modulus->GetOutput() );  
  m_Viewer_Gradient_Modulus->Show();

}



 
/************************************
 *
 *  Show Max Eigen Value
 *
 ***********************************/
void
ceExtractorConsole
::ShowMaxEigenValue( void )
{

  m_Eigen->Update(); 
  m_Viewer_Max_EigenValue->SetImage( m_Eigen->GetMaxEigenValue() );  
  m_Viewer_Max_EigenValue->Show();

}



 
/************************************
 *
 *  Show Min Eigen Value
 *
 ***********************************/
void
ceExtractorConsole
::ShowMinEigenValue( void )
{

  m_Eigen->Update(); 
  m_Viewer_Min_EigenValue->SetImage( m_Eigen->GetMinEigenValue() );  
  m_Viewer_Min_EigenValue->Show();

}




 
/************************************************
 *
 *  Show Gradient projected on max EigenVector
 *
 ***********************************************/
void
ceExtractorConsole
::ShowGradientOnEigenVector( void )
{

  m_ScalarProduct->Update(); 
  m_Viewer_Gradient_On_EigenVector->SetImage( m_ScalarProduct->GetOutput() );  
  m_Viewer_Gradient_On_EigenVector->Show();

}



 
/************************************
 *
 *  Show the Parametric Space
 *
 ***********************************/
void
ceExtractorConsole
::ShowParametricSpace( void )
{

  m_ParametricSpace->Update(); 
  m_ParametricSpaceViewer.Show();

}

 
/************************************
 *
 *  Execute
 *
 ***********************************/
void
ceExtractorConsole
::Execute( void )
{

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }


  this->ShowStatus("Filtering Image with a Gaussian...");

  ceExtractorConsoleBase::Execute();


  this->ShowStatus("Filtering done ");
  
}











/**
 *
 *   Main program 
 *
 */
int main()
{

  ceExtractorConsole * console = new ceExtractorConsole();

  console->Show();

  Fl::run();

  delete console;
  
  return 0;

}


















