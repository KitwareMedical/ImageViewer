/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageCorrector.cxx
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
#include "itkMultivariateLegendrePolynomial.h"

typedef itk::MultivariateLegendrePolynomial BiasField ;

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: ImageCorrector --input file" ) ;
  print_line("       --output file") ;
  print_line("       --use-log [yes|no]") ;
  print_line("       --degree int --coefficients c0,..,cn" ) ;
  print_line("       [--output-mask file]" ) ;

  print_line("");

  print_line("--input file") ;
  print_line("        input image file name [meta image format]" );
  print_line("--output file") ;
  print_line("        output image file name [meta image format]" );
  print_line("--output-mask file") ;
  print_line("        mask diff(output) with file [meta image format]");
  print_line("--degree int") ;
  print_line("        degree of legendre polynomial used for the") ;
  print_line("        approximation of the bias field [default 3]" );
  print_line("--use-log [yes|no]") ;
  print_line("        if yes, assume a multiplicative bias field") ;
  print_line("        (use log of image, class-mean, class-sigma,") ;
  print_line("         and init-step-size)" );
  print_line("--coefficients c(0),..,c(n)") ;
  print_line("        coefficients of the polynomial") ;
  print_line("        (used for generating bias field)") ;

  print_line("");

  print_line("example: ImageCorrector --input sample.mhd") ;
  print_line("         --output biascorrectedsample.mhd") ;
  print_line("         --output-mask sample.mask.mhd") ;
  print_line("         --degree 3 --use-log yes");
  print_line("         --coefficients 0.056789 -1.00004 0.78945 ... -0.02345");
}

    
void correctBias(ImagePointer input, MaskPointer mask,
                 BiasField& biasField, ImagePointer output,
                 bool useLog) 
{
  std::cout << "Correcting bias..." << std::endl ;

  typedef ImageType::PixelType Pixel ;

  ImageType::RegionType region = input->GetLargestPossibleRegion() ;

  output->SetLargestPossibleRegion(region) ;
  output->SetBufferedRegion(region) ;
  output->Allocate() ;

  output->SetSpacing(input->GetSpacing()) ;

  bool maskAvailable = true ;
  if (region.GetSize() != mask->GetLargestPossibleRegion().GetSize())
    {
      maskAvailable = false ;
    }
  
  itk::ImageRegionIteratorWithIndex<ImageType> iIter(input, region) ;
  
  BiasField::SimpleForwardIterator bIter(&biasField) ;

  itk::ImageRegionIteratorWithIndex<ImageType> oIter(output, region) ;

  if (maskAvailable)
    {
      itk::ImageRegionIteratorWithIndex<MaskType> mIter(mask, region) ;
      mIter.GoToBegin() ;
      // mask diff image
      while (!oIter.IsAtEnd())
        {
          double inputPixel = iIter.Get() ;
          double diff = inputPixel - bIter.Get() ;

          if (mIter.Get() > 0.0)
            {
              if (useLog)
                oIter.Set( (Pixel) ( exp(diff) - 1 )) ;
              else
                oIter.Set( (Pixel) diff) ;
            }
          else
            {
              if (useLog)
                oIter.Set( (Pixel) ( exp(inputPixel) - 1) ) ;
              else
                oIter.Set( (Pixel) inputPixel) ;
            }
          ++mIter ;
          ++oIter ;
          ++bIter ;
          ++iIter ;
        }
    }
  else
    {
      while (!oIter.IsAtEnd())
        {
          double diff = iIter.Get() - bIter.Get() ;
          if (useLog)
            oIter.Set( (Pixel) ( exp(diff) - 1)) ;
          else
            oIter.Set( (Pixel) diff) ;
          ++oIter ;
          ++bIter ;
          ++iIter ;
        }
    }

  std::cout << "bias correction done." << std::endl ;
}

int main(int argc, char* argv[])
{
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  std::string inputFileName ;
  std::string outputMaskFileName = "" ;
  std::string outputFileName ;
  bool useLog ;
  int degree ;
  vnl_vector<double> coefficientVector ;

  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      options.GetStringOption("output", &outputFileName, true) ;
      options.GetStringOption("output-mask", &outputMaskFileName, false) ;
      
      // get bias field options
      useLog = options.GetBooleanOption("use-log", true, true) ;
      std::vector<double> coefficients ;
      options.GetMultiDoubleOption("coefficients", &coefficients, true) ;
      int length = coefficients.size() ;
      coefficientVector.resize(length) ;
      for (int i = 0 ; i < length ; i++)
        coefficientVector[i] = coefficients[i] ;
      
      degree = options.GetIntOption("degree", 3, true) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }
  

  // load images
  ImagePointer input = ImageType::New() ;
  MaskPointer outputMask = MaskType::New() ;
  
  try
    {
      std::cout << "Loading images..." << std::endl ;
      loadImage(inputFileName, input) ;
      if (outputMaskFileName != "")
        loadMask(outputMaskFileName, outputMask) ;
      std::cout << "Images loaded." << std::endl ;
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }

      
  if (useLog)
    {
      logImage(input, input) ;
    }
      
  BiasField::DomainSizeType biasSize ;
  ImageType::SizeType size = input->GetLargestPossibleRegion().GetSize() ;
  long dim = 0 ;
  biasSize.clear() ;
  for(dim = 0 ; dim < ImageType::ImageDimension ; dim++)
    {
      if (size[dim] > 1)
        biasSize.push_back(size[dim]) ;
    }

  BiasField biasField(biasSize.size(), degree, biasSize) ;

  try
    {
      biasField.SetCoefficients(coefficientVector) ;
    }
  catch(BiasField::CoefficientVectorSizeMismatch m)
    {
      std::cout << "Error: Invalid number of Coefficients for the bias field." 
                << std::endl ;
      std::cout << "given size: " << m.Given 
                << "required size: " 
                << m.Required  
                << std::endl ;
      exit(0) ;
    }
      
  ImagePointer output = ImageType::New() ;
  correctBias(input, outputMask, biasField, output, useLog) ;

  std::cout << "Writing corrected image..." << std::endl ;
  writeImage(outputFileName, output) ;
  std::cout << "Corrected image created." << std::endl ;

  return 0 ;
}
