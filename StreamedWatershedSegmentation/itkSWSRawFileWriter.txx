/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSWSRawFileWriter_txx
#define _itkSWSRawFileWriter_txx

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include <fstream>

namespace itk
{
namespace sws
{
  
template <class TOutputImage>
void RawFileWriter<TOutputImage>::GenerateData()
{
  unsigned int i, num;

  // Get input
  typename  TOutputImage::ConstPointer input = this->GetInput();

  num = 1;
  for (i = 0; i < ImageDimension; ++i)
    { num *= input->GetBufferedRegion().GetSize()[i]; }
  
  // Open chunk file
  std::ofstream out;
  out.open(m_FileName.c_str());
  if (!out) throw ExceptionObject();

  // JUST DUMPS THE WHOLE IMAGE BUFFER
  out.write((char *)(input->GetBufferPointer()), num *
            sizeof(PixelType));
  out.close();
}

} // namespace sws
} // namespace ITK

#endif
