/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Preprocessor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _Preprocessor_h
#define _Preprocessor_h

#include "itkObject.h"
#include "itkPermuteAxesImageFilter.h"
#include "itkFlipImageFilter.h"


/** \class Preprocessor
 *
 * This component pre-processes the moving and fixed image before
 * registration.
 *
 * This particular, this class normalizes both images to have
 * mean intensity of zero and variance of one.
 *
 * The origin of both images are modified so that point zero
 * refers to the center of the image.
 *
 * The moving image is permuted and flip according to user
 * specified parameters.
 *
 * The preprocessing is activatived by method Execute().
 *
 * Inputs:
 *    - pointer to original fixed image
 *    - pointer original moving image
 *    - axes permutation order
 *    - boolean array of which axes to flip after permutation
 *
 * Outputs:
 *    - pointer to processed fixed image
 *    - pointer to processed moving image
 *    - pointer to transform representing the pre-transform
 *    - pointer to transform representing the post-transform
 *
 * After registration, the overall transform is obtained by
 * composing pre-transform, the registration transform and
 * the post-transform.
 *
 */
template <typename TInputImage, typename TOutputImage>
class ITK_EXPORT Preprocessor : public itk::Object
{
public:

  /** Standard class typedefs. */
  typedef Preprocessor Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(Preprocessor, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input Image Type. */
  typedef TInputImage InputImageType;

  /** Output Image Type. */
  typedef TOutputImage OutputImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TInputImage::ImageDimension };

  /** Affine transform type. */
  typedef itk::AffineTransform<double,ImageDimension>   AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Axes permuter type. */
  typedef itk::PermuteAxesImageFilter<TInputImage> PermuterType;
  typedef typename PermuterType::PermuteOrderArrayType PermuteOrderArrayType;

  /** Axes flipper type. */
  typedef itk::FlipImageFilter<TInputImage> FlipperType;
  typedef typename FlipperType::FlipAxesArrayType  FlipAxesArrayType;

  /** Set the input fixed image. */
  itkSetObjectMacro( InputFixedImage, InputImageType );
  
  /** Set the input moving image. */
  itkSetObjectMacro( InputMovingImage, InputImageType );

  /** Set the permutation order. */
  itkSetMacro( PermuteOrder, PermuteOrderArrayType );

  /** Set which axes to flip (after permutation). */
  itkSetMacro( FlipAxes, FlipAxesArrayType );

  /** Method to execute the preprocessing. */
  virtual void Execute();

  /** Get the output fixed image. */
  itkGetObjectMacro( OutputFixedImage, OutputImageType );

  /** Get the output moving image. */
  itkGetObjectMacro( OutputMovingImage, OutputImageType );

  /** Returns true if a permute is required. Return false otherwise */
  bool NeedToPermute()
    {
    for ( int j = 0; j < ImageDimension; j++ )
      {
      if ( static_cast<int>(m_PermuteOrder[j]) != j ) { return true; }
      }
    return false;
    } 

  /** Returns true if flipping is required. Return false otherwise */
  bool NeedToFlip()
    {
    for ( int j = 0; j < ImageDimension; j++ )
      {
      if ( m_FlipAxes[j] ) { return true; }
      }
    return false;
    } 

  /** Get computed post transform. */
  AffineTransformPointer GetPostTransform()
    { return m_PostTransform; }

  /** Get computed pre transform. */
  AffineTransformPointer GetPreTransform()
    { return m_PreTransform; }


protected:
  Preprocessor();
  ~Preprocessor(){};

private:
  Preprocessor( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename InputImageType::Pointer       m_InputFixedImage;
  typename InputImageType::Pointer       m_InputMovingImage;
  typename OutputImageType::Pointer      m_OutputFixedImage;
  typename OutputImageType::Pointer      m_OutputMovingImage;

  PermuteOrderArrayType                  m_PermuteOrder;
  FlipAxesArrayType                      m_FlipAxes;

  AffineTransformPointer                 m_PreTransform;
  AffineTransformPointer                 m_PostTransform;

};


#ifndef ITK_MANUAL_INSTANTIATION
#include "Preprocessor.txx"
#endif

#endif
