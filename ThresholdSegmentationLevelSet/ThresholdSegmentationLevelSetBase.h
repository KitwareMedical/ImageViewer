/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThresholdSegmentationLevelSetBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef THRESHOLDSEGMENTATIONLEVELSETSBASE
#define THRESHOLDSEGMENTATIONLEVELSETSBASE

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"


/**
 * \brief ThresholdSegmentationLevelSetsBase class that instantiate
 * the elements required for a LevelSet segmentation.
 *
 */
class ThresholdSegmentationLevelSetBase
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = 3 };

  /** Pixel type used for reading the input image */
  typedef   unsigned char                         InputPixelType;

  /** Pixel type to be used internally */
  typedef   float                                 InternalPixelType;
  
  /** Pixel type to be used for the seeds */
  typedef   unsigned char                         SeedPixelType;
  
  /** Type of the input image */
  typedef   itk::Image<InputPixelType,ImageDimension>    InputImageType;

  /** Type of the internal image */
  typedef   itk::Image<InternalPixelType,ImageDimension> InternalImageType;

  /** Type of the seeds image */
  typedef   itk::Image<SeedPixelType,ImageDimension>     SeedImageType;

  /** Filter for reading the input image */
  typedef   itk::ImageFileReader< InputImageType >       ImageReaderType;

  /** Cast filter needed because ThresholdSegmentationLevelSetImageFilter 
      expects double images */
  typedef   itk::CastImageFilter< 
                 InputImageType, 
                 InternalImageType >     CastImageFilterType;

  /** Curvature flow image filter for producing homogeneous regions */
  typedef   itk::ThresholdSegmentationLevelSetImageFilter< 
                 SeedImageType, 
                 InternalImageType >     ThresholdLevelSetImageFilterType;

public:
  ThresholdSegmentationLevelSetBase();
  virtual ~ThresholdSegmentationLevelSetBase();

  virtual void LoadInputImage(void)=0;
  virtual void LoadInputImage(const char * filename);

  virtual void ShowStatus(const char * text)=0;

  virtual void Stop(void);

  virtual void AddSeed( const SeedImageType::IndexType & seed );

protected:

  ImageReaderType::Pointer                    m_ImageReader;

  bool                                        m_InputImageIsLoaded;

  CastImageFilterType::Pointer                m_CastImageFilter;

  ThresholdLevelSetImageFilterType::Pointer   m_ThresholdLevelSetFilter;

  SeedImageType::Pointer                      m_SeedImage;

  SeedImageType::PixelType                    m_SeedValue;

};



#endif

