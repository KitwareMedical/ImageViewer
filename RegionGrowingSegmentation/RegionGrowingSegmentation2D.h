/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RegionGrowingSegmentation2D.h
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

#include <RegionGrowingSegmentation2DGUI.h>
#include <fltkImage2DViewer.h>



/**
 * \brief RegionGrowingSegmentation2D class that instantiate
 * the elements required for a Region Growing approach for segmentation
 *
 */
class RegionGrowingSegmentation2D : public RegionGrowingSegmentation2DGUI
{
public:

  /** Dimension of the images to be registered */ 
  enum { ImageDimension = RegionGrowingSegmentationBase2D::ImageDimension };

  /** Pixel type used for reading the input image */
  typedef   RegionGrowingSegmentationBase2D::InputPixelType       InputPixelType;

  /** Pixel type to be used internally */
  typedef   RegionGrowingSegmentationBase2D::InternalPixelType    InternalPixelType;

  /** Input image type */
  typedef   RegionGrowingSegmentationBase2D::InputImageType       InputImageType;

  /** Internal image type */
  typedef   RegionGrowingSegmentationBase2D::InternalImageType    InternalImageType;

  /** Image 2DViewer Types */
  typedef fltk::Image2DViewer< InputPixelType >         InputImageViewerType;
  typedef InputImageViewerType::Pointer                 InputImageViewerPointer;

  typedef fltk::Image2DViewer< InternalPixelType >      InternalImageViewerType;
  typedef InternalImageViewerType::Pointer              InternalImageViewerPointer;



public:
  RegionGrowingSegmentation2D();
  virtual ~RegionGrowingSegmentation2D();

  virtual void LoadInputImage(void);

  virtual void ShowConsole(void);

  virtual void ShowStatus(const char * text);

  virtual void ShowInputImage(void);
  virtual void ShowConnectedThresholdImage(void);
  virtual void ShowConfidenceConnectedImage(void);
  virtual void ShowFuzzyConnectedImage(void);
  virtual void ShowHomogeneousImage(void);
  virtual void ShowCurvatureFlowImage(void);
  virtual void ShowGradientAnisotropicDiffusionImage(void);
  virtual void ShowCurvatureAnisotropicDiffusionImage(void);
  virtual void ShowComposedImage(void);

  virtual void Quit(void);

  virtual void SelectSeedPoint( int x, int y );
  
  static void ClickSelectCallback(void * ptrObject, int & x, int & y ); 

private:

  InternalImageViewerPointer     m_CurvatureFlowImageViewer;
  InternalImageViewerPointer     m_GradientAnisotropicDiffusionImageViewer;
  InternalImageViewerPointer     m_CurvatureAnisotropicDiffusionImageViewer;
  InternalImageViewerPointer     m_ConnectedThresholdImageViewer;
  InternalImageViewerPointer     m_ConfidenceConnectedImageViewer;
  InternalImageViewerPointer     m_FuzzyConnectedImageViewer;
  InternalImageViewerPointer     m_HomogeneousImageViewer;
  InternalImageViewerPointer     m_ComposedImageViewer;

  InputImageViewerPointer        m_InputImageViewer;

};



#endif
