/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageRegistrationConsole.h
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

#include <fltkImage2DViewer.h>
#include <fltkRegularStepGradientDescent.h>
#include <ImageRegistrationConsoleGUI.h>

class ImageRegistrationConsole : public ImageRegistrationConsoleGUI {

public:

  typedef ImageRegistrationConsoleBase::PixelType PixelType;

  typedef fltk::Image2DViewer< PixelType >    ImageViewerType;
  typedef ImageViewerType::Pointer            ImageViewerPointer;
  typedef fltk::RegularStepGradientDescent    OptimizerGUIType;


public:

  ImageRegistrationConsole();
  virtual ~ImageRegistrationConsole();

  virtual void Show(void);
  virtual void Hide(void);

  virtual void LoadFixedImage(void);
  virtual void LoadMovingImage(void);
  virtual void SaveRegisteredMovingImage(void);

  virtual void Quit(void);
  virtual void ShowStatus(const char * text);

  virtual void ShowFixedImage(void);
  virtual void ShowMovingImage(void);
  virtual void ShowRegisteredMovingImage(void);

  virtual void Execute(void);

  virtual void GenerateRegisteredMovingImage(void);


private:

  ImageViewerPointer            m_MovingImageViewer;
  ImageViewerPointer            m_FixedImageViewer;
  ImageViewerPointer            m_RegisteredMovingImageViewer;
  OptimizerGUIType              m_OptimizerInterface;

};



#endif



