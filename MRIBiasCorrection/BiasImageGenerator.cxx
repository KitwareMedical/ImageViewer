/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BiasImageGenerator.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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

  BiasField::DomainSizeType biasSize = biasField.GetDomainSize() ;
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

  itk::ImageRegionIteratorWithIndex<ImageType> oIter(output, region) ;

  while (!oIter.IsAtEnd())
    {
      oIter.Set( (Pixel) bIter.Get()) ;

      ++oIter ;
      ++bIter ;
    }
}

int main(int argc, char* argv[])
{

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
  BiasField::DomainSizeType biasSize ;

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
      degree = options.GetIntOption("degree", 3, true) ;
      dimension = options.GetIntOption("dimension", 3,  true) ;

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
            biasSize.push_back(sizes[i]) ;
        }
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }
  
  
  
  BiasField biasField(biasSize.size(), degree, biasSize) ;
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
