/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataReader.txx
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
#ifndef __itkSWSBoundaryDataReader_txx
#define __itkSWSBoundaryDataReader_txx

#include "itkImageRegionIterator.h"

namespace itk
{
namespace sws
{

template <class TScalarType, unsigned int TDimension>
void BoundaryDataReader<TScalarType, TDimension>::GenerateData()
{
  typedef typename BoundaryDataType::face_pixel_t FacePixelType;
  typedef typename BoundaryDataType::face_t::RegionType RegionType;
  typedef typename BoundaryDataType::flat_region_t FlatRegionType;
  typedef typename BoundaryDataType::flat_hash_t FlatHashType;

  unsigned int i;

  unsigned long *FRBuf;
  FlatHashType *hash;
  typename FlatHashType::iterator hash_it;
  typename FlatHashType::value_type hash_val;
  FlatRegionType flat_reg;
  ScalarType fl_bounds_min;
  ScalarType fl_value;
  unsigned long  fl_min_label;
  unsigned long  fl_key;
  unsigned long  offset;
  bool val;
  
  typename BoundaryDataType::IndexType idx;  
  typename BoundaryDataType::Pointer output = this->GetOutput();
  typename RegionType::SizeType sz;
  RegionType region;

  unsigned long imgsz, hashsz, offsz;
  
  std::ifstream in(m_FileName.c_str());
  if (!in)
    {
      std::cerr << "BoundaryDataReader: couldn't open file"
                << m_FileName << std::endl; 

      exit(-1);
    }

  // First read all of the valid indicies
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
          in.read((unsigned char *)&val, sizeof(bool));
          output->SetValid(val, idx);
        }
    }

  // Now read all of the image faces.
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
  
          if ( output->GetValid(idx) != true) continue;
          
          in.read((unsigned char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetLargestPossibleRegion(region);

          in.read((unsigned char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetRequestedRegion(region);
                   
          in.read((unsigned char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetBufferedRegion(region);

          output->GetFace(idx)->Allocate();

          sz = region.GetSize();
          imgsz = 1;
          for (i = 0; i < Dimension; ++i) imgsz *= sz[i];

          in.read((unsigned char *)output->GetFace(idx)->GetBufferPointer(),
                    imgsz * sizeof(FacePixelType));
        }
    }

  // Now read all of the face hashes
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
          if ( output->GetValid(idx) != true) continue;
          hash    = output->GetFlatHash(idx);

          in.read((unsigned char *)&hashsz, sizeof (unsigned long));
  
          for (unsigned j = 0; j < hashsz; j++)
            {
              in.read((unsigned char *)&fl_key, sizeof (unsigned long));          
              in.read((unsigned char *)&fl_bounds_min, sizeof (ScalarType));          
              in.read((unsigned char *)&fl_min_label, sizeof (unsigned long));          
              in.read((unsigned char *)&fl_value, sizeof (ScalarType));
              in.read((unsigned char *)&offsz, sizeof (unsigned long));

              flat_reg.bounds_min = fl_bounds_min;
              flat_reg.min_label  = fl_min_label;
              flat_reg.value      = fl_value;
          
              hash->insert(typename FlatHashType::value_type(fl_key, flat_reg));
              hash_it = hash->find(fl_key);

              for (i = 0; i < offsz; i++)
                {
                  in.read((unsigned char *)&offset, sizeof (unsigned long));
                  (*hash_it).second.offset_list.push_back(offset);
                }
            }
        }
    }
  in.close();
}

} // end namespace sws
} // end namespace itk

#endif

