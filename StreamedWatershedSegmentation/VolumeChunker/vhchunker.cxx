/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vhchunker.cxx
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
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "param.h"
#include "chunks.h"
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}


/*
  This application breaks raw volume files into chunks.  The number and
  size of the chunks are specified in the input parameter file, chunks.in (or
  filename supplied on the command line).  Parameter files look like the
  following with parameters described below.  See also the documentation for
  chunker.cxx.

  This application is specific for chunking Visible Human color data where
  the color data is non-interlaced.  The output is a set of raw, interlaced
  rgb color data.  Cropping and padding of the data is supported.  The
  headers for each chunk are stored in the file you specify by the
  chunk_filename parameter.

  (input_filename "/path/to/myfile.raw")  // name (and path) of the input file
  (output_prefix "/path/to/mychunkfile")  // prefix (and path) for chunk output
  (reassembled_filename "/path/to/mynewfile.raw") // prefix (and path) for the
                                                  // reassembled volume file
                                                  // from the set of chunks
                                                  // OPTIONAL for chunking
                                                  // REQUIRED for unchunking
  (volume_dimensions 256 256 681)// dimensions of the total volume
  (cube_dimensions 2 2 5)        // number of cubes along each dimension
  (pixel_size 4)                 // size in bytes of each pixel
  (pad 0 0 0)                    // desired padding in pixels for each block
  (chunk_filename "chunkset")    // OPTIONAL name of the file record of the
                                                              chunks created

  Requires the vispack ParameterFile library libparam
  jc 8/14/01
*/


// A simple hash to map an integer number into a Visible Female fullcolor data
// filename.
std::string vf_filename_map(unsigned int z)
{
    std::cout << z << "-->";
  char buf[250];
  std::string suffix = ".raw";
  std::string prefix = "avf";
  std::string letter, ans;
  float perc;
  ::div_t res;
  res = ::div(z, 3);
  if (z != 0)  perc = (float)(((float)res.rem) / 3.0f);
  else perc = 0.0f;
  ::sprintf(buf, "%d", res.quot + 1001);
  
  if (res.rem == 0) letter = "a";
  else if (perc > 0.50f) letter = "c";
  else letter = "b";
  ans = prefix + buf + letter + suffix;
    std::cout << ans << std::endl;
  return ans;
}

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(0);
}

int main(int argc, char * argv[])
{
  unsigned int n[3], v[3], pad[3], a[3], e[3],
    pixel_size, i, j, k, extent[6], cropped_size[3];
  unsigned int a1, a2, a3;
  std::string fn, vh_dataset;
  ::div_t res;
  std::string input_path, outfile_prefix, cfn;
  ctk::chunk_info_struct *chunk_list;
  int chunknumber;
  ctk::chunk_info_struct temp_chunk;
  char *default_fn = "chunks.in";
  if (argc >1) fn = argv[1];
  else fn = default_fn;

  VPF::ParameterFile pf;

  // Open and parse the chunks.in file.  Required parameters.
  pf.Initialize(fn.c_str());
      
  if (VPF::set(input_path, pf["input_path"][0]) == VPF::INVALID)
    die ("Cannot find input_path parameter");
  if (VPF::set(outfile_prefix, pf["output_prefix"][0]) == VPF::INVALID)
    die ("Cannot find output_prefix parameter");
  if (VPF::set(pixel_size, pf["pixel_size"][0]) == VPF::INVALID)
    die ("Cannot find pixel_size parameter");
  if (VPF::set(vh_dataset, pf["vh_dataset"][0]) == VPF::INVALID)
    die ("Cannot find vh_dataset parameter");
  
  for (i = 0; i < 6; ++i)
    {
      if (VPF::set(extent[i], pf["volume_extent"][i]) == VPF::INVALID)
        die ("Error setting volume_extent parameter");
    }
  
  for (i = 0; i < 3; ++i)
    {
      if (VPF::set(v[i], pf["volume_dimensions"][i]) == VPF::INVALID)
        die ("Error setting volume_dimensions parameter");
      if (VPF::set(n[i], pf["cube_dimensions"][i]) == VPF::INVALID)
        die ("Error setting cube_dimensions parameter");
    }
  for (i = 0; i < 3; ++i)
    {
      if (VPF::set(pad[i], pf["pad"][i]) == VPF::INVALID)
        die ("Error setting pad parameter");
    }

  
  // Optional parameters
  if (VPF::set(cfn, pf["chunk_filename"][0]) == VPF::INVALID)
    {
      cfn = "chunkset.record";
    }

  if (vh_dataset != "female")
    {
      std::cerr << "Currently only the visible female dataset is supported" <<
        std::endl;
      ::exit(1);
    }
  
  // Calculate the chunks we need
  cropped_size[0] = extent[1] - extent[0];
  cropped_size[1] = extent[3] - extent[2];
  cropped_size[2] = extent[5] - extent[4];
  
  for (i = 0; i < 3; ++i)
    {
      res = ::div(cropped_size[i], n[i]);
      a[i] = res.quot;
      e[i] = res.quot + res.rem;
      //      std::cout << a[i] << " " << e[i] << std::endl;
    }

  // Allocate memory for the list of chunks.
  chunknumber = n[0] * n[1] * n[2];
  chunk_list = new ctk::chunk_info_struct[chunknumber];
  
  temp_chunk.set_largest_sz(v[0], v[1], v[2]);
  temp_chunk.set_largest_idx(0,0,0);

  // Make the chunks
  int nc = 0;
  for (i =0; i < n[2]; ++i)
    for (j = 0; j < n[1]; ++j)
      for (k = 0; k < n[0]; ++k, nc++)
        {
          //  std::cout << k << " " << j << " " << i << std::endl;
          temp_chunk.set_pixel_sz(pixel_size);
          temp_chunk.set_region_idx(a[0] * k + extent[0], a[1] * j + extent[2],
                                    a[2] * i + extent[4]);
          if (k == n[0] -1) a1 = e[0]; else a1 = a[0];
          if (j == n[1] -1) a2 = e[1]; else a2 = a[1];
          if (i == n[2] -1) a3 = e[2]; else a3 = a[2];
          temp_chunk.set_region_sz(a1,a2,a3);
          temp_chunk.pad(pad[0], pad[1], pad[2]);
          chunk_list[nc] = temp_chunk;
          //  std::cout << temp_chunk << std::endl << std::endl;
        }

  // Write chunk list to disk
  std::ofstream cfo;
  cfo.open(cfn.c_str());
  //write the number of chunks to follow
  cfo.write((char *)&chunknumber, sizeof(int));
  cfo.write((char *)chunk_list, chunknumber *
            sizeof(ctk::chunk_info_struct));
  cfo.close();

  
// Now split the file into its chunks
  std::ifstream in;
  
  // Write each chunk
  unsigned int y_scan_offset, start_offset;
  unsigned int x_stride, y_stride, z_stride;
  std::string chunkfilename;
  char strbuf[256];
  char *scanline, *scanline2, *scanline3, *interlaced_scanline;
  std::ofstream out;
  unsigned scanlen;
  std::streampos pos;
  for (i = 0; i < chunknumber; ++i)
    {
      std::cout << "Writing chunk " << i << "/" << chunknumber-1 << std::endl;
      std::cout << chunk_list[i] << std::endl;

      // we'll read one contiguous line at a time (x dimension)
      scanlen   = chunk_list[i].get_padded_sz()[0] * chunk_list[i].get_pixel_sz();
      scanline  = new char[scanlen];
      scanline2 = new char[scanlen];
      scanline3 = new char[scanlen];
      interlaced_scanline = new char[scanlen *3];
      
      std::cout << "scanlen = " << scanlen << std::endl;
      
      // Stride lengths in bytes
      x_stride = chunk_list[i].get_pixel_sz();
      y_stride = x_stride * chunk_list[i].get_largest_sz()[0];
      z_stride = y_stride * chunk_list[i].get_largest_sz()[1];

      y_scan_offset
        = (chunk_list[i].get_largest_sz()[0] -
           chunk_list[i].get_padded_sz()[0]) * x_stride;

      start_offset =
          chunk_list[i].get_padded_idx()[0] * x_stride
        + chunk_list[i].get_padded_idx()[1] * y_stride;

      std::cout << "start_offset = " << start_offset  << std::endl;
      std::cout << "y_scan_offset = " << y_scan_offset << std::endl;
      std::cout << "z_stride = " << z_stride  << std::endl;
      std::cout << "x_stride = " << x_stride  << std::endl;
      std::cout << "y_stride = " << y_stride  << std::endl;

      // Open an output chunk file
      ::sprintf(strbuf, "%u", i);
      chunkfilename = outfile_prefix + ".chunk." + strbuf;
      std::cout << chunkfilename << std::endl;

      // Read and write scanlines
      out.open(chunkfilename.c_str());
      unsigned ggg, fff;
      unsigned z_start = chunk_list[i].get_padded_idx()[2];
      unsigned z_max = z_start + chunk_list[i].get_padded_sz()[2];
      std::cout << "z_start " << z_start << std::endl;
      std::cout << "z_max " << z_max << std::endl;
 
      unsigned y_max = chunk_list[i].get_padded_sz()[1];
      for (unsigned zz = z_start; zz < z_max; zz++)
        {
          in.open( (input_path + vf_filename_map(zz)).c_str() );
          if (!in)
            {
              std::cerr << "Could not open " << input_path <<
                vf_filename_map(zz) << std::endl;
              ::exit(1);
            }
          in.seekg(start_offset, std::ios::beg);
          for (unsigned yy = 0; yy < y_max; yy++)
            {
              in.read((char *)scanline, scanlen);
              pos = in.tellg();
              in.seekg(z_stride - scanlen, std::ios::cur);
              in.read((char *)scanline2, scanlen);
              in.seekg(z_stride - scanlen, std::ios::cur);
              in.read((char *)scanline3, scanlen);

              for (ggg = 0, fff = 0; ggg < scanlen; ++ggg)
                {
                  interlaced_scanline[fff] = scanline[ggg];
                  fff++;
                  interlaced_scanline[fff] = scanline2[ggg];
                  fff++;
                  interlaced_scanline[fff] = scanline3[ggg];
                  fff++;
                }

              out.write((char *)interlaced_scanline, scanlen *3);
              
              in.seekg(pos);
              in.seekg(y_scan_offset, std::ios::cur);
            }
          in.close();
        }
      out.close();
      
      std::cout << "..file written successfully." << std::endl;
      std::cout << "---------------------------------------------" <<
        std::endl;
      
      delete[] scanline;
      delete[] scanline2;
      delete[] scanline3;
      delete[] interlaced_scanline;
    }

  return 0;
}
