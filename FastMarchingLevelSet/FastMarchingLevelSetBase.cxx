/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FastMarchingLevelSetBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "FastMarchingLevelSetBase.h"
#include <FL/fl_ask.H>


/************************************
 *
 *  Constructor
 *
 ***********************************/
FastMarchingLevelSetBase 
::FastMarchingLevelSetBase()
{

  m_ImageReader                  = ImageReaderType::New();

  m_CastImageFilter = CastImageFilterType::New();
  m_CastImageFilter->SetInput( m_ImageReader->GetOutput() );

  m_DerivativeFilter   = DerivativeFilterType::New();
  m_DerivativeFilter->SetInput( m_CastImageFilter->GetOutput() );

  m_MagnitudeFilter    = MagnitudeImageFilterType::New();
  m_MagnitudeFilter->SetInput( m_DerivativeFilter->GetOutput() );

  m_FastMarchingFilter = FastMarchingFilterType::New();
  m_FastMarchingFilter->SetSpeedImage( m_MagnitudeFilter->GetOutput() );

  m_ThresholdFilter = ThresholdFilterType::New();
  m_ThresholdFilter->SetInput( m_FastMarchingFilter->GetOutput() );
  m_ThresholdFilter->SetInsideValue( 200 );
  m_ThresholdFilter->SetOutsideValue(  0 );

  m_TrialPoints = NodeContainer::New();
  m_FastMarchingFilter->SetTrialPoints( m_TrialPoints );

  m_SeedValue     = 0;
  m_NumberOfSeeds = 0;

  m_InputImageIsLoaded  = false;

}



/************************************
 *
 *  Destructor
 *
 ***********************************/
FastMarchingLevelSetBase 
::~FastMarchingLevelSetBase()
{

}



 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
FastMarchingLevelSetBase 
::LoadInputImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_ImageReader->SetFileName( filename );
  m_ImageReader->Update();
  
  m_InputImageIsLoaded = true;

  m_FastMarchingFilter->SetOutputSize( m_ImageReader->GetOutput()->GetBufferedRegion().GetSize() );

}



  
/************************************
 *
 *  Clear Seeds
 *
 ***********************************/
void
FastMarchingLevelSetBase 
::ClearSeeds()
{
  m_TrialPoints->Initialize(); 
  m_NumberOfSeeds = 0;
}




/************************************
 *
 *  Add a seed
 *
 ***********************************/
void
FastMarchingLevelSetBase 
::AddSeed( const IndexType & seedPosition  )
{

  // setup trial points
  NodeType node;
  
  node.SetValue( m_SeedValue );
  node.SetIndex( seedPosition );

  m_TrialPoints->InsertElement( m_NumberOfSeeds, node );
  m_NumberOfSeeds++;
  
  std::cout << "AddSeed( " << seedPosition << " ) " << std::endl;
  std::cout << "m_NumberOfSeeds = " << m_NumberOfSeeds << std::endl;

}




/************************************
 *
 *  Compute Gradient Magnitude
 *
 ***********************************/
void
FastMarchingLevelSetBase
::ComputeGradientMagnitude( void )
{
  this->ShowStatus("Computing Gradient Image");
  m_DerivativeFilter->Update();
  this->ShowStatus("Computing Gradient Magnitude Image");
  m_MagnitudeFilter->Update();
}


   

/************************************
 *
 *  Start Segmentation
 *
 ***********************************/
void
FastMarchingLevelSetBase
::RunFastMarching( void )
{

  std::cout << "Number of Seeds = " << m_TrialPoints->Size() << std::endl;

  // update the marching filter
  try
    {
    this->ComputeGradientMagnitude();
    this->ShowStatus("Computing Fast Marching Filter");
    m_FastMarchingFilter->Update();
    }
  catch( itk::ExceptionObject & exp )
    {
    fl_alert( exp.GetDescription() );
    }
  m_FastMarchingFilter->SetTrialPoints( m_TrialPoints );


}





/************************************
 *
 *  Stop Segmentation
 *
 ***********************************/
void
FastMarchingLevelSetBase
::Stop( void )
{
  // TODO: add a Stop() method to Filters

}



