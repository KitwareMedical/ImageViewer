/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsole.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef liIMAGEREGISTRATIONCONSOLE
#define liIMAGEREGISTRATIONCONSOLE

#include <fltkImageViewer.h>
#include <liImageRegistrationConsoleGUI.h>

class liImageRegistrationConsole : public liImageRegistrationConsoleGUI {

public:

  typedef unsigned char  OverlayPixelType;
  
  typedef fltk::ImageViewer< PixelType,
                             OverlayPixelType > ImageViewerType;


public:

  liImageRegistrationConsole();
  virtual ~liImageRegistrationConsole();

  virtual void Show(void);
  virtual void Hide(void);

  virtual void LoadFixedImage(void);
  virtual void LoadMovingImage(void);

  virtual void Quit(void);
  virtual void ShowStatus(const char * text);

  virtual void ShowFixedImage(void);
  virtual void ShowMovingImage(void);
  virtual void ShowInputMovingImage(void);
  virtual void ShowMappedMovingImage(void);

  virtual void Execute(void);
  virtual void UpdateTransformParameters(void);

  virtual void GenerateMovingImage(void);
  virtual void GenerateMappedMovingImage(void);


private:

  ImageViewerType            m_InputMovingImageViewer;
  ImageViewerType            m_MovingImageViewer;
  ImageViewerType            m_FixedImageViewer;
  ImageViewerType            m_MappedMovingImageViewer;

};



#endif



