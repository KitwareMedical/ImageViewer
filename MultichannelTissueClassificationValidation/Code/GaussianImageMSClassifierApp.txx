/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GaussianImageMSClassifierApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _GaussianImageMSClassifierApp_txx
#define _GaussianImageMSClassifierApp_txx

#include "GaussianImageMSClassifierApp.h"

namespace itk
{

template <typename TVectorInputImage, typename TMaskImage>
GaussianImageMSClassifierApp<TVectorInputImage,TMaskImage>
::GaussianImageMSClassifierApp()
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
GaussianImageMSClassifierApp<TVectorInputImage,TMaskImage>
::Execute()
{


  //----------------------------------------------------------------------
  //Set membership function (Using the statistics objects)
  //----------------------------------------------------------------------

  typedef Statistics::MahalanobisDistanceMembershipFunction< VectorInputPixelType > 
    MembershipFunctionType ;

  typedef typename MembershipFunctionType::Pointer MembershipFunctionPointer ;

  //----------------------------------------------------------------------
  //Set the decision rule 
  //----------------------------------------------------------------------  
  typedef typename DecisionRuleBase::Pointer DecisionRuleBasePointer;

  typedef MinimumDecisionRule DecisionRuleType;
  typename DecisionRuleType::Pointer  
    classifierDecisionRule = DecisionRuleType::New();

  //------------------------------------------------------
  //Instantiate the MSClassifier model (as the input image is in right format)
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

  this->SetClassifiedImage( 
    classifierPointer->GetClassifiedImage() );

}

} // namespace itk

#endif
