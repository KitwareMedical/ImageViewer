/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawSliceVolumeReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawSliceVolumeReader_txx
#define _RawSliceVolumeReader_txx

#include "itkRawImageSequenceReader.h"
#include "itkRawImageIO.h"

#include "RawSliceVolumeReader.h"
#include "itkByteSwapper.h"

namespace itk
{

template <typename TPixel, typename TImage>
RawSliceVolumeReader<TPixel,TImage>
::RawSliceVolumeReader()
{

  m_FilePrefix = "";
  m_FilePattern = "%d";
  m_Size.Fill( 0 );
  m_Spacing.Fill( 1.0 );
  m_BigEndian = true;
  m_Image = NULL;
  m_StartSliceNumber = 1;

}

template <typename TPixel, typename TImage>
void
RawSliceVolumeReader<TPixel,TImage>
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
  io->SetDimensions( 2, 1 );

  typedef itk::RawImageSequenceReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();

  reader->SetFilePrefix( m_FilePrefix.c_str() );
  reader->SetFilePattern( m_FilePattern.c_str() );
  reader->SetStartSliceNumber( m_StartSliceNumber );
  reader->SetEndSliceNumber( m_StartSliceNumber + (signed long) m_Size[2] - 1 );

  reader->SetImageIO( io );
  reader->Update();

  m_Image = reader->GetOutput();


}


} // namespace itk

#endif
