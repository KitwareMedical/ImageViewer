/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsole.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

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

  typedef fltk::ImageViewer< PixelType,     
                       OverlayPixelType > TargetImageViewerType;

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

private:

  ImageViewerType           * m_TargetViewer;
  ImageViewerType           * m_ReferenceViewer;
  ImageViewerType           * m_MappedReferenceViewer;

};



#endif
