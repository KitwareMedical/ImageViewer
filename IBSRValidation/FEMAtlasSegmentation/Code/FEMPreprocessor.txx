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

#include "itkImageToListAdaptor.h"
#include "itkMeanCalculator.h"
#include "FEMPreprocessor.h"
#include "itkMinimumMaximumImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkOtsuThresholdImageCalculator.h"
#include "itkMultiplyImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

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
  this->ThresholdImage( m_OutputFixedImage, m_OutputFixedImage );
  this->ThresholdImage( m_OutputMovingImage, m_OutputMovingImage);
  
  if ( m_EdgeFilter >= 0)
  {
    float variance=7.0;
    this->EdgeFilterImage( m_OutputFixedImage, m_OutputFixedImage, 2, variance );
    this->EdgeFilterImage( m_OutputMovingImage, m_OutputMovingImage,2, variance );
  }
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
::ThresholdImage( 
OutputImageType *  input,
OutputImagePointer & output)
{

  typedef itk::OtsuThresholdImageCalculator<OutputImageType>  CalculatorType;
  typename CalculatorType::Pointer calculator = CalculatorType::New();
   
  calculator->SetImage(input);
  calculator->SetNumberOfHistogramBins( 255);

  calculator->Compute(); 
  /*
  // creates an ImageToListAdaptor object
  typedef  Statistics::ImageToListAdaptor< InputImageType > ImageToListAdaptorType ;

  ImageToListAdaptorType::Pointer sample = ImageToListAdaptorType::New() ;
  sample->SetImage(input) ;

  typedef itk::Statistics::MeanCalculator< ImageToListAdaptorType > 
    CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New() ;
  
  calculator->SetInputSample(sample.GetPointer()) ;
  calculator->Update();
*/
  typedef BinaryThresholdImageFilter<OutputImageType,OutputImageType> InputThresholderType;
  typename InputThresholderType::Pointer inputThresholder = InputThresholderType::New();

  inputThresholder->SetInput( input );
  inputThresholder->SetInsideValue( 1.0 );
  inputThresholder->SetOutsideValue( 0.0 );
  inputThresholder->SetLowerThreshold( (float)calculator->GetThreshold()*(float)0.7 );
  inputThresholder->SetUpperThreshold( 10000 );

  // Declare the type for the MULTIPLY filter
  typedef itk::MultiplyImageFilter<OutputImageType,
                                OutputImageType,OutputImageType >  MultFilterType;
 // Create an MULTIPLY Filter                                
  typename MultFilterType::Pointer filter = MultFilterType::New();

  // Connect the input images
  filter->SetInput1( input ); 
  filter->SetInput2(  inputThresholder->GetOutput());

  // Execute the filter
  filter->Update();
  // Get the Smart Pointer to the Filter Output 
  output = filter->GetOutput();

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


template <typename TInputImage, typename  TOutputImage>
void
FEMPreprocessor<TInputImage,TOutputImage>
::EdgeFilterImage( 
OutputImageType * input,
OutputImagePointer & output, int whichfilter, float variance)
{ 
  typedef ImageToImageFilter<OutputImageType, OutputImageType> filtertype;
  typedef CannyEdgeDetectionImageFilter<OutputImageType, OutputImageType> filtertype0;
  typedef SobelEdgeDetectionImageFilter<OutputImageType, OutputImageType> filtertype1;
  typedef ZeroCrossingBasedEdgeDetectionImageFilter<OutputImageType, OutputImageType> filtertype2;
  typename filtertype::Pointer  edgefilter;
  typename filtertype0::Pointer  edgefilter0;
  typename filtertype1::Pointer  edgefilter1;
  typename filtertype2::Pointer  edgefilter2;

  switch (whichfilter){
  case 0:
    edgefilter0 = filtertype0::New();
    edgefilter0->SetVariance(variance);
    edgefilter0->SetMaximumError(.01f);
    edgefilter=edgefilter0;
    break;
  case 1:
    edgefilter1 = filtertype1::New();
    edgefilter=edgefilter1;
    break;
  case 2:
    edgefilter2 = filtertype2::New();
    edgefilter2->SetVariance(variance);
    edgefilter2->SetMaximumError(.01f);
    edgefilter=edgefilter2;
    break;
  default:
    edgefilter0 = filtertype0::New();
    edgefilter0->SetVariance(variance);
    edgefilter0->SetMaximumError(.01f);
    edgefilter=edgefilter0;
  }

  edgefilter->SetInput(input);
  edgefilter->Update();

  output=edgefilter->GetOutput();

}


} // namespace itk

#endif
