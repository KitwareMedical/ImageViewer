/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMPreprocessor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMPreprocessor_txx
#define _FEMPreprocessor_txx

#include "FEMPreprocessor.h"
#include "itkMinimumMaximumImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
FEMPreprocessor<TInputImage,TOutputImage>
::FEMPreprocessor()
{
  m_InputFixedImage   = NULL;
  m_InputMovingImage  = NULL;
  m_OutputFixedImage  = NULL;
  m_OutputMovingImage = NULL;

  m_NumberOfHistogramLevels = 256;
  m_NumberOfMatchPoints = 1;

  m_FixedImageMinimum = 0;
  m_MovingImageMinimum = 0;
}


template <typename TInputImage, typename TOutputImage>
void
FEMPreprocessor<TInputImage,TOutputImage>
::Execute()
{

  PRINTOUT( NumberOfHistogramLevels );
  PRINTOUT( NumberOfMatchPoints );
  this->NormalizeImage( m_InputFixedImage, m_OutputFixedImage, m_FixedImageMinimum );
  this->NormalizeImage( m_InputMovingImage, m_OutputMovingImage, m_MovingImageMinimum );

  typedef HistogramMatchingImageFilter<OutputImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput( m_OutputMovingImage );
  filter->SetReferenceImage( m_OutputFixedImage );
  filter->SetNumberOfHistogramLevels( m_NumberOfHistogramLevels );
  filter->SetNumberOfMatchPoints( m_NumberOfMatchPoints );
  filter->ThresholdAtMeanIntensityOn();
  filter->Update();

  m_OutputMovingImage = filter->GetOutput();//m_OutputMovingImage;// 
  
}


template <typename TInputImage, typename  TOutputImage>
void
FEMPreprocessor<TInputImage,TOutputImage>
::NormalizeImage( 
InputImageType * input,
OutputImagePointer & output,
InputPixelType & min )
{

  typedef MinimumMaximumImageFilter<InputImageType> MinMaxFilterType;
  typename MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();

  minMaxFilter->SetInput( input );
  minMaxFilter->Update();

  min = minMaxFilter->GetMinimum();
  double shift = -1.0 * static_cast<double>( min );
  double scale = static_cast<double>( minMaxFilter->GetMaximum() );
  scale += shift;
  scale = 255.0 / scale;

  typedef ShiftScaleImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput( input );
  filter->SetShift( shift );
  filter->SetScale( scale );
  filter->Update();

  output = filter->GetOutput();

}



} // namespace itk

#endif
