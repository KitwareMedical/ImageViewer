/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsole.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/




#ifndef ce_ExtractorConsole_h
#define ce_ExtractorConsole_h

#include "ceExtractorConsoleGUI.h"
#include "fltkImage2DViewer.h"
#include "fltkDisplayGlWindowGUI.h"
#include "fltkPointSet3D.h"
#include "fltkPointSet2D.h"
#include "itkPointSet.h"



class ceExtractorConsole : public ceExtractorConsoleGUI 
{

public:

  typedef fltk::Image2DViewer< PixelType >      ImageViewerType;
  typedef fltk::Image2DViewer< InputPixelType > InputImageViewerType;
  typedef ImageViewerType::Pointer              ImageViewerPointer;
  typedef InputImageViewerType::Pointer         InputImageViewerPointer;
  typedef fltkDisplayGlWindowGUI                GlDisplayType;
  
  typedef ceExtractorConsoleBase::MeshType      PointSetType;
  typedef fltk::PointSet3D< PointSetType >      PointSetShapeType;
  typedef PointSetShapeType::Pointer            PointSetShapePointer;

  typedef ceExtractorConsoleBase::ImageSpaceMeshType ImageSpaceMeshType;
  typedef fltk::PointSet2D< ImageSpaceMeshType > ImageSpacePointSetShapeType;   
  typedef ImageSpacePointSetShapeType::Pointer   ImageSpacePointSetShapePointer;

public:
  ceExtractorConsole();
  virtual ~ceExtractorConsole();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Load(const char * filename);
  virtual void Quit(void);
  virtual void ShowStatus(const char * text);
  virtual void ShowInput(void);
  virtual void ShowFilteredX(void);
  virtual void ShowFilteredY(void);
  virtual void ShowSecondDerivativeX(void);
  virtual void ShowSecondDerivativeY(void);
  virtual void ShowCrossDerivativeXY(void);
  virtual void ShowLaplacian(void);
  virtual void ShowSmoothed(void);
  virtual void ShowGradientModulus(void);
  virtual void ShowMaxEigenValue(void);
  virtual void ShowMinEigenValue(void);
  virtual void ShowGradientOnEigenVector(void);
  virtual void ShowParametricSpace(void);
  virtual void ShowCurve2DPoints(void);
  virtual void ShowExtractedParametricPoints(void);
  virtual void ResetViewOfParametricSpace(void);
  virtual void ResetViewOfExtractedParametricSpace(void);
  virtual void SetSigma( ceExtractorConsoleBase::ComputationType );
  virtual void Execute(void);

private:

  InputImageViewerPointer      m_InputViewer;

  ImageViewerPointer           m_Viewer_H1x;
  ImageViewerPointer           m_Viewer_H1y;

  ImageViewerPointer           m_Viewer_H2x;
  ImageViewerPointer           m_Viewer_H2y;

  ImageViewerPointer           m_Viewer_Hxy;
  ImageViewerPointer           m_Viewer_H1xy;

  ImageViewerPointer           m_Viewer_Smoothed;

  ImageViewerPointer           m_Viewer_Laplacian;

  ImageViewerPointer           m_Viewer_Gradient_Modulus;

  ImageViewerPointer           m_Viewer_Max_EigenValue;
  ImageViewerPointer           m_Viewer_Min_EigenValue;

  ImageViewerPointer           m_Viewer_Gradient_On_EigenVector;

  ImageViewerPointer           m_Viewer_Extracted_Points;
    
  GlDisplayType                m_ParametricSpaceViewer;
  GlDisplayType                m_ExtractedParametricSpaceViewer;

  PointSetShapePointer         m_ParametricSpaceSamplesShape;
  PointSetShapePointer         m_ExtractedParametricSpaceSamplesShape;
  
  ImageSpacePointSetShapePointer    m_ImageSpaceSamplesShape;

};



#endif
