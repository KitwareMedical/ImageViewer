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

  typedef fltk::ImageViewer< InputPixelType,     
                             OverlayPixelType > InputImageViewerType;

public:
  liImageRegistrationConsole();
  virtual ~liImageRegistrationConsole();
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Load(void);
  virtual void Quit(void);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void ShowTarget(void);
  virtual void ShowReference(void);
  virtual void ShowMappedReference(void);
  virtual void ShowAbout(void);
  virtual void Execute(void);
  virtual void UpdateTransformParameters(void);
  virtual void GenerateReference(void);
  virtual void GenerateMappedReference(void);

private:

  InputImageViewerType      * m_InputViewer;
  ImageViewerType           * m_ReferenceViewer;
  ImageViewerType           * m_MappedReferenceViewer;

};



#endif
