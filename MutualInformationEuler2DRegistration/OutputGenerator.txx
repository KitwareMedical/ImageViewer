/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    OutputGenerator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _OutputGenerator_txx
#define _OutputGenerator_txx

#include "OutputGenerator.h"

#include "itkResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExceptionObject.h"

template <typename TImage>
OutputGenerator<TImage>
::OutputGenerator()
{
  m_FixedImage = NULL;
  m_MovingImage = NULL;
  m_ResampledImage = NULL;

  m_Transform = NULL;

  m_DirectoryName = "";
}


template <typename TImage>
void
OutputGenerator<TImage>
::Execute()
{

  if ( !m_MovingImage || !m_FixedImage || !m_Transform ||
    m_DirectoryName == "" )
    {
    itk::ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  // set up the resampler
  typedef AffineTransformType::ScalarType CoordRepType;
  typedef itk::LinearInterpolateImageFunction<ImageType,CoordRepType> 
    InterpolatorType;
  typedef itk::ResampleImageFilter<ImageType,ImageType,
    AffineTransformType,InterpolatorType> ResamplerType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  ResamplerType::Pointer resampler = ResamplerType::New();
  resampler->SetInput( m_MovingImage );

  resampler->SetTransform( m_Transform );
  resampler->SetInterpolator( interpolator );
  resampler->SetSize( m_FixedImage->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin( m_FixedImage->GetOrigin() );
  resampler->SetOutputSpacing( m_FixedImage->GetSpacing() );
  resampler->SetDefaultPixelValue( 0 );

  // resample the moving image
  resampler->Update();

  m_ResampledImage = resampler->GetOutput();
}

#endif
