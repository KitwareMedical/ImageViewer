/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTreeWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTreeWriter_txx
#define __itkSWSSegmentTreeWriter_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

template <class TScalarType>
void SegmentTreeWriter<TScalarType>
::SafeWrite()
{
  const unsigned BUFSZ = 16384;

  typename SegmentTreeType::Pointer input = this->GetInput();

  std::ofstream out(m_FileName.c_str());
  if (!out)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // write header
  unsigned long listsz = input->Size();
  out.write((char *)&listsz, sizeof(unsigned long));
  
  // now write data
  typename SegmentTreeType::ValueType *buf =
    new typename SegmentTreeType::ValueType[BUFSZ];
  
  typename SegmentTreeType::Iterator it;

  it = input->Begin();
  unsigned n = 0;
  while ( it != input->End() )
    {
      buf[n] = *it;
      n++;
      ++it;
      if (n == BUFSZ)
        {
          out.write((char *)buf,
                    sizeof (typename SegmentTreeType::ValueType) *  BUFSZ);
          n = 0;
        }
    }
  out.write((char *)buf,
            sizeof (typename SegmentTreeType::ValueType) *  n);

  out.close();
  delete[] buf; 
}

} // end namespace sws
} // end namespace itk

#endif

