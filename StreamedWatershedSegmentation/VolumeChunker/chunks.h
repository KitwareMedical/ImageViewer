/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    chunks.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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

