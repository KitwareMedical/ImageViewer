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

  m_InputImageViewer.ClickSelectCallBack( ClickSelectCallback, (void *)this);

  m_ConnectedThresholdImageViewer.SetLabel("Connected Threshold Image");

  m_ConnectedThresholdImageFilter = ConnectedThresholdImageFilterType::New();

  m_ConnectedThresholdImageViewer.SetImage( m_ConnectedThresholdImageFilter->GetOutput() );  

  m_ConnectedThresholdImageFilter->SetInput( m_ImageReader->GetOutput() );

  // Initialize ITK filter with GUI values
  m_ConnectedThresholdImageFilter->SetLower( 
      static_cast<InputPixelType>( lowerThresholdCounter->value() ) );

  m_ConnectedThresholdImageFilter->SetUpper( 
      static_cast<InputPixelType>( upperThresholdCounter->value() ) );

  inputImageButton->Observe( m_ImageReader.GetPointer() );
  loadInputImageButton->Observe(  m_ImageReader.GetPointer() );

  progressSlider->Observe( m_ImageReader.GetPointer() );
  progressSlider->Observe( m_ConnectedThresholdImageFilter.GetPointer() );

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



