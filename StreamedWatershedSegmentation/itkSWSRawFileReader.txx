/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
  ptr = ptr.Begin();
 
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
  std::cout << "Reading a region of size: " << output->GetRequestedRegion() <<
    std::endl; 
  while ( !in.eof() )
    {
      in.read((char *)buf, sizeof(PixelType) * 16384);

      num = in.gcount() / sizeof(PixelType);
      for (i = 0; i < num; ++i, ++ptr)
        {
          if (ptr.IsAtEnd())
            {
              std::cout << "itkRawFileReader: File is larger than the region specified."
                        << std::endl;
              exit(-1);
            }
          ptr.Value() = buf[i];
        }
    }

    delete[] buf;
 
}

} // end namespace sws
} // end namespace itk

#endif
