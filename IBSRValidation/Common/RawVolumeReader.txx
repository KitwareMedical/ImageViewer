/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawVolumeReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawVolumeReader_txx
#define _RawVolumeReader_txx

#include "itkImageFileReader.h"
#include "itkRawImageIO.h"

#include "RawVolumeReader.h"

namespace itk
{

template <typename TPixel, typename TImage>
RawVolumeReader<TPixel,TImage>
::RawVolumeReader()
{

  m_FileName = "";
  m_Size.Fill( 0 );
  m_Spacing.Fill( 1.0 );
  m_BigEndian = true;
  m_Image = NULL;

}

template <typename TPixel, typename TImage>
void
RawVolumeReader<TPixel,TImage>
::Execute()
{

  typedef itk::RawImageIO<PixelType,ImageDimension> IOType;
  typename IOType::Pointer io = IOType::New();

  io->SetFileTypeToBinary();
  io->SetFileDimensionality( ImageDimension );

  if (m_BigEndian)
    {
    io->SetByteOrderToBigEndian();
    }
  else
    {
    io->SetByteOrderToLittleEndian();
    }

  for ( unsigned int j = 0; j < ImageDimension; j++ )
    {
    io->SetDimensions(j, m_Size[j]);
    io->SetSpacing(j, m_Spacing[j]);
    }

  typedef itk::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( m_FileName.c_str() );
  reader->SetImageIO( io );
  reader->Update();

  m_Image = reader->GetOutput();

}


} // namespace itk

#endif
