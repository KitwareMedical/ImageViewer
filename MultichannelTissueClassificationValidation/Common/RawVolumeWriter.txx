/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawVolumeWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawVolumeWriter_txx
#define _RawVolumeWriter_txx

#include "itkImageFileWriter.h"
#include "itkRawImageIO.h"

#include "RawVolumeWriter.h"

namespace itk
{

template <typename TImage>
RawVolumeWriter<TImage>
::RawVolumeWriter()
{

  m_FileName = "";
  m_BigEndian = true;
  m_InputImage = NULL;

}

template <typename TImage>
void
RawVolumeWriter<TImage>
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

  typedef itk::ImageFileWriter<ImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( m_FileName.c_str() );
  writer->SetImageIO( io );
  writer->SetInput( m_InputImage );
  writer->Write();

}


} // namespace itk

#endif
