/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    InterSliceIntensityCorrector.cxx
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

#include "imageutils.h"
#include "metaITKUtils.h"
#include "OptionList.h"
#include "itkMRIBiasFieldCorrectionFilter.h"

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: InterSliceIntensityCorrector --input file" ) ;
  print_line("       --output file") ;
  print_line("       --class-mean mean(1) ... mean(i)" ) ;
  print_line("       --class-sigma sigma(1) ... sigma(i)" ) ;
  print_line("       --use-log [yes|no]") ;
  print_line("       [--input-mask file]  [--output-mask file]" ) ;
  print_line("       [--grow double] [--shrink double]") ;
  print_line("       [--inter-slice-max-iteration int]");
  print_line("       [--init-step-size double] [--slice-direction [0 - 2]]");

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
  print_line("        [default 1.05]" ) ;
  print_line("--shrink double") ;
  print_line("        stepsize shrink factor ") ;
  print_line("        [default grow^(-0.25)]" ) ; 
  print_line("--inter-slice-max-iteration int") ;
  print_line("        maximal number of iterations for inter-slice intensity correction") ;
  print_line("        [default 20]" ) ;
  print_line("--init-step-size double") ;
  print_line("        inital step size [default 1.02]" );
  print_line("--slice-direction [0 - 2]") ;
  print_line("        along which axes the slices are formed (0 - x, 1 - y,") ;
  print_line("        2 - z) [default 2 (z axis)]" );

  print_line("");

  print_line("example: InterSliceIntensityCorrector --input sample.mhd") ;
  print_line("         --output intersliceintensitycorrectedsample.mha") ;
  print_line("         --class-mean 1500 570") ;
  print_line("         --class-sigma 100 70 --use-log yes");
  print_line("         --input-mask sample.mask.mhd ") ;
  print_line("         --output-mask sample.mask2.mhd") ;
  print_line("         --grow 1.05 --shrink 0.9");
  print_line("         --max-iteration 2000 --init-step-size 1.02") ;

}

    
int main(int argc, char* argv[])
{
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  typedef itk::MRIBiasFieldCorrectionFilter<ImageType, ImageType, MaskType> Corrector ;
  Corrector::Pointer filter = Corrector::New() ;

  std::string inputFileName ;
  std::string inputMaskFileName = "" ;
  std::string outputMaskFileName = "" ;
  std::string outputFileName ;
  int sliceDirection ;
  bool useLog ;
  itk::Array<double> classMeans ;
  itk::Array<double> classSigmas ;
  int maximumIteration ;
  double initialRadius ;
  double grow ;
  double shrink ;

  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      options.GetStringOption("output", &outputFileName, true) ;
      options.GetStringOption("input-mask", &inputMaskFileName, false) ;
      options.GetStringOption("output-mask", &outputMaskFileName, false) ;

      sliceDirection = options.GetIntOption("slice-direction", 2, false) ;
      // get bias field options
      useLog = options.GetBooleanOption("use-log", true, true) ;
  
      // get energyfunction options
      options.GetMultiDoubleOption("class-mean", &classMeans, true) ;
      options.GetMultiDoubleOption("class-sigma", &classSigmas, true) ;

      // get optimizer options
      maximumIteration = options.GetIntOption("inter-slice-max-iteration", 20, false) ;
      grow = options.GetDoubleOption("grow", 1.05, false) ;
      shrink = pow(grow, -0.25) ;
      shrink = options.GetDoubleOption("shrink", shrink, false) ;
      initialRadius = options.GetDoubleOption("step-size", 1.02, false) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }


  // load images
  ImagePointer input ;
  MaskPointer inputMask ;
  MaskPointer outputMask ;
  
  ImageReaderType::Pointer imageReader = ImageReaderType::New() ;
  MaskReaderType::Pointer maskReader = MaskReaderType::New() ;
  MaskReaderType::Pointer maskReader2 = MaskReaderType::New() ;
  try
    {
      std::cout << "Loading images..." << std::endl ;
      imageReader->SetFileName(inputFileName.c_str()) ;
      imageReader->Update() ;
      input = imageReader->GetOutput() ;
      filter->SetInput(input) ;
      std::cout << "Input image loaded." << std::endl ;
      if (inputMaskFileName != "")
        {
          maskReader->SetFileName(inputMaskFileName.c_str()) ;
          maskReader->Update() ;
          inputMask = maskReader->GetOutput() ;
          filter->SetInputMask(inputMask) ;
          std::cout << "Input mask image loaded." << std::endl ;
        }
      if (outputMaskFileName != "")
        {
          maskReader2->SetFileName(outputMaskFileName.c_str()) ;
          maskReader2->Update() ;
          outputMask = maskReader2->GetOutput() ;
          filter->SetOutputMask(outputMask) ;
          std::cout << "Output mask image loaded." << std::endl ;
        }
      std::cout << "Images loaded." << std::endl ;
    }
  catch (itk::ExceptionObject e)
    {
      e.Print(std::cout);
      exit(0) ;
    }

  filter->SetSlicingDirection(sliceDirection) ;
  filter->IsBiasFieldMultiplicative(useLog) ;
  // sets tissue classes' statistics for creating the energy function
  filter->SetTissueClassStatistics(classMeans, classSigmas) ;
  // setting standard optimizer parameters 
  filter->SetOptimizerGrowthFactor(grow) ;
  filter->SetOptimizerShrinkFactor(shrink) ;
  filter->SetInterSliceCorrectionMaximumIteration(maximumIteration) ;
  filter->SetOptimizerInitialRadius(initialRadius) ;
  // this member function call is not necessary since the filter's internal
  // InterSliceIntensityCorrection() member sets the bias field degree to
  // zero.
  filter->SetBiasFieldDegree(0) ;
  // turn on inter-slice intensity correction 
  filter->SetUsingInterSliceIntensityCorrection(true) ;
  // disable slab identifcation
  // the filter will think the largest possible region as the only one
  // slab.
  filter->SetUsingSlabIdentification(false) ;
  // disable 3D bias correction
  filter->SetUsingBiasFieldCorrection(false) ;

  std::cout << "Correcting the input image..." << std::endl ;
  filter->Update() ;

  ImageType::Pointer output = filter->GetOutput() ;

  std::cout << "Writing the output image..." << std::endl ;
  metaITKUtilSaveImage<ImageType>(outputFileName.c_str(), NULL,
                                  output, MET_FLOAT, 1, MET_FLOAT);

  return 0 ;
}
