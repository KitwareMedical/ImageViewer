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

#include "QtImageViewer_Export.h"
//Qt includes
#include <QWidget>
#include <QGridLayout>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "ui_QtSliceControlsWidgetGUI.h"


class QtImageViewer_EXPORT QtSliceControlsWidget : public QWidget
{
  Q_OBJECT
public:
  QtSliceControlsWidget(QWidget* parent);
  ~QtSliceControlsWidget();
  void setInputImage();
  /// CTK methods, comes from ctkUtils.cxx class
  static int significantDecimals(double value, int defaultDecimals);
public slots:
  /// Calcul and set the Slider value
  void setValueIntensityMin(double value);
  /// Calcul and set the Slider value
  void setValueIntensityMax(double value);
  /// Display text into a text box.
  /// \sa setTextVisible()
  void setText(const QString& newText);
  /// Show/Hide the text box.
  /// \sa setText()
  void setTextVisible(bool visible);
  /// Display the maximum intensity in the doubleSpinBox
  void setDisplayIMax(double value);
  /// Display the minimum intensity in the doubleSpinBox
  void setDisplayIMin(double value);
  void setDisplayPosition(double x, double y, double z, double value);
  void setSliceView(QtGlSliceView *sliceView);
protected slots:
  /// Convert Slider value to double and set minimum intensity to th SliceView
  void setMinIntensity(int intensity);
  /// Convert Slider value to double and set maximum intensity to th SliceView
  void setMaxIntensity(int intensity);
private:
  Ui::Controls *UI;
  QtGlSliceView *SliceView;
  int decimalsNumber;

};

#endif
