/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FastMarchingLevelSet.h
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

#include "FastMarchingLevelSetGUI.h"
#include "fltkImageViewer.h"

#include "fltkVTKImageViewer.h"


/**
 * \brief FastMarchingLevelSet class that instantiate
 * the elements required for a LevelSet approach for segmentation
 *
 */
class FastMarchingLevelSet : public FastMarchingLevelSetGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = FastMarchingLevelSetBase::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   FastMarchingLevelSetBase::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   FastMarchingLevelSetBase::InternalPixelType    InternalPixelType;

  /** Input image type */
  typedef   FastMarchingLevelSetBase::InputImageType       InputImageType;

  /** Internal image type */
  typedef   FastMarchingLevelSetBase::InternalImageType    InternalImageType;

  /** Pixel type to be used for the seeds */
  typedef   unsigned char                                  SeedPixelType;
  
  /** Type of the seeds image */
  typedef   itk::Image<SeedPixelType,ImageDimension>       SeedImageType;

  /** Image Viewer Types */
  typedef SeedPixelType OverlayPixelType;

  typedef fltk::ImageViewer< InputPixelType, 
                                    OverlayPixelType >          InputImageViewerType;

  typedef fltk::ImageViewer< InternalPixelType, 
                                    OverlayPixelType >          InternalImageViewerType;

  typedef fltk::VTKImageViewer< ThresholdPixelType >             VTKImageViewerType;


public:
  FastMarchingLevelSet();
  virtual ~FastMarchingLevelSet();

  virtual void LoadInputImage();

  virtual void ShowConsole();

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage();

  virtual void ShowTimeCrossingMapImage();

  virtual void ShowGradientMagnitudeImage();

  virtual void ShowEdgePotentialImage();

  virtual void ShowThresholdedImage();

  virtual void ShowThresholdedImageWithVTK();

  virtual void Quit();

  virtual void UpdateGUIAfterIteration();

  virtual void SelectSeedPoint( float x, float y, float z );
  
  static void ClickSelectCallback(float x, float y, float z, float value, void * args );

private:

  SeedImageType::Pointer          m_SeedImage;

  InternalImageViewerType         m_ThresholdedImageViewer;

  InternalImageViewerType         m_TimeCrossingMapViewer;

  InternalImageViewerType         m_InputImageViewer;

  InternalImageViewerType         m_GradientMagnitudeImageViewer;

  InternalImageViewerType         m_EdgePotentialImageViewer;

  VTKImageViewerType::Pointer     m_VTKSegmentedImageViewer;


};



#endif
