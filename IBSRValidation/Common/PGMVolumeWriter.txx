/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    PGMVolumeWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _PGMVolumeWriter_txx
#define _PGMVolumeWriter_txx


#include "PGMVolumeWriter.h"

#include "itkImageRegionIterator.h"
#include "itkStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkExceptionObject.h"


namespace itk
{

template <typename TImage>
PGMVolumeWriter<TImage>
::PGMVolumeWriter()
{
  m_Image = NULL;
  m_DirectoryName = "";
  m_FilePrefix = "";
  
}


template <typename TImage>
void
PGMVolumeWriter<TImage>
::Execute()
{
  if ( !m_Image || m_DirectoryName == "" || m_FilePrefix == "" )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Not all the inputs are valid." );
    throw err;
    }

  // Rescale the image so the intensity is between 0 and 255
  // and cast it to an unsigned char image
  typedef Image<unsigned char, ImageDimension> CharImageType;
  typedef Image<float, ImageDimension> FloatImageType;
  typedef StatisticsImageFilter<ImageType> StatsCalculatorType;
  typedef ShiftScaleImageFilter<ImageType,FloatImageType> RescalerType;
  typedef CastImageFilter<FloatImageType,CharImageType> CasterType;

  typename StatsCalculatorType::Pointer statsCalculator = StatsCalculatorType::New();
  typename RescalerType::Pointer rescaler = RescalerType::New();
  typename CasterType::Pointer caster = CasterType::New();

  statsCalculator->SetInput( m_Image );
  statsCalculator->Update();

  double scale = static_cast<double>( statsCalculator->GetMaximum() - 
    statsCalculator->GetMinimum() ) / 255.0;
  if ( !scale ) { scale = 1.0; }
  scale = 1.0 / scale;

  rescaler->SetInput( m_Image );
  rescaler->SetScale( scale );
  rescaler->SetShift( -1.0 * static_cast<double>( statsCalculator->GetMinimum() ) );
  
  caster->SetInput( rescaler->GetOutput() );
  caster->Update();

  // Write out images as a sequence of 2D slices
  unsigned long ncol = caster->GetOutput()->GetBufferedRegion().GetSize()[0];
  unsigned long nrow = 1;
  if ( ImageDimension > 1 )
    {
    nrow = caster->GetOutput()->GetBufferedRegion().GetSize()[1];
    }

  unsigned long pixelsPerSlice = ncol * nrow;
  unsigned long nslice = 
    caster->GetOutput()->GetBufferedRegion().GetNumberOfPixels() / pixelsPerSlice;

  char filename[256];
  char buffer[50];

  sprintf( buffer, "P5 %ld %ld 255\n", ncol, nrow );
  unsigned int nchar = strlen( buffer );

  typedef ImageRegionIterator<CharImageType> Iterator;
  Iterator iter( caster->GetOutput(), caster->GetOutput()->GetBufferedRegion() );

  for ( unsigned int k = 0; k < nslice; k++ )
    {
    // NOTE: "%03d" generates a 3-digit number with leading zeros
    sprintf(filename, "%s/%s%03d.pgm", m_DirectoryName.c_str(), 
      m_FilePrefix.c_str(), k );
    
    // open the stream
    std::ofstream imgStream( filename, std::ios::out | std::ios::binary );
    
    if( !imgStream.is_open() )
      {
      ExceptionObject err(__FILE__, __LINE__);
      err.SetLocation( "Execute()" );
      err.SetDescription( "Can't open file stream." );
      throw err;
      }

    // write the header
    imgStream.write( buffer, nchar );

    // write the bytes
    for( unsigned long i = 0; i < pixelsPerSlice; i++ )
      {
      imgStream.put( iter.Get() );
      ++iter;
      }

    // close this file
    imgStream.close();

    }
  
}


} // namespace itk

#endif
