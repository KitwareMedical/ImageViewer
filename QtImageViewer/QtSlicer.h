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

// Qt includes
#include <QDialog>

// ITK includes
#include <itkImage.h>

// ImageViewer includes
#include "QtImageViewer_Export.h"
class QtGlSliceView;
class QtSlicerPrivate;

class QtImageViewer_EXPORT QtSlicer : public QDialog
{
  Q_OBJECT;
public:
  typedef QDialog Superclass;
  typedef itk::Image<double,3>                ImageType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>      OverlayType;

  QtSlicer( QWidget* parent = 0,
            Qt::WindowFlags fl = Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
  virtual ~QtSlicer();

  QtGlSliceView* sliceView()const;

public slots:
  bool loadOverlayImage(QString overlayImagePath = QString());
  bool loadInputImage(QString filePathTLoad = QString());

  void setInputImage(ImageType * newImData);
  void setOverlayImage(OverlayType * newImData);

  void showHelp(bool checked = true);
  void hideHelp();

protected slots:
  void onDisplayStateChanged(int details);
  void updateSliceRange();

protected:
  QScopedPointer<QtSlicerPrivate> d_ptr;

  /// Reimplemented to propagate key events to QtGlSliceView.
  virtual void keyPressEvent(QKeyEvent* event);
  virtual bool eventFilter(QObject* obj, QEvent* event);

private:
  Q_DECLARE_PRIVATE(QtSlicer);
  Q_DISABLE_COPY(QtSlicer);
};

#endif
