/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole3D.h
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

#include <fltkImageViewer.h>
#include <liFilterConsole3DGUI.h>

class liFilterConsole3D : public liFilterConsole3DGUI {

public:

  typedef unsigned short OverlayPixelType;
  typedef fltk::ImageViewer< PixelType, OverlayPixelType > ImageViewerType;
  typedef fltk::ImageViewer< InputPixelType, OverlayPixelType > InputImageViewerType;

public:
  liFilterConsole3D();
  virtual ~liFilterConsole3D();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Quit(void);
  virtual void ShowStatus(const char * text);
  virtual void ShowInputImage(void);
  virtual void ShowGradientImage(void);
  virtual void ShowCurvatureImage(void);
  virtual void SaveGradientImage(void);  
  virtual void SaveCurvatureImage(void);

private:
  InputImageViewerType*      m_InputViewer;

  ImageViewerType*           m_ViewerG;

  ImageViewerType*           m_ViewerC;

};



#endif
