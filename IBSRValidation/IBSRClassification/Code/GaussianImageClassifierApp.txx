/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianImageClassifierApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _GaussianImageClassifierApp_txx
#define _GaussianImageClassifierApp_txx

#include "GaussianImageClassifierApp.h"

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
GaussianImageClassifierApp<TVectorInputImage,TMaskImage>
::GaussianImageClassifierApp()
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
    m_ClassCovariances[i].fill( 0 );
    }

  m_ClassNumberOfSamples.resize( m_NumberOfClasses, m_NumberOfChannels );
  m_ClassNumberOfSamples.fill( 0 );

}


template <typename TVectorInputImage, typename TMaskImage>
void
GaussianImageClassifierApp<TVectorInputImage,TMaskImage>
::Execute()
{


  //----------------------------------------------------------------------
  //Set membership function (Using the statistics objects)
  //----------------------------------------------------------------------

  typedef Statistics::MahalanobisDistanceMembershipFunction< VectorInputPixelType > 
    MembershipFunctionType ;

  typedef MembershipFunctionType::Pointer MembershipFunctionPointer ;

 
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

  SupervisedClassifierType::Pointer 
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
  for (unsigned int classIndex = 0 ; 
    classIndex < m_NumberOfClasses ; classIndex++)
    {
   
    membershipFunction = MembershipFunctionType::New() ;

    membershipFunction->
      SetNumberOfSamples( m_ClassNumberOfSamples( classIndex, 0 ) ) ;     

    membershipFunction->
      SetMean( m_ClassMeans.get_row( classIndex) ) ;

    membershipFunction->
      SetCovariance( m_ClassCovariances[ classIndex ] ) ;

    classifierPointer->AddMembershipFunction( membershipFunction ); 

    }  
  
  //Do the classification
  //Run the gaussian classifier algorithm
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
