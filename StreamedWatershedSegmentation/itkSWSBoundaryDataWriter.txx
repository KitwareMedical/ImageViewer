/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSBoundaryDataWriter_txx
#define __itkSWSBoundaryDataWriter_txx
#include "itkImageRegionIterator.h"

namespace itk
{
namespace sws
{

template <class TScalarType, unsigned int TDimension>
void BoundaryDataWriter<TScalarType, TDimension>::SafeWrite()
{
  typedef typename BoundaryDataType::face_pixel_t FacePixelType;
  typedef typename BoundaryDataType::face_t::RegionType RegionType;
  typedef typename BoundaryDataType::flat_region_t FlatRegionType;
  typedef typename BoundaryDataType::flat_hash_t FlatHashType;
  

  unsigned long *FRBuf;
  FlatHashType *hash;
  typename FlatHashType::iterator hash_it;
  ScalarType fl_bounds_min;
  ScalarType fl_value;
  unsigned long fl_key;
  unsigned long  fl_min_label;
  
  typename BoundaryDataType::IndexType idx;  
  typename BoundaryDataType::Pointer input = this->GetInput();
  typename RegionType::SizeType sz;
  RegionType region;

  unsigned int i;
  unsigned long imgsz, hashsz;
  
  std::ofstream out(m_FileName.c_str());
  if (!out)
    {
      std::cerr << "BoundaryDataWriter: couldn't open file" << m_FileName <<
        std::endl; 

      ::exit(-1);
    }

  // First write all of the valid indicies
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
          bool val = input->GetValid(idx);
          out.write((char *)&val, sizeof(bool));
        }
    }

  // Now write all of the image faces.
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
          if ( input->GetValid(idx) != true) continue;
          region = input->GetFace(idx)->GetLargestPossibleRegion();
          out.write((char *)&region, sizeof(RegionType));
          region = input->GetFace(idx)->GetRequestedRegion();
          out.write((char *)&region, sizeof(RegionType));
          region = input->GetFace(idx)->GetBufferedRegion();
          out.write((char *)&region, sizeof(RegionType));

          sz = region.GetSize();
          imgsz = 1;
          for (i = 0; i < Dimension; ++i) imgsz *= sz[i];

          out.write((char *)input->GetFace(idx)->GetBufferPointer(),
                    imgsz * sizeof(FacePixelType));
        }
    }

  // Now write all of the face hashes
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
          if ( input->GetValid(idx) != true) continue;
          hash   = input->GetFlatHash(idx);
          hash_it = hash->begin();
          hashsz = hash->size();

          out.write((char *)&hashsz, sizeof(unsigned long));

          while ( hash_it != hash->end() )
            {
              fl_key    = (*hash_it).first;
              out.write((char *)&fl_key, sizeof (unsigned long));
              fl_bounds_min = (*hash_it).second.bounds_min;
              out.write((char *)&fl_bounds_min, sizeof (ScalarType));
              fl_min_label = (*hash_it).second.min_label;
              out.write((char *)&fl_min_label, sizeof (unsigned long));
              fl_value = (*hash_it).second.value;
              out.write((char *)&fl_value, sizeof (ScalarType));

              hashsz = (*hash_it).second.offset_list.size();

              out.write((char *)&hashsz, sizeof (unsigned long));
          
              FRBuf = new unsigned long[hashsz];
       
              typename std::list<unsigned long>::iterator it
                = (*hash_it).second.offset_list.begin();
              i = 0;
              while (it != (*hash_it).second.offset_list.end() )
                {
                  FRBuf[i] = *it;
                  ++i;
                  ++it;
                }

              out.write((char *)FRBuf, sizeof(unsigned long) * i);
              delete[] FRBuf;
              hash_it++;

            }
                        
        }
    }
  out.close();
}

} // end namespace sws
} // end namespace itk

#endif

