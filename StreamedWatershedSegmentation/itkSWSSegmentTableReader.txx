/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTableReader.txx
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
  in.read((unsigned char *)&listsz, sizeof(unsigned long));

  ScalarType depth;
  in.read((unsigned char *)&depth, sizeof(ScalarType));
  output->SetMaximumDepth(depth);
  
  // now read data, one painful record at a time...since records are not all
  // of the same length.
  typename SegmentTableType::Iterator it;

  typename SegmentTableType::edge_list_t::iterator list_ptr;

  unsigned n = 0;
  for (unsigned k = 0; k < listsz; ++k)
    {
      in.read((unsigned char *)&record, sizeof(record_t));
    
      segment.min = record.min;

      output->Add(record.label, segment);

      segptr = output->Lookup(record.label); // grab the pointer in the table
      segptr->edge_list.resize(record.edge_list_sz);
      for (list_ptr = segptr->edge_list.begin();
           list_ptr != segptr->edge_list.end();
           ++list_ptr)
        {
          in.read((unsigned char *)&edge, sizeof(edge_pair_t));
          list_ptr->label  = edge.label;
          list_ptr->height = edge.height;
        } 
    }
  in.close();

}

} // end namespace sws
} // end namespace itk

#endif

