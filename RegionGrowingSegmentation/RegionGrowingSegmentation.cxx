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

  m_ConnectedThresholdImageViewer.SetLabel("Connected Threshold Image");
  m_ConnectedThresholdImageViewer.SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  

  m_ConfidenceConnectedImageViewer.SetLabel("Confidence Connected Image");
  m_ConfidenceConnectedImageViewer.SetImage( m_ConfidenceConnectedImageFilter->GetOutput() );  

  m_HomogeneousImageViewer.SetLabel("Curvature Flow Image");
  m_HomogeneousImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);
  m_HomogeneousImageViewer.SetImage( m_CurvatureFlowImageFilter->GetOutput() );

  // Initialize ITK filter with GUI values
  m_ConnectedThresholdImageFilter->SetLower( 
      static_cast<InputPixelType>( lowerThresholdCounter->value() ) );

  m_ConnectedThresholdImageFilter->SetUpper( 
      static_cast<InputPixelType>( upperThresholdCounter->value() ) );

  m_CurvatureFlowImageFilter->SetNumberOfIterations(
                                   static_cast<unsigned int>(iterationsValueInput->value()) );

  m_CurvatureFlowImageFilter->SetTimeStep(
                                   timeStep->value() );

  m_ConfidenceConnectedImageFilter->SetMultiplier( multiplierValueInput->value() );

  m_ConfidenceConnectedImageFilter->SetNumberOfIterations( 
      static_cast<InputPixelType>( iterationsConfidenceValueInput->value() ) );


  // Connect Observers in the GUI 
  inputImageButton->Observe( m_ImageReader.GetPointer() );
  loadInputImageButton->Observe(  m_ImageReader.GetPointer() );
  homogeneousImageButton->Observe( m_CurvatureFlowImageFilter.GetPointer() );
  thresholdConnectedImageButton->Observe( m_ConnectedThresholdImageFilter.GetPointer() );
  confidenceConnectedImageButton->Observe( m_ConfidenceConnectedImageFilter.GetPointer() );

  progressSlider->Observe( m_ImageReader.GetPointer() );
  progressSlider->Observe( m_ConnectedThresholdImageFilter.GetPointer() );
  progressSlider->Observe( m_ConfidenceConnectedImageFilter.GetPointer() );
  progressSlider->Observe( m_CurvatureFlowImageFilter.GetPointer() );

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
  m_ConnectedThresholdImageViewer.Hide();
  m_ConfidenceConnectedImageViewer.Hide();
  m_HomogeneousImageViewer.Hide();
  consoleWindow->hide();
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

  const char * filename = fl_file_chooser("Input Image filename","*.mh[da]","");
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
 *  Show Homogeneous Image
 *
 ***********************************/
void
RegionGrowingSegmentation
::ShowHomogeneousImage( void )
{
  m_CurvatureFlowImageFilter->Update();
  m_HomogeneousImageViewer.SetImage( m_CurvatureFlowImageFilter->GetOutput() );  
  m_HomogeneousImageViewer.Show();

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



