/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSWSRawFileReader_txx
#define _itkSWSRawFileReader_txx

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include <fstream>

namespace itk
{
namespace sws
{

  
template <class TOutputImage>
void 
RawFileReader<TOutputImage>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();
  
  // we need to compute the output spacing, the output origin, the
  // output image size, and the output image start index
  //  outputPtr->SetSpacing( m_Spacing );
  //  outputPtr->SetOrigin( m_Origin );
  outputPtr->SetLargestPossibleRegion( m_Region );
}

  
template <class TOutputImage>
void RawFileReader<TOutputImage>::SafeRead()
{
  
  PixelType *buf;
  unsigned int i, num;

  // Allocate output
  typename TOutputImage::Pointer output = this->GetOutput();

  output->SetLargestPossibleRegion(m_Region);
  output->SetRequestedRegion(m_Region);
  output->SetBufferedRegion(m_Region);
  output->Allocate();

  // Set up an iterator for the image
  ImageRegionIterator<TOutputImage> ptr(output, output->GetRequestedRegion());

  std::cout << output->GetRequestedRegion() << std::endl;

  
  ptr = ptr.Begin();
  //ptr.GoToBegin();

  // Open chunk file

  std::ifstream in;
  in.open(m_FileName.c_str());
  if (!in)
    {
      std::cout << "COULD NOT OPEN FILE " << std::endl;
      exit(-1);
    }

  unsigned rdsz = 16384;
  buf = new PixelType[rdsz];
 
  while ( !in.eof() )
    {
      in.read((unsigned char *)buf, sizeof(PixelType) * 16384);

      num = in.gcount() / sizeof(PixelType);
      for (i = 0; i < num; ++i, ++ptr)
        {
          if (ptr.IsAtEnd())
            {
              std::cout << "itkRawFileReader: Request for a region larger than the file."
                        << std::endl;
              exit(-1);
            }

          ptr.Value() = buf[i];
          //          cout<<i<<" "<<buf[i]<<" "<<flush;

        }
    }

    delete[] buf;
 
}

} // end namespace sws
} // end namespace itk

#endif
