/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectionLevelSet.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <ShapeDetectionLevelSet.h>
#include <FL/fl_file_chooser.H>





/************************************
 *
 *  Constructor
 *
 ***********************************/
ShapeDetectionLevelSet
::ShapeDetectionLevelSet()
{

  m_InputImageViewer.SetLabel("Input Image");

  m_ThresholdedImageViewer.SetLabel("Thresholded Image");

  m_ZeroSetImageViewer.SetLabel("Fast Marching Output");

  m_FastMarchingImageViewer.SetLabel("Zero Set Image");

  m_GradientMagnitudeImageViewer.SetLabel("Gradient Magnitude Image");

  m_EdgePotentialImageViewer.SetLabel("Edge Potential Image");

  m_InputImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);

  // Initialize ITK filter with GUI values
  m_SigmoidFilter->SetAlpha( sigmoidAlphaValueInput->value() );
  m_SigmoidFilter->SetBeta(  sigmoidBetaValueInput->value()  );

  this->SetZeroSetValue( zeroSetValueInput->value() );
  m_ShapeDetectionFilter->SetNumberOfIterations( 
        static_cast<unsigned int>( shapeDetectionIterationsValueInput->value() ) );

  m_ShapeDetectionFilter->SetNarrowBandwidth( 
               shapeDetectionIterationsValueInput->value() );

  m_ShapeDetectionFilter->SetNarrowBanding( 
       static_cast<int>( shapeDetectionNarrowBandingCheckButton->value() ) );

  m_DerivativeFilter->SetSigma( sigmaValueInput->value() );

  m_FastMarchingFilter->SetStoppingValue( fastMarchingStoppingValueInput->value() );

  m_VTKSegmentedImageViewer = VTKImageViewerType::New();
  m_VTKSegmentedImageViewer->SetImage( m_ThresholdFilter->GetOutput() );

  m_OutputLevelSetViewer.SetLabel("Output Level Set");

  // Connect Observers in the GUI 
  inputImageButton->Observe( m_ImageReader.GetPointer() );
  thresholdedImageButton->Observe( m_ThresholdFilter.GetPointer() );
  outputLevelSetButton->Observe( m_ShapeDetectionFilter.GetPointer() );
  gradientMagnitudeButton->Observe( m_DerivativeFilter.GetPointer() );
  edgePotentialButton->Observe( m_SigmoidFilter.GetPointer() );
  fastMarchingResultButton->Observe( m_FastMarchingFilter.GetPointer() );

  progressSlider->Observe( m_CastImageFilter.GetPointer() );
  progressSlider->Observe( m_DerivativeFilter.GetPointer() );
  progressSlider->Observe( m_ThresholdFilter.GetPointer() );
  progressSlider->Observe( m_SigmoidFilter.GetPointer() );
  progressSlider->Observe( m_ImageReader.GetPointer() );
  progressSlider->Observe( m_ShapeDetectionFilter.GetPointer() );
  progressSlider->Observe( m_FastMarchingFilter.GetPointer() );
  progressSlider->Observe( m_InputThresholdFilter.GetPointer() );
  

}



/************************************
 *
 *  Destructor
 *
 ***********************************/
ShapeDetectionLevelSet
::~ShapeDetectionLevelSet()
{

}




/************************************
 *
 * Show main console
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowConsole(void)
{
  consoleWindow->show();
}




/********************************************
 *
 * Quit : requires to hide all fltk windows
 *
 *******************************************/
void
ShapeDetectionLevelSet
::Quit(void)
{
  m_InputImageViewer.Hide();
  m_ThresholdedImageViewer.Hide();
  m_EdgePotentialImageViewer.Hide();
  m_GradientMagnitudeImageViewer.Hide();
  m_OutputLevelSetViewer.Hide();
  m_ZeroSetImageViewer.Hide();
  m_FastMarchingImageViewer.Hide();
  
  m_VTKSegmentedImageViewer->Hide();

  consoleWindow->hide();
}






 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::LoadInputImage( void )
{

  const char * filename = fl_file_chooser("Input Image filename","*.*","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Loading input image file...");
  
  try 
  {
    ShapeDetectionLevelSetBase::LoadInputImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }

  this->ShowStatus("Input Image Loaded");

  controlsGroup->activate();

}



 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowStatus( const char * message )
{
  statusTextOutput->value( message );
  Fl::check();
}




 
/************************************
 *
 *  Show Input Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowInputImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }

  m_CastImageFilter->Update();
  m_InputImageViewer.SetImage( m_CastImageFilter->GetOutput() );  
  m_InputImageViewer.SetOverlay( m_SeedImage );
  m_InputImageViewer.Show();

}




 
/************************************
 *
 *  Show Time Crossing Map Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowOutputLevelSet( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }
  this->RunShapeDetection();
  m_OutputLevelSetViewer.SetImage( m_ShapeDetectionFilter->GetOutput() );  
  m_OutputLevelSetViewer.Show();

}


  


/************************************
 *
 *  Show Gradient Magnitude
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowGradientMagnitudeImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }
  this->ComputeGradientMagnitude();
  m_GradientMagnitudeImageViewer.SetImage( m_DerivativeFilter->GetOutput() );  
  m_GradientMagnitudeImageViewer.Show();

}



 
/************************************
 *
 *  Show The Edge Potential Map
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowEdgePotentialImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }
  this->ComputeEdgePotential();
  m_EdgePotentialImageViewer.SetImage( m_SigmoidFilter->GetOutput() );  
  m_EdgePotentialImageViewer.Show();

}





 
/************************************
 *
 *  Show Thresholded Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowThresholdedImage( void )
{
  m_CastImageFilter->Update();
  m_ThresholdFilter->Update();
  m_ThresholdedImageViewer.SetImage( m_CastImageFilter->GetOutput() );
  m_ThresholdedImageViewer.SetOverlay( m_ThresholdFilter->GetOutput() );  
  m_ThresholdedImageViewer.Show();
  m_ThresholdedImageViewer.SetOverlayOpacity( 0.5 );

}




 
/************************************
 *
 *  Show Fast Marching Result Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowFastMarchingResultImage( void )
{
  m_CastImageFilter->Update();
  m_InputThresholdFilter->Update();
  m_FastMarchingImageViewer.SetImage( m_CastImageFilter->GetOutput() );
  m_FastMarchingImageViewer.SetOverlay( m_InputThresholdFilter->GetOutput() );
  m_FastMarchingImageViewer.Show();
  m_FastMarchingImageViewer.SetOverlayOpacity( 0.5 );
}







 
/************************************
 *
 *  Show Homogeneous Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowThresholdedImageWithVTK( void )
{
  m_VTKSegmentedImageViewer->Show();
}




 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
ShapeDetectionLevelSet
::ClickSelectCallback(float x, float y, float z, float value, void * args )
{

  ShapeDetectionLevelSet * self = 
     static_cast<ShapeDetectionLevelSet *>( args );

  self->SelectSeedPoint( x, y, z );

}



 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
ShapeDetectionLevelSet
::SelectSeedPoint(float x, float y, float z)
{

  typedef SeedImageType::IndexType IndexType;
  IndexType seed;
  seed[0] = static_cast<IndexType::IndexValueType>( x );
  seed[1] = static_cast<IndexType::IndexValueType>( y );
  seed[2] = static_cast<IndexType::IndexValueType>( z );

  ShapeDetectionLevelSetBase::AddSeed( seed );

  m_InputImageViewer.Update();

}


  
  

  
/*****************************************
 *
 *  Clear the seed used to generate the 
 *  initial level set front
 *
 *****************************************/
void
ShapeDetectionLevelSet
::ClearSeeds()
{
  this->ShapeDetectionLevelSetBase::ClearSeeds();
  m_ZeroSetImageViewer.Update();
  m_InputImageViewer.Update();
  Fl::check();
}
  
 


/*  Finaly the main() that will instantiate the application  */
int main()
{

  try 
    {
    ShapeDetectionLevelSet * console = new ShapeDetectionLevelSet();
    console->ShowConsole();
    Fl::run();
    delete console;
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "ITK exception caught in main" << std::endl;
    std::cerr << e << std::endl;
    }
  catch( std::exception & e )
    {
    std::cerr << "STD exception caught in main" << std::endl;
    std::cerr << e.what() << std::endl;
    }
  catch( ... )
    {
    std::cerr << "unknown exception caught in main" << std::endl;
    }


  return 0;

}



