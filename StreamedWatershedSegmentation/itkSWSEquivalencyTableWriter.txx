/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSEquivalencyTableWriter_txx
#define __itkSWSEquivalencyTableWriter_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

void EquivalencyTableWriter
::SafeWrite()
{
  const unsigned BUFSZ = 16384;
  unsigned long tablesz;

  EquivalencyTableType::Pointer input = this->GetInput();
  typedef std::pair< unsigned long, unsigned long> VType;
  
  std::ofstream out(m_FileName.c_str());
  if (!out)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // Write the header, the number of records
  tablesz = input->Size();
  out.write((char *)&tablesz, sizeof(unsigned long));

 // now write data
  VType *buf = new VType[BUFSZ];
  
  EquivalencyTableType::Iterator it;

  it = input->Begin();
  unsigned n = 0;
  while ( it != input->End() )
    {
      (buf[n]).first  = (*it).first;
      (buf[n]).second = (*it).second;
      n++;
      ++it;
      if (n == BUFSZ)
        {
          out.write((char *)buf,   sizeof (VType) *  BUFSZ);
          n = 0;
        }
    }
  out.write((char *)buf,  sizeof ( VType) *  n);

  out.close();
  delete[] buf; 
}
  
} // end namespace sws
} // end namespace itk

#endif

