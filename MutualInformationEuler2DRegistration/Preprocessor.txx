/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Preprocessor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _Preprocessor_txx
#define _Preprocessor_txx

#include "Preprocessor.h"
#include "itkChangeInformationImageFilter.h"
#include "itkNormalizeImageFilter.h"

template <typename TInputImage, typename TOutputImage>
Preprocessor<TInputImage,TOutputImage>
::Preprocessor()
{
  m_InputFixedImage   = NULL;
  m_InputMovingImage  = NULL;
  m_OutputFixedImage  = NULL;
  m_OutputMovingImage = NULL;

  for ( int j = 0; j < ImageDimension; j++ )
    {
    m_PermuteOrder[j] = j;
    }

  m_FlipAxes.Fill( false );
 
  m_PostTransform = AffineTransformType::New();
  m_PreTransform = AffineTransformType::New();

}


template <typename TInputImage, typename TOutputImage>
void
Preprocessor<TInputImage,TOutputImage>
::Execute()
{

  typedef itk::ChangeInformationImageFilter<TInputImage>
    CentererType;
  typedef itk::NormalizeImageFilter<TInputImage,TOutputImage>
    NormalizerType;

  AffineTransformType::MatrixType matrix;
  AffineTransformType::OffsetType offset;

  CentererType::Pointer   centerer;
  NormalizerType::Pointer normalizer;
  PermuterType::Pointer   permuter;
  FlipperType::Pointer    flipper;

  // Process the fixed image
  centerer =  CentererType::New();
  normalizer = NormalizerType::New();

  centerer->CenterImageOn();
  centerer->SetInput( m_InputFixedImage );
  
  normalizer->SetInput( centerer->GetOutput() );
  normalizer->Update();
  m_OutputFixedImage = normalizer->GetOutput();

  m_PreTransform->SetIdentity();
  for ( int j = 0; j < ImageDimension; j++ )
    {
    offset[j] = centerer->GetOutput()->GetOrigin()[j];
    }
  m_PreTransform->SetOffset( offset );


  // Process the moving image
  centerer   = CentererType::New();
  normalizer = NormalizerType::New();
  permuter   = PermuterType::New();
  flipper    = FlipperType::New();

  centerer->CenterImageOn();
  centerer->SetInput( m_InputMovingImage );

  bool needToPermute = this->NeedToPermute();
  bool needToFlip    = this->NeedToFlip();

  if ( needToPermute )
    {
    std::cout << "Need to permute: " << m_PermuteOrder;
    permuter->SetInput( centerer->GetOutput() );
    permuter->SetOrder( m_PermuteOrder );
    }
  else if ( needToFlip )
    {
    flipper->SetInput( centerer->GetOutput() );
    }

  if ( needToFlip )
    {
    std::cout << "Need to flip: " << m_FlipAxes;
    flipper->SetFlipAxes( m_FlipAxes );
    normalizer->SetInput( flipper->GetOutput() );
    }
  else if ( needToPermute )
    {
    normalizer->SetInput( permuter->GetOutput() );
    }
  else
    {
    normalizer->SetInput( centerer->GetOutput() );
    }

  if ( needToPermute && needToFlip )
    {
    flipper->SetInput( permuter->GetOutput() );
    }

  normalizer->Update();
  m_OutputMovingImage = normalizer->GetOutput();

  // Compute the affine transform

  m_PostTransform->SetIdentity();
  for ( int j = 0; j < ImageDimension; j++ )
    {
    offset[j] = - 1.0 * centerer->GetOutput()->GetOrigin()[j];
    }
  m_PostTransform->SetOffset( offset );

  if ( needToPermute )
    {
    matrix.Fill( 0.0 );
    offset.Fill( 0.0 );

    AffineTransformType::Pointer transform = AffineTransformType::New();
 
    PermuteOrderArrayType invOrder = permuter->GetInverseOrder();

    for ( int j = 0; j < ImageDimension; j++ )
      {
      matrix[j][invOrder[j]] = 1.0;
      }    

    transform->SetMatrix( matrix );
    transform->SetOffset( offset );

    m_PostTransform->Compose( transform, true );

    }

  if ( needToFlip )
    {

    AffineTransformType::Pointer transform = AffineTransformType::New();
    matrix.SetIdentity();
    offset.Fill( 0.0 );

    for ( int j = 0; j < ImageDimension; j++ )
      {
      if ( m_FlipAxes[j] )
          {
          matrix[j][j] *= -1.0;
          }
      }

    transform->SetMatrix( matrix );
    transform->SetOffset( offset );

    m_PostTransform->Compose( transform, true );

    }
  
}

#endif
