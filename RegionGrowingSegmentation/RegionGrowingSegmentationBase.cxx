/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RegionGrowingSegmentationBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <RegionGrowingSegmentationBase.h>
#include <FL/fl_ask.H>
#include <itkMetaImageIOFactory.h>
#include <itkRawImageIO.h>


/************************************
 *
 *  Constructor
 *
 ***********************************/
RegionGrowingSegmentationBase 
::RegionGrowingSegmentationBase()
{

  m_ImageReader                  = ImageReaderType::New();

  itk::MetaImageIOFactory::RegisterOneFactory();

  itk::RawImageIOFactory< InputImageType::PixelType, 
                          InputImageType::ImageDimension >::RegisterOneFactory();

  m_CastImageFilter = CastImageFilterType::New();
  m_CastImageFilter->SetInput( m_ImageReader->GetOutput() );

  m_CurvatureFlowImageFilter = CurvatureFlowImageFilterType::New();
  m_CurvatureFlowImageFilter->SetInput( m_CastImageFilter->GetOutput() );

  m_ConnectedThresholdImageFilter = ConnectedThresholdImageFilterType::New();
  m_ConnectedThresholdImageFilter->SetInput( m_CurvatureFlowImageFilter->GetOutput() );

  m_ConfidenceConnectedImageFilter = ConfidenceConnectedImageFilterType::New();
  m_ConfidenceConnectedImageFilter->SetInput( m_CurvatureFlowImageFilter->GetOutput() );

  m_InputImageIsLoaded  = false;

}




/************************************
 *
 *  Destructor
 *
 ***********************************/
RegionGrowingSegmentationBase 
::~RegionGrowingSegmentationBase()
{

}



 
/************************************
 *
 *  Load Input Image
 *
 ***********************************/
void
RegionGrowingSegmentationBase 
::LoadInputImage( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_ImageReader->SetFileName( filename );
  m_ImageReader->Update();
  
  m_InputImageIsLoaded = true;

}




/************************************
 *
 *  Stop Registration
 *
 ***********************************/
void
RegionGrowingSegmentationBase
::Stop( void )
{
  // TODO: add a Stop() method to Filters

}




