/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    KmeansImageClassifierApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _KmeansImageClassifierApp_txx
#define _KmeansImageClassifierApp_txx

#include "KmeansImageClassifierApp.h"

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
KmeansImageClassifierApp<TVectorInputImage,TMaskImage>
::KmeansImageClassifierApp()
{
  m_VectorInputImage   = NULL;
  m_MaskInputImage     = NULL;
  m_ClassifiedImage    = NULL;
  m_NumberOfClasses    = 1;
  m_NumberOfChannels   = 1;

  //-------------------------------------------------------------------
  // Initialize the containers for means/covariance/number of samples 
  //-------------------------------------------------------------------

  m_ClassMeans.resize( m_NumberOfClasses, m_NumberOfChannels );
  m_ClassMeans.fill( 0 );

  m_ClassCovariances.resize( m_NumberOfClasses );

  for(unsigned int i = 0; i < m_NumberOfClasses; i++ )
    {
    m_ClassCovariances[i].resize( m_NumberOfChannels, m_NumberOfChannels );
    m_ClassCovariances[i].set_identity( );
    }

  m_ClassNumberOfSamples.resize( m_NumberOfClasses, m_NumberOfChannels );
  m_ClassNumberOfSamples.fill( 0 );

}


template <typename TVectorInputImage, typename TMaskImage>
void
KmeansImageClassifierApp<TVectorInputImage,TMaskImage>
::Execute()
{

  //----------------------------------------------------------------------
  //Set membership function (Using the statistics objects)
  //----------------------------------------------------------------------

  typedef Statistics::DistanceToCentroidMembershipFunction< VectorInputPixelType > 
    MembershipFunctionType ;

  typedef typename MembershipFunctionType::Pointer MembershipFunctionPointer ;

  typedef std::vector< MembershipFunctionPointer > 
    MembershipFunctionPointerVector;



  //----------------------------------------------------------------------
  //Set the image model estimator
  //----------------------------------------------------------------------
  typedef itk::ImageKmeansModelEstimator< VectorInputImageType, 
    MembershipFunctionType> ImageKmeansModelEstimatorType;

  typename ImageKmeansModelEstimatorType::Pointer 
    applyKmeansModelEstimator = ImageKmeansModelEstimatorType::New();

  //----------------------------------------------------------------------
  //Set the parameters of the clusterer
  //----------------------------------------------------------------------

  std::cout << "Starting to build the K-means model ....." << std::endl;

  applyKmeansModelEstimator->SetInputImage( m_VectorInputImage );
  applyKmeansModelEstimator->SetNumberOfModels(m_NumberOfClasses);
  applyKmeansModelEstimator->SetThreshold(0.01);
  applyKmeansModelEstimator->Update();

  MembershipFunctionPointerVector membershipFunctions = 
    applyKmeansModelEstimator->GetMembershipFunctions(); 

  typedef std::vector<double> TempVectorType;
  typedef TempVectorType::iterator TempVectorIterator;
  TempVectorIterator  start, end;

  std::vector<double> kmeansResultForClass(membershipFunctions.size());
  
  
  std::cout << "Result of K-Means clustering" << std::endl;

  for(unsigned int classIndex=0; classIndex < membershipFunctions.size(); 
    classIndex++ )
    {
    kmeansResultForClass[classIndex] = 
      (double) (membershipFunctions[classIndex]->GetCentroid())[0];
    }

  start = kmeansResultForClass.begin();
  end   = kmeansResultForClass.end();
  
  std::sort( start, end );

  vnl_vector<double> temp =  membershipFunctions[0]->GetCentroid();
  for(unsigned int classIndex=0; classIndex < membershipFunctions.size(); 
    classIndex++ )
    {
    temp[0] = (double) kmeansResultForClass[classIndex];
    membershipFunctions[classIndex]->SetCentroid(temp);
    }  

  for(unsigned int classIndex=0; classIndex < membershipFunctions.size(); 
    classIndex++ )
    {
    std::cout <<  (membershipFunctions[classIndex]->GetCentroid())[0] << std::endl;
    }
 
  //----------------------------------------------------------------------
  //Set the decision rule 
  //----------------------------------------------------------------------  
  typedef DecisionRuleBase::Pointer DecisionRuleBasePointer;

  typedef MinimumDecisionRule DecisionRuleType;
  DecisionRuleType::Pointer  
    classifierDecisionRule = DecisionRuleType::New();

  //------------------------------------------------------
  //Instantiate the classifier model (as the input image is in right format)
  //------------------------------------------------------  

  //Assign a class label image type
  typedef ImageClassifierBase< VectorInputImageType,ClassifiedImageType > 
    SupervisedClassifierType;

  typename SupervisedClassifierType::Pointer 
    classifierPointer = SupervisedClassifierType::New();


  typedef ShowProgressObject ProgressType;

  ProgressType progressWatch(classifierPointer);
  typename SimpleMemberCommand<ProgressType>::Pointer command;
  command = SimpleMemberCommand<ProgressType>::New();

  command->SetCallbackFunction(&progressWatch,
                               &ProgressType::ShowProgress);

  classifierPointer->AddObserver(itk::ProgressEvent(), command);

  //------------------------------------------------------  
  // Set the Classifier parameters
  //------------------------------------------------------  
  classifierPointer->SetNumberOfClasses( m_NumberOfClasses );
  classifierPointer->SetInputImage( m_VectorInputImage );

  // Set the decison rule 
  classifierPointer->
    SetDecisionRule( (DecisionRuleBasePointer) classifierDecisionRule );

  MembershipFunctionPointer membershipFunction;
  //------------------------------------------------------
  //Set the classifier membership functions
  //------------------------------------------------------
  for( unsigned int i=0; i<m_NumberOfClasses; i++ )
    {
    classifierPointer->AddMembershipFunction( membershipFunctions[i] );
    }
  
  //Do the classification
  //Run the kmeans classifier algorithm
  classifierPointer->Update();

  //Get the classified image
  typedef typename ClassifiedImageType::Pointer ClassifiedImagePointer;

  ClassifiedImagePointer    
    outClassImage = classifierPointer->GetClassifiedImage();

  //------------------------------------------------------
  //Mask the output of the classifier
  //------------------------------------------------------

  // Declare the type for the MaskInput filter

  typedef MaskImageFilter< ClassifiedImageType,
                           MaskImageType,
                           ClassifiedImageType  >   MaskFilterType;

  typedef typename ClassifiedImageType::Pointer   MaskedOutputImagePointer;
  typedef typename MaskFilterType::Pointer        MaskFilterTypePointer;

  // Create an ADD Filter                                
  MaskFilterTypePointer maskfilter = MaskFilterType::New();

  // Connect the input images
  maskfilter->SetInput1( outClassImage ); 
  maskfilter->SetInput2( m_MaskInputImage );

  // Execute the filter
  maskfilter->Update();

  // Get the Smart Pointer to the Filter Output 
  MaskedOutputImagePointer maskedOutputImage = maskfilter->GetOutput();

  this->SetClassifiedImage( maskedOutputImage );

}

} // namespace itk

#endif
