/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRawImageSequenceReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkRawImageSequenceReader_txx
#define _itkRawImageSequenceReader_txx
#include "itkRawImageSequenceReader.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageIOFactory.h"
#include "itkConvertPixelBuffer.h"


namespace itk
{

template <class TOutputImage, class ConvertPixelTraits>
RawImageSequenceReader<TOutputImage, ConvertPixelTraits>
::RawImageSequenceReader()
{
  m_ImageIO = 0;
  m_FilePrefix = "";
  m_FilePattern = "%d";
  m_StartSliceNumber = 0;
  m_EndSliceNumber = 0;
  
}

template <class TOutputImage, class ConvertPixelTraits>
RawImageSequenceReader<TOutputImage, ConvertPixelTraits>::~RawImageSequenceReader()
{
}

template <class TOutputImage, class ConvertPixelTraits>
void RawImageSequenceReader<TOutputImage, ConvertPixelTraits>::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (m_ImageIO)
    {       
    os << indent << "ImageIO: " << m_ImageIO << "\n";
    }
  else
    {
    os << indent << "ImageIO: (null)" << "\n";
    }
  os << indent << "FilePrefix: " << m_FilePrefix << "\n";
  os << indent << "FilePattern: " << m_FilePattern << "\n";
  os << indent << "StartSliceNumber: " << m_StartSliceNumber << "\n";
  os << indent << "EndSliceNumber: " << m_EndSliceNumber << "\n";
  
}


template <class TOutputImage, class ConvertPixelTraits>
void RawImageSequenceReader<TOutputImage, ConvertPixelTraits>::GenerateData()
{

  if ( TOutputImage::ImageDimension != 3 )
    {
    throw RawImageSequenceReaderException(__FILE__, __LINE__, "Image dimension must be 3");
    }

  typename TOutputImage::Pointer output = this->GetOutput();

  itkDebugMacro(<<"Reading files ");
  
  // Check to see if we can read the file given the name or prefix
  //
  if ( m_FilePrefix == "" )
    {
    throw RawImageSequenceReaderException(__FILE__, __LINE__, "FilePrefix must be non-empty");
    }
  
  if ( m_ImageIO.IsNull() )
    {
    RawImageSequenceReaderException e(__FILE__, __LINE__);
    e.SetDescription("ImageIO must be specified");
    throw e;
    return;
    }

  // Got to allocate space for the image. Determine the characteristics of
  // the image.
  //  
  if( m_ImageIO->GetNumberOfDimensions() < TOutputImage::ImageDimension )
    {
    RawImageSequenceReaderException e(__FILE__, __LINE__);
    OStringStream msg;
    msg << "Number of dimensions in file ("
        << m_ImageIO->GetNumberOfDimensions()
        << ") does not match number of dimensions in output ("
        << TOutputImage::ImageDimension
        << ")";
    e.SetDescription(msg.str().c_str());
    throw e;
    }
  
  Size dimSize;
  double spacing[ TOutputImage::ImageDimension ];
  double origin[ TOutputImage::ImageDimension ];

  for(unsigned int i=0; i<TOutputImage::ImageDimension; i++)
    {
    dimSize[i] = m_ImageIO->GetDimensions(i);
    spacing[i] = m_ImageIO->GetSpacing(i);
    origin[i]  = m_ImageIO->GetOrigin(i);
    }

  output->SetSpacing( spacing );   // Set the image spacing
  output->SetOrigin( origin );     // Set the image origin

  // Compute dimSize[2] from slice number
  signed long nslice = m_EndSliceNumber - m_StartSliceNumber + 1;
  if ( nslice < 0 )
    {
    nslice = 0;
    }
  dimSize[2] = static_cast<unsigned long>( nslice );

  typedef typename TOutputImage::IndexType   IndexType;

  IndexType start;
  start.Fill(0);

  Region region;
  region.SetSize(dimSize);
  region.SetIndex(start);

  ImageIORegion ioRegion(TOutputImage::ImageDimension);
  
  ImageIORegion::SizeType ioSize = ioRegion.GetSize();
  ImageIORegion::IndexType ioStart = ioRegion.GetIndex();

  for(unsigned int i = 0; i < dimSize.GetSizeDimension(); ++i)
    {
    ioSize[i] = dimSize[i];
    }
  for(unsigned int i = 0; i < start.GetIndexDimension(); ++i)
    {
    ioStart[i] = start[i];
    }
  ioRegion.SetSize(ioSize);
  ioRegion.SetIndex(ioStart);

  itkDebugMacro (<< "ioRegion: " << ioRegion);
  
  output->SetLargestPossibleRegion(region);
  output->SetRequestedRegion(region);
  output->SetBufferedRegion(region);
  output->Allocate();
  
  // Tell the ImageIO to read a slice at time
  //
  OutputImagePixelType *buffer = 
    output->GetPixelContainer()->GetBufferPointer();

  signed int offset = output->GetOffsetTable()[2];
  int numPixelsPerSilce = dimSize[0] * dimSize[1];
  int k = 0;

  char nameBuffer[100];
  std::string fileName;

  for ( k = m_StartSliceNumber; k <= m_EndSliceNumber; buffer += offset, k++ )
    {

    sprintf( nameBuffer, m_FilePattern.c_str(), k );
    fileName = m_FilePrefix + nameBuffer;

    itkDebugMacro(<<"Reading file: " << fileName );

    m_ImageIO->SetFileName(fileName.c_str());
    m_ImageIO->SetIORegion(ioRegion);

    if ( m_ImageIO->GetPixelType() == typeid(TOutputImage::PixelType) &&
         (m_ImageIO->GetNumberOfComponents() == 
          ConvertPixelTraits::GetNumberOfComponents()))
      {
      itkDebugMacro(<< "No buffer conversion required.");
      // allocate a buffer and have the ImageIO read directly into it
      m_ImageIO->Read(buffer);
      }
    else // a type conversion is necessary
      {
      char * loadBuffer = 
        new char[m_ImageIO->GetImageSizeInBytes()];
      m_ImageIO->Read(loadBuffer);
      itkDebugMacro(<< "Buffer conversion required from: "
                   << m_ImageIO->GetPixelType().name()
                   << " to: " << typeid(TOutputImage::PixelType).name());
      this->DoConvertBuffer(loadBuffer, buffer, numPixelsPerSilce);
      delete [] loadBuffer;
      }

    }

}


template <class TOutputImage, class ConvertPixelTraits>
void 
RawImageSequenceReader<TOutputImage, ConvertPixelTraits>
::DoConvertBuffer(void* inputData,
                  OutputImagePixelType * outputData,
                  unsigned long numberOfPixels)
{

// Create a macro as this code is a bit lengthy and repetitive
// if the ImageIO pixel type is typeid(type) then use the ConvertPixelBuffer
// class to convert the data block to TOutputImage's pixel type
// see DefaultConvertPixelTraits and ConvertPixelBuffer
#define ITK_CONVERT_BUFFER_IF_BLOCK(type)               \
 else if( m_ImageIO->GetPixelType() == typeid(type) )   \
    {                                                   \
    ConvertPixelBuffer<                                 \
      type,                                             \
      OutputImagePixelType,                             \
      ConvertPixelTraits                                \
      >                                                 \
      ::Convert(                                        \
        static_cast<type*>(inputData),                  \
        m_ImageIO->GetNumberOfComponents(),             \
        outputData,                                     \
        numberOfPixels);                                \
    }
  if(0)
    {
    }
  ITK_CONVERT_BUFFER_IF_BLOCK(unsigned char)
  ITK_CONVERT_BUFFER_IF_BLOCK(char)
  ITK_CONVERT_BUFFER_IF_BLOCK(unsigned short)
  ITK_CONVERT_BUFFER_IF_BLOCK( short)
  ITK_CONVERT_BUFFER_IF_BLOCK(unsigned int)
  ITK_CONVERT_BUFFER_IF_BLOCK( int)
  ITK_CONVERT_BUFFER_IF_BLOCK(unsigned long)
  ITK_CONVERT_BUFFER_IF_BLOCK( long)
  ITK_CONVERT_BUFFER_IF_BLOCK(float)
  ITK_CONVERT_BUFFER_IF_BLOCK( double)
  else
    {
    RawImageSequenceReaderException e(__FILE__, __LINE__);
    OStringStream msg;
    msg <<"Couldn't convert pixel type: "
        << std::endl << "    " << m_ImageIO->GetPixelType().name()
        << std::endl << "to one of: "
        << std::endl << "    " << typeid(unsigned char).name()
        << std::endl << "    " << typeid(char).name()
        << std::endl << "    " << typeid(unsigned short).name()
        << std::endl << "    " << typeid(short).name()
        << std::endl << "    " << typeid(unsigned int).name()
        << std::endl << "    " << typeid(int).name()
        << std::endl << "    " << typeid(unsigned long).name()
        << std::endl << "    " << typeid(long).name()
        << std::endl << "    " << typeid(float).name()
        << std::endl << "    " << typeid(double).name()
        << std::endl;
    e.SetDescription(msg.str().c_str());
    throw e;
    return;
    }
#undef ITK_CONVERT_BUFFER_IF_BLOCK
}


} //namespace ITK

#endif
