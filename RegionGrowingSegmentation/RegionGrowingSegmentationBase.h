/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RegionGrowingSegmentationBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef REGIONGROWINGSEGMENTATIONBASE
#define REGIONGROWINGSEGMENTATIONBASE

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkExtractImageFilter.h"

#include "itkConnectedThresholdImageFilter.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkSimpleFuzzyConnectednessScalarImageFilter.h"

#include "itkCurvatureFlowImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"


/**
 * \brief RegionGrowingSegmentationBase class that instantiate
 * the elements required for a Region Growing approach for segmentation
 *
 */
class RegionGrowingSegmentationBase
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = 3 };

  /** Pixel type used for reading the input image */
  typedef   signed short                         InputPixelType;

  /** Pixel type to be used internally */
  typedef   float                                 InternalPixelType;
  
  /** Type of the input image */
  typedef   itk::Image<InputPixelType,ImageDimension>    InputImageType;

  /** Type of the internal image */
  typedef   itk::Image<InternalPixelType,ImageDimension> InternalImageType;

  /** Filter for reading the input image */
  typedef   itk::ImageFileReader< InputImageType >       ImageReaderType;

  /** Cast filter needed because Curvature flow expects double images */
  typedef   itk::CastImageFilter< 
                 InputImageType, 
                 InternalImageType >     CastImageFilterType;

  /** Extract Image Filter used to restrict the computation to a region */
  typedef   itk::ExtractImageFilter<
                 InternalImageType,
                 InternalImageType >     ExtractImageFilterType;

  /** Null filter used to select smoothing filter */
  typedef   itk::CastImageFilter< 
                 InternalImageType, 
                 InternalImageType >     NullImageFilterType;



  /** Curvature flow image filter for producing homogeneous regions */
  typedef   itk::CurvatureFlowImageFilter< 
                 InternalImageType, 
                 InternalImageType >     CurvatureFlowImageFilterType;

  /** Curvature Anisotropic Diffusion Image Filter */
  typedef   itk::CurvatureAnisotropicDiffusionImageFilter< 
                 InternalImageType, 
                 InternalImageType > CurvatureAnisotropicDiffusionImageFilterType;

  /** Gradient Anisotropic Diffusion Image Filter */
  typedef   itk::GradientAnisotropicDiffusionImageFilter< 
                 InternalImageType, 
                 InternalImageType > GradientAnisotropicDiffusionImageFilterType;



  /** Threshold Connected Image Filter */
  typedef   itk::ConnectedThresholdImageFilter< 
                 InternalImageType, 
                 InternalImageType >     ConnectedThresholdImageFilterType;

  /** Confidence Connected Image Filter */
  typedef   itk::ConfidenceConnectedImageFilter< 
                 InternalImageType, 
                 InternalImageType >     ConfidenceConnectedImageFilterType;

  /** Fuzzy Connected Image Filter */
  typedef   itk::SimpleFuzzyConnectednessScalarImageFilter< 
                 InternalImageType, 
                 InternalImageType >     FuzzyConnectedImageFilterType;

public:
  RegionGrowingSegmentationBase();
  virtual ~RegionGrowingSegmentationBase();

  virtual void LoadInputImage(void)=0;
  virtual void LoadInputImage(const char * filename);

  virtual void ShowStatus(const char * text)=0;

  virtual void Stop(void);

  virtual void SelectSmoothingFilter( unsigned int );


protected:

  ImageReaderType::Pointer                    m_ImageReader;

  bool                                        m_InputImageIsLoaded;

  CastImageFilterType::Pointer                m_CastImageFilter;

  ExtractImageFilterType::Pointer             m_ExtractImageFilter;

  NullImageFilterType::Pointer                m_NullImageFilter;

  CurvatureFlowImageFilterType::Pointer       m_CurvatureFlowImageFilter;

  CurvatureAnisotropicDiffusionImageFilterType::Pointer  m_CurvatureAnisotropicDiffusionImageFilter;

  GradientAnisotropicDiffusionImageFilterType::Pointer   m_GradientAnisotropicDiffusionImageFilter;

  ConnectedThresholdImageFilterType::Pointer  m_ConnectedThresholdImageFilter;

  ConfidenceConnectedImageFilterType::Pointer m_ConfidenceConnectedImageFilter;

  FuzzyConnectedImageFilterType::Pointer      m_FuzzyConnectedImageFilter;

};



#endif

