/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianMinimumErrorClassifier.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "OptionList.h"
#include "itkImage.h"
#include "itkReadMetaImage.h"
#include "itkWriteMetaImage.h"
#include "itkImageRegionIterator.h"

#include "vnl/vnl_matrix.h"

#include "itkImageToListAdaptor.h"
#include "itkSubsample.h"
#include "itkMembershipSample.h"
#include "itkMembershipSampleGenerator.h"
#include "itkGaussianDensityFunction.h"
#include "itkMeanCalculator.h"
#include "itkCovarianceCalculator.h"
#include "itkTableLookupSampleClassifier.h"
#include "itkDecisionRuleBase.h"
#include "MaximumLikelihoodRatioDecisionRule.h"
#include "itkStatisticsAlgorithm.h"

void print_usage()
{
  std::cout << "GaussianMinimumErrorClassifier 1.0 (17. Dec. 2001)" << std::endl ;

  std::cout << "usage: GaussianMinimumErrorClassifier --training file"  << std::endl ;
  std::cout << "       --class-mask file" << std::endl ;
  std::cout << "       --number-of-classes int" << std::endl ;
  std::cout << "       --target file"  << std::endl ;
  std::cout << "       --output file"  << std::endl ;

  std::cout << "" << std::endl ;

  std::cout << "--training file" << std::endl ;
  std::cout << "        image file name with intesnity values [meta image format]"  
    << std::endl ;
  std::cout << "--class-mask file" << std::endl ;
  std::cout << "        image file name with class labels [meta image format]"  
    << std::endl ;
  std::cout << "--number-of-classes int" << std::endl ;
  std::cout << "        the number of classes in the training image."
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
  std::cout << "         --number-of-classes 10" << std::endl ;
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
  unsigned int numberOfClasses ;
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
      // get the number of classes
      numberOfClasses = options.GetIntOption("number-of-classes", 10, true) ;
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
  typedef stat::ImageToListAdaptor< ImageType, stat::ScalarImageAccessor< ImageType > > 
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
  generator->SetNumberOfClasses(numberOfClasses) ;
  generator->GenerateData() ;
  MembershipSampleGeneratorType::OutputPointer membershipSample = 
    generator->GetOutput() ;
  
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
  std::cout << "Inducing the gaussian density function parameters and apriori probabilities..." << std::endl ;
  for (unsigned int i = 0 ; i < numberOfClasses ; i++)
    {
      std::cout << "gaussian [" << i << "]" << std::endl ;
      // add the class sample size to the decision rule 
      // for the a priori probability calculation
      std::cout << "  Sample size = " ;
      sampleSize = membershipSample->GetClassSampleSize(i) ;
      std::cout << sampleSize << std::endl ;
      rule->AddClassSampleSize(sampleSize) ;

      ClassSampleType::Pointer subSample = 
        membershipSample->GetClassSample(i) ;
      meanCalculator->SetSample(subSample) ;
      meanCalculator->Update() ;
      mean = meanCalculator->GetOutput() ;
      
      covarianceCalculator->SetSample(subSample) ;
      covarianceCalculator->SetMean(mean) ;
      covarianceCalculator->Update() ;
      covariance = covarianceCalculator->GetOutput() ;

      densityFunctions[i] = DensityFunctionType::New() ;
      (densityFunctions[i])->SetMean(mean) ;
      (densityFunctions[i])->SetCovariance(covariance) ;
      std::cout << "  mean = " << (densityFunctions[i])->GetMean()
                << std::endl ;
      std::cout << "  covariance = " << std::endl ;
      (densityFunctions[i])->GetCovariance().print(std::cout) ;
      
    }
  
  /* =================================================== */
  std::cout << "Classifying..." << std::endl ;
  
  typedef stat::TableLookupSampleClassifier< ImageListSampleType >
    ClassifierType ;

  ClassifierType::Pointer classifier = ClassifierType::New() ;
  classifier->SetNumberOfClasses(numberOfClasses) ;
  classifier->SetSample(targetSample) ;
  for (unsigned int i = 0 ; i < numberOfClasses ; i++)
    {
      classifier->AddMembershipFunction(densityFunctions[i]) ;
    }

  classifier->SetDecisionRule((itk::DecisionRuleBase::Pointer)rule) ;
  ImageListSampleType::MeasurementVectorType upper ;
  ImageListSampleType::MeasurementVectorType lower ;
  
  
  stat::FindSampleBound< ImageListSampleType >(targetSample, targetSample->Begin(),
                                               targetSample->End(), lower, upper) ;
  std::cout << "min = " << lower[0] << " max = " << upper[0] << std::endl ;

  classifier->SetLookupTableLowerBound(lower) ;
  classifier->SetLookupTableUpperBound(upper) ;
  classifier->Update() ;
  
  ClassifierType::OutputPointer result = classifier->GetOutput() ;

  /* ===================================================== */
  std::cout << "Creating a image with result class labels..." << std::endl ;
  typedef itk::ImageRegionIterator< ImageType > ImageIteratorType ;
  typedef itk::WriteMetaImage< ImageType > Writer ;
  Writer::Pointer writer = Writer::New() ;

  ImagePointer output = ImageType::New() ;
  output->SetBufferedRegion(target->GetLargestPossibleRegion()) ;
  output->SetLargestPossibleRegion(target->GetLargestPossibleRegion()) ;
  output->Allocate() ;
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

