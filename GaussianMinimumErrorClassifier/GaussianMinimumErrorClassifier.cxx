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

#include "vnl/vnl_matrix.h"

#include "itkImageListSampleAdaptor.h"
#include "itkSubsample.h"
#include "itkMembershipSample.h"
#include "itkMembershipSampleGenerator.h"
#include "itkGaussianDensityFunction.h"
#include "itkMeanCalculator.h"
#include "itkCovarianceCalculator.h"
#include "itkGenericClassifier.h"
#include "itkMaxDecisionRule.h"

int main(int argc, char* argv[])
{
  namespace stat = itk::Statistics ;

  if (argc <= 1)
    {
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  std::string inputFileName ;
  std::string classMaskFileName ;

  try
    {
      // get image file options
      options.GetStringOption("input", &inputFileName, true) ;
      // get image file options
      options.GetStringOption("class-mask", &classMaskFileName, true) ;
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

  ImagePointer input ;
  ImagePointer classMask ;

  std::cout << "Loading image(s)..." << std::endl ;
  //      readMetaImageHeader(inputFileName, inputDimension, inputSize) ;
  typedef itk::ReadMetaImage<ImageType> Reader ;
  Reader::Pointer reader = Reader::New() ;
      
  reader->SetFileName(inputFileName.c_str()) ;
  reader->Update() ;
  input = reader->GetOutput() ;
  std::cout << "Input image loaded." << std::endl ;

  Reader::Pointer reader2 = Reader::New() ;
  reader2->SetFileName(classMaskFileName.c_str()) ;
  reader2->Update() ;
  classMask = reader2->GetOutput() ;
  std::cout << "Class mask loaded." << std::endl ;

  /* ================================================== */
  std::cout << "Importing the image and the class mask image to samples..."
            << std::endl ;
  typedef stat::ImageListSampleAdaptor<ImageType, short, 1> 
    ImageListSampleType;

  ImageListSampleType::Pointer sample =
    ImageListSampleType::New() ;
  
  ImageListSampleType::Pointer mask =
    ImageListSampleType::New() ;

  sample->SetImage(input);
  mask->SetImage(classMask) ;
  /* ==================================================== */
  std::cout << "Creating the membership sample.." << std::endl ;
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
  std::cout << "Inducing the gaussian density function parameters..." 
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
    }
  /* =================================================== */
  std::cout << "Classifying..." << std::endl ;
  
  typedef stat::MaxDecisionRule DecisionRuleType ;
  DecisionRuleType::Pointer rule = DecisionRuleType::New() ;

  typedef stat::GenericClassifier< ImageListSampleType, DensityFunctionType, DecisionRuleType > ClassifierType ;

  ClassifierType::Pointer classifier = ClassifierType::New() ;
  classifier->SetSample(sample) ;
  for (unsigned int i = 0 ; i < numberOfClasses ; i++)
    {
      classifier->AddMembershipCalculator(densityFunctions[i]) ;
    }

  classifier->SetDecisionRule(rule) ;
  
  classifier->GenerateData() ;
  
  ClassifierType::ClassSampleVectorType output = classifier->GetOutput() ;
  
  vnl_matrix< int > classMatrix(numberOfClasses, numberOfClasses, 0) ;
  unsigned long id ;
  int count ;
  unsigned int classLabel ;
  for (unsigned int c = 0 ; c < numberOfClasses ; c++)
    {
      ClassifierType::ClassSamplePointer classSample = output[c] ;
       std::cout << "length of sample of class [" << c << "] = "
                << classSample->GetSize() << std::endl ;
      ClassifierType::ClassSampleType::Iterator iter = classSample->Begin() ;
      while (iter != classSample->End())
        {
          id = iter.GetInstanceIdentifier() ;
          classLabel = membershipSample->GetClassLabel(id) ;
          count = classMatrix.get(c, classLabel) ;
          classMatrix.put(c, classLabel, ++count) ;
          ++iter ;
        }
    }
  classMatrix.print(std::cout) ;
  return 0 ;
}



