/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTableReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTableReader_txx
#define __itkSWSSegmentTableReader_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

template <class TScalarType>
void SegmentTableReader<TScalarType>
::GenerateData()
{
  record_t record;

  typedef typename SegmentTableType::edge_list_t::value_type edge_value_t;
  unsigned long edge_list_sz;
  edge_pair_t edge;
  typename SegmentTableType::segment_t segment;
  typename SegmentTableType::DataType *segptr;

  typename SegmentTableType::Pointer output = this->GetOutput();
  std::ifstream in(m_FileName.c_str());
  if (!in)
    {
      itkDebugMacro ("Couldn't open " << m_FileName);
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // Read the header, the number of records
  unsigned long listsz;
  in.read((char *)&listsz, sizeof(unsigned long));

  ScalarType depth;
  in.read((char *)&depth, sizeof(ScalarType));
  output->SetMaximumDepth(depth);
  
  // now read data, one painful record at a time...since records are not all
  // of the same length.
  typename SegmentTableType::Iterator it;

  typename SegmentTableType::edge_list_t::iterator list_ptr;

  unsigned n = 0;
  for (unsigned k = 0; k < listsz; ++k)
    {
      in.read((char *)&record, sizeof(record_t));
    
      segment.min = record.min;

      output->Add(record.label, segment);

      segptr = output->Lookup(record.label); // grab the pointer in the table
      segptr->edge_list.resize(record.edge_list_sz);
      for (list_ptr = segptr->edge_list.begin();
           list_ptr != segptr->edge_list.end();
           ++list_ptr)
        {
          in.read((char *)&edge, sizeof(edge_pair_t));
          list_ptr->label  = edge.label;
          list_ptr->height = edge.height;
        } 
    }
  in.close();

}

} // end namespace sws
} // end namespace itk

#endif

