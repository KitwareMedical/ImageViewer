/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef QtSlicer_h
#define QtSlicer_h

#include "QtImageViewer_Export.h"

//Qt include
#include <QDialog>

//itk include
#include "itkImage.h"
#include "itkImageFileReader.h"

//QtImageViewer includes
#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"


class QtImageViewer_EXPORT QtSlicer : public QDialog, public Ui::GuiDialogBase
{
  Q_OBJECT
public:
    
  QtSlicer( QWidget* parent = 0, Qt::WindowFlags fl = Qt::WindowTitleHint |
      Qt::WindowCloseButtonHint );
  ~QtSlicer();
  
  typedef itk::Image<double,3>                ImageType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>      OverlayType;

  bool loadOverlayImage(QString overlayImagePath = QString());
  bool loadInputImage(QString filePathTLoad = QString());

public slots:
  void hideHelp();
  void showHelp(bool checked = true);
  void updateSliceMaximum();
  void setInputImage(ImageType * newImData);
  void setOverlayImage(OverlayType * newImData);
  void setDisplaySliceNumber(int number);
protected:
  QDialog* HelpDialog;
};

#endif
