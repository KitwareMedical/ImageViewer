/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    chunks.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "chunks.h"
namespace ctk {

chunk_info_struct::chunk_info_struct()
{
  for (int i = 0; i < 3; ++i)
    { largest_idx[i] = largest_sz[i] = region_idx[i] = region_sz[i]
        = padded_idx[i] = padded_sz[i] = 0; }
}

bool chunk_info_struct::pad(int x, int y, int z)
{
  bool clipped = false;
  int pad[3], largest_end[3];
  pad[0] = x; pad[1] = y; pad[2] = z;
  
  for (int i = 0; i < 3; i++)
    {
      largest_end[i] = largest_idx[i] + largest_sz[i];
      padded_sz[i]  = region_sz[i] + (2*pad[i]);
      padded_idx[i] = region_idx[i] - pad[i];

      if (padded_idx[i] < largest_idx[i])
        {
          clipped = true;
          padded_sz[i] -= largest_idx[i] - padded_idx[i];
          padded_idx[i] = largest_idx[i];
        }
      if ( (padded_idx[i] + padded_sz[i]) > largest_end[i] )
        {
          clipped = true;
          padded_sz[i] -= (padded_idx[i] + padded_sz[i]) - largest_end[i];
        }
    }

  if (! clipped) return true;
  else return false;
}

std::ostream & operator<<(std::ostream &os, const chunk_info_struct &c)
{
  int i;
  os << "chunk_info_struct { this=" << &c << " bytes per pixel = " <<
    c.get_pixel_sz() << " region_idx={";
  for (i = 0; i < 3; ++i) os << c.get_region_idx()[i] << " ";
  os << "} region_sz={";
  for (i = 0; i < 3; ++i) os << c.get_region_sz()[i] << " ";
  os << "} padded_idx={";
  for (i = 0; i < 3; ++i) os << c.get_padded_idx()[i] << " ";
  os << "} padded_sz={";
  for (i = 0; i < 3; ++i) os << c.get_padded_sz()[i] << " ";
  os << "} largest_idx={";
  for (i = 0; i < 3; ++i) os << c.get_largest_idx()[i] << " ";
  os << "} largest_sz={";
  for (i = 0; i < 3; ++i) os << c.get_largest_sz()[i] << " ";
  os << "} }";
  return os;
}
  
} // end namespace ctk

