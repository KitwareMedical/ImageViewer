/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BrainStripValidationApp.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _BrainStripValidationApp_txx
#define _BrainStripValidationApp_txx

#include "BrainStripValidationApp.h"

namespace itk
{

template <typename TImage, typename TLabelImage, typename TRealImage>
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::BrainStripValidationApp()
{ 
  m_ImageDirectoryName = "";
  m_BrainSegmentationDirectoryName = "";
  m_AtlasPatientID = "";
  m_SubjectPatientID = "";
  m_AtlasStartSliceNumber = 1;
  m_SubjectStartSliceNumber = 1;
  m_AtlasNumberOfSlices = 1;
  m_SubjectNumberOfSlices = 1;

  m_ParameterFileName = "";
  m_OutputFileName = "";
  m_AppendOutputFile = true;
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeParser()
{
  m_Parser->SetImageDirectoryName( m_ImageDirectoryName.c_str() );
  m_Parser->SetBrainSegmentationDirectoryName( m_BrainSegmentationDirectoryName.c_str() );

  m_Parser->SetAtlasPatientID( m_AtlasPatientID.c_str() );
  m_Parser->SetAtlasStartSliceNumber( m_AtlasStartSliceNumber );
  m_Parser->SetAtlasNumberOfSlices( m_AtlasNumberOfSlices );

  m_Parser->SetSubjectPatientID( m_SubjectPatientID.c_str() );
  m_Parser->SetSubjectStartSliceNumber( m_SubjectStartSliceNumber );
  m_Parser->SetSubjectNumberOfSlices( m_SubjectNumberOfSlices );

  m_Parser->SetParameterFileName( m_ParameterFileName.c_str() );
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializePreprocessor()
{
  m_Preprocessor->SetInputFixedImage( m_Parser->GetSubjectImage() );
  m_Preprocessor->SetInputMovingImage( m_Parser->GetAtlasImage() );

  m_Preprocessor->SetNumberOfHistogramLevels( m_Parser->GetNumberOfHistogramLevels() );
  m_Preprocessor->SetNumberOfMatchPoints( m_Parser->GetNumberOfMatchPoints() );
}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeRegistrator()
{
  m_Registrator->SetFixedImage( m_Preprocessor->GetOutputFixedImage() );
  m_Registrator->SetMovingImage( m_Preprocessor->GetOutputMovingImage() );

  m_Registrator->SetNumberOfLevels( m_Parser->GetNumberOfLevels() );
  m_Registrator->SetNumberOfIterations( m_Parser->GetNumberOfIterations() );

  m_Registrator->SetFixedImageShrinkFactors( m_Parser->GetSubjectImageShrinkFactors() );
  m_Registrator->SetMovingImageShrinkFactors( m_Parser->GetAtlasImageShrinkFactors() );

}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeLabeler()
{

  m_Labeler->SetAtlasLabelImage( m_Parser->GetAtlasLabelImage() );
  m_Labeler->SetDeformationField( m_Registrator->GetDeformationField() );

  typedef typename TLabelImage::PixelType LabelPixelType;
  m_Labeler->SetLowerThreshold( 1 );
  m_Labeler->SetUpperThreshold( NumericTraits<LabelPixelType>::max() );

  m_Labeler->SetOutputValue( 1 );

}


template <typename TImage, typename TLabelImage, typename TRealImage>
void
BrainStripValidationApp<TImage,TLabelImage,TRealImage>
::InitializeGenerator()
{

  m_Generator->SetPatientID( m_SubjectPatientID.c_str() );
  m_Generator->SetGroundTruthImage( m_Parser->GetSubjectLabelImage() );

  m_Generator->SetLabelImage( m_Labeler->GetOutputLabelImage() );

  m_Generator->SetOutputFileName( m_OutputFileName.c_str() );
  m_Generator->SetAppendOutputFile( m_AppendOutputFile );
}


} // namespace itk

#endif
