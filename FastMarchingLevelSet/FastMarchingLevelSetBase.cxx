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

  m_SigmoidFilter  = SigmoidFilterType::New();
  m_SigmoidFilter->SetInput( m_DerivativeFilter->GetOutput() );
  m_SigmoidFilter->SetOutputMinimum( 0.0 );
  m_SigmoidFilter->SetOutputMaximum( 1.0 );

  m_FastMarchingFilter = FastMarchingFilterType::New();
  m_FastMarchingFilter->SetInput( m_SigmoidFilter->GetOutput() );


  m_FastMarchingFilter = FastMarchingFilterType::New();
  m_FastMarchingFilter->SetInput( m_SigmoidFilter->GetOutput() );

  m_SeedValue     = 0;

  m_FastMarchingWindowingFilter = FastMarchingWindowingFilterType::New();
  m_FastMarchingWindowingFilter->SetInput( m_FastMarchingFilter->GetOutput() );
  m_FastMarchingWindowingFilter->SetWindowMinimum( m_SeedValue );
  m_FastMarchingWindowingFilter->SetWindowMaximum( 
                                  m_FastMarchingFilter->GetStoppingValue() );
  m_FastMarchingWindowingFilter->SetOutputMinimum( m_SeedValue );
  m_FastMarchingWindowingFilter->SetOutputMaximum( 
                                  m_FastMarchingFilter->GetStoppingValue() );


  m_ThresholdFilter = ThresholdFilterType::New();
  m_ThresholdFilter->SetInput( m_FastMarchingWindowingFilter->GetOutput() );
  m_ThresholdFilter->SetInsideValue(   1 );
  m_ThresholdFilter->SetOutsideValue(  0 );

  m_TrialPoints = NodeContainer::New();
  m_FastMarchingFilter->SetTrialPoints( m_TrialPoints );

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
  m_FastMarchingFilter->Modified();
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
  m_FastMarchingFilter->Modified();
  m_NumberOfSeeds++;
  
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
}


 

/************************************
 *
 *  Compute Edge Potential Image
 *
 ***********************************/
void
FastMarchingLevelSetBase
::ComputeEdgePotential( void )
{
  this->ComputeGradientMagnitude();
  this->ShowStatus("Computing Edge Potential Image");
  m_SigmoidFilter->Update();
}


/***********************************************************
 *
 *   Set the stopping value of the FastMarching and update
 *   the value for the subsequent intensity window filter.
 *
 ***********************************************************/
void
FastMarchingLevelSetBase
::SetStoppingValue( double value )
{
   m_FastMarchingFilter->SetStoppingValue( value );
   m_FastMarchingWindowingFilter->SetWindowMaximum( value );
   m_FastMarchingWindowingFilter->SetOutputMaximum( value );
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

  // update the marching filter
  try
    {
    this->ComputeGradientMagnitude();
    this->ShowStatus("Computing Fast Marching Filter");
    m_FastMarchingWindowingFilter->Update();
    }
  catch( itk::ExceptionObject & exp )
    {
    fl_alert( exp.GetDescription() );
    }


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



