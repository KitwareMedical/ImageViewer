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

  m_ZeroSetImageViewer.SetLabel("Zero Set Image");

  m_GradientMagnitudeImageViewer.SetLabel("Gradient Magnitude Image");

  m_EdgePotentialImageViewer.SetLabel("Edge Potential Image");

  m_InputImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);

  // Initialize ITK filter with GUI values
  m_ThresholdFilter->SetLowerThreshold( 
      static_cast<InternalPixelType>( lowerThresholdValueInput->value() ) );

  m_ThresholdFilter->SetUpperThreshold( 
      static_cast<InputPixelType>( upperThresholdValueInput->value() ) );

  m_DerivativeFilter->SetSigma( sigmaValueInput->value() );

  m_AddImageFilter->GetAccessor().SetValue( zeroSetValueInput->value() );

  m_VTKSegmentedImageViewer = VTKImageViewerType::New();
  m_VTKSegmentedImageViewer->SetImage( m_ThresholdFilter->GetOutput() );

  m_TimeCrossingMapViewer.SetLabel("Time Crossing Map");

  // Connect Observers in the GUI 
  inputImageButton->Observe( m_ImageReader.GetPointer() );
  thresholdedImageButton->Observe( m_ThresholdFilter.GetPointer() );
  timeCrossingButton->Observe( m_ShapeDetectionFilter.GetPointer() );
  gradientMagnitudeButton->Observe( m_DerivativeFilter.GetPointer() );
  edgePotentialButton->Observe( m_ExpNegativeFilter.GetPointer() );
  zeroSetButton->Observe( m_AddImageFilter.GetPointer() );
  zeroSetButton->Observe( m_FastMarchingFilter.GetPointer() );

  progressSlider->Observe( m_CastImageFilter.GetPointer() );
  progressSlider->Observe( m_DerivativeFilter.GetPointer() );
  progressSlider->Observe( m_ThresholdFilter.GetPointer() );
  progressSlider->Observe( m_ExpNegativeFilter.GetPointer() );
  progressSlider->Observe( m_ImageReader.GetPointer() );
  progressSlider->Observe( m_ShapeDetectionFilter.GetPointer() );
  progressSlider->Observe( m_AddImageFilter.GetPointer() );
  
  typedef itk::SimpleMemberCommand< ShapeDetectionLevelSet > SimpleCommandType;

  SimpleCommandType::Pointer fastMarchingIterationCommand = SimpleCommandType::New();
  fastMarchingIterationCommand->SetCallbackFunction( this, 
      & ShapeDetectionLevelSet::UpdateGUIAfterFastMarchingIteration );

  m_FastMarchingFilter->AddObserver( itk::IterationEvent(), 
                                       fastMarchingIterationCommand );     


  SimpleCommandType::Pointer shapeDetectionIterationCommand = SimpleCommandType::New();
  shapeDetectionIterationCommand->SetCallbackFunction( this, 
      & ShapeDetectionLevelSet::UpdateGUIAfterShapeDetectionIteration );

  m_ShapeDetectionFilter->AddObserver( itk::IterationEvent(), 
                                       shapeDetectionIterationCommand );     

  SimpleCommandType::Pointer shapeDetectionStartCommand = SimpleCommandType::New();
  shapeDetectionStartCommand->SetCallbackFunction( this, 
      & ShapeDetectionLevelSet::CommandOnStartShapeDetection );

  m_ShapeDetectionFilter->AddObserver( itk::StartEvent(), shapeDetectionStartCommand );      

  SimpleCommandType::Pointer fastMarchingStartCommand = SimpleCommandType::New();
  fastMarchingStartCommand->SetCallbackFunction( this, 
      & ShapeDetectionLevelSet::CommandOnStartShapeDetection );

  m_ShapeDetectionFilter->AddObserver( itk::StartEvent(), fastMarchingStartCommand );      



  m_ThresholdFilter->SetLowerThreshold( lowerThresholdValueInput->value() );
  m_ThresholdFilter->SetUpperThreshold( upperThresholdValueInput->value() );

  m_ShapeDetectionIterationCounter = 0;
  m_FastMarchingIterationCounter   = 0;

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
  m_TimeCrossingMapViewer.Hide();
  m_ZeroSetImageViewer.Hide();
  
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
::ShowTimeCrossingMapImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }
  this->RunShapeDetection();
  m_TimeCrossingMapViewer.SetImage( m_ShapeDetectionFilter->GetOutput() );  
  m_TimeCrossingMapViewer.Show();

}


  
/************************************
 *
 *  Show Zero Set Image
 *
 ***********************************/
void
ShapeDetectionLevelSet
::ShowZeroSetImage( void )
{

  if( !m_InputImageIsLoaded )
    {
    return;
    }
  this->ComputeZeroSet();
  m_ZeroSetImageViewer.SetImage( m_AddImageFilter->GetOutput() );  
  m_ZeroSetImageViewer.Show();

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
  m_EdgePotentialImageViewer.SetImage( m_ExpNegativeFilter->GetOutput() );  
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
  m_ThresholdFilter->Update();
  m_ThresholdedImageViewer.SetImage( m_ThresholdFilter->GetOutput() );  
  m_ThresholdedImageViewer.SetOverlay( m_SeedImage );
  m_ThresholdedImageViewer.Show();

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
 *  Command to be run when the ShapeDetection
 *  filter starts execution
 *
 *****************************************/
void
ShapeDetectionLevelSet
::CommandOnStartShapeDetection()
{
  m_ShapeDetectionIterationCounter = 0;
  shapeDetectionIterationValueOutput->value( m_ShapeDetectionIterationCounter );
  Fl::check();
}



  
/*****************************************
 *
 *  Command to be run when the FastMarching
 *  filter starts execution
 *
 *****************************************/
void
ShapeDetectionLevelSet
::CommandOnStartFastMarching()
{
  m_FastMarchingIterationCounter = 0;
  fastMarchingIterationValueOutput->value( 
                    m_FastMarchingIterationCounter );
  Fl::check();
}



 
/*****************************************
 *
 *  Update GUI after iteration
 *
 *****************************************/
void
ShapeDetectionLevelSet
::UpdateGUIAfterShapeDetectionIteration()
{
  shapeDetectionIterationValueOutput->value( m_ShapeDetectionIterationCounter );
  m_ShapeDetectionIterationCounter++;
  Fl::check();
}



  
/*****************************************
 *
 *  Update GUI after fast marching iteration
 *
 *****************************************/
void
ShapeDetectionLevelSet
::UpdateGUIAfterFastMarchingIteration()
{
  fastMarchingIterationValueOutput->value( m_FastMarchingIterationCounter );
  m_FastMarchingIterationCounter++;
  Fl::check();
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



