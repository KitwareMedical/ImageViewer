/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SlabIdentifier.cxx
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
#include "mydefs.h"
#include "imageutils.h"
#include "OptionList.h"
#include "itkMRASlabIdentifier.h"

typedef itk::MRASlabIdentifier<ImageType> SlabIdentifier ;

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: SlabIdentifier --input file" ) ;
  print_line("       --samples-per-slice int" ) ;
  print_line("       --slice-direction [0-2]" ) ;

  print_line("");

  print_line("--input file") ;
  print_line("        input image file name [meta image format]" );
  print_line("--samples-per-slice int") ;
  print_line("        specifies how many pixels per pixel will be") ;
  print_line("        included for identification purpose.") ;
  print_line("--slice-direction [0-2]" ) ;
  print_line("        slice creation direction ( 0 - x axis, 1 - y axis") ;
  print_line("        2 - z axis)") ;

  print_line("");

  print_line("example: SlabIdentifier --input sample.mhd") ;
  print_line("         --samples-per-slice 10 --slice-direction 2") ;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  std::string inputFileName ;
  int sliceDirection ;
  int samplesPerSlice ;
  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      samplesPerSlice = options.GetIntOption("samples-per-slice", 10, true) ;
      sliceDirection = options.GetIntOption("slice-direction", 2, true) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }

  ImagePointer image = ImageType::New() ;

  std::cout << "Loading image..." << std::endl ;
  loadImage(inputFileName, image) ;
  std::cout << "Image loaded..." << std::endl ;
  SlabIdentifier::Pointer identifier = SlabIdentifier::New() ;

  identifier->SetImage(image) ;
  identifier->SetNumberOfMinimumsPerSlice(samplesPerSlice) ;
  identifier->SetSlicingDirection(sliceDirection) ;
  std::cout << "Searching slabs..." << std::endl ;
  identifier->GenerateSlabRegions() ;
  std::cout << "Search finished..." << std::endl ;
  SlabIdentifier::SlabRegionVectorType ranges = 
    identifier->GetSlabRegionVector() ;
  SlabIdentifier::SlabRegionVectorType::iterator iter = ranges.begin() ;
  int count = 0 ;
  std::cout << "slab\t\tfirst slice\t\tlength" << std::endl ; 
  while (iter != ranges.end())
    {
      std::cout << count << "\t\t" << iter->GetIndex()[sliceDirection] 
                << "\t\t" << iter->GetSize()[sliceDirection]
                << std::endl ; 
      count++ ;
      iter++ ;
    }
  return 0;
}
