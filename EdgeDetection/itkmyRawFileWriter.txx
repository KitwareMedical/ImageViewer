/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkmyRawFileWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkmyRawFileWriter_txx
#define _itkmyRawFileWriter_txx

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include <fstream>

namespace itk
{

 
template <class TOutputImage>
void myRawFileWriter<TOutputImage>::GenerateData()
{
  PixelType *buf;
  unsigned int i, num;

  // Get input
  typename  TOutputImage::Pointer input = this->GetInput();

  num = 1;
  for (i = 0; i < ImageDimension; ++i)
    {
      num *= input->GetBufferedRegion().GetSize()[i];
    }
  
  // Open chunk file
  std::ofstream out;
  out.open(m_FileName.c_str());
  if (!out) throw ExceptionObject();

  std::cout << "Requested input Region of writer: " << std::endl;
  std::cout << input->GetRequestedRegion() << std::endl;

  // JUST DUMPS THE WHOLE IMAGE BUFFER
  std::cout << "DUMPING OUT A REGION (BUFFERED REGION):"
            << input->GetBufferedRegion();

  // Test the values
  //   for (i = 0; i < num; ++i)
  //           {
  //              std::cout << input->GetBufferPounsigned inter()[i] << std::endl;
  //            }

  
  out.write(input->GetBufferPointer(), num * sizeof(PixelType));
  out.close();

  
}


} //namespace ITK

#endif
