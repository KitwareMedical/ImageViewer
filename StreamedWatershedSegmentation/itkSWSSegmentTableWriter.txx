/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTableWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTableWriter_txx
#define __itkSWSSegmentTableWriter_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

template <class TScalarType>
void SegmentTableWriter<TScalarType>
::SafeWrite()
{
  record_t record;
  const unsigned BUFSZ = 16384;
  itkDebugMacro("SegmentTableWriter::GenerateData()");

  typedef typename SegmentTableType::edge_list_t::value_type edge_value_t;
  edge_pair_t edge;
  typename SegmentTableType::edge_list_t::iterator edge_it;
  typename SegmentTableType::Pointer input = this->GetInput();
  
  std::ofstream out(m_FileName.c_str());
  if (!out)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // Write the header, the number of records
  unsigned long listsz = input->Size();
  out.write((char *)&listsz, sizeof(unsigned long));

  // write the maximum depth
  ScalarType depth = input->GetMaximumDepth();
  out.write((char *)&depth, sizeof(ScalarType));
  
  std::cout << "Writing segment table of size " << listsz << std::endl;

  
  // now write data, one painful record at a time...since records are not all
  // of the same length.
  typename SegmentTableType::Iterator it;

  it = input->Begin();
  unsigned n = 0;
  while ( it != input->End() )
    {
      record.label    = (*it).first;
      record.min      = (*it).second.min;
      record.edge_list_sz = (*it).second.edge_list.size();
      out.write((char *)&record, sizeof(record_t));

      //now write the edge table
      edge_it = (*it).second.edge_list.begin();
      while (edge_it != (*it).second.edge_list.end() )
        {
          edge.label  = (*edge_it).label;
          edge.height = (*edge_it).height;
          out.write( (char *)&edge, sizeof(edge_pair_t) );
          edge_it++;
        }
      ++it;
    }
  out.close();
}

} // end namespace sws
} // end namespace itk

#endif

