/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIMRegistrator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MIMRegistrator_h
#define _MIMRegistrator_h

#include "itkObject.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkAffineTransform.h"

#include "itkQuaternionRigidTransform.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkQuaternionRigidTransformGradientDescentOptimizer.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"

#include "itkArray.h"

namespace itk
{

template <typename TFixedImage, typename TMovingImage>
class MIMRegistrator : public Object
{
public:

  /** Standard class typedefs. */
  typedef MIMRegistrator Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MIMRegistrator, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Fixed Image Type. */
  typedef TFixedImage FixedImageType;

  /** Moving Image Type. */
  typedef TMovingImage MovingImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TFixedImage::ImageDimension };

  /** Transform Type. */
  typedef QuaternionRigidTransform< double >       TransformType;

  /** Optimizer Type. */
  typedef QuaternionRigidTransformGradientDescentOptimizer 
                                                         OptimizerType;

  /** Metric Type. */
  typedef MutualInformationImageToImageMetric< 
                                    FixedImageType, 
                                    MovingImageType >    MetricType;

  /** Interpolation Type. */
  typedef LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double          >    InterpolatorType;

  /** Fixed Image Pyramid Type. */
  typedef RecursiveMultiResolutionPyramidImageFilter<
                                    FixedImageType,
                                    FixedImageType  >    FixedImagePyramidType;

  /** Moving Image Pyramid Type. */
  typedef RecursiveMultiResolutionPyramidImageFilter<
                                    MovingImageType,
                                    MovingImageType  >   MovingImagePyramidType;

  /** Registration Method. */
  typedef MultiResolutionImageRegistrationMethod< 
                                    FixedImageType, 
                                    MovingImageType >    RegistrationType;

  /** Transform parameters type. */
  typedef typename RegistrationType::ParametersType     ParametersType;

  /** DoubleArray type. */
  typedef Array<double>  DoubleArray;

  /** UnsignedIntArray type. */
  typedef Array<unsigned int> UnsignedIntArray;

  /** ShrinkFactorsArray type. */
  typedef FixedArray<unsigned int,ImageDimension> ShrinkFactorsArray;

  /** Affine transform type. */
  typedef AffineTransform<double>   AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, FixedImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, MovingImageType );

  /** Set the number of resolution levels. */
  itkSetClampMacro( NumberOfLevels, unsigned short, 1,
    NumericTraits<unsigned short>::max() );

  /** Set the translation parameter scales. */
  itkSetClampMacro( TranslationScale, double, 0.0,
    NumericTraits<double>::max() );

  /** Set the image parzen window widths. */
  itkSetClampMacro( MovingImageStandardDeviation, double, 0.0,
    NumericTraits<double>::max() );
  itkSetClampMacro( FixedImageStandardDeviation, double, 0.0,
    NumericTraits<double>::max() );

  /** Set the number of spatial samples. */
  itkSetClampMacro( NumberOfSpatialSamples, unsigned short, 1,
    NumericTraits<unsigned short>::max() );

  /** Set the number of iterations per level. */
  itkSetMacro( NumberOfIterations, UnsignedIntArray );

  /** Set the learning rate per level. */
  itkSetMacro( LearningRates, DoubleArray );

  /** Set the initial transform parameters. */
  itkSetMacro( InitialParameters, ParametersType );

  /** Set the fixed and moving image shrink factors. */
  itkSetMacro( FixedImageShrinkFactors, ShrinkFactorsArray );
  itkSetMacro( MovingImageShrinkFactors, ShrinkFactorsArray );

  /** Method to execute the registration. */
  void Execute();

  /** Get number of parameters. */
  unsigned long GetNumberOfParameters()
    { return m_Transform->GetNumberOfParameters(); }

  /** Get computed transform parameters. */
  const ParametersType& GetTransformParameters();

  /** Get compute affine transform. */
  AffineTransformPointer GetAffineTransform();

  /** Initialize registration at the start of new level. */
  void StartNewLevel();

protected:
  MIMRegistrator();
  ~MIMRegistrator();

private:
  MIMRegistrator( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename FixedImageType::Pointer            m_FixedImage;
  typename MovingImageType::Pointer           m_MovingImage;
  typename TransformType::Pointer             m_Transform;
  typename OptimizerType::Pointer             m_Optimizer;
  typename MetricType::Pointer                m_Metric;
  typename InterpolatorType::Pointer          m_Interpolator;
  typename FixedImagePyramidType::Pointer     m_FixedImagePyramid;
  typename MovingImagePyramidType::Pointer    m_MovingImagePyramid;
  typename RegistrationType::Pointer          m_Registration;

  unsigned short                              m_NumberOfLevels;
  double                                      m_TranslationScale;
  double                                      m_MovingImageStandardDeviation;
  double                                      m_FixedImageStandardDeviation;
  unsigned short                              m_NumberOfSpatialSamples;

  UnsignedIntArray                            m_NumberOfIterations;
  DoubleArray                                 m_LearningRates;

  ShrinkFactorsArray                          m_MovingImageShrinkFactors;
  ShrinkFactorsArray                          m_FixedImageShrinkFactors;

  ParametersType                              m_InitialParameters;
  AffineTransformPointer                      m_AffineTransform;

  unsigned long                               m_Tag;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "MIMRegistrator.txx"
#endif

#endif
