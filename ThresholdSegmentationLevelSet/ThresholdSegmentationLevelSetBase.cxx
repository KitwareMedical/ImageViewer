/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThresholdSegmentationLevelSetBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ThresholdSegmentationLevelSetBase.h"
#include <FL/fl_ask.H>


/************************************
 *
 *  Constructor
 *
 ***********************************/
ThresholdSegmentationLevelSetBase 
::ThresholdSegmentationLevelSetBase()
{

  m_ImageReader                  = ImageReaderType::New();

  m_SeedImage = SeedImageType::New();

  m_CastImageFilter = CastImageFilterType::New();
  m_CastImageFilter->SetInput( m_ImageReader->GetOutput() );

  m_ThresholdLevelSetFilter = ThresholdLevelSetImageFilterType::New();
  m_ThresholdLevelSetFilter->SetFeatureImage( m_CastImageFilter->GetOutput() );

  m_ThresholdLevelSetFilter->SetInput( m_SeedImage );

  m_ThresholdLevelSetFilter->SetUpperThreshold(63);
  m_ThresholdLevelSetFilter->SetLowerThreshold(50);

  m_ThresholdLevelSetFilter->SetMaximumRMSError(0.02);
  m_ThresholdLevelSetFilter->SetMaximumIterations(1000);

  m_ThresholdLevelSetFilter->SetIsoSurfaceValue(0.5); 

  m_InputImageIsLoaded  = false;

  m_SeedValue = 1;

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
ThresholdSegmentationLevelSetBase 
::~ThresholdSegmentationLevelSetBase()
{

}



 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
ThresholdSegmentationLevelSetBase 
::LoadInputImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_ImageReader->SetFileName( filename );
  m_ImageReader->Update();
  
  m_InputImageIsLoaded = true;

  m_SeedImage->SetRegions( m_ImageReader->GetOutput()->GetBufferedRegion() );
  m_SeedImage->Allocate();
  m_SeedImage->FillBuffer( itk::NumericTraits<SeedImageType::PixelType>::Zero );

}



 
/************************************
 *
 *  Add a seed
 *
 ***********************************/
void
ThresholdSegmentationLevelSetBase 
::AddSeed( const SeedImageType::IndexType & seed  )
{
  m_SeedImage->SetPixel( seed, m_SeedValue );
}




/************************************
 *
 *  Stop Registration
 *
 ***********************************/
void
ThresholdSegmentationLevelSetBase
::Stop( void )
{
  // TODO: add a Stop() method to Filters

}




