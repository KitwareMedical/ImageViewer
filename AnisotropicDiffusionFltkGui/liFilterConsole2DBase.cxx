/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole2DBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <liFilterConsole2DBase.h>



/************************************
 *
 *  Constructor
 *
 ***********************************/
liFilterConsole2DBase 
::liFilterConsole2DBase()
{

  m_ImageFileNameAvailable = false;

  m_Reader = VolumeReaderType::New();
  
  m_FilterG = GradientAnisotropicDiffusionFilterType::New();
  m_FilterG->SetNumberOfIterations( 10 );
  m_FilterG->SetTimeStep( 0.25 );
  m_FilterG->SetConductanceParameter( 1.0 );
  m_FilterG->SetInput( m_Reader->GetOutput() );

  m_FilterC = CurvatureAnisotropicDiffusionFilterType::New();
  m_FilterC->SetNumberOfIterations( 10 );
  m_FilterC->SetTimeStep( 0.25 );
  m_FilterC->SetConductanceParameter( 1.0 );
  m_FilterC->SetInput( m_Reader->GetOutput() );

  m_Rescaler  = RescaleIntensityImageFilterType::New();
  m_Rescaler->SetOutputMinimum( 0 );
  m_Rescaler->SetOutputMaximum( 255 );
  m_Rescaler->SetInput(  m_FilterG->GetOutput()  );

  m_Writer   = VolumeWriterType::New();

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
liFilterConsole2DBase 
::~liFilterConsole2DBase()
{

}



 
/************************************
 *
 *  Load
 *
 ***********************************/
void
liFilterConsole2DBase 
::Load( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_Reader->SetFileName( filename );

  m_ImageFileNameAvailable = true;

}


 
/************************************
 *
 *  Save Gradient Image
 *
 ***********************************/
void
liFilterConsole2DBase 
::SaveGradientImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_Rescaler->SetInput( m_FilterG->GetOutput() );
  m_Rescaler->Update();

  m_Writer->SetFileName( filename );
  m_Writer->SetInput(  m_Rescaler->GetOutput()  );
  //m_Writer->SetInput(  m_FilterG->GetOutput()  );
  m_Writer->Update();

}

/************************************
 *
 *  Save Curvature Image
 *
 ***********************************/
void
liFilterConsole2DBase 
::SaveCurvatureImage( const char * filename )
{
  if( !filename )
  {
    return;
  }
  m_Rescaler->SetInput( m_FilterC->GetOutput() );
  m_Rescaler->Update();

  m_Writer->SetFileName( filename );
  m_Writer->SetInput(  m_Rescaler->GetOutput()  );
  //m_Writer->SetInput(  m_FilterC->GetOutput()  );
  m_Writer->Update();

}

 
/************************************
 *
 *  Show Progress
 *
 ***********************************/
void
liFilterConsole2DBase 
::ShowProgress( float )
{

}


 
/************************************
 *
 *  Show Status
 *
 ***********************************/
void
liFilterConsole2DBase 
::ShowStatus( const char * )
{

}


/************************************
 *
 *  Show Gradient Image
 *
 ***********************************/
void
liFilterConsole2DBase 
::ShowGradientImage()
{

}

/************************************
 *
 *  Show Curvature Image
 *
 ***********************************/
void
liFilterConsole2DBase 
::ShowCurvatureImage()
{

}
 
/************************************
 *
 *  Set Iterations For Gradient
 *
 ***********************************/
void
liFilterConsole2DBase 
::SetIterationsForGradient( double value )
{
  m_FilterG->SetNumberOfIterations( (unsigned int)value );
}

/************************************
 *
 *  Set Conductance Parameter For Gradient
 *
 ***********************************/
void
liFilterConsole2DBase 
::SetConductanceForGradient( double value )
{
  m_FilterG->SetConductanceParameter( value );
}

/************************************
 *
 *  Set Iterations For Curvature
 *
 ***********************************/
void
liFilterConsole2DBase 
::SetIterationsForCurvature( double value )
{
  m_FilterC->SetNumberOfIterations( (unsigned int)value );
}

/************************************
 *
 *  Set Conductance Parameter For Curvature
 *
 ***********************************/
void
liFilterConsole2DBase 
::SetConductanceForCurvature( double value )
{
  m_FilterC->SetConductanceParameter( value );
}

