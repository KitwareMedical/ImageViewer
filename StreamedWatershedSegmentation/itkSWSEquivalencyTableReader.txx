/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSEquivalencyTableReader_txx
#define __itkSWSEquivalencyTableReader_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

void EquivalencyTableReader
::GenerateData()
{
  const unsigned BUFSZ = 16384;

  EquivalencyTableType::Pointer output = this->GetOutput();

  std::ifstream in(m_FileName.c_str());
  if (!in)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // read header
  unsigned long listsz;
  in.read((char *)&listsz, sizeof(unsigned long));

  // now read data
  EquivalencyTableType::ValueType *buf =
    new EquivalencyTableType::ValueType[BUFSZ];
  EquivalencyTableType::Iterator it;
  unsigned segsize = sizeof( EquivalencyTableType::ValueType );
  unsigned n = 0;
  while ( ! in.eof() )
    {

      in.read((char *)buf, segsize * BUFSZ );
      n = in.gcount();

      for (unsigned int i = 0; i < (n / segsize );  ++i) 
        {          output->Add(buf[i].first, buf[i].second);  }
    }
  
  in.close();
  delete[] buf; 
}

} // end namespace sws
} // end namespace itk

#endif

