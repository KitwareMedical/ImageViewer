/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RegionGrowingSegmentation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <RegionGrowingSegmentation.h>
#include <FL/fl_file_chooser.H>




/************************************
 *
 *  Constructor
 *
 ***********************************/
RegionGrowingSegmentation
::RegionGrowingSegmentation()
{

  m_InputImageViewer.SetLabel("Input Image");

  m_CurvatureFlowImageViewer.SetLabel("Curvature Flow Image");
  m_CurvatureFlowImageViewer.SetImage( m_CurvatureFlowImageFilter->GetOutput() );  

  m_GradientAnisotropicDiffusionImageViewer.SetLabel("Gradient Anisotropic Diffusion Image");
  m_GradientAnisotropicDiffusionImageViewer.SetImage( m_GradientAnisotropicDiffusionImageFilter->GetOutput() );  

  m_CurvatureAnisotropicDiffusionImageViewer.SetLabel("Curvature Anisotropic Diffusion Image");
  m_CurvatureAnisotropicDiffusionImageViewer.SetImage( m_CurvatureAnisotropicDiffusionImageFilter->GetOutput() );  

  m_ConnectedThresholdImageViewer.SetLabel("Connected Threshold Image");
  m_ConnectedThresholdImageViewer.SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  

  m_ConfidenceConnectedImageViewer.SetLabel("Confidence Connected Image");
  m_ConfidenceConnectedImageViewer.SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );  

  m_FuzzyConnectedImageViewer.SetLabel("Fuzzy Connected Image");
  m_FuzzyConnectedImageViewer.SetImage( m_FuzzyConnectedImageFilter->GetOutput() );  

  m_HomogeneousImageViewer.SetLabel("Homogeneous Image");
  m_HomogeneousImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);
  m_HomogeneousImageViewer.SetImage( m_NullImageFilter->GetOutput() );

  // Initialize ITK filter with GUI values
  m_ConnectedThresholdImageFilter->SetLower( 
      static_cast<InputPixelType>( lowerThresholdCounter->value() ) );

  m_ConnectedThresholdImageFilter->SetUpper( 
      static_cast<InputPixelType>( upperThresholdCounter->value() ) );

  m_CurvatureFlowImageFilter->SetNumberOfIterations(
         static_cast<unsigned int>(curvatureFlowIterationsValueInput->value()) );

  m_CurvatureFlowImageFilter->SetTimeStep(
                                   curvatureFlowTimeStepValueInput->value() );

  m_CurvatureAnisotropicDiffusionImageFilter->SetNumberOfIterations(
         static_cast<unsigned int>(curvatureAnisotropicDiffusionIterationsValueInput->value()) );

  m_CurvatureAnisotropicDiffusionImageFilter->SetTimeStep(
                                   curvatureAnisotropicDiffusionTimeStepValueInput->value() );

  m_CurvatureAnisotropicDiffusionImageFilter->SetConductanceParameter(
                                   curvatureAnisotropicDiffusionConductanceValueInput->value() );

  m_GradientAnisotropicDiffusionImageFilter->SetNumberOfIterations(
         static_cast<unsigned int>(gradientAnisotropicDiffusionIterationsValueInput->value()) );

  m_GradientAnisotropicDiffusionImageFilter->SetTimeStep(
                                   gradientAnisotropicDiffusionTimeStepValueInput->value() );

  m_GradientAnisotropicDiffusionImageFilter->SetConductanceParameter(
                                   gradientAnisotropicDiffusionConductanceValueInput->value() );

  m_ConfidenceConnectedImageFilter->SetMultiplier( multiplierValueInput->value() );

  m_ConfidenceConnectedImageFilter->SetNumberOfIterations( 
      static_cast<InputPixelType>( iterationsConfidenceValueInput->value() ) );

  m_FuzzyConnectedImageFilter->SetMean( fuzzyMeanValueInput->value() );
  m_FuzzyConnectedImageFilter->SetVariance( fuzzyVarianceValueInput->value() );
  m_FuzzyConnectedImageFilter->SetDiff_Mean( fuzzyDiffMeanValueInput->value() );
  m_FuzzyConnectedImageFilter->SetDiff_Variance( fuzzyDiffVarianceValueInput->value() );


  m_VTKSegmentedImageViewer = VTKImageViewerType::New();
  m_VTKSegmentedImageViewer->SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );

  // Connect Observers in the GUI 
  inputImageButton->Observe( m_ImageReader.GetPointer() );
  homogeneousImageButton->Observe( m_NullImageFilter.GetPointer() );
  thresholdConnectedImageButton->Observe( m_ConnectedThresholdImageFilter.GetPointer() );
  confidenceConnectedImageButton->Observe( m_ConfidenceConnectedImageFilter.GetPointer() );
  fuzzyConnectedImageButton->Observe( m_FuzzyConnectedImageFilter.GetPointer() );
  curvatureFlowImageButton->Observe( m_CurvatureFlowImageFilter.GetPointer() );
  gradientAnisotropicDiffusionImageButton->Observe( m_GradientAnisotropicDiffusionImageFilter.GetPointer() );
  curvatureAnisotropicDiffusionImageButton->Observe( m_CurvatureAnisotropicDiffusionImageFilter.GetPointer() );

  progressSlider->Observe( m_CastImageFilter.GetPointer() );
  progressSlider->Observe( m_NullImageFilter.GetPointer() );
  progressSlider->Observe( m_ImageReader.GetPointer() );
  progressSlider->Observe( m_CurvatureFlowImageFilter.GetPointer() );
  progressSlider->Observe( m_CurvatureAnisotropicDiffusionImageFilter.GetPointer() );
  progressSlider->Observe( m_GradientAnisotropicDiffusionImageFilter.GetPointer() );
  progressSlider->Observe( m_ConnectedThresholdImageFilter.GetPointer() );
  progressSlider->Observe( m_ConfidenceConnectedImageFilter.GetPointer() );
  progressSlider->Observe( m_FuzzyConnectedImageFilter.GetPointer() );

}



/************************************
 *
 *  Destructor
 *
 ***********************************/
RegionGrowingSegmentation
::~RegionGrowingSegmentation()
{

}




/************************************
 *
 * Show main console
 *
 ***********************************/
void
RegionGrowingSegmentation
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
RegionGrowingSegmentation
::Quit(void)
{
  m_InputImageViewer.Hide();
  m_CurvatureFlowImageViewer.Hide();
  m_GradientAnisotropicDiffusionImageViewer.Hide();
  m_CurvatureAnisotropicDiffusionImageViewer.Hide();
  m_ConnectedThresholdImageViewer.Hide();
  m_ConfidenceConnectedImageViewer.Hide();
  m_HomogeneousImageViewer.Hide();
  m_VTKSegmentedImageViewer->Hide();
  consoleWindow->hide();
}



 
/************************************
 *
 *  Write Output Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::WriteOutputImage( void )
{

  const char * filename = fl_file_chooser("Output Image filename","*.*","");
  if( !filename )
  {
    return;
  }

  this->ShowStatus("Writing output image file...");
  
  try 
  {
    RegionGrowingSegmentationBase::WriteOutputImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems writing image");
    return;
  }

  this->ShowStatus("Output Image saved");

}


 
/************************************
 *
 *  Write Confidence Connected Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::WriteConfidenceConnectedImage( void )
{
  m_ImageWriter->SetInput( 
        m_ConfidenceConnectedImageFilter->GetOutput() );

  this->WriteOutputImage();
}

  
/************************************
 *
 *  Write Fuzzy Connectedness Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::WriteFuzzyConnectedImage( void )
{
  m_ImageWriter->SetInput( 
        m_FuzzyConnectedImageFilter->GetOutput() );

  this->WriteOutputImage();
}

 
/************************************
 *
 *  Write Confidence Connected Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::WriteConnectedThresholdImage( void )
{
  m_ImageWriter->SetInput( 
        m_ConnectedThresholdImageFilter->GetOutput() );

  this->WriteOutputImage();
}


/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
RegionGrowingSegmentation
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
    RegionGrowingSegmentationBase::LoadInputImage( filename );
  }
  catch( ... ) 
  {
    this->ShowStatus("Problems reading file format");
    controlsGroup->deactivate();
    return;
  }


  this->ShowStatus("Input Image Loaded");

  controlsGroup->activate();

  InputImageType::RegionType region = m_ImageReader->GetOutput()->GetBufferedRegion();

  InputImageType::IndexType start = region.GetIndex();
  InputImageType::SizeType  size  = region.GetSize();
 
  xStartValueInput->value( start[0] );
  yStartValueInput->value( start[1] );
  zStartValueInput->value( start[2] );

  xEndValueInput->value( start[0]+size[0] );
  yEndValueInput->value( start[1]+size[1] );
  zEndValueInput->value( start[2]+size[2] );

  this->UpdateExtract();

}



 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
RegionGrowingSegmentation
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
RegionGrowingSegmentation
::ShowInputImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }

  m_InputImageViewer.SetImage( m_ImageReader->GetOutput() );  
  m_InputImageViewer.Show();

}




 
/************************************
 *
 *  Show Threshold Connected Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowConnectedThresholdImage( void )
{
  m_ConnectedThresholdImageFilter->Update();
  m_ConnectedThresholdImageViewer.SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  
  m_ConnectedThresholdImageViewer.Show();

}



 
/************************************
 *
 *  Show Confidence Connected Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowConfidenceConnectedImage( void )
{
  m_ConfidenceConnectedImageFilter->Update();
  m_ConfidenceConnectedImageViewer.SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );  
  m_ConfidenceConnectedImageViewer.Show();

}



 
/************************************
 *
 *  Show Fuzzy Connected Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowFuzzyConnectedImage( void )
{
  m_FuzzyConnectedImageFilter->Update();
  m_FuzzyConnectedImageViewer.SetImage( m_FuzzyConnectedImageFilter->GetOutput() );  
  m_FuzzyConnectedImageViewer.Show();

}





 
/************************************
 *
 *  Show Homogeneous Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowHomogeneousImage( void )
{
  m_NullImageFilter->Update();
  m_HomogeneousImageViewer.SetImage( m_NullImageFilter->GetOutput() );  
  m_HomogeneousImageViewer.Show();

}




 
/************************************
 *
 *  Show Homogeneous Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowHomogeneousImageWithVTK( void )
{
  m_VTKSegmentedImageViewer->Show();
}





 
/************************************
 *
 *  Show Curvature Flow Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowCurvatureFlowImage( void )
{
  m_CurvatureFlowImageFilter->Update();
  m_CurvatureFlowImageViewer.SetImage( m_CurvatureFlowImageFilter->GetOutput() );  
  m_CurvatureFlowImageViewer.Show();

}



 
/************************************
 *
 *  Show Gradient Anisotropic Diffusion Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowGradientAnisotropicDiffusionImage( void )
{
  m_GradientAnisotropicDiffusionImageFilter->Update();
  m_GradientAnisotropicDiffusionImageViewer.SetImage( m_GradientAnisotropicDiffusionImageFilter->GetOutput() );  
  m_GradientAnisotropicDiffusionImageViewer.Show();

}



 
/************************************
 *
 *  Show Curvature Anisotropic Diffusion Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowCurvatureAnisotropicDiffusionImage( void )
{
  m_CurvatureAnisotropicDiffusionImageFilter->Update();
  m_CurvatureAnisotropicDiffusionImageViewer.SetImage( m_CurvatureAnisotropicDiffusionImageFilter->GetOutput() );  
  m_CurvatureAnisotropicDiffusionImageViewer.Show();

}






 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
RegionGrowingSegmentation
::ClickSelectCallback(float x, float y, float z, float value, void * args )
{

  RegionGrowingSegmentation * self = 
     static_cast<RegionGrowingSegmentation *>( args );

  self->SelectSeedPoint( x, y, z );

}



 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
RegionGrowingSegmentation
::SelectSeedPoint(float x, float y, float z)
{
  xSeedPointValueOutput->value( x );
  ySeedPointValueOutput->value( y );
  zSeedPointValueOutput->value( z );

  typedef ConnectedThresholdImageFilterType::IndexType IndexType;
  IndexType seed;
  seed[0] = static_cast<IndexType::IndexValueType>( x );
  seed[1] = static_cast<IndexType::IndexValueType>( y );
  seed[2] = static_cast<IndexType::IndexValueType>( z );

  m_ConnectedThresholdImageFilter->SetSeed( seed );
  m_ConfidenceConnectedImageFilter->SetSeed( seed );

}


  
/******************************************************
 *
 *  Define the Extraction region from the input image
 *
 ******************************************************/
void
RegionGrowingSegmentation
::UpdateExtract()
{

  typedef InputImageType::IndexType IndexType;
  typedef InputImageType::SizeType  SizeType;

  IndexType start;
  SizeType  size;
 
  start[0] = static_cast<IndexType::IndexValueType>( xStartValueInput->value() );
  start[1] = static_cast<IndexType::IndexValueType>( yStartValueInput->value() );
  start[2] = static_cast<IndexType::IndexValueType>( zStartValueInput->value() );

  size[0] = static_cast<SizeType::SizeValueType>( xEndValueInput->value() ) - start[0];
  size[1] = static_cast<SizeType::SizeValueType>( yEndValueInput->value() ) - start[1];
  size[2] = static_cast<SizeType::SizeValueType>( zEndValueInput->value() ) - start[2];

  InputImageType::RegionType region;

  region.SetIndex( start );
  region.SetSize( size );

  m_ExtractImageFilter->SetExtractionRegion( region );

}



/*  Finaly the main() that will instantiate the application  */
int main()
{

  try 
    {
    RegionGrowingSegmentation * console = new RegionGrowingSegmentation();
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



