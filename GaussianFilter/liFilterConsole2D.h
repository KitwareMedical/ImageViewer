/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef liFILTERCONSOLE
#define liFILTERCONSOLE

#include <fltkImage2DViewer.h>
#include <liFilterConsole2DGUI.h>

class liFilterConsole2D : public liFilterConsole2DGUI {

public:

  typedef fltk::Image2DViewer< PixelType >      ImageViewerType;
  typedef fltk::Image2DViewer< InputPixelType > InputImageViewerType;

public:
  liFilterConsole2D();
  virtual ~liFilterConsole2D();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Quit(void);
  virtual void ShowStatus(const char * text);
  virtual void ShowInput(void);
  virtual void ShowFilteredX(void);
  virtual void ShowFilteredY(void);
  virtual void ShowSecondDerivativeX(void);
  virtual void ShowSecondDerivativeY(void);
  virtual void ShowLaplacian(void);
  virtual void ShowSmoothed(void);
  virtual void ShowGradientModulus(void);
  virtual void Execute(void);

private:
  InputImageViewerType::Pointer      m_InputViewer;

  ImageViewerType::Pointer           m_Viewer_H1x;
  ImageViewerType::Pointer           m_Viewer_H1y;

  ImageViewerType::Pointer           m_Viewer_H2x;
  ImageViewerType::Pointer           m_Viewer_H2y;

  ImageViewerType::Pointer           m_Viewer_Laplacian;
  ImageViewerType::Pointer           m_Viewer_Smoothed;
  ImageViewerType::Pointer           m_Viewer_Gradient_Modulus;

};



#endif
