/*=========================================================================
  
  Program:   Insight Segmentation & Registration Toolkit
  Module:    chunks.h
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
#ifndef __CHUNKS_H_
#define __CHUNKS_H_

#include <fstream>

namespace ctk {
/** \class chunk_info_struct
 * A structure that holds information about the size, extent, and data type
 * of a chunk of volumetric data.
 * \code
 *  __________________________________
 * |        largest region            |
 * |    ___________________           |
 * |   |   padded region   |          |
 * |   |   __________      |          |
 * |   |  | region   |     |          |
 * |   |  |          |     |          |
 * |   |  |          |     |          |
 * |   |  |          |     |          |
 * |   |  |          |     |          |
 * |   |  |__________|     |          |
 * |   |___________________|          |
 * |                                  |
 * |__________________________________|
 *
 * \endcode
 *
 * ``region'' refers to the active region of interest.  This is the region
 *  within the volume that this chunk represents.
 *
 * ``padded'' refers to a superset of the region with extra ghost pixels around
 * the edges.
 *
 * ``largest'' are the dimensions of the entire volume.  This is necessary when
 * looking at how a chunk can be padded.  */
class chunk_info_struct
{
  // Start index and size of the chunk region.
  int region_idx[3];
  int region_sz[3];

  // Start index and size of the padded region.
  int padded_idx[3];
  int padded_sz[3];

  // Start index and size of the entire volume region of which this is a
  // chunk.
  int largest_idx[3];
  int largest_sz[3];

  int pixel_sz;
public:
  chunk_info_struct();
  ~chunk_info_struct() {}

  void set_largest_idx(int x, int y, int z)
    { largest_idx[0] = x; largest_idx[1] = y; largest_idx[2] = z; }
  void set_region_idx(int x, int y, int z)
    { region_idx[0] = x; region_idx[1] = y; region_idx[2] = z; }
  void set_padded_idx(int x, int y, int z)
    { padded_idx[0] = x; padded_idx[1] = y; padded_idx[2] = z; }
  void set_largest_sz(int x, int y, int z)
    { largest_sz[0] = x; largest_sz[1] = y; largest_sz[2] = z; }
  void set_region_sz(int x, int y, int z)
    { region_sz[0] = x; region_sz[1] = y; region_sz[2] = z; }
  void set_padded_sz(int x, int y, int z)
    { padded_sz[0] = x; padded_sz[1] = y; padded_sz[2] = z; }
  void set_pixel_sz(int s)
    { pixel_sz = s; }

  const int *get_largest_idx() const {return largest_idx;}
  const int *get_region_idx()  const {return region_idx;}
  const int *get_padded_idx()  const {return padded_idx;}
  const int *get_largest_sz()  const {return largest_sz;}
  const int *get_region_sz()   const {return region_sz;}
  const int *get_padded_sz()   const {return padded_sz;}
  int get_pixel_sz() const {return pixel_sz;}
  
  /** Automatically pad out x, y, z pixels symmetrically in those respective
   * directions from the region.  Calculates padded_idx and padded_sz
   * accordingly, accounting for the largest possible size.
   *
   * Returns true if full padding took place and false if padding was clipped
   * by the volume boundaries (largest region). */
  bool pad(int x, int y, int z);
};

std::ostream & operator<<(std::ostream &os, const chunk_info_struct &c);

} // end namespace ctk
#endif

