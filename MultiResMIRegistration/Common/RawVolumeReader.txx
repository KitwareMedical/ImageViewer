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

//#include "itkImageFileReader.h"
//#include "itkRawImageIO.h"

#include "RawVolumeReader.h"
#include "itkByteSwapper.h"

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

/*
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

  for ( int j = 0; j < ImageDimension; j++ )
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
*/


/*
 * Old code waiting for RawImageIO to be fixed 
 */

  // allocate memory in the image
  typename ImageType::RegionType region;
  region.SetSize( m_Size );

  m_Image = ImageType::New();
  m_Image->SetLargestPossibleRegion( region );
  m_Image->SetBufferedRegion( region );
  m_Image->Allocate();

  m_Image->SetSpacing( m_Spacing.GetDataPointer() );

  unsigned int numPixels = region.GetNumberOfPixels(); 

  // open up the file
  std::ifstream imgStream( m_FileName.c_str(), std::ios::binary | std::ios::in );
  
  if( !imgStream.is_open() )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Can't open file." );
    throw err;
    }

  // read the file
  // NOTE: VC++ requires below that buffer be cast to (char *) rather
  // than the more sensible (unsigned char *).
  PixelType * buffer = m_Image->GetBufferPointer();
  imgStream.read( (char *) buffer,
                  numPixels * sizeof(PixelType) );

  // clost the file
  imgStream.close();


  // swap bytes if neccessary
  if( m_BigEndian )
    {
    itk::ByteSwapper<PixelType>::SwapRangeBE( buffer, numPixels );
    }
  else
    {
    itk::ByteSwapper<PixelType>::SwapRangeLE( buffer, numPixels );
    }


}


} // namespace itk

#endif
