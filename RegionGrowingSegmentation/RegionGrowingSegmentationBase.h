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

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkConnectedThresholdImageFilter.h>


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
  typedef   unsigned char                         InputPixelType;

  /** Pixel type to be used internally */
  typedef   unsigned char                         InternalPixelType;
  
  /** Type of the input image */
  typedef   itk::Image<InputPixelType,ImageDimension>    InputImageType;

  /** Type of the internal image */
  typedef   itk::Image<InternalPixelType,ImageDimension> InternalImageType;

  /** Filter for reading the input image */
  typedef   itk::ImageFileReader< InputImageType >       ImageReaderType;

  /** Threshold Connected Image Filter */
  typedef   itk::ConnectedThresholdImageFilter< 
                 InputImageType, InternalImageType >     ConnectedThresholdImageFilterType;


public:
  RegionGrowingSegmentationBase();
  virtual ~RegionGrowingSegmentationBase();

  virtual void LoadInputImage(void)=0;
  virtual void LoadInputImage(const char * filename);

  virtual void ShowStatus(const char * text)=0;

  virtual void Stop(void);

protected:

  ImageReaderType::Pointer                    m_ImageReader;

  bool                                        m_InputImageIsLoaded;

  ConnectedThresholdImageFilterType::Pointer  m_ConnectedThresholdImageFilter;

};



#endif

