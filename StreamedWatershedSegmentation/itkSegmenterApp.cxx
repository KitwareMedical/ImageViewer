/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSegmenterApp.cxx
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
#include "itkWatershedSegmenter.h"
#include "itkSWSRawFileReader.h"
#include "itkSWSRawFileWriter.h"
#include "itkImage.h"
#include "itkSWSSegmentTableWriter.h"
#include "itkSWSBoundaryDataWriter.h"
#include "itkSWSBoundaryDataReader.h"

#include "param.h"
#include <string>

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

/** This application performs the initial segmentation on each of the
    volumetric image chunks.  It fills in and stores boundary data for use
    later in resolving segments connected across boundaries and relabling image 
    chunks accordingly.  This is the first step in the streaming watershed
    segmentation algorithm. */
int main(int argc, char *argv[])
{  
  typedef itk::Image<float, 3> FloatImageType;
  typedef itk::Image<unsigned long, 3> LongImageType;
  float threshold;
  float maximum_flood_level;
  float maximum, minimum;
  int  boundary_analysis;
  char strbuf[255];
  float global_minimum;
  float global_maximum;
  float maxDepth, prune_value;
  int prune_as_we_go;

  try{
 
  itk::sws::RawFileReader<FloatImageType>::Pointer rawFileReader
    = itk::sws::RawFileReader<FloatImageType>::New();

  itk::sws::SegmentTableWriter<float>::Pointer segwriter
    = itk::sws::SegmentTableWriter<float>::New(); 

  itk::watershed::Segmenter<FloatImageType>::Pointer seg =
    itk::watershed::Segmenter<FloatImageType>::New();
  
  itk::sws::RawFileWriter<LongImageType>::Pointer writer =
    itk::sws::RawFileWriter<LongImageType>::New();

  itk::sws::BoundaryDataWriter<float, 3>::Pointer BDWriter =
    itk::sws::BoundaryDataWriter<float, 3>::New();
  
  itk::sws::BoundaryDataReader<float, 3>::Pointer BDReader =
    itk::sws::BoundaryDataReader<float, 3>::New();
    
  std::string fn, in_chunkfile, chunk_name, chunk_prefix, output_chunk_file,
              segment_file_name, out_filename, output_prefix;

  int pixel_size;
  char *default_fn = "segmenter.in";
  if (argc >1) fn = argv[1];
  else fn = default_fn;

  VPF::ParameterFile pf;
  // Open and parse the parametr file.
  pf.Initialize(fn.c_str());
  
  if (VPF::set(in_chunkfile, pf["input_chunk_file"][0]) == VPF::INVALID)
    die ("Cannot find input_chunk_file parameter");
  if (VPF::set(chunk_prefix, pf["chunk_prefix"][0]) == VPF::INVALID)
    die ("Cannot find chunk_prefix parameter");
  if (VPF::set(segment_file_name, pf["segment_file_name"][0]) == VPF::INVALID)
    die ("Cannot find segment_file_name parameter");
  if (VPF::set(threshold, pf["threshold"][0]) == VPF::INVALID)
    die ("Cannot find threshold parameter");
  if (VPF::set(output_prefix, pf["output_prefix"][0]) == VPF::INVALID)
    die ("Cannot find output_prefix parameter");
  if (VPF::set(output_chunk_file, pf["output_chunk_file"][0]) == VPF::INVALID)
    die ("Cannot find output_chunk_file parameter");
  if (VPF::set(boundary_analysis, pf["boundary_analysis"][0]) == VPF::INVALID)
    die ("Cannot find boundary_analysis parameter");
  if (VPF::set(pixel_size, pf["pixel_size"][0]) == VPF::INVALID)
    die ("Cannot find pixel_size parameter");
  
  if (VPF::set(maximum_flood_level, pf["maximum_flood_level"][0]) == VPF::INVALID)
    die ("Cannot find maximum_flood_level parameter");

  if (VPF::set(prune_value, pf["prune_value"][0]) == VPF::INVALID)
    die ("Cannot find prune value parameter");
  if (VPF::set(prune_as_we_go, pf["prune_as_we_go"][0]) == VPF::INVALID)
      die ("Cannot find prune_as_we_go parameter");

  // Read a set of chunk definitions
  int chunknumber;
  ctk::chunk_info_struct *chunk_list;
  
  std::ifstream in;
  in.open(in_chunkfile.c_str());
  if (!in)
    {
      std::cerr << "Could not open file" << in_chunkfile << std::endl;
      ::exit(1);
    }
  //read the number of   chunks to follow 
  in.read((char *)&chunknumber, sizeof(int)); 
  chunk_list = new ctk::chunk_info_struct[chunknumber];
  in.read((char *)chunk_list, chunknumber *
          sizeof(ctk::chunk_info_struct)); 
  in.close();

  FloatImageType::RegionType largestReg;
  FloatImageType::RegionType requestedReg;
  FloatImageType::SizeType    sz;
  FloatImageType::IndexType   idx;
  int counter = 0;
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
      rawFileReader->SetPixelSize(pixel_size);
      rawFileReader->SetRegion(requestedReg);

      seg->SetLargestPossibleRegion(largestReg);
      seg->SetInputImage(rawFileReader->GetOutput());
      seg->SetThreshold((double)threshold);
      seg->SetDoBoundaryAnalysis((bool)boundary_analysis);
      seg->SetSortEdgeLists(false); // we'll do this after all chunks are
                                    // processed 

      // Dump an image to disk for debugging purposes:
      out_filename = output_prefix + ".segmented." + strbuf;
      writer->SetFileName(out_filename.c_str());
      writer->SetInput(seg->GetOutputImage());

      std::cout << "CurrentLabel = " << seg->GetCurrentLabel() << std::endl;
      writer->UpdateLargestPossibleRegion();

      // Dump the boundary information to disk
      out_filename = output_prefix + ".segmented.boundary." + strbuf;
      BDWriter->SetInput(seg->GetBoundary());
      BDWriter->SetFileName(out_filename.c_str());
      BDWriter->SafeWrite();

      // Modify the chunk record region size to reflect the output size
      sz = seg->GetOutputImage()->GetBufferedRegion().GetSize();
      idx= seg->GetOutputImage()->GetBufferedRegion().GetIndex();
      chunk_list[i].set_padded_sz(sz[0], sz[1], sz[2]);
      chunk_list[i].set_padded_idx(idx[0], idx[1], idx[2]);
      chunk_list[i].set_pixel_sz(4);

      // Modify the maximum and minimum values
      if (counter == 0 )
        {
          maximum = seg->GetMaximum();
          minimum = seg->GetMinimum();
          counter = 1;
        }
      else
        {
          if (seg->GetMaximum() > maximum) maximum = seg->GetMaximum();
          if (seg->GetMinimum() > minimum) minimum = seg->GetMinimum();
        }

      std::cout << "MAXIMUM = " << seg->GetMaximum() << std::endl;
      std::cout << "MINIMUM = " << seg->GetMinimum()<< std::endl;

      // Keep edge list sizes down during processing
      if (prune_as_we_go == 1)
        {
          //   seg->GetSegmentTable()->SortEdgeLists();
          //   seg->GetSegmentTable()->PruneEdgeLists(prune_value);
        }
    }

  // Set the segment tree's maximum depth
  seg->GetSegmentTable()->SetMaximumDepth(maximum - minimum);
  std::cout << "Prune value is " << prune_value << std::endl;
  std::cout << "Maximum depth is " << maximum - minimum << std::endl;
  maxDepth = (maximum - minimum) * maximum_flood_level;

  std::cout << "Pruning the edge lists" << std::endl;

  // Prune the segment table edge lists
  seg->GetSegmentTable()->SortEdgeLists();
  seg->GetSegmentTable()->PruneEdgeLists(maxDepth);
  std::cout << "done Pruning the edge lists" << std::endl;
  
  // Dump the segment tree to disk
  segwriter->SetInput(seg->GetSegmentTable());
  segwriter->SetFileName(segment_file_name.c_str());
  segwriter->SafeWrite();

  // Now write the chunk headers
  std::ofstream out;
  out.open(output_chunk_file.c_str());
  if (!out)
    {
      std::cerr << "Could not open output chunk file " << output_chunk_file <<
        std::endl;
      ::exit(1);
    }
  //write the number of chunks to follow
  out.write((char *)&chunknumber, sizeof(int));
  out.write((char *)chunk_list, chunknumber *
            sizeof(ctk::chunk_info_struct));
  out.close();
  
  }
  catch (itk::ExceptionObject & e)
    {
      std::cout << "DETECTED ITK ERROR " << std::endl;
      std::cout << e << std::endl;
      exit (-1);
    }
  
  return 0;
}
