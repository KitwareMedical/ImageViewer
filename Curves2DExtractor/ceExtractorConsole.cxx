/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsole.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "ceExtractorConsole.h"
#include "itkMetaImageIOFactory.h"
#include "itkPNGImageIOFactory.h"
#include <FL/fl_file_chooser.H>
 


/************************************
 *
 *  Constructor
 *
 ***********************************/
ceExtractorConsole
::ceExtractorConsole()
{
 
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::MetaImageIOFactory::RegisterOneFactory();
  

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

  m_Viewer_Extracted_Points = ImageViewerType::New();

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

  m_Viewer_Extracted_Points->SetLabel("Points of Extracted Curves");

  m_Viewer_Gradient_On_EigenVector->SetLabel( "Gradient Projected on EigenVector" );

  m_ParametricSpaceSamplesShape = PointSetShapeType::New();

  m_ParametricSpaceSamplesShape->SetPointSet( 
                          m_ParametricSpace->GetOutput().GetPointer() );
  
  // OpenGL display list mode
  m_ParametricSpaceSamplesShape->SetCompileMode( fltk::Shape3D::compileExecute ); 

  m_ParametricSpaceViewer.SetLabel("Parametric Space");



  m_ExtractedParametricSpaceSamplesShape = PointSetShapeType::New();

  m_ExtractedParametricSpaceSamplesShape->SetPointSet( 
                          m_SpatialFunctionFilter->GetOutput().GetPointer() );

  progressSlider->Observe( m_Reader.GetPointer() );
  progressSlider->Observe( m_H1x.GetPointer() );
  progressSlider->Observe( m_H1y.GetPointer() );
  progressSlider->Observe( m_H2x.GetPointer() );
  progressSlider->Observe( m_H2y.GetPointer() );
  progressSlider->Observe( m_Hx.GetPointer() );
  progressSlider->Observe( m_Hy.GetPointer() );
  progressSlider->Observe( m_Hxy.GetPointer() );
  progressSlider->Observe( m_H1xy.GetPointer() );
  progressSlider->Observe( m_Add.GetPointer() );
  progressSlider->Observe( m_Modulus.GetPointer() );
  progressSlider->Observe( m_Eigen.GetPointer() );
  progressSlider->Observe( m_Gradient.GetPointer() );
  progressSlider->Observe( m_ScalarProduct.GetPointer() );
  progressSlider->Observe( m_ParametricSpace.GetPointer() );
  progressSlider->Observe( m_SpatialFunctionFilter.GetPointer() );
  progressSlider->Observe( m_InverseParametricFilter.GetPointer() );

  loadButton->Observe( m_Reader.GetPointer() );
  inputButton->Observe( m_Reader.GetPointer() );
  HxButton->Observe( m_Hx.GetPointer() );
  HyButton->Observe( m_Hy.GetPointer() );
  HxyButton->Observe( m_Hxy.GetPointer() );
  H1xButton->Observe( m_H1x.GetPointer() );
  H1yButton->Observe( m_H1y.GetPointer() );
  H2xButton->Observe( m_H2x.GetPointer() );
  H2yButton->Observe( m_H2y.GetPointer() );
  H1xyButton->Observe( m_H1xy.GetPointer() );
  laplacianButton->Observe( m_Add.GetPointer() );
  modulusButton->Observe( m_Modulus.GetPointer() );
  maxEigenValueButton->Observe( m_Eigen.GetPointer() );
  minEigenValueButton->Observe( m_Eigen.GetPointer() );
  maxEigenVectorButton->Observe( m_Eigen.GetPointer() );
  gradientOnEigenVectorButton->Observe( m_ScalarProduct.GetPointer() );
  parametricSpaceButton->Observe( m_ParametricSpace.GetPointer() );
  extractedParametricPointsButton->Observe( m_SpatialFunctionFilter.GetPointer() );
  curve2DPointsButton->Observe( m_InverseParametricFilter.GetPointer() );

  m_Reader->AddObserver( itk::ModifiedEvent(), HxyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), H1xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), H1yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), H2xButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), H2yButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), H1xyButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), laplacianButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), modulusButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), maxEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), minEigenValueButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), maxEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), gradientOnEigenVectorButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), parametricSpaceButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), extractedParametricPointsButton->GetRedrawCommand().GetPointer() );
  m_Reader->AddObserver( itk::ModifiedEvent(), curve2DPointsButton->GetRedrawCommand().GetPointer() );


  // Register the PointSet as a Drawer in the OpenGL window
  m_ParametricSpaceViewer.GetNotifier()->AddObserver( 
                         fltk::GlDrawEvent(), 
                         m_ParametricSpaceSamplesShape->GetDrawCommand().GetPointer() );

  // Notify the OpenGL window when the set of points changes
  m_ParametricSpace->AddObserver( itk::EndEvent(),
                        m_ParametricSpaceViewer.GetRedrawCommand().GetPointer() );


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

  m_ImageSpaceSamplesShape = ImageSpacePointSetShapeType::New();

  fltk::Shape3D::ColorType curve2DColor;
  curve2DColor.SetRed(   1.0 );
  curve2DColor.SetGreen( 0.0 );
  curve2DColor.SetBlue(  0.0 );
  m_ImageSpaceSamplesShape->SetColor( curve2DColor );

  m_ImageSpaceSamplesShape->SetPointSet(
                  m_InverseParametricFilter->GetOutput().GetPointer() );

  m_Viewer_Extracted_Points->GetNotifier()->AddObserver(
                  fltk::GlDrawEvent(), 
                  m_ImageSpaceSamplesShape->GetDrawCommand().GetPointer() );
  
  m_InverseParametricFilter->AddObserver( 
                  itk::EndEvent(),
                  m_Viewer_Extracted_Points->GetRedrawCommand().GetPointer() );

  // Register the SpatialFunctionControl as a Drawer in the OpenGL window
  m_ParametricSpaceViewer.GetNotifier()->AddObserver( 
                  fltk::GlDrawEvent(), 
                  m_SpatialFunctionControl->GetDrawCommand().GetPointer() );

  // Notify the OpenGL window when the spatial function changes
  m_SpatialFunctionControl->AddObserver( 
                  fltk::RedrawEvent(),
                  m_ParametricSpaceViewer.GetRedrawCommand().GetPointer() );


  
  m_ExtractedParametricSpaceSamplesShape->SetCompileMode( fltk::Shape3D::compileExecute ); 
  
  m_ExtractedParametricSpaceViewer.SetLabel("Filtered Parametric Space");
  
  
  m_SpatialFunctionControl->AddObserver( 
                  fltk::RedrawEvent(),
                  m_ExtractedParametricSpaceViewer.GetRedrawCommand().GetPointer() );

  m_ExtractedParametricSpaceViewer.GetNotifier()->AddObserver( 
                  fltk::GlDrawEvent(), 
                  m_ExtractedParametricSpaceSamplesShape->GetDrawCommand().GetPointer() );

  m_SpatialFunctionControl->SetParametersFromGUI();
  
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

  const char * filename = fl_file_chooser("Image filename","*.*","latmag0Acor.mha");
  if( !filename )
  {
    return;
  }

  this->Load( filename );

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
ceExtractorConsole
::Load( const char * filename )
{

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


  m_Reader->InvokeEvent( itk::EndEvent() );

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
  m_Viewer_Extracted_Points->Hide();
  
  m_ParametricSpaceViewer.Hide();
  m_ExtractedParametricSpaceViewer.Hide();

  this->ceExtractorConsoleBase::HideSpatialFunctionControl();

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



 
/*******************************************
 *
 *  Extracted Parametric Points
 *
 ******************************************/
void
ceExtractorConsole
::ShowExtractedParametricPoints( void )
{

  m_ExtractedParametricSpaceViewer.Show();
  this->ResetViewOfExtractedParametricSpace();

}

 
/*******************************************
 *
 *  Show Input Image and Extracted Points
 *
 ******************************************/
void
ceExtractorConsole
::ShowCurve2DPoints( void )
{

  if( ! (m_ImageLoaded) ) 
  {
    this->ShowStatus("Please load an image first");
    return;
  }

  m_InverseParametricFilter->Update();

  m_Viewer_Extracted_Points->SetImage( m_Reader->GetOutput() );  
  m_Viewer_Extracted_Points->Show();

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

  m_ParametricSpaceViewer.Show();
  this->ResetViewOfParametricSpace();

}



 
/****************************************
 *
 *  Reset View of the Parametric Space
 *
 ****************************************/
void
ceExtractorConsole
::ResetViewOfParametricSpace( void )
{

  m_ParametricSpace->Update(); 

  fltk::GlWindowInteractive::Point3DType center;
  center[0] =   0;
  center[1] =   0;
  center[2] = -60;

  m_ParametricSpaceViewer.GetGlWindow()->ResetViewingParameters();

  m_ParametricSpaceViewer.GetGlWindow()->SetCenter( center );
  m_ParametricSpaceViewer.GetGlWindow()->SetZoom( 2.0 );
  m_ParametricSpaceViewer.GetGlWindow()->SetAltitude( -90.0 );
  m_ParametricSpaceViewer.GetGlWindow()->SetAzimuth( 45.0 );
  
  m_ParametricSpaceViewer.GetGlWindow()->redraw();


}

 
/*************************************************
 *
 *  Reset View of the Extracted Parametric Space
 *
 *************************************************/
void
ceExtractorConsole
::ResetViewOfExtractedParametricSpace( void )
{

  m_SpatialFunctionFilter->Update(); 

  fltk::GlWindowInteractive::Point3DType center;
  center[0] =   0;
  center[1] =   0;
  center[2] = -60;

  m_ExtractedParametricSpaceViewer.GetGlWindow()->ResetViewingParameters();
  
  m_ExtractedParametricSpaceViewer.GetGlWindow()->SetZoom( 2.0 );
  m_ExtractedParametricSpaceViewer.GetGlWindow()->SetAltitude( -90.0 );
  m_ExtractedParametricSpaceViewer.GetGlWindow()->SetAzimuth( 45.0 );
  m_ExtractedParametricSpaceViewer.GetGlWindow()->SetCenter( center );

  m_ExtractedParametricSpaceViewer.GetGlWindow()->redraw();

}
 
 
/************************************
 *
 *  Set Sigma
 *
 ***********************************/
void
ceExtractorConsole
::SetSigma( ceExtractorConsoleBase::ComputationType value )
{
  sigmaCounter->value( value );
  this->ceExtractorConsoleBase::SetSigma( value );
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











