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
#include "metaITKUtils.h"
#include "itkImageRegionIterator.h"
#include "itkScalarToArrayCastImageFilter.h"

#include "itkFixedArray.h"
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
  training = metaITKUtilLoadImage3D<short>(trainingFileName.c_str(),
                                    MET_SHORT);
  std::cout << "Training image loaded." << std::endl ;

  classMask = metaITKUtilLoadImage3D<short>(classMaskFileName.c_str(),
                                     MET_SHORT);
  std::cout << "Class mask loaded." << std::endl ;

  target = metaITKUtilLoadImage3D<short>(targetFileName.c_str(),
                                  MET_SHORT);
  std::cout << "Target image loaded." << std::endl ;

  /* ================================================== */
  // convert image's pixel type from scalar to one dimensional array 
  typedef itk::FixedArray< ImageType::PixelType, 1 > ArrayPixelType ;
  typedef itk::Image< ArrayPixelType, 3 > ArrayPixelImageType ;
  typedef itk::ScalarToArrayCastImageFilter< ImageType, ArrayPixelImageType >
    CastFilterType ;
  
  std::cout << "Importing the images to samples..."
            << std::endl ;
  typedef stat::ImageToListAdaptor< ArrayPixelImageType > ImageListSampleType;

  ImageListSampleType::Pointer sample =
    ImageListSampleType::New() ;
  
  ImageListSampleType::Pointer mask =
    ImageListSampleType::New() ;

  ImageListSampleType::Pointer targetSample =
    ImageListSampleType::New() ;

  CastFilterType::Pointer castFilter = CastFilterType::New() ;
  castFilter->SetInput(training) ;
  castFilter->Update() ;
  ArrayPixelImageType::Pointer castedTraining = castFilter->GetOutput() ;
  sample->SetImage(castedTraining);

  CastFilterType::Pointer castFilter2 = CastFilterType::New() ;
  castFilter2->SetInput(classMask) ;
  castFilter2->Update() ;
  ArrayPixelImageType::Pointer castedClassMask = castFilter2->GetOutput() ;
  mask->SetImage(castedClassMask) ;

  CastFilterType::Pointer castFilter3 = CastFilterType::New() ;
  castFilter3->SetInput(target) ;
  castFilter3->Update() ;
  ArrayPixelImageType::Pointer castedTarget = castFilter3->GetOutput() ;
  targetSample->SetImage(castedTarget) ;
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
  MembershipSampleGeneratorType::OutputType* membershipSample = 
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
  std::vector< MeanCalculatorType::Pointer > meanCalculatorVector ;
  meanCalculatorVector.resize(numberOfClasses) ;
  std::vector< CovarianceCalculatorType::Pointer > covarianceCalculatorVector ;
  covarianceCalculatorVector.resize(numberOfClasses) ;

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
      meanCalculatorVector[i] = MeanCalculatorType::New() ;
      meanCalculatorVector[i]->SetInputSample(subSample.GetPointer()) ;
      meanCalculatorVector[i]->Update() ;
      
      covarianceCalculatorVector[i] = CovarianceCalculatorType::New() ;
      covarianceCalculatorVector[i]->SetInputSample(subSample.GetPointer()) ;
      covarianceCalculatorVector[i]->SetMean(meanCalculatorVector[i]->GetOutput()) ;
      covarianceCalculatorVector[i]->Update() ;

      densityFunctions[i] = DensityFunctionType::New() ;
      (densityFunctions[i])->SetMean(meanCalculatorVector[i]->GetOutput()) ;
      (densityFunctions[i])->
        SetCovariance(covarianceCalculatorVector[i]->GetOutput() ) ;
      std::cout << "  mean = " << (densityFunctions[i])->GetMean()
                << std::endl ;
      std::cout << "  covariance = " << std::endl ;
      (densityFunctions[i])->GetCovariance()->GetVnlMatrix().print(std::cout) ;
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
  
  ClassifierType::OutputType* result = classifier->GetOutput() ;

  /* ===================================================== */
  std::cout << "Creating a image with result class labels..." << std::endl ;
  typedef itk::ImageRegionIterator< ImageType > ImageIteratorType ;

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

  metaITKUtilSaveImage<ImageType>(outputFileName.c_str(), NULL,
                                  output, MET_SHORT, 1, MET_SHORT);

  return 0 ;
}

