/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MISimilarity2DRegistrator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MISimilarity2DRegistrator_h
#define _MISimilarity2DRegistrator_h

#include "itkObject.h"
#include "itkImageRegistrationMethod.h"
#include "itkAffineTransform.h"


#include "itkAffineTransform.h"
#include "itkSimilarity2DTransform.h"
#include "itkGradientDescentOptimizer.h"
#include "itkMutualInformationImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"

#include "itkArray.h"

/** \class MISimilarity2DRegistrator
 *
 * This component computes the transform to register a
 * moving image onto a fixed image.
 *
 * In particular, it uses the ITK registration framework with
 * the following combination of components:
 *   - MutualInformationImageToImageMetric
 *   - EulerAnglesRigid2DTransform
 *   - EulerAnglesRigid2DTransformGradientDescentOptimizer
 *   - LinearInterpolateImageFunction
 *
 * The registration is done using a multiresolution strategy.
 * At each resolution level, the downsampled images are obtained
 * using a RecursiveMultiResolutionPyramidImageFilter.
 * 
 * \warning This class requires both images to be 3D and with
 * pixels of a real type.
 *
 * The registration process is activated by method Execute().
 *
 * Inputs:
 *   - pointer to fixed image
 *   - pointer to moving image
 *   - number of resolution levels
 *   - scaling applied to the translation parameters during optimization
 *   - parzen window width for the fixed image
 *   - parzen window width for the moving image
 *   - number of optimization iterations at each level
 *   - the optimization learning rate at each level
 *   - the initial rigid (quaternion) transform parameters
 *   - the coarest level shrink factors for the fixed image
 *   - the coarest level shrink factors for the moving image
 *
 * Outputs:
 *   - rigid (quaternion) transform parameters to maps points from
 *     the fixed image to the moving image.
 *   - pointer to equivalent affine transform.
 *
 */ 
template <typename TFixedImage, typename TMovingImage>
class MISimilarity2DRegistrator : public itk::Object
{
public:

  /** Standard class typedefs. */
  typedef MISimilarity2DRegistrator Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MISimilarity2DRegistrator, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Fixed Image Type. */
  typedef TFixedImage FixedImageType;

  /** Moving Image Type. */
  typedef TMovingImage MovingImageType;

  typedef typename TFixedImage::RegionType FixedImageRegionType ;

  /** Image dimension enumeration. */
  enum { ImageDimension = TFixedImage::ImageDimension };

  /** Transform Type. */
  typedef itk::Similarity2DTransform< double > TransformType;

  /** Optimizer Type. */
  typedef itk::GradientDescentOptimizer OptimizerType;

  /** Metric Type. */
  typedef itk::MutualInformationImageToImageMetric< 
                                    FixedImageType, 
                                    MovingImageType >    MetricType;

  /** Interpolation Type. */
  typedef itk::LinearInterpolateImageFunction< 
                                    MovingImageType,
                                    double          >    InterpolatorType;

  /** Registration Method. */
  typedef itk::ImageRegistrationMethod< FixedImageType, 
    MovingImageType >    RegistrationType;

  /** Transform parameters type. */
  typedef typename RegistrationType::ParametersType     ParametersType;

  /** DoubleArray type. */
  typedef itk::Array<double>  DoubleArray;

  /** UnsignedIntArray type. */
  typedef itk::Array<unsigned int> UnsignedIntArray;

  /** ShrinkFactorsArray type. */
  typedef itk::FixedArray<unsigned int,ImageDimension> ShrinkFactorsArray;

  /** Affine transform type. */
  typedef itk::AffineTransform< double, ImageDimension >   AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, FixedImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, MovingImageType );

  itkSetMacro( FixedImageRegion, FixedImageRegionType ) ;

  /** Set the translation parameter scales. */
  itkSetClampMacro( TranslationScale, double, 0.0,
    itk::NumericTraits<double>::max() );

  /** Set the translation parameter scales. */
  itkSetClampMacro( RotationScale, double, 0.0,
    itk::NumericTraits<double>::max() );

  /** Set the image parzen window widths. */
  itkSetClampMacro( MovingImageStandardDeviation, double, 0.0,
    itk::NumericTraits<double>::max() );
  itkSetClampMacro( FixedImageStandardDeviation, double, 0.0,
    itk::NumericTraits<double>::max() );

  /** Set the number of spatial samples. */
  itkSetClampMacro( NumberOfSpatialSamples, unsigned short, 1,
    itk::NumericTraits<unsigned short>::max() );

  /** Set the number of iterations per level. */
  itkSetMacro( NumberOfIterations, unsigned int );

  /** Set the learning rate per level. */
  itkSetMacro( LearningRates, double );

  /** Set the initial transform parameters. */
  itkSetMacro( InitialParameters, ParametersType );

  /** Method to execute the registration. */
  virtual void Execute();

  /** Get number of parameters. */
  unsigned long GetNumberOfParameters()
    { return m_Transform->GetNumberOfParameters(); }

  /** Get computed transform parameters. */
  const ParametersType& GetTransformParameters();

  /** Get computed affine transform. */
  AffineTransformPointer GetAffineTransform();

  /** Get Optimizer */
  itkGetObjectMacro(Optimizer, OptimizerType);

protected:
  MISimilarity2DRegistrator();
  ~MISimilarity2DRegistrator();

private:
  MISimilarity2DRegistrator( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename FixedImageType::Pointer            m_FixedImage;
  typename MovingImageType::Pointer           m_MovingImage;
  typename TransformType::Pointer             m_Transform;
  typename OptimizerType::Pointer             m_Optimizer;
  typename MetricType::Pointer                m_Metric;
  typename InterpolatorType::Pointer          m_Interpolator;
  typename RegistrationType::Pointer          m_Registration;
  FixedImageRegionType m_FixedImageRegion ;
  unsigned short                              m_NumberOfLevels;
  double                                      m_TranslationScale;
  double                                      m_RotationScale;
  double                                      m_MovingImageStandardDeviation;
  double                                      m_FixedImageStandardDeviation;
  unsigned short                              m_NumberOfSpatialSamples;

  unsigned int                                m_NumberOfIterations;
  double                                      m_LearningRates;

  ParametersType                              m_InitialParameters;
  AffineTransformPointer                      m_AffineTransform;
  
  unsigned long                               m_Tag;
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "MISimilarity2DRegistrator.txx"
#endif

#endif
