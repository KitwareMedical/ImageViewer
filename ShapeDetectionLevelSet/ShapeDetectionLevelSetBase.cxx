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

  m_SigmoidFilter  = SigmoidFilterType::New();
  m_SigmoidFilter->SetInput( m_DerivativeFilter->GetOutput() );
  m_SigmoidFilter->SetOutputMaximum( 1.0 );
  m_SigmoidFilter->SetOutputMinimum( 0.0 );

  m_TrialPoints = NodeContainer::New();

  m_FastMarchingFilter = FastMarchingFilterType::New();
  m_FastMarchingFilter->SetSpeedConstant( 1.0 );
  m_FastMarchingFilter->SetTrialPoints( m_TrialPoints );

  m_InputThresholdFilter = ThresholdFilterType::New();
  m_InputThresholdFilter->SetInput( m_FastMarchingFilter->GetOutput() );
  m_InputThresholdFilter->SetUpperThreshold( itk::NumericTraits<InternalPixelType>::Zero ); 
  m_InputThresholdFilter->SetLowerThreshold( itk::NumericTraits<InternalPixelType>::NonpositiveMin() ); 
  m_InputThresholdFilter->SetInsideValue( 1 );
  m_InputThresholdFilter->SetOutsideValue(  0 );

  m_ShapeDetectionFilter = ShapeDetectionFilterType::New();
  m_ShapeDetectionFilter->SetInput(  m_FastMarchingFilter->GetOutput() );
  m_ShapeDetectionFilter->SetEdgeImage(   m_SigmoidFilter->GetOutput() );

  m_ShapeDetectionFilter->NarrowBandingOn();
  m_ShapeDetectionFilter->SetTimeStepSize( 0.125 );

  m_ThresholdFilter = ThresholdFilterType::New();
  m_ThresholdFilter->SetInput( m_ShapeDetectionFilter->GetOutput() );
  m_ThresholdFilter->SetUpperThreshold( itk::NumericTraits<InternalPixelType>::Zero ); 
  m_ThresholdFilter->SetLowerThreshold( itk::NumericTraits<InternalPixelType>::NonpositiveMin() ); 
  m_ThresholdFilter->SetInsideValue(   1 );
  m_ThresholdFilter->SetOutsideValue(  0 );

  m_SeedImage = SeedImageType::New();

  m_SeedValue = 0; // It must be set to the minus distance of the initial level set.

  m_NumberOfSeeds = 0;

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

  m_FastMarchingFilter->SetOutputSize( region.GetSize() );

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
 *  Compute Fast Marching
 *
 ***********************************/
void
ShapeDetectionLevelSetBase
::ComputeFastMarching( void )
{
  this->ShowStatus("Computing Fast Marching");
  m_FastMarchingFilter->Update();
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
  // This will also update the FastMarching
  m_InputThresholdFilter->Update();
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
  m_SigmoidFilter->Update();
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
    this->ComputeEdgePotential();
    this->ComputeFastMarching();
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
  // By starting the FastMarching front at this value,
  // the zero set will end up being placed at distance
  // = value from the seeds. That can be seen as computing
  // a distance map from the seeds.
  m_SeedValue = - value;

  NodeContainer::Iterator nodeItr = m_TrialPoints->Begin();
  NodeContainer::Iterator endNode = m_TrialPoints->End();
  while( nodeItr != endNode )
    {
    nodeItr.Value().SetValue( m_SeedValue );
    ++nodeItr;
    }
  m_FastMarchingFilter->Modified();
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



