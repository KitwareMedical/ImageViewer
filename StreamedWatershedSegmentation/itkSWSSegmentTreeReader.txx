/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTreeReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTreeReader_txx
#define __itkSWSSegmentTreeReader_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

template <class TScalarType>
void SegmentTreeReader<TScalarType>
::GenerateData()
{
  const unsigned BUFSZ = 16384;

  typename SegmentTreeType::Pointer output = this->GetOutput();

  std::ifstream in(m_FileName.c_str());
  if (!in)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // read header
  unsigned long listsz;
  in.read((char *)&listsz, sizeof(unsigned long));

  // now read data
  typename SegmentTreeType::ValueType *buf =
    new typename SegmentTreeType::ValueType[BUFSZ];
  typename SegmentTreeType::Iterator it;
  unsigned segsize = sizeof( typename SegmentTreeType::ValueType );
  unsigned n = 0;
  while ( ! in.eof() )
    {

      in.read((char *)buf, segsize * BUFSZ );
      n = in.gcount();

      for (unsigned int i = 0; i < (n / segsize );  ++i) 
        {          output->PushBack(buf[i]);  }
    }

  in.close();
  delete[] buf; 
}

} // end namespace sws
} // end namespace itk

#endif

