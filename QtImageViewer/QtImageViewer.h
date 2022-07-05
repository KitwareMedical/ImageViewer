/*=========================================================================

Library:   TubeTK

Copyright Kitware Inc.

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
#ifndef __QtImageViewer_h
#define __QtImageViewer_h

// Qt includes
#include <QDialog>
#include <QJsonObject>

// ITK includes
#include <itkImage.h>

// ImageViewer includes
#include "QtImageViewer_Export.h"
    class QtGlSliceView;
class QtImageViewerPrivate;

class QtImageViewer_EXPORT QtImageViewer : public QDialog
{
  Q_OBJECT;
public:
  typedef QDialog Superclass;
  typedef double                              ImagePixelType;
  typedef itk::Image<double,3>                ImageType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>      OverlayImageType;

  QtImageViewer( QWidget* parent = 0,
                 Qt::WindowFlags fl = Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
  virtual ~QtImageViewer();

  QtGlSliceView* sliceView()const;

public slots:
  /// Load an image from a file path.
  /// If the path is empty, a file dialog is prompted to the user.
  /// \sa loadOverlayImage(), setInputImage()
  bool loadInputImage(QString filePath = QString());

  /// Load an image from a file path and apply as overlay in the viewer.
  //// \sa loadInputImage(), setOverlayImage()
  bool loadOverlayImage(QString filePath = QString());

  /// Load a JSON annotation file.
  bool loadJSONAnnotations(QString filePath = QString());

  /// Set the image to view.
  /// \sa setOverlayImage(), loadInputImage()
  virtual void setInputImage(ImageType * newImData);
  /// Set an image as overlay.
  /// \sa setInputImage(), loadOverlayImage()
  virtual void setOverlayImage(OverlayImageType * newImData);

protected slots:
  void releaseFixedSize();

protected:
  QScopedPointer<QtImageViewerPrivate> d_ptr;

  /// Reimplemented to propagate key events to QtGlSliceView.
  virtual void keyPressEvent(QKeyEvent* event);

private:
  Q_DECLARE_PRIVATE(QtImageViewer);
  Q_DISABLE_COPY(QtImageViewer);
  bool loadBoxAnnotations(const QJsonObject& root);
  bool loadRulerAnnotations(const QJsonObject& root);
};

#endif
