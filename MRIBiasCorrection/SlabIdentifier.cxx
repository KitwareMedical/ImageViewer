/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    SlabIdentifier.cxx
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) 2002 Insight Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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

  
  std::cout << "Loading image..." << std::endl ;
  ImageReaderType::Pointer imageReader = ImageReaderType::New() ;
  try
    {
      std::cout << "Loading images..." << std::endl ;
      imageReader->SetFileName(inputFileName.c_str()) ;
      imageReader->Update() ;
    }
  catch (itk::ExceptionObject e)
    {
      e.Print(std::cout);
      exit(0) ;
    }

  ImagePointer image = imageReader->GetOutput() ;
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
