/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MRFKMeansImageClassifierApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MRFKMeansImageClassifierApp_txx
#define _MRFKMeansImageClassifierApp_txx

#include "MRFKMeansImageClassifierApp.h"
#include <algorithm>

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
MRFKMeansImageClassifierApp<TVectorInputImage,TMaskImage>
::MRFKMeansImageClassifierApp()
{
  m_VectorInputImage   = NULL;
  m_MaskInputImage     = NULL;
  m_ClassifiedImage    = NULL;
  m_NumberOfClasses    = 1;
  m_NumberOfChannels   = 1;
  m_MaximumNumberOfIterations = 0;
  m_NeighborhoodRadius = 0;
  m_ErrorTolerance = 0;    

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
MRFKMeansImageClassifierApp<TVectorInputImage,TMaskImage>
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
  SimpleMemberCommand<ProgressType>::Pointer command;
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
  ClassifiedImagePointer outClassImage = 
    classifierPointer->GetClassifiedImage();

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

  //------------------------------------------------------
  //Set the MRF labeller and populate the parameters
  //------------------------------------------------------
  //Set the MRF labeller
  typedef itk::MRFImageFilter<VectorInputImageType,ClassifiedImageType> 
    MRFFilterType;

  typename MRFFilterType::Pointer applyMRFFilter = MRFFilterType::New();

  // Set the MRF labeller parameters
  applyMRFFilter->SetNumberOfClasses(m_NumberOfClasses);
  applyMRFFilter->SetMaximumNumberOfIterations(m_MaximumNumberOfIterations);
  applyMRFFilter->SetErrorTolerance(m_ErrorTolerance);
  applyMRFFilter->SetSmoothingFactor( m_SmoothingFactor );

  //For setting up a square/cubic or hypercubic neighborhood
  applyMRFFilter->SetNeighborhoodRadius( m_NeighborhoodRadius );
 
  applyMRFFilter->SetInput(m_VectorInputImage);
  applyMRFFilter->SetClassifier( classifierPointer ); 
  
  //Kick off the MRF labeller function
  applyMRFFilter->Update();
  
  outClassImage = applyMRFFilter->GetOutput();
  
  //------------------------------------------------------
  //Mask the output of the classifier
  //------------------------------------------------------

  // Declare the type for the MaskInput filter

  // Create an ADD Filter                                
  MaskFilterTypePointer maskfilter2 = MaskFilterType::New();

  // Connect the input images
  maskfilter2->SetInput1( outClassImage ); 
  maskfilter2->SetInput2( m_MaskInputImage );

  // Execute the filter
  maskfilter2->Update();

  // Get the Smart Pointer to the Filter Output 
  maskedOutputImage = maskfilter2->GetOutput();

  this->SetClassifiedImage( maskedOutputImage );


  /*** Write out the classified volume ****/
/*
  typedef itk::RawVolumeWriter<ClassifiedImageType> ByteWriterType;
  ByteWriterType::Pointer bytewriter = ByteWriterType::New();

  bytewriter->SetInputImage( maskedOutputImage );
  bytewriter->SetFileName( "classified.raw" );
  bytewriter->Execute();
*/
  //------------------------------------------------------
  //Compare the result with the ground truth
  //------------------------------------------------------ 

}

} // namespace itk

#endif
