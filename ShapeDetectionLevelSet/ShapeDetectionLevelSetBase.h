/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectionLevelSetBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef SHAPEDETECTIONLEVELSETSBASE
#define SHAPEDETECTIONLEVELSETSBASE

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkShapeDetectionLevelSetFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkExpNegativeImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkAdaptImageFilter.h"
#include "itkAddPixelAccessor.h"



/**
 * \brief ShapeDetectionLevelSetsBase class that instantiate
 * the elements required for a LevelSet segmentation.
 *
 */
class ShapeDetectionLevelSetBase
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = 3 };

  /** Pixel type used for reading the input image */
  typedef   unsigned char                                InputPixelType;

  /** Pixel type to be used internally */
  typedef   float                                        InternalPixelType;
  
  /** Pixel type to be used for the thresholded image */
  typedef   unsigned char                                ThresholdPixelType;
  
  /** Type of the input image */
  typedef   itk::Image<InputPixelType,ImageDimension>    InputImageType;

  /** Type of the internal image */
  typedef   itk::Image<InternalPixelType,ImageDimension> InternalImageType;

  /** Pixel type to be used for the initial level set generated from seeds */
  typedef   unsigned char                                  SeedPixelType;
  
  /** Type of the seeds image */
  typedef   itk::Image<SeedPixelType,ImageDimension>       SeedImageType;


  /** Type of the thresholded image */
  typedef   itk::Image<ThresholdPixelType,ImageDimension> ThresholdedImageType;

  /** Index type used to specify the seed point */
  typedef   InternalImageType::IndexType                 IndexType;
  
  /** Filter for reading the input image */
  typedef   itk::ImageFileReader< InputImageType >       ImageReaderType;

  /** Cast filter needed because ShapeDetectionLevelSetImageFilter 
      expects double images */
  typedef   itk::CastImageFilter< 
                 InputImageType, 
                 InternalImageType >     CastImageFilterType;

  /** Derivative filter type, */
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<
                                    InternalImageType>  DerivativeFilterType;

  /** Image Adaptor for selecting the zero set of the level set */
  typedef itk::Accessor::AddPixelAccessor< 
                                InternalPixelType >       AddPixelAccessorType;

  typedef itk::AdaptImageFilter< InternalImageType,
                                InternalImageType,
                                AddPixelAccessorType >     AddImageFilterType;


  /** ShapeDetection filter used to evolve the contours */
  typedef   itk::ShapeDetectionLevelSetFilter< 
                                   InternalImageType, 
                                   InternalImageType >     ShapeDetectionFilterType;

  /** Filter to compute negative exponential of the gradient magnitude */
  typedef   itk::ExpNegativeImageFilter< 
                    InternalImageType,
                    InternalImageType >                   ExpNegativeFilterType;
 
  /** Threshold filter used to select a time from the time map */
  typedef   itk::BinaryThresholdImageFilter< 
                    InternalImageType,
                    ThresholdedImageType >                ThresholdFilterType;
    
  /** Fast Marching filte use to evolve the contours */
  typedef   itk::FastMarchingImageFilter< 
                               InternalImageType >     FastMarchingFilterType;


  /** Auxiliary types for seeds (Nodes) */
  typedef FastMarchingFilterType::NodeType                NodeType;
  typedef FastMarchingFilterType::NodeContainer           NodeContainer;


public:
  ShapeDetectionLevelSetBase();
  virtual ~ShapeDetectionLevelSetBase();

  virtual void LoadInputImage()=0;
  virtual void LoadInputImage(const char * filename);

  virtual void ShowStatus(const char * text)=0;

  virtual void ComputeGradientMagnitude();

  virtual void ComputeEdgePotential();

  virtual void ComputeZeroSet();

  virtual void RunShapeDetection();

  virtual void Stop();

  virtual void AddSeed( const IndexType & seed );

  virtual void ClearSeeds();

  virtual void SetZeroSetValue( InternalPixelType value );

protected:


  ImageReaderType::Pointer                    m_ImageReader;

  bool                                        m_InputImageIsLoaded;

  CastImageFilterType::Pointer                m_CastImageFilter;

  FastMarchingFilterType::Pointer             m_FastMarchingFilter;

  ShapeDetectionFilterType::Pointer           m_ShapeDetectionFilter;

  DerivativeFilterType::Pointer               m_DerivativeFilter;

  ExpNegativeFilterType::Pointer              m_ExpNegativeFilter;

  ThresholdFilterType::Pointer                m_ThresholdFilter;
  
  AddImageFilterType::Pointer                 m_AddImageFilter;
  
  SeedImageType::Pointer                      m_SeedImage;

  NodeContainer::Pointer                      m_TrialPoints;

  InternalPixelType                           m_SeedValue;

  unsigned int                                m_NumberOfSeeds;


};



#endif

