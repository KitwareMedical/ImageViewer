/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThresholdSegmentationLevelSet.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <ThresholdSegmentationLevelSet.h>
#include <FL/fl_file_chooser.H>





/************************************
 *
 *  Constructor
 *
 ***********************************/
ThresholdSegmentationLevelSet
::ThresholdSegmentationLevelSet()
{

  m_InputImageViewer.SetLabel("Input Image");

  m_ThresholdedImageViewer.SetLabel("Thresholded Image");
  m_ThresholdedImageViewer.SetImage( m_ThresholdLevelSetFilter->GetOutput() );  

  m_InputImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);

  // Initialize ITK filter with GUI values
  m_ThresholdLevelSetFilter->SetLowerThreshold( 
      static_cast<InternalPixelType>( lowerThresholdValueInput->value() ) );

  m_ThresholdLevelSetFilter->SetUpperThreshold( 
      static_cast<InputPixelType>( upperThresholdValueInput->value() ) );

  m_VTKSegmentedImageViewer = VTKImageViewerType::New();
  m_VTKSegmentedImageViewer->SetImage( m_ThresholdLevelSetFilter->GetOutput() );

  // Connect Observers in the GUI 
  inputImageButton->Observe( m_ImageReader.GetPointer() );
  thresholdedImageButton->Observe( m_ThresholdLevelSetFilter.GetPointer() );

  progressSlider->Observe( m_CastImageFilter.GetPointer() );
  progressSlider->Observe( m_ThresholdLevelSetFilter.GetPointer() );
  progressSlider->Observe( m_ImageReader.GetPointer() );
  
  typedef itk::SimpleMemberCommand< ThresholdSegmentationLevelSet > SimpleCommandType;

  SimpleCommandType::Pointer iterationCommand = SimpleCommandType::New();
  iterationCommand->SetCallbackFunction( this, 
      & ThresholdSegmentationLevelSet::UpdateGUIAfterIteration );

  m_ThresholdLevelSetFilter->AddObserver( itk::IterationEvent(), iterationCommand );      

  /** synchronize the filter with the parameters in the GUI */
  m_ThresholdLevelSetFilter->SetMaximumIterations( 
          static_cast<int>( maxIterationsValueInput->value() ) );
  m_ThresholdLevelSetFilter->SetLowerThreshold( lowerThresholdValueInput->value() );
  m_ThresholdLevelSetFilter->SetUpperThreshold( upperThresholdValueInput->value() );
  m_ThresholdLevelSetFilter->SetIsoSurfaceValue( isoSurfaceValueInput->value() );
  m_ThresholdLevelSetFilter->SetMaximumRMSError( maxRMSValueInput->value() );

}



/************************************
 *
 *  Destructor
 *
 ***********************************/
ThresholdSegmentationLevelSet
::~ThresholdSegmentationLevelSet()
{

}




/************************************
 *
 * Show main console
 *
 ***********************************/
void
ThresholdSegmentationLevelSet
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
ThresholdSegmentationLevelSet
::Quit(void)
{
  m_InputImageViewer.Hide();
  m_ThresholdedImageViewer.Hide();
  m_VTKSegmentedImageViewer->Hide();
  consoleWindow->hide();
}






 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
ThresholdSegmentationLevelSet
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
    ThresholdSegmentationLevelSetBase::LoadInputImage( filename );
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
ThresholdSegmentationLevelSet
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
ThresholdSegmentationLevelSet
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
 *  Show Thresholded Image
 *
 ***********************************/
void
ThresholdSegmentationLevelSet
::ShowThresholdedImage( void )
{
  m_ThresholdLevelSetFilter->Update();
  m_ThresholdedImageViewer.SetImage( m_ThresholdLevelSetFilter->GetOutput() );  
  m_ThresholdedImageViewer.Show();

}





 
/************************************
 *
 *  Show Homogeneous Image
 *
 ***********************************/
void
ThresholdSegmentationLevelSet
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
ThresholdSegmentationLevelSet
::ClickSelectCallback(float x, float y, float z, float value, void * args )
{

  ThresholdSegmentationLevelSet * self = 
     static_cast<ThresholdSegmentationLevelSet *>( args );

  self->SelectSeedPoint( x, y, z );

}



 
/*****************************************
 *
 *  Callback for Selecting a seed point
 *
 *****************************************/
void
ThresholdSegmentationLevelSet
::SelectSeedPoint(float x, float y, float z)
{

  typedef SeedImageType::IndexType IndexType;
  IndexType seed;

  seed[0] = static_cast<IndexType::IndexValueType>( x );
  seed[1] = static_cast<IndexType::IndexValueType>( y );
  seed[2] = static_cast<IndexType::IndexValueType>( z );

  ThresholdSegmentationLevelSetBase::AddSeed( seed );

  m_InputImageViewer.Update();

}


 
 
/*****************************************
 *
 *  Update GUI after iteration
 *
 *****************************************/
void
ThresholdSegmentationLevelSet
::UpdateGUIAfterIteration()
{
  static unsigned int iterationCounter = 0;
  rmsValueOutput->value( m_ThresholdLevelSetFilter->GetRMSChange() );
  iterationValueOutput->value( iterationCounter );
  iterationCounter++;
  m_InputImageViewer.Update();
  Fl::check();
}



 


/*  Finaly the main() that will instantiate the application  */
int main()
{

  try 
    {
    ThresholdSegmentationLevelSet * console = new ThresholdSegmentationLevelSet();
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



