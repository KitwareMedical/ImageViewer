/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
          in.read((char *)&val, sizeof(bool));
          output->SetValid(val, idx);
        }
    }

  // Now read all of the image faces.
  for (idx.first = 0;  idx.first < Dimension; ++idx.first)
    {
      for (idx.second = 0; idx.second < 2; ++idx.second)
        {
  
          if ( output->GetValid(idx) != true) continue;
          
          in.read((char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetLargestPossibleRegion(region);

          in.read((char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetRequestedRegion(region);
                   
          in.read((char *)&region, sizeof(RegionType));
          output->GetFace(idx)->SetBufferedRegion(region);

          output->GetFace(idx)->Allocate();

          sz = region.GetSize();
          imgsz = 1;
          for (i = 0; i < Dimension; ++i) imgsz *= sz[i];

          in.read((char *)output->GetFace(idx)->GetBufferPointer(),
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

          in.read((char *)&hashsz, sizeof (unsigned long));
  
          for (unsigned j = 0; j < hashsz; j++)
            {
              in.read((char *)&fl_key, sizeof (unsigned long));          
              in.read((char *)&fl_bounds_min, sizeof (ScalarType));          
              in.read((char *)&fl_min_label, sizeof (unsigned long));          
              in.read((char *)&fl_value, sizeof (ScalarType));
              in.read((char *)&offsz, sizeof (unsigned long));

              flat_reg.bounds_min = fl_bounds_min;
              flat_reg.min_label  = fl_min_label;
              flat_reg.value      = fl_value;
          
              hash->insert(typename FlatHashType::value_type(fl_key, flat_reg));
              hash_it = hash->find(fl_key);

              for (i = 0; i < offsz; i++)
                {
                  in.read((char *)&offset, sizeof (unsigned long));
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

