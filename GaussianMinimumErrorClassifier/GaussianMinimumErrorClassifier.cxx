/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianMinimumErrorClassifier.cxx
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

#include "OptionList.h"
#include "itkImage.h"
#include "itkReadMetaImage.h"
#include "itkWriteMetaImage.h"
#include "itkImageRegionIteratorWithIndex.h"

#include "vnl/vnl_matrix.h"

#include "itkImageListSampleAdaptor.h"
#include "itkSubsample.h"
#include "itkMembershipSample.h"
#include "itkMembershipSampleGenerator.h"
#include "itkGaussianDensityFunction.h"
#include "itkMeanCalculator.h"
#include "itkCovarianceCalculator.h"
#include "itkGenericClassifier.h"
#include "MaximumLikelihoodRatioDecisionRule.h"

void print_usage()
{
  std::cout << "GaussianMinimumErrorClassifier 1.0 (17. Dec. 2001)" << std::endl ;

  std::cout << "usage: GaussianMinimumErrorClassifier --training file"  << std::endl ;
  std::cout << "       --class-mask file" << std::endl ;
  std::cout << "       --target file"  << std::endl ;
  std::cout << "       --output file"  << std::endl ;

  std::cout << "" << std::endl ;

  std::cout << "--training file" << std::endl ;
  std::cout << "        image file name with intesnity values [meta image format]"  
    << std::endl ;
  std::cout << "--class-mask file" << std::endl ;
  std::cout << "        image file name with class labels [meta image format]"  
    << std::endl ;
  std::cout << "--target file" << std::endl ;
  std::cout << "        target image file name with intensity values [meta image format]" 
    << std::endl ;
  std::cout << "--output file" << std::endl ;
  std::cout << "        output image file name that will have the class labels for pixels" 
    << std::endl ;
  std::cout << "        in the target image file [meta image format]"  << std::endl ;

  std::cout << "" << std::endl ;

  std::cout << "example: GaussianMinimumErrorClassifier --training train.mhd" << std::endl ;
  std::cout << "         --class-mask class_mask.mhd" << std::endl ;
  std::cout << "         --target target.mhd" << std::endl ;
  std::cout << "         --output output.mhd" << std::endl ;
}


int main(int argc, char* argv[])
{
  namespace stat = itk::Statistics ;
 
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  std::string trainingFileName ;
  std::string classMaskFileName ;
  std::string targetFileName ;
  std::string outputFileName ;

  try
    {
      // get image file options
      options.GetStringOption("training", &trainingFileName, true) ;
      // get image file options
      options.GetStringOption("class-mask", &classMaskFileName, true) ;
      // get image file options
      options.GetStringOption("target", &targetFileName, true) ;
      // get image file options
      options.GetStringOption("output", &outputFileName, true) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      return 1 ;
    }

  typedef itk::Image< short, 3 > ImageType ;
  typedef ImageType::Pointer ImagePointer ;

  ImagePointer training ;
  ImagePointer classMask ;
  ImagePointer target ;

  std::cout << "Loading image(s)..." << std::endl ;
  //      readMetaImageHeader(trainingFileName, trainingDimension, trainingSize) ;
  typedef itk::ReadMetaImage<ImageType> Reader ;
  Reader::Pointer reader = Reader::New() ;
      
  reader->SetFileName(trainingFileName.c_str()) ;
  reader->Update() ;
  training = reader->GetOutput() ;
  std::cout << "Training image loaded." << std::endl ;

  Reader::Pointer reader2 = Reader::New() ;
  reader2->SetFileName(classMaskFileName.c_str()) ;
  reader2->Update() ;
  classMask = reader2->GetOutput() ;
  std::cout << "Class mask loaded." << std::endl ;

  Reader::Pointer reader3 = Reader::New() ;
  reader3->SetFileName(targetFileName.c_str()) ;
  reader3->Update() ;
  target = reader3->GetOutput() ;
  std::cout << "Target image loaded." << std::endl ;

  /* ================================================== */
  std::cout << "Importing the images to samples..."
            << std::endl ;
  typedef stat::ImageListSampleAdaptor<ImageType, short, 1> 
    ImageListSampleType;

  ImageListSampleType::Pointer sample =
    ImageListSampleType::New() ;
  
  ImageListSampleType::Pointer mask =
    ImageListSampleType::New() ;

  ImageListSampleType::Pointer targetSample =
    ImageListSampleType::New() ;

  sample->SetImage(training);
  mask->SetImage(classMask) ;
  targetSample->SetImage(target) ;
  /* ==================================================== */
  std::cout << "Creating the membership sample for training.." << std::endl ;
  typedef stat::MembershipSampleGenerator< ImageListSampleType, 
    ImageListSampleType > MembershipSampleGeneratorType ;  
  MembershipSampleGeneratorType::Pointer generator = 
    MembershipSampleGeneratorType::New() ;

  generator->SetInput(sample) ;
  generator->SetClassMask(mask) ;
  generator->GenerateData() ;
  MembershipSampleGeneratorType::OutputPointer membershipSample = 
    generator->GetOutput() ;
  unsigned int numberOfClasses = membershipSample->GetNumberOfClasses() ;
  /* =================================================== */
  std::cout << "Inducing the gaussian density function parameters and apriori probabilities..." 
            << std::endl ;
  typedef ImageListSampleType::MeasurementVectorType
    MeasurementVectorType ;
  typedef stat::GaussianDensityFunction< MeasurementVectorType > 
    DensityFunctionType ;
  typedef DensityFunctionType::Pointer DensityFunctionPointer ;
  std::vector< DensityFunctionPointer > densityFunctions ;
  densityFunctions.resize(numberOfClasses) ;

  typedef MembershipSampleGeneratorType::OutputType::ClassSampleType 
    ClassSampleType ;
  typedef stat::MeanCalculator< ClassSampleType > 
    MeanCalculatorType ;
  typedef stat::CovarianceCalculator< ClassSampleType >
    CovarianceCalculatorType ;
  MeanCalculatorType::Pointer meanCalculator = MeanCalculatorType::New() ;
  CovarianceCalculatorType::Pointer covarianceCalculator = 
    CovarianceCalculatorType::New() ;
  vnl_vector< double > mean ;
  vnl_matrix< double > covariance ;

  typedef MaximumLikelihoodRatioDecisionRule DecisionRuleType ;
  DecisionRuleType::Pointer rule = DecisionRuleType::New() ;

  unsigned int sampleSize = 0 ;

  for (unsigned int i = 0 ; i < numberOfClasses ; i++)
    {
      meanCalculator->SetSample(membershipSample->GetClassSample(i)) ;
      meanCalculator->GenerateData() ;
      mean = meanCalculator->GetOutput() ;

      covarianceCalculator->SetSample(membershipSample->GetClassSample(i)) ;
      covarianceCalculator->SetMean(mean) ;
      covarianceCalculator->GenerateData() ;
      covariance = covarianceCalculator->GetOutput() ;
      
      densityFunctions[i] = DensityFunctionType::New() ;
      (densityFunctions[i])->SetMean(mean) ;
      (densityFunctions[i])->SetCovariance(covariance) ;
      std::cout << "gaussian [" << i << "]" << std::endl ;
      std::cout << "  mean = " << (densityFunctions[i])->GetMean()
                << std::endl ;
      std::cout << "  covariance = " << std::endl ;
      (densityFunctions[i])->GetCovariance().print(std::cout) ;
      
      // add the class sample size to the decision rule 
      // for the a priori probability calculation
      std::cout << "  Sample size = " ;
      sampleSize = (membershipSample->GetClassSample(i))->GetSize(0) ;
      std::cout << sampleSize << std::endl ;
      rule->AddClassSampleSize(sampleSize) ;
    }
  /* =================================================== */
  std::cout << "Classifying..." << std::endl ;
  
  typedef stat::GenericClassifier< ImageListSampleType, DensityFunctionType, DecisionRuleType > ClassifierType ;

  ClassifierType::Pointer classifier = ClassifierType::New() ;
  classifier->SetSample(targetSample) ;
  for (unsigned int i = 0 ; i < numberOfClasses ; i++)
    {
      classifier->AddMembershipCalculator(densityFunctions[i]) ;
    }

  classifier->SetDecisionRule(rule) ;
  
  classifier->GenerateData() ;
  
  ClassifierType::OutputPointer result = classifier->GetOutput() ;

  /* ===================================================== */
  std::cout << "Creating a image with result class labels..." << std::endl ;
  typedef itk::WriteMetaImage< ImageType > Writer ;
  Writer::Pointer writer = Writer::New() ;

  ImagePointer output = ImageType::New() ;
  output->SetBufferedRegion(target->GetLargestPossibleRegion()) ;
  output->SetLargestPossibleRegion(target->GetLargestPossibleRegion()) ;
  output->Allocate() ;
  typedef itk::ImageRegionIteratorWithIndex< ImageType > ImageIteratorType ;
  ImageIteratorType i_iter(output, output->GetLargestPossibleRegion()) ;
  i_iter.GoToBegin() ;
  ClassifierType::OutputType::Iterator m_iter = result->Begin() ;
  while (!i_iter.IsAtEnd())
    {
      i_iter.Set((ImageType::PixelType)m_iter.GetClassLabel()) ;
      ++i_iter ;
      ++m_iter ;
    }

  writer->SetInput(output) ;
  writer->SetFileName(outputFileName.c_str()) ;
  writer->GenerateData() ;

  return 0 ;
}



