/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FastMarchingLevelSetBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef FASTMARCHINGLEVELSETSBASE
#define FASTMARCHINGLEVELSETSBASE

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"


/**
 * \brief FastMarchingLevelSetsBase class that instantiate
 * the elements required for a LevelSet segmentation.
 *
 */
class FastMarchingLevelSetBase
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = 3 };

  /** Pixel type used for reading the input image */
  typedef   signed short                                 InputPixelType;

  /** Pixel type to be used internally */
  typedef   float                                        InternalPixelType;
  
  /** Pixel type to be used for the thresholded image */
  typedef   unsigned char                                ThresholdPixelType;
  
  /** Type of the input image */
  typedef   itk::Image<InputPixelType,ImageDimension>    InputImageType;

  /** Type of the internal image */
  typedef   itk::Image<InternalPixelType,ImageDimension> InternalImageType;

  /** Type of the thresholded image */
  typedef   itk::Image<ThresholdPixelType,ImageDimension> ThresholdedImageType;

  /** Index type used to specify the seed point */
  typedef   InternalImageType::IndexType                 IndexType;
  
  /** Filter for reading the input image */
  typedef   itk::ImageFileReader< InputImageType >       ImageReaderType;

  /** Cast filter needed because FastMarchingLevelSetImageFilter 
      expects double images */
  typedef   itk::CastImageFilter< 
                 InputImageType, 
                 InternalImageType >     CastImageFilterType;

  /** Derivative filter type, */
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<
                                    InternalImageType>  DerivativeFilterType;

  /** Fast Marching filte use to evolve the contours */
  typedef   itk::FastMarchingImageFilter< 
                 InternalImageType, 
                 InternalImageType >     FastMarchingFilterType;

  /** Filter to compute negative exponential of the gradient magnitude */
  typedef   itk::SigmoidImageFilter< 
                    InternalImageType,
                    InternalImageType >                   SigmoidFilterType;
 
  /** Threshold filter used to eliminate the infinite times assigned 
      to non-visited pixels in the FastMarching filter. */
  typedef   itk::IntensityWindowingImageFilter< 
                    InternalImageType,
                    InternalImageType >      FastMarchingWindowingFilterType;
    
  /** Threshold filter used to select a time from the time map */
  typedef   itk::BinaryThresholdImageFilter< 
                    InternalImageType,
                    ThresholdedImageType >                ThresholdFilterType;
    

  /** Auxiliary types for seeds (Nodes) */
  typedef FastMarchingFilterType::NodeType                NodeType;
  typedef FastMarchingFilterType::NodeContainer           NodeContainer;

public:
  FastMarchingLevelSetBase();
  virtual ~FastMarchingLevelSetBase();

  virtual void LoadInputImage()=0;
  virtual void LoadInputImage(const char * filename);

  virtual void ShowStatus(const char * text)=0;

  virtual void ComputeGradientMagnitude();

  virtual void ComputeEdgePotential();

  virtual void RunFastMarching();

  virtual void Stop();

  virtual void AddSeed( const IndexType & seed );

  virtual void ClearSeeds();

protected:

  void SetStoppingValue( double value );

protected:

  ImageReaderType::Pointer                    m_ImageReader;

  bool                                        m_InputImageIsLoaded;

  CastImageFilterType::Pointer                m_CastImageFilter;

  FastMarchingFilterType::Pointer             m_FastMarchingFilter;

  FastMarchingWindowingFilterType::Pointer    m_FastMarchingWindowingFilter;

  DerivativeFilterType::Pointer               m_DerivativeFilter;

  SigmoidFilterType::Pointer                  m_SigmoidFilter;

  ThresholdFilterType::Pointer                m_ThresholdFilter;
  
  NodeContainer::Pointer                      m_TrialPoints;

  InternalPixelType                           m_SeedValue;

  unsigned int                                m_NumberOfSeeds;

};



#endif

