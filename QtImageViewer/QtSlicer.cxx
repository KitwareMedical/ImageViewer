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
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSlider>

// QtImageViewer includes
#include "QtSlicer.h"
#include "QtSliceControlsWidget.h"
#include "QtGlSliceView.h"
#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"

// ITK includes
#include <itkImageFileReader.h>

// STD includes
#include <iostream>

class QtSlicerPrivate: public Ui::QtSlicerGUI
{
  Q_DECLARE_PUBLIC(QtSlicer);
public:
  typedef Ui::QtSlicerGUI Superclass;
  enum DisplayStates{
    OFF = 0x00,
    ON_SLICEVIEW = 0x01,
    ON_TEXTBOX = 0x02,
    OFF_COLLAPSE = 0x04,
    ON_COLLAPSE = 0x08
  };

  QtSlicerPrivate(QtSlicer& obj);

  virtual void setupUi(QDialog* widgetToSetup);

  template <class PixelType>
  typename itk::Image<PixelType,3>::Pointer loadImage(QString& filePath,
                                                      const QString& imageType = QString());

  template <class PixelType>
  typename itk::Image<PixelType,3>::Pointer readImage(const QString& filePath);

  /// Resize the entire dialog based on the current size and to ensure it fits
  /// the contents.
  /// \sa QWidget::resize(), QWidget::adjustSize()
  void updateSize();

  /// Utility method that returns all the widgets of a layout that belong to
  /// n levels.
  static QWidgetList layoutWidgets(QLayout* layout, int level = -1);

  QDialog* HelpDialog;
  bool IsRedirectingEvent;

protected:
  QtSlicer* const q_ptr;
};


QtSlicerPrivate::QtSlicerPrivate(QtSlicer& obj)
  : q_ptr(&obj)
  , HelpDialog(0)
  , IsRedirectingEvent(false)
{
}


void QtSlicerPrivate::setupUi(QDialog* widgetToSetup)
{
  Q_Q(QtSlicer);
  this->Superclass::setupUi(widgetToSetup);

  this->Controls->setSliceView(this->OpenGlWindow);
  this->OpenGlWindow->setMaxDisplayStates(5);

  QObject::connect(this->ButtonBox, SIGNAL(accepted()),
                   q, SLOT(accept()));
  QObject::connect(this->ButtonBox, SIGNAL(rejected()),
                   q, SLOT(reject()));
  QObject::connect(this->ButtonBox, SIGNAL(helpRequested()),
                   q, SLOT(showHelp()));
  QObject::connect(this->SliceNumSlider, SIGNAL(sliderMoved(int)),
                   this->OpenGlWindow, SLOT(changeSlice(int)));
  QObject::connect(this->OpenGlWindow, SIGNAL(sliceNumChanged(int)),
                   this->SliceNumSlider, SLOT(setValue(int)));
  QObject::connect(this->OpenGlWindow, SIGNAL(sliceNumChanged(int)),
                   this->SliceValue, SLOT(setValue(int)));
  QObject::connect(this->OpenGlWindow, SIGNAL(orientationChanged(int)),
                   q, SLOT(updateSliceRange()));
  QObject::connect(this->OpenGlWindow, SIGNAL(displayStateChanged(int)),
                   q, SLOT(onDisplayStateChanged(int)));

  // Install event filter on all the double spinboxes. They eat letter key
  // events for no reason
  foreach(QDoubleSpinBox* spinBox, q->findChildren<QDoubleSpinBox*>())
    {
    spinBox->installEventFilter(q);
    }
}

template <class PixelType>
typename itk::Image<PixelType, 3>::Pointer QtSlicerPrivate
::loadImage(QString& filePath, const QString& imageType)
{
  Q_Q(QtSlicer);
  typename itk::Image<PixelType, 3>::Pointer res;
  // If the path is empty, prompt a dialog to give a chance to select the image
  // to load.
  if (filePath.isEmpty())
    {
    filePath = QFileDialog::getOpenFileName(
        q, imageType, QDir::currentPath());
    }
  // Empty if the user cancelled the dialog.
  if (filePath.isEmpty())
    {
    return res;
    }
  // Might be a non existing path.
  QFileInfo fileInfo(filePath);
  if (!fileInfo.exists())
    {
    const QString messageTitle("Failed loading");
    const QString message =
      QString("The file you have selected does not exist. %1").arg(filePath);
    QMessageBox::warning(q, messageTitle, message);
    return res;
    }

  // Read the file.
  res = this->readImage<PixelType>(filePath);
  return res;
}

template <class PixelType>
typename itk::Image<PixelType, 3>::Pointer QtSlicerPrivate::readImage(const QString& filePath)
{
  Q_Q(QtSlicer);
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filePath.toLatin1().data() );

  std::cout << "Read image " << filePath.toStdString() << "...";
  std::cout.flush();
  typename itk::Image<PixelType, 3>::Pointer res;
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cout << " failed" << std::endl;
    std::cerr << "Exception when reading image" << std::endl;
    std::cerr << e << std::endl;

    const QString title("Failed to read image");
    const QString message = QString("%1").arg(filePath);
    QMessageBox::warning(q, title, message);
    return res;
    }
  std::cout << " ok" << std::endl;
  res = reader->GetOutput();
  return res;
}

void QtSlicerPrivate::updateSize()
{
  Q_Q(QtSlicer);
  q->updateGeometry();

  QWidgetList childWidgets = this->layoutWidgets(q->layout(), 2);
  // Find the true width of the widget. Discard the children that are hidden.
  // We do not need the height information.
  QRect geom;
  foreach(QWidget* child, childWidgets)
    {
    if (child->isVisibleTo(q))
      {
      QPoint childPos = child->mapTo(q, QPoint(0,0));
      int width = child->width() ? child->width() : child->sizeHint().width();
      int height = child->height() ? child->height() : child->sizeHint().height();
      //qDebug() << child << width << height;
      geom = geom.united(QRect(childPos, QSize(width, height)));
      }
    }
  int width = geom.width();
  int height = geom.height();
  if (this->Controls->isVisibleTo(q))
    {
    height = q->heightForWidth(width);
    }
  else
    {
    height = this->OpenGlWindow->heightForWidth(width);
    }
  QSize newSize = QSize(width, height);
  // Can't use QWidget::adjustSize() here because it resizes to the sizeHint of
  // the widget. Instead we want to resize using the current size, just update the height.
  q->resize(newSize);
}


QWidgetList QtSlicerPrivate::layoutWidgets(QLayout* layout, int level)
{
  QWidgetList res;
  if (level == 0)
    {
    return res;
    }
  for (int i = 0; i < layout->count(); ++i)
    {
    QLayoutItem* item = layout->itemAt(i);
    if (item->widget())
      {
      res << item->widget();
      }
    if (item->layout())
      {
      res << layoutWidgets(item->layout(), level - 1);
      }
    }
  return res;
}


QtSlicer::QtSlicer(QWidget* parent, Qt::WindowFlags fl )
  : QDialog( parent, fl )
  , d_ptr(new QtSlicerPrivate(*this))
{
  Q_D(QtSlicer);
  d->setupUi(this);

  this->onDisplayStateChanged(d->OpenGlWindow->displayState());
  d->OpenGlWindow->setFocus();
}


QtSlicer::~QtSlicer()
{
}


QtGlSliceView* QtSlicer::sliceView()const
{
  Q_D(const QtSlicer);
  return d->OpenGlWindow;
}


void QtSlicer::setInputImage(ImageType* newImData)
{
  Q_D(QtSlicer);
  d->OpenGlWindow->setInputImage(newImData);
  /// \todo fire this signal from OpenGlWindow.
  this->updateSliceRange();
  d->OpenGlWindow->changeSlice((d->OpenGlWindow->maxSliceNum() - 1)/2);

  d->Controls->setInputImage();

  // Use adjustSize() instead of updateSize() because there is no valid prior
  // size.
  this->layout()->activate();
  this->adjustSize();
}


void QtSlicer::setOverlayImage(OverlayImageType* newImData)
{
  Q_D(QtSlicer);
  d->OpenGlWindow->setInputOverlay(newImData);
  //d->OpenGlWindow->update();
  d->updateSize();
}


bool QtSlicer::loadInputImage(QString filePathToLoad)
{
  Q_D(QtSlicer);

  ImageType::Pointer image = d->loadImage<double>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setInputImage( image );
    this->setWindowTitle(filePathToLoad);
    }
  return image.IsNotNull();
}


bool QtSlicer::loadOverlayImage(QString filePathToLoad)
{
  Q_D(QtSlicer);

  OverlayImageType::Pointer image = d->loadImage<OverlayPixelType>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setOverlayImage( image );
    }
  return image.IsNotNull();
}


void QtSlicer::showHelp()
{
  Q_D(QtSlicer);
  /// \todo don't show help in OpenGlWindow
  d->OpenGlWindow->showHelp();
  d->HelpDialog = d->OpenGlWindow->helpWindow();
}


void QtSlicer::onDisplayStateChanged(int state)
{
  Q_D(QtSlicer);
  if (state & QtSlicerPrivate::ON_COLLAPSE)
    {
    d->OpenGlWindow->setDisplayState(state | QtSlicerPrivate::ON_SLICEVIEW);
    return;
    }
  // Keep the OpenGlWindow size in the following.
  d->OpenGlWindow->setFixedSize(d->OpenGlWindow->geometry().size());
  const bool controlsVisible =
    !(state & QtSlicerPrivate::OFF_COLLAPSE) && !(state & QtSlicerPrivate::ON_COLLAPSE);
  d->Slider->setVisible(controlsVisible);
  d->Controls->setVisible(controlsVisible);
  d->Controls->setTextVisible(state & QtSlicerPrivate::ON_TEXTBOX);
  d->ButtonBoxWidget->setVisible(controlsVisible);
  qobject_cast<QGridLayout*>(this->layout())->setHorizontalSpacing(
    controlsVisible ? this->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing) : 0);
  qobject_cast<QGridLayout*>(this->layout())->setVerticalSpacing(
    controlsVisible ? this->style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing) : 0);
  d->updateSize();
  d->OpenGlWindow->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}


void QtSlicer::updateSliceRange()
{
  Q_D(QtSlicer);
  const int maximum = d->OpenGlWindow->maxSliceNum() - 1;
  d->SliceNumSlider->setMaximum(maximum);
  d->SliceValue->setMaximum(maximum);
  d->SliceNumSlider->setValue(d->SliceValue->value());
}


void QtSlicer::keyPressEvent(QKeyEvent* event)
{
  Q_D(QtSlicer);
  if (!d->IsRedirectingEvent)
    {
    d->IsRedirectingEvent = true;
    d->OpenGlWindow->keyPressEvent(event);
    d->IsRedirectingEvent = false;
    return;
    }
  this->Superclass::keyPressEvent(event);
}

bool QtSlicer::eventFilter(QObject *obj, QEvent *event)
{
  Q_D(QtSlicer);
  if (qobject_cast<QDoubleSpinBox*>(obj) &&
      event->type() == QEvent::KeyPress)
    {
    QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
    if ((keyEvent->key() < Qt::Key_0 ||
         keyEvent->key() > Qt::Key_9) &&
        keyEvent->key() != Qt::Key_Period &&
        keyEvent->key() != Qt::Key_Comma &&
        keyEvent->key() != Qt::Key_Minus &&
        keyEvent->key() != Qt::Key_Left &&
        keyEvent->key() != Qt::Key_Right &&
        keyEvent->key() != Qt::Key_Up &&
        keyEvent->key() != Qt::Key_Down &&
        keyEvent->key() != Qt::Key_PageUp &&
        keyEvent->key() != Qt::Key_PageDown &&
        keyEvent->key() != Qt::Key_Home &&
        keyEvent->key() != Qt::Key_End)
      {
      d->OpenGlWindow->keyPressEvent(keyEvent);
      return true;
      }
    }
  return this->Superclass::eventFilter(obj, event);
}
