  /*=========================================================================

    Program:   Insight Segmentation & Registration Toolkit
    Module:    RegionGrowingSegmentation2D.cxx
    Language:  C++
    Date:      $Date$
    Version:   $Revision$

    Copyright (c) 2002 Insight Consortium. All rights reserved.
    See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

       This software is distributed WITHOUT ANY WARRANTY; without even 
       the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
       PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/

  #include <RegionGrowingSegmentation2D.h>
  #include <FL/fl_file_chooser.H>




  /************************************
   *
   *  Constructor
   *
   ***********************************/
  RegionGrowingSegmentation2D
  ::RegionGrowingSegmentation2D()
  {

    m_CurvatureFlowImageViewer                  = InternalImageViewerType::New();
    m_GradientAnisotropicDiffusionImageViewer   = InternalImageViewerType::New();
    m_CurvatureAnisotropicDiffusionImageViewer  = InternalImageViewerType::New();
    m_ConnectedThresholdImageViewer             = InternalImageViewerType::New();
    m_ConfidenceConnectedImageViewer            = InternalImageViewerType::New();
    m_FuzzyConnectedImageViewer                 = InternalImageViewerType::New();
    m_HomogeneousImageViewer                    = InternalImageViewerType::New();
    m_ComposedImageViewer                       = InternalImageViewerType::New();
    m_InputImageViewer                          = InputImageViewerType::New();

    m_CurvatureFlowImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_GradientAnisotropicDiffusionImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_CurvatureAnisotropicDiffusionImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_ConnectedThresholdImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_ConfidenceConnectedImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_FuzzyConnectedImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_HomogeneousImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_ComposedImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );
    m_InputImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );

    m_InputImageViewer->SetLabel("Input Image");

    m_CurvatureFlowImageViewer->SetLabel("Curvature Flow Image");
    m_CurvatureFlowImageViewer->SetImage( m_CurvatureFlowImageFilter->GetOutput() );  

    m_GradientAnisotropicDiffusionImageViewer->SetLabel("Gradient Anisotropic Diffusion Image");
    m_GradientAnisotropicDiffusionImageViewer->SetImage( m_GradientAnisotropicDiffusionImageFilter->GetOutput() );  

    m_CurvatureAnisotropicDiffusionImageViewer->SetLabel("Curvature Anisotropic Diffusion Image");
    m_CurvatureAnisotropicDiffusionImageViewer->SetImage( m_CurvatureAnisotropicDiffusionImageFilter->GetOutput() );  

    m_ConnectedThresholdImageViewer->SetLabel("Connected Threshold Image");
    m_ConnectedThresholdImageViewer->SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  

    m_ConfidenceConnectedImageViewer->SetLabel("Confidence Connected Image");
    m_ConfidenceConnectedImageViewer->SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );  

    m_FuzzyConnectedImageViewer->SetLabel("Fuzzy Connected Image");
    m_FuzzyConnectedImageViewer->SetImage( m_FuzzyConnectedImageFilter->GetOutput() );  

    m_HomogeneousImageViewer->SetLabel("Homogeneous Image");

    m_HomogeneousImageViewer->SetClickCallBack( (void *)this, ClickSelectCallback );
    m_HomogeneousImageViewer->SetImage( m_NullImageFilter->GetOutput() );
    m_HomogeneousImageViewer->SetInteractionMode( fltk::Image2DViewerWindow::ClickMode );

    m_ComposedImageViewer->SetLabel("Overlaped Contours");
    m_ComposedImageViewer->SetImage( m_MaximumImageFilter->GetOutput() );

    // Initialize ITK filter with GUI values
    m_ConnectedThresholdImageFilter->SetLower( 
        static_cast<InputPixelType>( lowerThresholdCounter->value() ) );

    m_ConnectedThresholdImageFilter->SetUpper( 
        static_cast<InputPixelType>( upperThresholdCounter->value() ) );

    m_CurvatureFlowImageFilter->SetNumberOfIterations(
           static_cast<unsigned int>(curvatureFlowIterationsValueInput->value()) );

    m_CurvatureFlowImageFilter->SetTimeStep(
                                     curvatureFlowTimeStepValueInput->value() );

    m_CurvatureAnisotropicDiffusionImageFilter->SetIterations(
           static_cast<unsigned int>(curvatureAnisotropicDiffusionIterationsValueInput->value()) );

    m_CurvatureAnisotropicDiffusionImageFilter->SetTimeStep(
                                     curvatureAnisotropicDiffusionTimeStepValueInput->value() );

    m_CurvatureAnisotropicDiffusionImageFilter->SetConductanceParameter(
                                     curvatureAnisotropicDiffusionConductanceValueInput->value() );

    m_GradientAnisotropicDiffusionImageFilter->SetIterations(
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

    // Connect Observers in the GUI 
    inputImageButton->Observe( m_ImageReader.GetPointer() );
    homogeneousImageButton->Observe( m_NullImageFilter.GetPointer() );
    thresholdConnectedImageButton->Observe( m_ConnectedThresholdImageFilter.GetPointer() );
    confidenceConnectedImageButton->Observe( m_ConfidenceConnectedImageFilter.GetPointer() );
    composedImageButton->Observe( m_SobelImageFilter.GetPointer() );
    composedImageButton->Observe( m_MaximumImageFilter.GetPointer() );
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
    progressSlider->Observe( m_SobelImageFilter.GetPointer() );
    progressSlider->Observe( m_MaximumImageFilter.GetPointer() );

  }



  /************************************
   *
   *  Destructor
   *
   ***********************************/
  RegionGrowingSegmentation2D
  ::~RegionGrowingSegmentation2D()
  {

  }




  /************************************
   *
   * Show main console
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
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
  RegionGrowingSegmentation2D
  ::Quit(void)
  {
    m_InputImageViewer->Hide();
    m_CurvatureFlowImageViewer->Hide();
    m_GradientAnisotropicDiffusionImageViewer->Hide();
    m_CurvatureAnisotropicDiffusionImageViewer->Hide();
    m_ConnectedThresholdImageViewer->Hide();
    m_ConfidenceConnectedImageViewer->Hide();
    m_HomogeneousImageViewer->Hide();
    m_ComposedImageViewer->Hide();
    consoleWindow->hide();
  }






   
  /************************************
   *
   *  Load Input Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
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
      RegionGrowingSegmentationBase2D::LoadInputImage( filename );
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
  RegionGrowingSegmentation2D
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
  RegionGrowingSegmentation2D
  ::ShowInputImage( void )
  {

    if( !m_InputImageIsLoaded )
      {
      return;
      }

    m_InputImageViewer->SetImage( m_ImageReader->GetOutput() );  
    m_InputImageViewer->Show();

  }




   
  /************************************
   *
   *  Show Threshold Connected Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowConnectedThresholdImage( void )
  {
    m_ConnectedThresholdImageFilter->Update();
    m_ConnectedThresholdImageViewer->SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  
    m_ConnectedThresholdImageViewer->Show();

  }



   
  /************************************
   *
   *  Show Confidence Connected Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowConfidenceConnectedImage( void )
  {
    m_ConfidenceConnectedImageFilter->Update();
    m_ConfidenceConnectedImageViewer->SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );  
    m_ConfidenceConnectedImageViewer->Show();

  }


   
  /************************************
   *
   *  Show Composed Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowComposedImage( void )
  {
    m_MaximumImageFilter->Update();
    m_ComposedImageViewer->SetImage( m_MaximumImageFilter->GetOutput() );  
    m_ComposedImageViewer->Show();

  }




   
  /************************************
   *
   *  Show Fuzzy Connected Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowFuzzyConnectedImage( void )
  {
    m_FuzzyConnectedImageFilter->Update();
    m_FuzzyConnectedImageViewer->SetImage( m_FuzzyConnectedImageFilter->GetOutput() );  
    m_FuzzyConnectedImageViewer->Show();

  }





   
  /************************************
   *
   *  Show Homogeneous Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowHomogeneousImage( void )
  {
    m_NullImageFilter->Update();
    m_HomogeneousImageViewer->SetImage( m_NullImageFilter->GetOutput() );  
    m_HomogeneousImageViewer->Show();

  }




   
  /************************************
   *
   *  Show Curvature Flow Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowCurvatureFlowImage( void )
  {
    m_CurvatureFlowImageFilter->Update();
    m_CurvatureFlowImageViewer->SetImage( m_CurvatureFlowImageFilter->GetOutput() );  
    m_CurvatureFlowImageViewer->Show();

  }



   
  /************************************
   *
   *  Show Gradient Anisotropic Diffusion Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowGradientAnisotropicDiffusionImage( void )
  {
    m_GradientAnisotropicDiffusionImageFilter->Update();
    m_GradientAnisotropicDiffusionImageViewer->SetImage( m_GradientAnisotropicDiffusionImageFilter->GetOutput() );  
    m_GradientAnisotropicDiffusionImageViewer->Show();

  }



   
  /************************************
   *
   *  Show Curvature Anisotropic Diffusion Image
   *
   ***********************************/
  void
  RegionGrowingSegmentation2D
  ::ShowCurvatureAnisotropicDiffusionImage( void )
  {
    m_CurvatureAnisotropicDiffusionImageFilter->Update();
    m_CurvatureAnisotropicDiffusionImageViewer->SetImage( m_CurvatureAnisotropicDiffusionImageFilter->GetOutput() );  
    m_CurvatureAnisotropicDiffusionImageViewer->Show();

  }






   
  /*****************************************
   *
   *  Callback for Selecting a seed point
   *
   *****************************************/
  void
  RegionGrowingSegmentation2D
  ::ClickSelectCallback(void * objectPtr, int & x, int & y )
  {

    RegionGrowingSegmentation2D * self = 
       static_cast<RegionGrowingSegmentation2D *>( objectPtr );

    self->SelectSeedPoint( x, y );

}



 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
RegionGrowingSegmentation2D
::SelectSeedPoint(int x, int y )
{
  xSeedPointValueOutput->value( x );
  ySeedPointValueOutput->value( y );

  typedef ConnectedThresholdImageFilterType::IndexType IndexType;
  IndexType seed;
  seed[0] = static_cast<IndexType::IndexValueType>( x );
  seed[1] = static_cast<IndexType::IndexValueType>( y );

  vSeedPointValueOutput->value( m_NullImageFilter->GetOutput()->GetPixel( seed ) );

  m_ConnectedThresholdImageFilter->SetSeed( seed );
  m_ConfidenceConnectedImageFilter->SetSeed( seed );

}


  


/*  Finaly the main() that will instantiate the application  */
int main()
{

  try 
    {
    RegionGrowingSegmentation2D * console = new RegionGrowingSegmentation2D();
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



