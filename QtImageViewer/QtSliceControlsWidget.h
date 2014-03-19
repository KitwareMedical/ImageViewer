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

//Qt includes
#include <QWidget>
#include <QGridLayout>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "ui_QtSliceControlsWidgetGUI.h"


class QtSliceControlsWidget : public QWidget
{
  Q_OBJECT
public:
  QtSliceControlsWidget(QWidget* parent);
  ~QtSliceControlsWidget();
  void setInputImage();
public slots:
  void setDisplayIMax(int value);
  void setDisplayIMin(int value);
  void setDisplayPosition(int x, int y, int z, double value);
  void setSliceView(QtGlSliceView *sliceView);
private:
  Ui::Form *UI;
  QtGlSliceView *SliceView;

};

#endif
