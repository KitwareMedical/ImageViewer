/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    chunks.cxx
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

