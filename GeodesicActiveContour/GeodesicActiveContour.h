/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GeodesicActiveContour.h
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

#include "GeodesicActiveContourGUI.h"
#include "fltkImageViewer.h"

#include "fltkVTKImageViewer.h"


/**
 * \brief GeodesicActiveContour class that instantiate
 * the elements required for a LevelSet approach for segmentation
 *
 */
class GeodesicActiveContour : public GeodesicActiveContourGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = GeodesicActiveContourBase::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   GeodesicActiveContourBase::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   GeodesicActiveContourBase::InternalPixelType    InternalPixelType;

  /** Input image type */
  typedef   GeodesicActiveContourBase::InputImageType       InputImageType;

  /** Internal image type */
  typedef   GeodesicActiveContourBase::InternalImageType    InternalImageType;

  /** Image Viewer Types */
  typedef GeodesicActiveContourBase::SeedPixelType        OverlayPixelType;

  /** FLTK viewer module */
  typedef fltk::ImageViewer< InternalPixelType, 
                                    OverlayPixelType >     InternalImageViewerType;

  typedef fltk::ImageViewer< InternalPixelType, 
                                    ThresholdPixelType >   SegmentedImageViewerType;

  typedef fltk::VTKImageViewer< ThresholdPixelType >       VTKImageViewerType;


public:
  GeodesicActiveContour();
  virtual ~GeodesicActiveContour();

  virtual void LoadInputImage();

  virtual void ShowConsole();

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage();

  virtual void ShowOutputLevelSet();

  virtual void ShowGradientMagnitudeImage();

  virtual void ShowEdgePotentialImage();

  virtual void ShowThresholdedImage();

  virtual void ShowFastMarchingResultImage();
    
  virtual void ShowThresholdedImageWithVTK();

  virtual void Quit();

  virtual void SelectSeedPoint( float x, float y, float z );
  
  static  void ClickSelectCallback(float x, float y, float z, float value, void * args );

  virtual void ClearSeeds();

private:

  SegmentedImageViewerType        m_ThresholdedImageViewer;

  SegmentedImageViewerType        m_FastMarchingImageViewer;
  
  InternalImageViewerType         m_OutputLevelSetViewer;

  InternalImageViewerType         m_InputImageViewer;

  InternalImageViewerType         m_GradientMagnitudeImageViewer;

  InternalImageViewerType         m_EdgePotentialImageViewer;

  InternalImageViewerType         m_ZeroSetImageViewer;

  VTKImageViewerType::Pointer     m_VTKSegmentedImageViewer;

};



#endif
