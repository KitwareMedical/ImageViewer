/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BiasImageGenerator.cxx
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
#include <vector>
#include <vnl/vnl_math.h>


#include "mydefs.h"
#include "imageutils.h"
#include "OptionList.h"
#include "BiasField.h"


void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: BiasImageGenerator --output" ) ;
  print_line("       --dimension int --size long long long") ;
  print_line("       --degree int") ;
  print_line("       --coefficients c0,..,cn" ) ;

  print_line("");

  print_line("--output file") ;
  print_line("        output image file [meta image format]" );
  print_line("--dimension int") ;
  print_line("        output image dimension") ;
  print_line("--size x y z") ;
  print_line("        size of each image dimension") ;
  print_line("--degree int") ;
  print_line("        degree of legendre polynomial used for the") ;
  print_line("        approximation of the bias field" );
  print_line("--coefficients c(0),..,c(n)") ;
  print_line("        coefficients of the polynomial") ;
  print_line("        (used for generating bias field)") ;

  print_line("") ;

  print_line("example: BiasImageGenerator --output biasfromsample.mhd") ;
  print_line("         --dimension 3 --size 256 256 100 --degree 3") ;
  print_line("         --coefficients 0.056789 -1.00004 0.78945 ... -0.02345");

}

    
void generateBiasImage(BiasField& biasField, ImagePointer output) 
{
  typedef ImageType::PixelType Pixel ;

  ImageType::IndexType index ;
  ImageType::SizeType size ;
  ImageType::RegionType region ;

  itk::Size<3> biasSize = biasField.GetSize() ;
  for (int i = 0 ; i < ImageType::ImageDimension ; i++)
    {
      if (i < biasField.GetDimension())
        size[i] = biasSize[i] ;
      else
        size[i] = 0 ;

      index[i] = 0 ;
    }

  region.SetSize(size) ;
  region.SetIndex(index) ;

  output->SetLargestPossibleRegion(region) ;
  output->SetBufferedRegion(region) ;
  output->Allocate() ;

  BiasField::SimpleForwardIterator bIter(&biasField) ;
  bIter.Begin() ;

  itk::SimpleImageRegionIterator<ImageType> oIter(output, region) ;
  oIter.Begin() ;

  while (!oIter.IsAtEnd())
    {
      oIter.Set( (Pixel) bIter.Get()) ;

      ++oIter ;
      ++bIter ;
    }
}

int main(int argc, char* argv[])
{
  int ret ;
  double d_ret ;

  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  // get image file options
  std::string outputFileName ;
  int dimension ;
  int degree ;
  vnl_vector<double> coefficientVector ;
  itk::Size<3> biasSize ;

  try
    {
      options.GetStringOption("output", &outputFileName, true) ;
      // get bias field options
      std::vector<double> coefficients ;
      options.GetMultiDoubleOption("coefficients", &coefficients, true) ;
      int length = coefficients.size() ;
      coefficientVector.resize(length) ;
      for (int i = 0 ; i < length ; i++)
        coefficientVector[i] = coefficients[i] ;
      degree = options.GetIntOption("degree", true) ;
      dimension = options.GetIntOption("dimension", true) ;

      std::vector<int> sizes ;
      options.GetMultiIntOption("size", &sizes, true) ;
      if (sizes.size() < 2)
        {
          std::cout << "Error: you have to provide more than one size values."
                    << std::endl ;
          exit(0) ;
        }

      for (int i = 0 ; i < sizes.size() ; i++)
        {
          if (i < 3)
            biasSize[i] = sizes[i] ;
        }
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }
  
  
  
  BiasField biasField(dimension, degree, biasSize) ;
  //biasField.IsMultiplicative(useLog) ;
  try
    {
      biasField.SetCoefficients(coefficientVector) ;
    }
  catch(BiasField::CoefficientVectorSizeMismatch m)
    {
      std::cout << "Error: Invalid Coefficients for the bias fiedl" 
                << std::endl ;
      std::cout << "given size: " << m.Given 
                << "required size: " 
                << m.Required  
                << std::endl ;
      exit(0) ;
    }
  
  ImagePointer output = ImageType::New() ;
  generateBiasImage(biasField, output) ;

  try
    {
      writeImage(outputFileName, output) ;
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }


  return 0 ;
}
