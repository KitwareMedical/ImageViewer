/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkResolverApp.cxx
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
#include "itkWatershedBoundary.h"
#include "itkSWSBoundaryDataReader.h"
#include "itkExceptionObject.h"
#include "itkWatershedBoundaryResolver.h"
#include "itkSWSEquivalencyTableReader.h"
#include "itkSWSEquivalencyTableWriter.h"

#include "param.h"
#include <string>
extern "C" {
#include <stdio.h>
}

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

/** This application identifies region equivalencies across volume chunk
    boundaries.  The input is the boundary data for each of the volume chunks.
    The output is a table containing all of the equivalencies among regions.*/
int main(int argc, char *argv[])
{
  typedef itk::watershed::Boundary<float, 3> BoundaryType;
  BoundaryType::IndexType idx;

  char strbuf[255];
  std::string segment_file_name, boundary_file_prefix, equivalency_file_name,
    fn, boundaryA_name, boundaryB_name;
  unsigned int chunk_dimensions[3], stride[3], sz[3], x, y, z;
  unsigned int chunk_number, dimension, edge, chunkA, chunkB, faceA, faceB, j;

  try
    {  // Read in each boundary face and process the equivalencies.
      itk::sws::BoundaryDataReader<float, 3>::Pointer BDReader =
        itk::sws::BoundaryDataReader<float, 3>::New();

      char *default_fn = "resolver.in";
      if (argc >1) fn = argv[1];
      else fn = default_fn;

      VPF::ParameterFile pf;
      // Open and parse the chunks.in file.  Required parameters.
      pf.Initialize(fn.c_str());

      if (VPF::set(segment_file_name, pf["segment_file_name"][0]) == VPF::INVALID)
        die ("Cannot find segment_file_name parameter");
      if (VPF::set(boundary_file_prefix, pf["boundary_file_prefix"][0]) == VPF::INVALID)
        die ("Cannot find boundary_file_prefix parameter");
      if (VPF::set(equivalency_file_name, pf["equivalency_file_name"][0]) == VPF::INVALID)
        die ("Cannot find equivalency_file_name parameter");

      if (VPF::set(chunk_dimensions[0], pf["chunk_dimensions"][0]) == VPF::INVALID)
        die ("Cannot find chunk_dimensions parameter");
      if (VPF::set(chunk_dimensions[1], pf["chunk_dimensions"][1]) == VPF::INVALID)
        die ("Cannot find chunk_dimensions parameter");
      if (VPF::set(chunk_dimensions[2], pf["chunk_dimensions"][2]) == VPF::INVALID)
        die ("Cannot find chunk_dimensions parameter");

      stride[0] = 1;
      
      itk::sws::BoundaryDataReader<float, 3>::Pointer readerA
        = itk::sws::BoundaryDataReader<float, 3>::New();
      itk::sws::BoundaryDataReader<float, 3>::Pointer readerB
        = itk::sws::BoundaryDataReader<float, 3>::New();
      itk::watershed::BoundaryResolver<float, 3>::Pointer resolver
        = itk::watershed::BoundaryResolver<float, 3>::New();

      for (dimension = 1; dimension < 3; ++dimension)
        {
          stride[dimension] = stride[dimension-1] *
            chunk_dimensions[dimension-1];
        }

      for (dimension = 0; dimension < 3; ++dimension)
        {
          chunkA = 0;
          for (j = 0; j < 3; j++)
            {
              if (j ==dimension) sz[j] = chunk_dimensions[j] - 1;
              else sz[j] = chunk_dimensions[j];
            }
          for (z = 0; z < sz[2]; z++)
            for (y = 0; y < sz[1]; y++)
              for (x = 0; x < sz[0]; x++)
                {
                  chunkA = x + y * stride[1] + z * stride[2];
                  chunkB = chunkA + stride[dimension];

                  ::sprintf(strbuf, "%u", chunkA);
                  boundaryA_name = boundary_file_prefix + "." + strbuf;

                  ::sprintf(strbuf, "%u", chunkB);
                  boundaryB_name = boundary_file_prefix + "." + strbuf;

                  std::cout << "chunk: " << chunkA << " face: "
                            << dimension << " 1 ";
                  std::cout << " <-> chunk: " << chunkB << " face: "
                            << dimension << " 0 ";
                  std::cout << std::endl;

                  readerA->SetFileName(boundaryA_name.c_str());
                  readerB->SetFileName(boundaryB_name.c_str());

                  resolver->SetFace(dimension);
                  resolver->SetBoundaryA(readerA->GetOutput());
                  resolver->SetBoundaryB(readerB->GetOutput());
                  resolver->UpdateLargestPossibleRegion();
                }
        }

      itk::sws::EquivalencyTableWriter::Pointer eqWriter
        = itk::sws::EquivalencyTableWriter::New();
      eqWriter->SetInput(resolver->GetEquivalencyTable());
      eqWriter->SetFileName(equivalency_file_name.c_str());
      eqWriter->SafeWrite();
    }
  catch (itk::ExceptionObject & e)
    {
      std::cout << "DETECTED ITK ERROR " << std::endl;
      std::cout << e << std::endl;
      exit (-1);
    }

  return 0;
}
