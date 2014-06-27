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

// Qt includes
#include <QAbstractSlider>
#include <QDebug>
#include <QSpinBox>

// QtImageViewer includes
#include "QtGlSliceView.h"
#include "QtSliceControlsWidget.h"
#include "ui_QtSliceControlsWidget.h"


class QtSliceControlsWidgetPrivate: public Ui_QtSliceControlsWidget
{
  Q_DECLARE_PUBLIC(QtSliceControlsWidget);
public:
  typedef Ui_QtSliceControlsWidget Superclass;
  QtSliceControlsWidgetPrivate(QtSliceControlsWidget& obj);

  virtual void setupUi(QWidget* widgetToSetup);
  virtual void connectOrDisconnectSlider(bool connect);
  virtual void connectOrDisconnectSpinBox(bool connect);

  /// Compute the ideal number of decimals for a given number.
  static int significantDecimals(double value, int defaultDecimals = 0);

  QtGlSliceView *SliceView;
  int Decimals;
  QAbstractSlider* SliceSlider;
  QSpinBox* SliceSpinBox;
protected:
  QtSliceControlsWidget* const q_ptr;
};


QtSliceControlsWidgetPrivate::QtSliceControlsWidgetPrivate(QtSliceControlsWidget& obj)
  : q_ptr(&obj)
  , SliceView(0)
  , Decimals(0)
  , SliceSlider(0)
  , SliceSpinBox(0)
{
}

void QtSliceControlsWidgetPrivate::setupUi(QWidget* widgetToSetup)
{
  Q_Q(QtSliceControlsWidget);
  this->Superclass::setupUi(widgetToSetup);
  QObject::connect(this->IntensityMax, SIGNAL(sliderMoved(int)),
                   q, SLOT(setMaxIntensity(int)));
  QObject::connect(this->IntensityMin, SIGNAL(sliderMoved(int)),
                   q, SLOT(setMinIntensity(int)));
}

void QtSliceControlsWidgetPrivate::connectOrDisconnectSlider(bool connect)
{
  if (!this->SliceView || !this->SliceSlider)
    {
    return;
    }
  if (connect)
    {
    QObject::connect(this->SliceSlider, SIGNAL(sliderMoved(int)),
                     this->SliceView, SLOT(changeSlice(int)), Qt::UniqueConnection);
    QObject::connect(this->SliceView, SIGNAL(sliceNumChanged(int)),
                     this->SliceSlider, SLOT(setValue(int)), Qt::UniqueConnection);
    }
  else
    {
    QObject::disconnect(this->SliceSlider, SIGNAL(sliderMoved(int)),
                        this->SliceView, SLOT(changeSlice(int)));
    QObject::disconnect(this->SliceView, SIGNAL(sliceNumChanged(int)),
                        this->SliceSlider, SLOT(setValue(int)));
    }
}

void QtSliceControlsWidgetPrivate::connectOrDisconnectSpinBox(bool connect)
{
  if (!this->SliceView || !this->SliceSpinBox)
    {
    return;
    }
  if (connect)
    {
    QObject::connect(this->SliceSpinBox, SIGNAL(valueChanged(int)),
                     this->SliceView, SLOT(changeSlice(int)), Qt::UniqueConnection);
    QObject::connect(this->SliceView, SIGNAL(sliceNumChanged(int)),
                     this->SliceSpinBox, SLOT(setValue(int)), Qt::UniqueConnection);
    }
  else
    {
    QObject::disconnect(this->SliceSpinBox, SIGNAL(valueChanged(int)),
                        this->SliceView, SLOT(changeSlice(int)));
    QObject::disconnect(this->SliceView, SIGNAL(sliceNumChanged(int)),
                        this->SliceSpinBox, SLOT(setValue(int)));
    }
}


/// CTK methods, comes from ctkUtils.cxx class
int QtSliceControlsWidgetPrivate::significantDecimals(double value, int defaultDecimals)
{
  if (value == 0.
      || qAbs(value) == std::numeric_limits<double>::infinity())
    {
    return 0;
    }
  if (value != value) // is NaN
    {
    return -1;
    }
  QString number = QString::number(value, 'f', 16);
  QString fractional = number.section('.', 1, 1);
  Q_ASSERT(fractional.length() == 16);
  QChar previous;
  int previousRepeat=0;
  bool only0s = true;
  bool isUnit = value > -1. && value < 1.;
  for (int i = 0; i < fractional.length(); ++i)
    {
    QChar digit = fractional.at(i);
    if (digit != '0')
      {
      only0s = false;
      }
    // Has the digit been repeated too many times ?
    if (digit == previous && previousRepeat == 2 &&
        !only0s)
      {
      if (digit == '0' || digit == '9')
        {
        return i - previousRepeat;
        }
      return i;
      }
    // Last digit
    if (i == fractional.length() - 1)
      {
      // If we are here, that means that the right number of significant
      // decimals for the number has not been figured out yet.
      if (previousRepeat > 2 && !(only0s && isUnit) )
        {
        return i - previousRepeat;
        }
      // If defaultDecimals has been provided, just use it.
      if (defaultDecimals >= 0)
        {
        return defaultDecimals;
        }
      return fractional.length();
      }
    // get ready for next
    if (previous != digit)
      {
      previous = digit;
      previousRepeat = 1;
      }
    else
      {
      ++previousRepeat;
      }
    }
  Q_ASSERT(false);
  return fractional.length();
}


QtSliceControlsWidget::QtSliceControlsWidget(QWidget* parent)
  : QWidget(parent)
  , d_ptr(new QtSliceControlsWidgetPrivate(*this))
{
  Q_D(QtSliceControlsWidget);
  d->setupUi(this);
}


QtSliceControlsWidget::~QtSliceControlsWidget()
{
}


void QtSliceControlsWidget::setSliceView(QtGlSliceView* sliceView)
{
  Q_D(QtSliceControlsWidget);

  if (d->SliceView)
    {
    QObject::disconnect(d->SliceView, 0, this, 0);
    d->connectOrDisconnectSlider(false);
    d->connectOrDisconnectSpinBox(false);
    }

  d->SliceView = sliceView;

  // Image
  QObject::connect(d->SliceView, SIGNAL(imageChanged()),
                   this, SLOT(updateImage()));
  // Position
  QObject::connect(d->SliceView, SIGNAL(positionChanged(double,double,double,double)),
                   this, SLOT(setDisplayPosition(double,double,double,double)));
  // Zoom
  QObject::connect(d->ZoomIn, SIGNAL(clicked()), d->SliceView, SLOT(zoomIn()));
  QObject::connect(d->ZoomOut, SIGNAL(clicked()), d->SliceView, SLOT(zoomOut()));
  // Window/Level
  QObject::connect(d->SliceView, SIGNAL(minIntensityChanged(double)),
                   this, SLOT(updateMinIntensity(double)));
  QObject::connect(d->SliceView, SIGNAL(maxIntensityChanged(double)),
                   this, SLOT(updateMaxIntensity(double)));
  QObject::connect(d->IntensityMinDisplay, SIGNAL(valueChanged(double)),
                   d->SliceView, SLOT(setMinIntensity(double)));
  QObject::connect(d->IntensityMaxDisplay,SIGNAL(valueChanged(double)),
                   d->SliceView, SLOT(setMaxIntensity(double)));
  // Details
  QObject::connect(d->SliceView, SIGNAL(detailsChanged(QString)),
                   this, SLOT(setText(QString)));
  // Slider
  QObject::connect(d->SliceView, SIGNAL(orientationChanged(int)),
                   this, SLOT(updateSliceRange()));
  d->connectOrDisconnectSlider(true);
  d->connectOrDisconnectSpinBox(true);

  this->updateImage();
}


QAbstractSlider* QtSliceControlsWidget::sliceSlider()const
{
  Q_D(const QtSliceControlsWidget);
  return d->SliceSlider;
}


void QtSliceControlsWidget::setSliceSlider(QAbstractSlider* slider)
{
  Q_D(QtSliceControlsWidget);
  d->connectOrDisconnectSlider(false);
  d->SliceSlider = slider;
  d->connectOrDisconnectSlider(true);
}


QSpinBox* QtSliceControlsWidget::sliceSpinBox()const
{
  Q_D(const QtSliceControlsWidget);
  return d->SliceSpinBox;
}


void QtSliceControlsWidget::setSliceSpinBox(QSpinBox* spinBox)
{
  Q_D(QtSliceControlsWidget);
  d->connectOrDisconnectSpinBox(false);
  d->SliceSpinBox = spinBox;
  d->connectOrDisconnectSpinBox(true);
}


void QtSliceControlsWidget
::setDisplayPosition(double x, double y, double z, double value)
{
  Q_D(QtSliceControlsWidget);
  d->PositionX->setValue( x );
  d->PositionY->setValue( y );
  d->PositionZ->setValue( z );
  d->PixelValue->setValue( value );
}


void QtSliceControlsWidget::updateMinIntensity(double minIntensity)
{
  Q_D(QtSliceControlsWidget);
  d->IntensityMinDisplay->setValue( minIntensity );
  d->IntensityMin->setValue( minIntensity / d->SliceView->singleStep() );
}


void QtSliceControlsWidget::updateMaxIntensity(double maxIntensity)
{
  Q_D(QtSliceControlsWidget);
  d->IntensityMaxDisplay->setValue( maxIntensity );
  d->IntensityMax->setValue( maxIntensity / d->SliceView->singleStep() );
}


void QtSliceControlsWidget::updateImage()
{
  Q_D(QtSliceControlsWidget);
  d->SliceView->setSingleStep( 0.02 * d->SliceView->intensityRange());
  d->Decimals = d->significantDecimals(d->SliceView->singleStep(),-1);

  const int min = 0;
  const int max = static_cast<int>(
    d->SliceView->intensityRange() / d->SliceView->singleStep());
  d->IntensityMin->setRange( min, max );
  d->IntensityMin->setValue( min );
  d->IntensityMax->setRange( min, max );
  d->IntensityMax->setValue( max );

  d->IntensityMinDisplay->setDecimals( d->Decimals );
  d->IntensityMinDisplay->setSingleStep( d->SliceView->singleStep() );
  d->IntensityMinDisplay->setRange( d->SliceView->minIntensity(),
                                    d->SliceView->maxIntensity() );
  d->IntensityMinDisplay->setValue( d->SliceView->minIntensity() );

  d->IntensityMaxDisplay->setDecimals( d->Decimals );
  d->IntensityMaxDisplay->setRange( d->SliceView->minIntensity(),
                                    d->SliceView->maxIntensity() );
  d->IntensityMaxDisplay->setValue( d->SliceView->maxIntensity() );
  d->IntensityMaxDisplay->setSingleStep( d->SliceView->singleStep() );

  d->PositionX->setDecimals( d->Decimals );
  d->PositionX->setMaximum( d->SliceView->imageSize(0) );
  d->PositionY->setDecimals( d->Decimals );
  d->PositionY->setMaximum( d->SliceView->imageSize(1) );
  d->PositionZ->setDecimals( d->Decimals );
  d->PositionZ->setMaximum( d->SliceView->imageSize(2) );
  d->PixelValue->setDecimals(d->Decimals);
  d->PixelValue->setRange( d->SliceView->minIntensity(),
                           d->SliceView->maxIntensity() );

  this->updateSliceRange();
}


void QtSliceControlsWidget::updateSliceRange()
{
  Q_D(QtSliceControlsWidget);
  if (!d->SliceView)
    {
    return;
    }
  const int minimum = 0;
  const int maximum = d->SliceView->maxSliceNum() - 1;
  const int slice = d->SliceView->sliceNum();
  if (d->SliceSlider)
    {
    d->SliceSlider->setRange(minimum, maximum);
    d->SliceSlider->setValue(slice);
    }
  if (d->SliceSpinBox)
    {
    d->SliceSpinBox->setRange(minimum, maximum);
    d->SliceSpinBox->setValue(slice);
    }
}


void QtSliceControlsWidget::setText(const QString& text)
{
  Q_D(QtSliceControlsWidget);
  d->Details->setText(text);
}


void QtSliceControlsWidget::setTextVisible(bool visible)
{
  Q_D(QtSliceControlsWidget);
  d->Details->setVisible(visible);
}


void QtSliceControlsWidget::setMinIntensity(int intensity)
{
  Q_D(QtSliceControlsWidget);
  double newMinIntensity = intensity * d->SliceView->singleStep();
  d->SliceView->setMinIntensity(newMinIntensity);
}


void QtSliceControlsWidget::setMaxIntensity(int intensity)
{
  Q_D(QtSliceControlsWidget);
  double newMaxIntensity = intensity * d->SliceView->singleStep();
  d->SliceView->setMaxIntensity(newMaxIntensity);
}

