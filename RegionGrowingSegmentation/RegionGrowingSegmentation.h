/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RegionGrowingSegmentation.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef REGIONGROWINGSEGMENTATION
#define REGIONGROWINGSEGMENTATION

#include "RegionGrowingSegmentationGUI.h"
#include "fltkImageViewer.h"

#include "fltkVTKImageViewer.h"


/**
 * \brief RegionGrowingSegmentation class that instantiate
 * the elements required for a Region Growing approach for segmentation
 *
 */
class RegionGrowingSegmentation : public RegionGrowingSegmentationGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = RegionGrowingSegmentationBase::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   RegionGrowingSegmentationBase::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   RegionGrowingSegmentationBase::InternalPixelType    InternalPixelType;

  /** Input image type */
  typedef   RegionGrowingSegmentationBase::InputImageType       InputImageType;

  /** Internal image type */
  typedef   RegionGrowingSegmentationBase::InternalImageType    InternalImageType;

  /** Image Viewer Types */
  typedef unsigned char  OverlayPixelType;

  typedef fltk::ImageViewer< InputPixelType, 
                                    OverlayPixelType >          InputImageViewerType;

  typedef fltk::ImageViewer< InternalPixelType, 
                                    OverlayPixelType >          InternalImageViewerType;

  typedef fltk::VTKImageViewer< InternalPixelType >             VTKImageViewerType;


public:
  RegionGrowingSegmentation();
  virtual ~RegionGrowingSegmentation();

  virtual void LoadInputImage();

  virtual void ShowConsole();

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage();

  virtual void ShowConnectedThresholdImage();

  virtual void ShowConfidenceConnectedImage();

  virtual void ShowFuzzyConnectedImage();

  virtual void ShowHomogeneousImage();
  virtual void ShowCurvatureFlowImage();
  virtual void ShowGradientAnisotropicDiffusionImage();
  virtual void ShowCurvatureAnisotropicDiffusionImage();

  virtual void ShowHomogeneousImageWithVTK();

  virtual void Quit();

  virtual void UpdateExtract();

  virtual void SelectSeedPoint( float x, float y, float z );
  
  static void ClickSelectCallback(float x, float y, float z, float value, void * args );

private:

  InternalImageViewerType     m_CurvatureFlowImageViewer;
  InternalImageViewerType     m_GradientAnisotropicDiffusionImageViewer;
  InternalImageViewerType     m_CurvatureAnisotropicDiffusionImageViewer;
  InternalImageViewerType     m_ConnectedThresholdImageViewer;
  InternalImageViewerType     m_ConfidenceConnectedImageViewer;
  InternalImageViewerType     m_FuzzyConnectedImageViewer;
  InternalImageViewerType     m_HomogeneousImageViewer;

  InputImageViewerType        m_InputImageViewer;

  VTKImageViewerType::Pointer m_VTKSegmentedImageViewer;


};



#endif
