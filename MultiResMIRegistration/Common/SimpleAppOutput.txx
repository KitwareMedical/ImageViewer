/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SimpleAppOutput.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SimpleAppOutput_txx
#define _SimpleAppOutput_txx

#include "SimpleAppOutput.h"

#include "itkResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExceptionObject.h"

#include "PGMVolumeWriter.h"

namespace itk
{

template <typename TImage>
SimpleAppOutput<TImage>
::SimpleAppOutput()
{
  m_FixedImage = NULL;
  m_MovingImage = NULL;
  m_ResampledImage = NULL;

  m_Transform = NULL;

  m_DirectoryName = "";
}


template <typename TImage>
void
SimpleAppOutput<TImage>
::Execute()
{

  if ( !m_MovingImage || !m_FixedImage || !m_Transform ||
    m_DirectoryName == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  // set up the resampler
  typedef AffineTransformType::ScalarType CoordRepType;
  typedef itk::LinearInterpolateImageFunction<ImageType,CoordRepType> 
    InterpolatorType;
  typedef itk::ResampleImageFilter<ImageType,ImageType> ResamplerType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  ResamplerType::Pointer resampler = ResamplerType::New();
  resampler->SetInput( m_MovingImage );

  resampler->SetTransform( m_Transform.GetPointer() );
  resampler->SetInterpolator( interpolator.GetPointer() );
  resampler->SetSize( m_FixedImage->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin( m_FixedImage->GetOrigin() );
  resampler->SetOutputSpacing( m_FixedImage->GetSpacing() );
  resampler->SetDefaultPixelValue( 0 );

  // resample the moving image
  resampler->Update();

  m_ResampledImage = resampler->GetOutput();


  // write out image as pgm files
  typedef PGMVolumeWriter<ImageType> PGMWriterType;
  PGMWriterType::Pointer writer = PGMWriterType::New();

  try
    {

    writer->SetImage( m_FixedImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "target" );
    writer->Execute();

    writer->SetImage( m_MovingImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "source" );
    writer->Execute();

    writer->SetImage( m_ResampledImage );
    writer->SetDirectoryName( m_DirectoryName.c_str() );
    writer->SetFilePrefix( "register" );
    writer->Execute();


    }
  catch(...)
    {
     std::cout << "Error occured while writing PGM files." << std::endl;
     std::cout << "Please make sure path " << m_DirectoryName.c_str()
               << " is valid. " << std::endl;              
     throw;
    }

}


} // namespace itk

#endif
