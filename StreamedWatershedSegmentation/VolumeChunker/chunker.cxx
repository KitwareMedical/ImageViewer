/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    chunker.cxx
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
  This application converts raw volume files into chunks.  It does not process
  Visible Human color data.  For Visible Human data, use the vhchunker
  application.   Cropping and padding of the data is supported.  The headers
  for each chunk is stored in the file specified by the chunk_filename
  parameter.
  

  The input to this application is a parameter file as shown below.  A
  ``parameter file'' is an ascii text file with the following syntax:

  (keyword value1 value2 value3 ... )  where valueX can be an integer (no
  decimal), floating point (decimal), or string (delimited by quotes "").
  C++-style commenting is supported in parameter files.

  (input_filename "/path/to/myfile.raw")  //  name (and path) of the
                                          //input file
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
  (vh_dataset "female") // specify male or female dataset

  Requires the vispack ParameterFile library libparam

  jc 8/14/01
*/

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

int main(int argc, char * argv[])
{
  unsigned int n[3], v[3], rem[3], pad[3], a[3], e[3], pixel_size, i, j, k;
  unsigned int a1, a2, a3;
  std::string fn;
  ::div_t res;
  std::string infile, outfile_prefix, cfn;
  ctk::chunk_info_struct *chunk_list;
  int chunknumber;
  ctk::chunk_info_struct temp_chunk;
  char *default_fn = "chunks.in";
  if (argc >1) fn = argv[1];
  else fn = default_fn;
  
  VPF::ParameterFile pf;

  // Open and parse the chunks.in file.  Required parameters.
  pf.Initialize(fn.c_str());
  
  if (VPF::set(infile, pf["input_filename"][0]) == VPF::INVALID)
    die ("COuld not set input_filename");
  if (VPF::set(outfile_prefix, pf["output_prefix"][0]) == VPF::INVALID)
    die ("Could not set output_prefix");
  if (VPF::set(pixel_size, pf["pixel_size"][0]) == VPF::INVALID)
    die ("COuld not set pixel_size");
  
  for (i = 0; i < 3; ++i)
    {
      if (VPF::set(v[i], pf["volume_dimensions"][i]) == VPF::INVALID)
        die ("Coudle not set volume_dimensions");
      if (VPF::set(n[i], pf["cube_dimensions"][i]) == VPF::INVALID)
        die ("could not set cube_dimensions");
    }
  for (i = 0; i < 3; ++i)
    {
      if (VPF::set(pad[i], pf["pad"][i]) == VPF::INVALID)
        die ("Could not set pad");
    }
  
  // Optional parameters  
  if (VPF::set(cfn, pf["chunk_filename"][0]) == VPF::INVALID)
          cfn = "chunkset.record";

  // Calculate the chunks we need
  for (i = 0; i < 3; ++i)
    {
      res = ::div(v[i], n[i]);
      a[i] = res.quot;
      e[i] = res.quot + res.rem;
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
          temp_chunk.set_pixel_sz(pixel_size);
          temp_chunk.set_region_idx(a[0] * k, a[1] * j, a[2] * i);
          if (k == n[0] -1) a1 = e[0]; else a1 = a[0];
          if (j == n[1] -1) a2 = e[1]; else a2 = a[1];
          if (i == n[2] -1) a3 = e[2]; else a3 = a[2];
          temp_chunk.set_region_sz(a1,a2,a3);
          temp_chunk.pad(pad[0], pad[1], pad[2]);
          chunk_list[nc] = temp_chunk;
        }

  // Write chunk list to disk
  std::ofstream cfo;
  cfo.open(cfn.c_str());
  cfo.write(&chunknumber, sizeof(int)); //write the number of chunks to follow
  cfo.write(chunk_list, chunknumber * sizeof(ctk::chunk_info_struct));
  cfo.close();

  // Now split the file into its chunks
  std::ifstream in;
  in.open(infile.c_str());
  if (!in)
    {
      std::cerr << "Could not open volume file" << std::endl;
      exit(1);
    }
  
  // Write each chunk
  unsigned int y_scan_offset, z_scan_offset, start_offset;
  unsigned int x_stride, y_stride, z_stride;
  std::string chunkfilename;
  char strbuf[256];
  char *scanline;
  std::ofstream out;
  unsigned scanlen;


  for (i = 0; i < chunknumber; ++i)
    {
      std::cout << "Writing chunk " << i << "/" << chunknumber-1 << std::endl;
      std::cout << chunk_list[i] << std::endl;

      // we'll read one contiguous line at a time (x dimension)
      scanlen =  chunk_list[i].get_padded_sz()[0] * chunk_list[i].get_pixel_sz();
      scanline = new char[scanlen];
      std::cout << "scanlen = " << scanlen << std::endl;
      
      // Stride lengths in bytes
      x_stride = chunk_list[i].get_pixel_sz();
      y_stride = x_stride * chunk_list[i].get_largest_sz()[0];
      z_stride = y_stride * chunk_list[i].get_largest_sz()[1];

      y_scan_offset
        = (chunk_list[i].get_largest_sz()[0] -
           chunk_list[i].get_padded_sz()[0]) * x_stride;

      z_scan_offset
        = (chunk_list[i].get_largest_sz()[1] -
           chunk_list[i].get_padded_sz()[1]) * y_stride;

      start_offset =
          chunk_list[i].get_padded_idx()[0] * x_stride
        + chunk_list[i].get_padded_idx()[1] * y_stride
        + chunk_list[i].get_padded_idx()[2] * z_stride;

      std::cout << "start_offset = " << start_offset  << std::endl;
      std::cout << "y_scan_offset = " << y_scan_offset << std::endl;
      std::cout << "z_scan_offset = " << z_scan_offset << std::endl;
      std::cout << "x_stride = " << x_stride  << std::endl;
      std::cout << "y_stride = " << y_stride  << std::endl;
      std::cout << "z_stride = " << z_stride  << std::endl;

      // Open an output chunk file
      ::sprintf(strbuf, "%u", i);
      chunkfilename = outfile_prefix + ".chunk." + strbuf;
      std::cout << chunkfilename << std::endl;

      // Read and write scanlines
      out.open(chunkfilename.c_str());

      in.seekg(start_offset, ios::beg);
      unsigned z_max = chunk_list[i].get_padded_sz()[2];
      unsigned y_max = chunk_list[i].get_padded_sz()[1];
      for (unsigned zz = 0; zz < z_max; zz++)
        {
          for (unsigned yy = 0; yy < y_max; yy++)
            {
              in.read(scanline, scanlen);
              out.write(scanline, scanlen);
              in.seekg(y_scan_offset,ios::cur);
            }
          in.seekg(z_scan_offset, ios::cur);
        }
      out.close();
      
      std::cout << "..file written successfully." << std::endl;
      std::cout << "---------------------------------------------" <<
        std::endl;
      
      delete[] scanline;
      }
  return 0;
}
