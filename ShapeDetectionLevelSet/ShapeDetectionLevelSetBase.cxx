/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectionLevelSetBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ShapeDetectionLevelSetBase.h"
#include <FL/fl_ask.H>


/************************************
 *
 *  Constructor
 *
 ***********************************/
ShapeDetectionLevelSetBase 
::ShapeDetectionLevelSetBase()
{

  m_ImageReader                  = ImageReaderType::New();

  m_CastImageFilter = CastImageFilterType::New();
  m_CastImageFilter->SetInput( m_ImageReader->GetOutput() );

  m_DerivativeFilter   = DerivativeFilterType::New();
  m_DerivativeFilter->SetInput( m_CastImageFilter->GetOutput() );

  m_ExpNegativeFilter  = ExpNegativeFilterType::New();
  m_ExpNegativeFilter->SetInput( m_DerivativeFilter->GetOutput() );

  m_TrialPoints = NodeContainer::New();

  m_FastMarchingFilter = FastMarchingFilterType::New();
  m_FastMarchingFilter->SetTrialPoints( m_TrialPoints );

  m_AddImageFilter = AddImageFilterType::New();
  m_AddImageFilter->SetInput( m_FastMarchingFilter->GetOutput() );

  m_ShapeDetectionFilter = ShapeDetectionFilterType::New();
  m_ShapeDetectionFilter->SetInput(  m_AddImageFilter->GetOutput() );
  m_ShapeDetectionFilter->SetEdgeImage( m_ExpNegativeFilter->GetOutput() );

  m_ThresholdFilter = ThresholdFilterType::New();
  m_ThresholdFilter->SetInput( m_ShapeDetectionFilter->GetOutput() );
  m_ThresholdFilter->SetInsideValue( 200 );
  m_ThresholdFilter->SetOutsideValue(  0 );

  m_SeedImage = SeedImageType::New();
  m_SeedValue = 1;

  m_InputImageIsLoaded  = false;

}



/************************************
 *
 *  Destructor
 *
 ***********************************/
ShapeDetectionLevelSetBase 
::~ShapeDetectionLevelSetBase()
{

}



 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
ShapeDetectionLevelSetBase 
::LoadInputImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_ImageReader->SetFileName( filename );
  m_ImageReader->Update();

  // Allocate a image of seeds of the same size
  InputImageType::RegionType region 
            = m_ImageReader->GetOutput()->GetBufferedRegion();

  m_SeedImage->SetRegions( region );
  m_SeedImage->Allocate();
  m_SeedImage->FillBuffer( itk::NumericTraits<SeedImageType::PixelType>::Zero );

  InternalImageType::Pointer speedImage = InternalImageType::New();
  speedImage->SetRegions( region );
  speedImage->Allocate();
  speedImage->FillBuffer( 1.0 );

  m_FastMarchingFilter->SetOutputSize( region.GetSize() );
  m_FastMarchingFilter->SetSpeedImage( speedImage );

  m_InputImageIsLoaded = true;

}



 

/********************************************
 *
 *  Set the seed. From this seed an Initial
 *  level set image is generated 
 *
 *******************************************/
void
ShapeDetectionLevelSetBase 
::AddSeed( const IndexType & seedPosition  )
{

  // setup trial points
  NodeType node;
  
  node.SetValue( m_SeedValue );
  node.SetIndex( seedPosition );

  m_TrialPoints->InsertElement( m_NumberOfSeeds, node );

  m_SeedImage->SetPixel( seedPosition, 1 );

  m_NumberOfSeeds++;
  
}



  
/************************************
 *
 *  Clear Seeds
 *
 ***********************************/
void
ShapeDetectionLevelSetBase 
::ClearSeeds()
{
  m_TrialPoints->Initialize(); 
  m_NumberOfSeeds = 0;
  m_FastMarchingFilter->Modified();
  m_SeedImage->FillBuffer( itk::NumericTraits<SeedImageType::PixelType>::Zero );
}




/************************************
 *
 *  Compute Gradient Magnitude
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::ComputeGradientMagnitude( void )
{
  this->ShowStatus("Computing Gradient Image");
  m_DerivativeFilter->Update();
}


 
/************************************
 *
 *  Compute Zero Set
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::ComputeZeroSet( void )
{
  this->ShowStatus("Computing Zero Set");
  m_AddImageFilter->Update();
}


 


/************************************
 *
 *  Compute Edge Potential Image
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::ComputeEdgePotential( void )
{
  this->ComputeGradientMagnitude();
  this->ShowStatus("Computing Edge Potential Image");
  m_ExpNegativeFilter->Update();
}


  

/************************************
 *
 *  Start Segmentation
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::RunShapeDetection( void )
{

  // update the marching filter
  try
    {
    this->ComputeGradientMagnitude();
    this->ShowStatus("Computing Shape Detection Filter");
    m_ShapeDetectionFilter->Update();
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
ShapeDetectionLevelSetBase
::SetZeroSetValue( InternalPixelType value )
{
  m_AddImageFilter->GetAccessor().SetValue( - value );
  m_AddImageFilter->Modified();
}



/************************************
 *
 *  Stop Segmentation
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::Stop( void )
{
  // TODO: add a Stop() method to Filters

}



