/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIAffine2DRegistrator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MIAffine2DRegistrator_txx
#define _MIAffine2DRegistrator_txx

#include "MIAffine2DRegistrator.h"

#include "itkCommand.h"


template <typename TFixedImage, typename TMovingImage>
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::MIAffine2DRegistrator()
{
  // Images need to be set from the outside
  m_FixedImage  = NULL;
  m_MovingImage = NULL;

  // Set up internal registrator with default components
  m_Transform          = TransformType::New();
  m_Optimizer          = OptimizerType::New();
  m_Metric             = MetricType::New();
  m_Interpolator       = InterpolatorType::New();
  m_Registration       = RegistrationType::New();

  m_Registration->SetTransform( m_Transform );
  m_Registration->SetOptimizer( m_Optimizer );
  m_Registration->SetMetric( m_Metric );
  m_Registration->SetInterpolator( m_Interpolator );

  m_AffineTransform  = AffineTransformType::New();

  // Default parameters
  m_NumberOfLevels = 1;
  m_TranslationScale = 1.0;
  m_RotationScale = 1.0;
  m_MovingImageStandardDeviation = 0.4;
  m_FixedImageStandardDeviation = 0.4;
  m_NumberOfSpatialSamples = 50 ;

  m_NumberOfIterations = 10 ;

  m_LearningRates = 1e-4 ;

  m_InitialParameters = ParametersType( m_Transform->GetNumberOfParameters() );
 
  m_Transform->SetIdentity();

  m_InitialParameters = m_Transform->GetParameters();

std::cout << "InitialParameters = " << m_InitialParameters << std::endl;
}

template <typename TFixedImage, typename TMovingImage>
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::~MIAffine2DRegistrator()
{
  m_Registration->RemoveObserver( m_Tag );
}


template <typename TFixedImage, typename TMovingImage>
void
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::Execute()
{

  // Setup the optimizer
  typename OptimizerType::ScalesType scales( 
    m_Transform->GetNumberOfParameters() );
  scales.Fill(1.0);
  
  // Affine Transform in 2D has 6 parameters 
  // the first 4 are coefficients of the rotation matrix
  // the last 2 are the components of a translation
  scales[0] = m_RotationScale ;
  scales[1] = m_RotationScale ;
  scales[2] = m_RotationScale ;
  scales[3] = m_RotationScale ;
  scales[4] = m_TranslationScale;
  scales[5] = m_TranslationScale;

  m_Optimizer->SetScales( scales );
  m_Optimizer->MaximizeOn();
  m_Optimizer->SetNumberOfIterations( m_NumberOfIterations );
  m_Optimizer->SetLearningRate( m_LearningRates );

  // Setup the metric
  m_Metric->SetMovingImageStandardDeviation( m_MovingImageStandardDeviation );
  m_Metric->SetFixedImageStandardDeviation( m_FixedImageStandardDeviation );
  m_Metric->SetNumberOfSpatialSamples( m_NumberOfSpatialSamples );

  // Setup the registrator
  m_Registration->SetFixedImage( m_FixedImage );
  m_Registration->SetMovingImage( m_MovingImage );
  m_Registration->SetFixedImageRegion( m_FixedImageRegion ) ;

  m_Registration->SetInitialTransformParameters( m_InitialParameters );

  try
    {
    m_Registration->StartRegistration();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cout << "Caught an exception: " << std::endl;
    std::cout << err << std::endl;
    throw err;
    }
}


template <typename TFixedImage, typename TMovingImage>
const 
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::ParametersType &
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::GetTransformParameters()
{
  return m_Registration->GetLastTransformParameters();
}


template <typename TFixedImage, typename TMovingImage>
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::AffineTransformPointer
MIAffine2DRegistrator<TFixedImage,TMovingImage>
::GetAffineTransform()
{
  m_Transform->SetParameters( m_Registration->GetLastTransformParameters() );
  
  m_AffineTransform->SetMatrix( m_Transform->GetMatrix() );
  m_AffineTransform->SetOffset( m_Transform->GetOffset() );

  return m_AffineTransform;
}

#endif
