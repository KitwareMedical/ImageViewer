/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThresholdSegmentationLevelSet.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef THRESHOLDSEGMENTATIONLEVELSET
#define THRESHOLDSEGMENTATIONLEVELSET

#include "ThresholdSegmentationLevelSetGUI.h"
#include "fltkImageViewer.h"

#include "fltkVTKImageViewer.h"


/**
 * \brief ThresholdSegmentationLevelSet class that instantiate
 * the elements required for a LevelSet approach for segmentation
 *
 */
class ThresholdSegmentationLevelSet : public ThresholdSegmentationLevelSetGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = ThresholdSegmentationLevelSetBase::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   ThresholdSegmentationLevelSetBase::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   ThresholdSegmentationLevelSetBase::InternalPixelType    InternalPixelType;

  /** Pixel type to be used for the seeds */
  typedef   ThresholdSegmentationLevelSetBase::SeedPixelType        SeedPixelType;

  /** Input image type */
  typedef   ThresholdSegmentationLevelSetBase::InputImageType       InputImageType;

  /** Internal image type */
  typedef   ThresholdSegmentationLevelSetBase::InternalImageType    InternalImageType;

  /** Seed image type */
  typedef   ThresholdSegmentationLevelSetBase::SeedImageType        SeedImageType;

  /** Image Viewer Types */
  typedef SeedPixelType OverlayPixelType;

  typedef fltk::ImageViewer< InputPixelType, 
                                    OverlayPixelType >          InputImageViewerType;

  typedef fltk::ImageViewer< InternalPixelType, 
                                    OverlayPixelType >          InternalImageViewerType;

  typedef fltk::VTKImageViewer< InternalPixelType >             VTKImageViewerType;


public:
  ThresholdSegmentationLevelSet();
  virtual ~ThresholdSegmentationLevelSet();

  virtual void LoadInputImage();

  virtual void ShowConsole();

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage();

  virtual void ShowThresholdedImage();

  virtual void ShowThresholdedImageWithVTK();

  virtual void Quit();

  virtual void UpdateGUIAfterIteration();

  virtual void SelectSeedPoint( float x, float y, float z );
  
  static void ClickSelectCallback(float x, float y, float z, float value, void * args );

private:

  InternalImageViewerType     m_ThresholdedImageViewer;
  InternalImageViewerType     m_InputImageViewer;

  VTKImageViewerType::Pointer m_VTKSegmentedImageViewer;


};



#endif
