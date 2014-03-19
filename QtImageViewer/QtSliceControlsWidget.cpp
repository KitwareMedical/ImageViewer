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

#include "QtSliceControlsWidget.h"


QtSliceControlsWidget::QtSliceControlsWidget(QWidget* parent)
  : QWidget(parent)
{
  this->UI = new Ui::Form;
  UI->setupUi(this);
}

QtSliceControlsWidget::~QtSliceControlsWidget()
{

}

void QtSliceControlsWidget::setDisplayPosition(int x,int y ,int z,double value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",x);
  UI->PositionX->setText(tr(tempchar));
  sprintf(tempchar,"%d",y);
  UI->PositionY->setText(tr(tempchar));
  sprintf(tempchar,"%d",z);
  UI->PositionZ->setText(tr(tempchar));
  sprintf(tempchar,"%3.1f",value);
  UI->PixelValue->setText(tr(tempchar));
  this->SliceView->update();
  delete tempchar;
}

void QtSliceControlsWidget::setDisplayIMin(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  UI->IntensityMinDisplay->setText(tempchar);
  delete tempchar;
}

void QtSliceControlsWidget::setDisplayIMax(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  UI->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;
}


void QtSliceControlsWidget::setInputImage()
{
  QObject::connect(this->SliceView, SIGNAL(positionChanged(int,int,int,double)), this,
                   SLOT(setDisplayPosition(int,int,int,double)));
  QObject::connect(UI->IntensityMax, SIGNAL(sliderMoved(int)), this->SliceView,
                   SLOT(setMaxIntensity(int)));
  QObject::connect(UI->IntensityMin, SIGNAL(sliderMoved(int)), this->SliceView,
                   SLOT(setMinIntensity(int)));
  QObject::connect(UI->ZoomIn, SIGNAL(clicked()), this->SliceView, SLOT(zoomIn()));
  QObject::connect(UI->ZoomOut, SIGNAL(clicked()), this->SliceView, SLOT(zoomOut()));
  QObject::connect(this->SliceView, SIGNAL(maxIntensityChanged(int)), this,
                   SLOT(setDisplayIMax(int)));
  QObject::connect(this->SliceView, SIGNAL(minIntensityChanged(int)), this,
                   SLOT(setDisplayIMin(int)));
  QObject::connect(this->SliceView, SIGNAL(maxIntensityChanged(int)), UI->IntensityMax,
                   SLOT(setValue(int)));
  QObject::connect(this->SliceView, SIGNAL(minIntensityChanged(int)), UI->IntensityMin,
                   SLOT(setValue(int)));
  QObject::connect(this->SliceView, SIGNAL(updateDetails(QString)), UI->Details,
                   SLOT(setText(QString)));

  UI->IntensityMin->setMinimum( static_cast<int>
                                  ( this->SliceView->minIntensity() ));
  UI->IntensityMin->setMaximum( static_cast<int>
                                  ( this->SliceView->maxIntensity() ));
  UI->IntensityMin->setValue( static_cast<int>
                                ( this->SliceView->minIntensity() ));
  UI->IntensityMax->setMinimum( static_cast<int>
                                  ( this->SliceView->minIntensity() ));
  UI->IntensityMax->setMaximum( static_cast<int>
                                  ( this->SliceView->maxIntensity() ));
  UI->IntensityMax->setValue( static_cast<int>
                                ( this->SliceView->maxIntensity() ));

  char* tempchar = new char[20];
  sprintf(tempchar,"%.0f",this->SliceView->minIntensity());
  UI->IntensityMinDisplay->setText(tempchar);
  sprintf(tempchar,"%.0f",this->SliceView->maxIntensity());
  UI->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;
}

void QtSliceControlsWidget::setSliceView(QtGlSliceView* sliceView)
{
  this->SliceView = sliceView;
}
