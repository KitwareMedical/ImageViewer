/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BiasFieldEstimator.cxx
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
#include "itkMRIBiasFieldCorrectionFilter.h"

typedef itk::MRIBiasFieldCorrectionFilter<ImageType, ImageType> Corrector ;

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: BiasFieldEstimator --input file" ) ;
  print_line("       --class-mean mean(1) ... mean(i)" ) ;
  print_line("       --class-sigma sigma(1) ... sigma(i)" ) ;
  print_line("       --use-log [yes|no]") ;
  print_line("       [--input-mask file]" ) ;
  print_line("       [--degree int] [--coefficients c0,..,cn]" ) ;
  print_line("       [--grow double] [--shrink double] [--max-iteration int]");
  print_line("       [--init-step-size double] ");

  print_line("");

  print_line("--input file") ;
  print_line("        input data set [meta image format]" );
  print_line("--class-mean mean(1),...,mean(i)" ) ;
  print_line("        intensity means  of the differen i tissue classes") ;
  print_line("--class-sigma sig(1),...,sig(i)" ) ; 
  print_line("        intensity sigmas of the different i tissue clases") ;
  print_line("        NOTE: The sigmas should be listed in the SAME ORDER") ;
  print_line("              of means");
  print_line("        and each value should be SEPERATED BY A SPACE)") ;
  print_line("--input-mask file" ) ;
  print_line("        mask input with file [meta image format]");
  print_line("--degree int") ;
  print_line("        degree of legendre polynomial used for the") ;
  print_line("        approximation of the bias field" );
  print_line("--use-log [yes|no]") ;
  print_line("        if yes, assume a multiplicative bias field") ;
  print_line("        (use log of image, class-mean, class-sigma,") ;
  print_line("         and init-step-size)" );
  print_line("--grow double") ;
  print_line("        stepsize growth factor. must be greater than 1.0") ;
  print_line("        [default 1.05]" ) ;
  print_line("--shrink double") ;
  print_line("        stepsize shrink factor ") ;
  print_line("        [default grow^(-0.25)]" ) ; 
  print_line("--max-iteration int") ;
  print_line("        maximal number of iterations") ;
  print_line("        [default 20]" ) ;
  print_line("--init-step-size double") ;
  print_line("        inital step size [default 1.02]" );
  print_line("--coefficients c(0),..,c(n)") ;
  print_line("        coefficients of the polynomial") ;
  print_line("        (used for generating bias field)") ;

  print_line("");

  print_line("example: BiasFieldEstimator --input sample.mhd") ;
  print_line("         --class-mean 1500 570") ;
  print_line("         --class-sigma 100 70 --use-log yes");
  print_line("         --input-mask sample.mask.mhd ") ;
  print_line("         --degree 3 --grow 1.05 --shrink 0.9");
  print_line("         --max-iteration 2000 --init-step-size 1.02") ;
  print_line("         --coefficients 0.056789 -1.00004 0.78945 ... -0.02345");
}


void printResult(Corrector::Pointer filter, OptionList& options)
{

  options.DumpOption("input") ;
  options.DumpOption("input-mask") ;
  options.DumpOption("class-mean") ;
  options.DumpOption("class-sigma") ;
  options.DumpOption("use-log") ;

  std::cout << " --degree " << filter->GetBiasFieldDegree() ;

  Corrector::BiasField::DomainSizeType sizes = 
    filter->GetBiasFieldDomainSize() ;
  std::cout << " --size " ;
  for (int i = 0 ; i < sizes.size() ; i++)
    std::cout << sizes[i] << " " ;
  
  std::cout << "--grow " << filter->GetOptimizerGrowFactor() ;
  std::cout << " --shrink " << filter->GetOptimizerShrinkFactor() ;
  std::cout << " --max-iteration " << filter->GetOptimizerMaximumIteration();
  
  if (filter->IsBiasFieldMultiplicative())
    std::cout << " --init-step-size " 
              << exp(filter->GetOptimizerInitialRadius()) ;
  else
    std::cout << " --init-step-size " << filter->GetOptimizerInitialRadius() ;


  std::cout << " --coefficient-length " << filter->GetNoOfBiasFieldCoefficients()
            << " --coefficients " ;

  Corrector::BiasField::CoefficientVector coefficients = 
    filter->GetEstimatedBiasFieldCoefficients() ;
  Corrector::BiasField::CoefficientVector::iterator iter =
    coefficients.begin() ;

  while (iter != coefficients.end()) 
    {
      std::cout << *iter << " " ;
      iter++ ;
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

  Corrector::Pointer filter = Corrector::New() ;

  std::string inputFileName ;
  std::string inputMaskFileName = "" ;
  bool useLog ;
  int degree ;
  vnl_vector<double> coefficientVector ;
  std::vector<double> classMeans ;
  std::vector<double> classSigmas ;
  int maximumIteration ; 
  double initialRadius ;
  double grow ;
  double shrink ;

  int inputDimension ;
  itk::Size<3> inputSize ;
  
  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      options.GetStringOption("input-mask", &inputMaskFileName, false) ;
      
      // get bias field options
      useLog = options.GetBooleanOption("use-log", true, true) ;
      degree = options.GetIntOption("degree", 3, false) ;
      
      std::vector<double> coefficients ;
      options.GetMultiDoubleOption("coefficients", &coefficients, false) ;

      int length = coefficients.size() ;
      coefficientVector.resize(length) ;
      for (int i = 0 ; i < length ; i++)
        coefficientVector[i] = coefficients[i] ;

      // get energyfunction options
      options.GetMultiDoubleOption("class-mean", &classMeans, true) ;
      options.GetMultiDoubleOption("class-sigma", &classSigmas, true) ;

      // get optimizer options
      maximumIteration = options.GetIntOption("max-iteration", 20, false) ;
      grow = options.GetDoubleOption("grow", 1.05, false) ;
      shrink = pow(grow, -0.25) ;
      shrink = options.GetDoubleOption("shrink", shrink, false) ;
      initialRadius = options.GetDoubleOption("init-step-size", 1.02, false) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
    }

      
  // load images
  ImagePointer input = ImageType::New() ;
  MaskPointer inputMask = MaskType::New() ;
  
  try
    {
      std::cout << "Loading images..." << std::endl ;
      readMetaImageHeader(inputFileName, inputDimension, inputSize) ;
      loadImage(inputFileName, input) ;
      filter->SetInput(input) ;
      if (inputMaskFileName != "")
        {
          loadMask(inputMaskFileName, inputMask) ;
          filter->SetInputMask(inputMask) ;
        }
      std::cout << "Images loaded." << std::endl ;
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }

  filter->IsBiasFieldMultiplicative(useLog) ;
  filter->SetInitialBiasFieldCoefficients(coefficientVector) ;
  // sets tissue classes' statistics for creating the energy function
  filter->SetTissueClassStatistics(classMeans, classSigmas) ;
  // setting standard optimizer parameters 
  filter->SetOptimizerGrowFactor(grow) ;
  filter->SetOptimizerShrinkFactor(shrink) ;
  filter->SetOptimizerMaximumIteration(maximumIteration) ;
  filter->SetOptimizerInitialRadius(initialRadius) ;
  // this member function call is not necessary since the filter's internal
  // InterSliceIntensityCorrection() member sets the bias field degree to
  // zero.
  filter->SetBiasFieldDegree(degree) ;
  // turn on inter-slice intensity correction 
  filter->SetUsingInterSliceIntensityCorrection(false) ;
  // disable slab identifcation
  // the filter will think the largest possible region as the only one
  // slab.
  filter->SetUsingSlabIdentification(false) ;
  // disable 3D bias correction
  filter->SetUsingBiasFieldCorrection(true) ;
  // disable output image generation
  filter->SetGeneratingOutput(false) ;

  std::cout << "Estimating the bias field..." << std::endl ;
  filter->Update() ;

  printResult(filter, options) ;

  return 0 ;
}
