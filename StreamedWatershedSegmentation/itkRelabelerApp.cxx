/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRelabelerApp.cxx
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
#include "itkWatershedRelabeler.h"
#include "itkSWSRawFileReader.h"
#include "itkSWSRawFileWriter.h"
#include "itkSWSSegmentTreeReader.h"
#include "param.h"
#include <string>

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

/** This application reads a set of merges from a SegmentTree to a specified
    saliency level and then relabels a set of image volume chunks. */

int main(int argc, char *argv[])
{  
  typedef itk::Image<unsigned long, 3> LongImageType;
  float flood_level;
  char strbuf[255];
  
  itk::sws::RawFileReader<LongImageType>::Pointer rawFileReader
    = itk::sws::RawFileReader<LongImageType>::New();
  
  itk::sws::SegmentTreeReader<float>::Pointer  treereader
    = itk::sws::SegmentTreeReader<float>::New();
  
  itk::sws::RawFileWriter<LongImageType>::Pointer writer
    = itk::sws::RawFileWriter<LongImageType>::New();
  
  itk::watershed::Relabeler<float, 3>::Pointer relabeler
      = itk::watershed::Relabeler<float, 3>::New();

  std::string fn, chunkfile, rel_chunk_name, chunk_name, chunk_prefix,
    segment_tree_name;

  char *default_fn = "relabeler.in";
  if (argc >1) fn = argv[1];
  else fn = default_fn;

  // Open and parse the parameter file.
  VPF::ParameterFile pf;
  pf.Initialize(fn.c_str());
  
  if (VPF::set(chunkfile, pf["chunk_file"][0]) == VPF::INVALID)
    die ("Cannot find chunk_file parameter");
  if (VPF::set(chunk_prefix, pf["chunk_prefix"][0]) == VPF::INVALID)
    die ("Cannot find chunk_prefix parameter");
  if (VPF::set(segment_tree_name, pf["segment_tree_name"][0]) == VPF::INVALID)
    die ("Cannot find segment_tree_name parameter");
  if (VPF::set(flood_level, pf["flood_level"][0]) == VPF::INVALID)
    die ("Cannot find flood_level parameter");

  // Read a set of chunk headers
  int chunknumber;
  ctk::chunk_info_struct *chunk_list;
  
  std::ifstream in;
  in.open(chunkfile.c_str());
  if (!in)
    {
      std::cerr << "Could not open file" << chunkfile << std::endl;
      ::exit(1);
    }
  
  //read the number of chunks to follow
  in.read((char *)&chunknumber, sizeof(int));
  chunk_list = new ctk::chunk_info_struct[chunknumber];
  in.read((char *)chunk_list, chunknumber *
          sizeof(ctk::chunk_info_struct));
  in.close();

  LongImageType::RegionType largestReg;
  LongImageType::RegionType requestedReg;
  LongImageType::SizeType   sz;
  LongImageType::IndexType  idx;

  try{  // Compile the list of merges and relabel each chunk.

    for (int i = 0; i < chunknumber; ++i)
    {
      std::cout << "________________________________________" << std::endl;
      std::cout << "PROCESSING CHUNK NUMBER " << i << std::endl;
      ::sprintf(strbuf, "%u", i);
      chunk_name = chunk_prefix + "." + strbuf;
      std::cout << "FROM FILE " << chunk_name << std::endl;
      std::cout << "________________________________________" << std::endl;
      std::cout << chunk_list[i] << std::endl;
      std::cout << " +++++++++++++++++++++++++ " << std::endl;

      for (int j = 0; j < 3; ++j)
        {
          sz[j] = chunk_list[i].get_largest_sz()[j];
          idx[j] = chunk_list[i].get_largest_idx()[j];
        }
      largestReg.SetSize(sz);
      largestReg.SetIndex(idx);

      for (int j = 0; j < 3; ++j)
        {
          sz[j] = chunk_list[i].get_padded_sz()[j];
          idx[j] = chunk_list[i].get_padded_idx()[j];
        }
      requestedReg.SetSize(sz);
      requestedReg.SetIndex(idx);

      rawFileReader->SetFileName(chunk_name.c_str());
      rawFileReader->SetPixelSize(4);
      rawFileReader->SetRegion(requestedReg);
      
      treereader->SetFileName(segment_tree_name.c_str());

      relabeler->SetInputImage(rawFileReader->GetOutput());
      relabeler->SetInputSegmentTree( treereader->GetOutput());
      relabeler->SetFloodLevel((double)flood_level);
      
      rel_chunk_name = chunk_prefix + ".merged." + strbuf;
      writer->SetFileName(rel_chunk_name.c_str());
      writer->SetInput(relabeler->GetOutputImage());
      writer->UpdateLargestPossibleRegion();
    }
  }
  catch (itk::ExceptionObject &o)
    {
      std::cout << "ITK EXCEPTION DETECTED: " << o << std::endl;
    }
  
  return 0;
}
