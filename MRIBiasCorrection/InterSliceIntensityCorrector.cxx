/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    InterSliceIntensityCorrector.cxx
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

#pragma warning( once : 4786 )        // Issue warning 4385
//  only once.


#include <string>
#include <cmath>
#include <vector>

#include "mydefs.h"
#include "imageutils.h"
#include "OptionList.h"
#include "EnergyFunction.h"
#include "BiasField.h"
#include "itkFastRandomUnitNormalVariateGenerator.h"

#include <itkOnePlusOneEvolutionaryOptimizer.h>


void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: InterSliceIntensityCorrector --input file" ) ;
  print_line("       --output file") ;
  print_line("       --class-mean mean(1) ... mean(i)" ) ;
  print_line("       --class-sigma sigma(1) ... sigma(i)" ) ;
  print_line("       --use-log [yes|no]") ;
  print_line("       [--input-mask file]  [--output-mask file]" ) ;
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
  print_line("--output-mask file") ;
  print_line("        mask diff(output) with file [meta image format]");
  print_line("--use-log [yes|no]") ;
  print_line("        if yes, assume a multiplicative bias field") ;
  print_line("        (use log of image, class-mean, class-sigma,") ;
  print_line("         and init-step-size)" );
  print_line("--grow double") ;
  print_line("        stepsize growth factor. must be greater than 1.0") ;
  print_line("        [default 2.0]" ) ;
  print_line("--shrink double") ;
  print_line("        stepsize shrink factor ") ;
  print_line("        [default grow^(-0.25)]" ) ; 
  print_line("--max-iteration int") ;
  print_line("        maximal number of iterations") ;
  print_line("        [default 20]" ) ;
  print_line("--init-step-size double") ;
  print_line("        inital step size [default 2.0]" );

  print_line("");

  print_line("example: InterSliceIntensityCorrector --input sample.mhd") ;
  print_line("         --output intersliceintensitycorrectedsample.mha") ;
  print_line("         --class-mean 1500 570") ;
  print_line("         --class-sigma 100 70 --use-log yes");
  print_line("         --input-mask sample.mask.mhd ") ;
  print_line("         --output-mask sample.mask2.mhd") ;
  print_line("         --grow 1.05 --shrink 0.9");
  print_line("         --max-iteration 2000 --init-step-size 1.1") ;

}

    
void updateSlice(ImagePointer image, MaskPointer mask, 
                 int sliceNo, BiasField& biasField)
{
  ImageType::RegionType region = image->GetLargestPossibleRegion() ;
  ImageType::IndexType index = region.GetIndex() ;
  ImageType::SizeType size = region.GetSize() ;

  index[2] = sliceNo ;
  size[2] = 1 ;

  region.SetSize(size) ;
  region.SetIndex(index) ;

  bool maskAvailable = true ;

  if (image->GetLargestPossibleRegion().GetSize() !=
      mask->GetLargestPossibleRegion().GetSize())
    maskAvailable = false ;

  itk::SimpleImageRegionIterator<ImageType> iIter(image, region) ;
  iIter.Begin() ;
  
  BiasField::SimpleForwardIterator bIter(&biasField) ;
  bIter.Begin() ;

  if (maskAvailable)
    {
      itk::SimpleImageRegionIterator<MaskType> 
        mIter(mask, region) ;
      mIter.Begin() ;
      // mask diff image
      while (!iIter.IsAtEnd())
        {
          double pixel = iIter.Get() ;
          double bias = bIter.Get() ;
          double diff = pixel - bias ;

          if (mIter.Get() != 0.0)
            {
              if (biasField.IsMultiplicative())
                iIter.Set(exp(diff) - 1) ;
              else
                iIter.Set(diff) ;
            }
          else
            {
              if (biasField.IsMultiplicative())
                iIter.Set(exp(pixel) - 1) ;
              else
                iIter.Set(pixel) ;
            }

          ++mIter ;
          ++bIter ;
          ++iIter ;
        }
    }
  else
    {
      while (!iIter.IsAtEnd())
        {
          double bias = bIter.Get() ;
          double diff = iIter.Get() - bias ;

          if (biasField.IsMultiplicative())
            iIter.Set(exp(diff) - 1) ;
          else
            iIter.Set(diff) ;

          ++bIter ;
          ++iIter ;
        }
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

  std::string inputFileName ;
  std::string inputMaskFileName = "" ;
  std::string outputMaskFileName = "" ;
  std::string outputFileName ;
  bool useLog ;
  int degree ;
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
      options.GetStringOption("output", &outputFileName, true) ;
      options.GetStringOption("input-mask", &inputMaskFileName, false) ;
      options.GetStringOption("output-mask", &outputMaskFileName, false) ;

      // get bias field options
      useLog = options.GetBooleanOption("use-log", true) ;
      degree = options.GetIntOption("degree", false) ;
  
      // get energyfunction options
      options.GetMultiDoubleOption("class-mean", &classMeans, true) ;
      options.GetMultiDoubleOption("class-sigma", &classSigmas, true) ;

      // get optimizer options
      maximumIteration = options.GetIntOption("max-iteration", false) ;
      initialRadius = options.GetDoubleOption("step-size", false) ;
      grow = options.GetDoubleOption("grow", false) ;
      shrink = options.GetDoubleOption("shrink", false) ;
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
  MaskPointer inputMask = MaskType::New() ;
  MaskPointer outputMask = MaskType::New() ;

  try
    {
      std::cout << "Loading images..." << std::endl ;
      readMetaImageHeader(inputFileName, inputDimension, inputSize) ;
      loadImage(inputFileName, input) ;
      if (inputMaskFileName != "")
        loadMask(inputMaskFileName, inputMask) ;
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

  if (initialRadius <= 0)
    initialRadius = 10.0 ;

  if (useLog)
    {
      for (int i = 0 ; i < classMeans.size() ; i++) 
        {
          classSigmas[i] = log(1.0 + classSigmas[i] / (classMeans[i] + 1.0)) ;
          classMeans[i] = log(classMeans[i] + 1.0) ;
        }

      if (initialRadius > 0)
        initialRadius = log(initialRadius) ;

      logImage(input, input) ;
    }

  // initialize energy function
  typedef EnergyFunction<ImageType, MaskType> MyEnergy ;
  std::cout << "Initializing energy table..." << std::endl ;
  MyEnergy energy(classMeans, classSigmas) ;
  std::cout << "Energy table initialized." << std::endl ;

  energy.SetImage(input) ;

  energy.SetMask(inputMask) ;

  // initialize optimizer
  typedef itk::OnePlusOneEvolutionaryOptimizer<MyEnergy, 
    itk::FastRandomUnitNormalVariateGenerator> MyOptimizer ;
  MyOptimizer::Pointer optimizer = MyOptimizer::New() ;
  optimizer->SetCostFunction(&energy) ;
  
  if (grow > 0)
    {
      if (shrink > 0)
        optimizer->Initialize(initialRadius, grow, shrink) ;
      else
        optimizer->Initialize(initialRadius, grow) ;
    }
  else
    {
      if (shrink > 0)
        optimizer->Initialize(initialRadius, shrink) ;
      else
        optimizer->Initialize(initialRadius) ;
    }

  optimizer->SetMaximumIteration(maximumIteration) ;


  BiasField biasField(2, 0, inputSize) ;
  biasField.IsMultiplicative(useLog) ;
  energy.SetBiasField(&biasField) ;
  optimizer->SetSpaceDimension(biasField.GetNoOfCoefficients()) ;
  int totalSlices = input->GetLargestPossibleRegion().GetSize()[2] ;
  std::cout << "There are " << totalSlices << " slices." << std::endl ;
  for (int slice = 0 ; slice < totalSlices ; slice++)
    {
      std::cout << "Updating Slice (" << slice << ")" << std::endl ; 
      biasField.Initialize() ;
      optimizer->SetInitialPosition(biasField.GetCoefficients()) ;
      energy.SetSliceNo(slice) ;
      optimizer->Run() ;
      biasField.SetCoefficients(optimizer->GetCurrentPosition()) ;
      updateSlice(input, outputMask, slice, biasField) ;
      print_line("Slice updated.") ;
    }

  std::cout << "Writing corrected image..." << std::endl ;
  writeImage(outputFileName, input) ;
  std::cout << "Corrected image created." << std::endl ;

  return 0 ;
}
