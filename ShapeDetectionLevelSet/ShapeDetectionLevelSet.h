/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectionLevelSet.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef SHAPEDETECTIONLEVELSET
#define SHAPEDETECTIONLEVELSET

#include "ShapeDetectionLevelSetGUI.h"
#include "fltkImageViewer.h"

#include "fltkVTKImageViewer.h"


/**
 * \brief ShapeDetectionLevelSet class that instantiate
 * the elements required for a LevelSet approach for segmentation
 *
 */
class ShapeDetectionLevelSet : public ShapeDetectionLevelSetGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = ShapeDetectionLevelSetBase::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   ShapeDetectionLevelSetBase::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   ShapeDetectionLevelSetBase::InternalPixelType    InternalPixelType;

  /** Input image type */
  typedef   ShapeDetectionLevelSetBase::InputImageType       InputImageType;

  /** Internal image type */
  typedef   ShapeDetectionLevelSetBase::InternalImageType    InternalImageType;

  /** Image Viewer Types */
  typedef ShapeDetectionLevelSetBase::SeedPixelType        OverlayPixelType;

  /** FLTK viewer module */
  typedef fltk::ImageViewer< InternalPixelType, 
                                    OverlayPixelType >     InternalImageViewerType;

  typedef fltk::ImageViewer< ThresholdPixelType, 
                                    OverlayPixelType >     ThresholdedImageViewerType;

  typedef fltk::VTKImageViewer< ThresholdPixelType >       VTKImageViewerType;


public:
  ShapeDetectionLevelSet();
  virtual ~ShapeDetectionLevelSet();

  virtual void LoadInputImage();

  virtual void ShowConsole();

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage();

  virtual void ShowTimeCrossingMapImage();

  virtual void ShowGradientMagnitudeImage();

  virtual void ShowEdgePotentialImage();

  virtual void ShowThresholdedImage();

  virtual void ShowZeroSetImage();

  virtual void ShowThresholdedImageWithVTK();

  virtual void Quit();

  virtual void UpdateGUIAfterShapeDetectionIteration();

  virtual void UpdateGUIAfterFastMarchingIteration();

  virtual void SelectSeedPoint( float x, float y, float z );
  
  static  void ClickSelectCallback(float x, float y, float z, float value, void * args );

  virtual void CommandOnStartShapeDetection();

  virtual void CommandOnStartFastMarching();
  
  virtual void ClearSeeds();

private:

  ThresholdedImageViewerType      m_ThresholdedImageViewer;

  InternalImageViewerType         m_TimeCrossingMapViewer;

  InternalImageViewerType         m_InputImageViewer;

  InternalImageViewerType         m_GradientMagnitudeImageViewer;

  InternalImageViewerType         m_EdgePotentialImageViewer;

  InternalImageViewerType         m_ZeroSetImageViewer;

  VTKImageViewerType::Pointer     m_VTKSegmentedImageViewer;

  unsigned long                   m_ShapeDetectionIterationCounter;

  unsigned long                   m_FastMarchingIterationCounter;

};



#endif
