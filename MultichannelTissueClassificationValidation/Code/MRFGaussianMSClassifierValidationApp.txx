/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MRFGaussianMSClassifierValidationApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MRFGaussianMSClassifierValidationApp_txx
#define _MRFGaussianMSClassifierValidationApp_txx

#include "MRFGaussianMSClassifierValidationApp.h"

namespace itk
{

template <typename TImage, typename TMaskImage>
MRFGaussianMSClassifierValidationApp<TImage,TMaskImage>
::MRFGaussianMSClassifierValidationApp()
{ 
  m_ImageDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";
  m_PatientID = "";
  m_StartSliceNumber = 1;
  m_NumberOfSlices = 1;
  m_StartSegSliceNumber = 1;
  m_NumberOfSegSlices = 1;
  m_NumberOfChannels = 1;

  m_ParameterFileName = "";
  m_OutputFileName = "";
  m_AppendOutputFile = true;
}


template <typename TImage, typename TMaskImage>
void
MRFGaussianMSClassifierValidationApp<TImage,TMaskImage>
::InitializeParser()
{
  m_Parser->SetImageDirectoryName( m_ImageDirectoryName.c_str() );
  m_Parser->SetFileExtensions( m_FileExtensions );
  m_Parser->SetBrainSegmentationDirectoryName( m_BrainSegmentationDirectoryName.c_str() );

  m_Parser->SetPatientID( m_PatientID.c_str() );
  m_Parser->SetStartSliceNumber( m_StartSliceNumber );
  m_Parser->SetNumberOfSlices( m_NumberOfSlices );
  m_Parser->SetStartSegSliceNumber( m_StartSegSliceNumber );
  m_Parser->SetNumberOfSegSlices( m_NumberOfSegSlices );
  m_Parser->SetNumberOfChannels( m_NumberOfChannels );

  m_Parser->SetParameterFileName( m_ParameterFileName.c_str() );
}


template <typename TImage, typename TMaskImage>
void
MRFGaussianMSClassifierValidationApp<TImage,TMaskImage>
::InitializeMSClassifier()
{
  typedef TImage VectorInputImageType;

  typename VectorInputImageType::Pointer testImage = m_Parser->GetVectorInputImage();

  m_MSClassifier->SetVectorInputImage( m_Parser->GetVectorInputImage() );
  m_MSClassifier->SetMaskInputImage( m_Parser->GetMaskImage() );
  m_MSClassifier->SetNumberOfClasses( m_Parser->GetNumberOfClasses() );  
  m_MSClassifier->SetNumberOfChannels( m_Parser->GetNumberOfChannels() );  
 
  m_MSClassifier->SetClassMeans( m_Parser->GetClassMeans() );
  m_MSClassifier->SetClassNumberOfSamples( m_Parser->GetClassNumberOfSamples() );
  m_MSClassifier->SetClassCovariances( m_Parser->GetClassCovariances() ); 

  m_MSClassifier->SetMaximumNumberOfIterations( m_MaximumNumberOfIterations );
  m_MSClassifier->SetNeighborhoodRadius( m_NeighborhoodRadius );
  m_MSClassifier->SetErrorTolerance( m_ErrorTolerance ); 
  m_MSClassifier->SetSmoothingFactor( m_SmoothingFactor );
  
}


template <typename TImage, typename TMaskImage>
void
MRFGaussianMSClassifierValidationApp<TImage,TMaskImage>
::InitializeOutputGenerator()
{
  m_OutputGenerator->SetPatientID( m_PatientID.c_str() );

  m_OutputGenerator->SetClassifiedImage( m_MSClassifier->GetClassifiedImage() );
  m_OutputGenerator->SetTruthImage( m_Parser->GetTruthImage() );

  m_OutputGenerator->SetStartSegSliceNumber( m_Parser->GetStartSegSliceNumber() );
  m_OutputGenerator->SetNumberOfSegSlices( m_Parser->GetNumberOfSegSlices() );

  m_OutputGenerator->SetTruthLabels( m_TruthLabels );

  m_OutputGenerator->SetOutputFileName( m_OutputFileName.c_str() );
  m_OutputGenerator->SetAppendOutputFile( m_AppendOutputFile );
} 


} // namespace itk

#endif
