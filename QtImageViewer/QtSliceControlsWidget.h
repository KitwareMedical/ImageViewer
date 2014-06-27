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

#ifndef __QtSliceControlsWidget_h
#define __QtSliceControlsWidget_h

// Qt includes
#include <QWidget>
class QAbstractSlider;
class QSpinBox;

// QtImageViewer includes
#include "QtImageViewer_Export.h"
class QtGlSliceView;
class QtSliceControlsWidgetPrivate;

class QtImageViewer_EXPORT QtSliceControlsWidget : public QWidget
{
  Q_OBJECT
public:
  QtSliceControlsWidget(QWidget* parent);
  virtual ~QtSliceControlsWidget();

  /// Set the slice view to control.
  void setSliceView(QtGlSliceView *sliceView);

  /// Connect the slider to the slice view.
  /// \sa setSliceSpinBox(), sliceSlider()
  void setSliceSlider(QAbstractSlider* slider);
  /// Return the connected slice slider.
  /// \sa sliceSpinBox(), setSliceSlider()
  QAbstractSlider* sliceSlider()const;

  /// Connect the spinbox to the slice view.
  /// \sa setSliceSlider(), sliceSpinBox()
  void setSliceSpinBox(QSpinBox* spinBox);
  /// Return the connected slice spinbox.
  /// \sa sliceSlider(), setSliceSpinBox()
  QSpinBox* sliceSpinBox()const;

public slots:
  /// Display text into a text box.
  /// \sa setTextVisible()
  void setText(const QString& newText);
  /// Show/Hide the text box.
  /// \sa setText()
  void setTextVisible(bool visible);

protected slots:

  /// Update the widgets from the current input image.
  void updateImage();

  /// Update the slice range for the current orientation.
  void updateSliceRange();

  /// Update the position coordinate widgets.
  void setDisplayPosition(double x, double y, double z, double value);

  /// Set minimum intensity to the slice view when slider is moved.
  /// \sa setMaxIntensity(), updateMinIntensity()
  void setMinIntensity(int intensity);

  /// Set maximum intensity to the slice view when slider is moved.
  /// \sa setMinIntensity(), updateMaxIntensity()
  void setMaxIntensity(int intensity);

  /// Set the min intensity to the slider and spinbox.
  /// \sa setMinIntensity(), updateMaxIntensity()
  void updateMinIntensity(double value);

  /// Set the max intensity to the slider and spinbox.
  //// \sa setMaxIntensity(), updateMinIntensity()
  void updateMaxIntensity(double value);

protected:
  QScopedPointer<QtSliceControlsWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(QtSliceControlsWidget);
  Q_DISABLE_COPY(QtSliceControlsWidget);
};

#endif
