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
#include <QTimer>

// QtImageViewer includes
#include "QtImageViewer.h"
#include "QtSliceControlsWidget.h"
#include "QtGlSliceView.h"
#include "ui_QtImageViewer.h"

// ITK includes
#include <itkImageFileReader.h>

// STD includes
#include <iostream>

class QtImageViewerPrivate: public Ui_QtImageViewer
{
  Q_DECLARE_PUBLIC(QtImageViewer);
public:
  typedef Ui_QtImageViewer Superclass;
  enum DisplayStates{
    OFF = 0x00,
    ON_SLICEVIEW = 0x01,
    ON_TEXTBOX = 0x02,
    OFF_COLLAPSE = 0x04,
    ON_COLLAPSE = 0x08
  };

  QtImageViewerPrivate(QtImageViewer& obj);

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
  QtImageViewer* const q_ptr;
};


QtImageViewerPrivate::QtImageViewerPrivate(QtImageViewer& obj)
  : HelpDialog(0)
  , IsRedirectingEvent(false)
  , q_ptr(&obj)
{
}


void QtImageViewerPrivate::setupUi(QDialog* widgetToSetup)
{
  Q_Q(QtImageViewer);
  this->Superclass::setupUi(widgetToSetup);

  this->Controls->setSliceView(this->OpenGlWindow);
  this->Controls->setSliceSlider(this->SliceSlider);
  this->Controls->setSliceSpinBox(this->SliceSpinBox);
  this->OpenGlWindow->setMaxDisplayStates(5);

  QObject::connect(this->ButtonBox, SIGNAL(accepted()),
                   q, SLOT(accept()));
  QObject::connect(this->ButtonBox, SIGNAL(rejected()),
                   q, SLOT(reject()));
  QObject::connect(this->ButtonBox, SIGNAL(helpRequested()),
                   q, SLOT(showHelp()));
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
typename itk::Image<PixelType, 3>::Pointer QtImageViewerPrivate
::loadImage(QString& filePath, const QString& imageType)
{
  Q_Q(QtImageViewer);
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
typename itk::Image<PixelType, 3>::Pointer QtImageViewerPrivate::readImage(const QString& filePath)
{
  Q_Q(QtImageViewer);
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

void QtImageViewerPrivate::updateSize()
{
  Q_Q(QtImageViewer);
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
  // Can't use QWidget::adjustSize() here because it resizes to the
  // sizeHint of the widget. Instead we want to resize using the current
  // size, just update the height.
  QSize newSize = QSize(width, height);
  q->resize(newSize);
}


QWidgetList QtImageViewerPrivate::layoutWidgets(QLayout* layout, int level)
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


QtImageViewer::QtImageViewer(QWidget* widgetParent, Qt::WindowFlags fl )
  : QDialog( widgetParent, fl )
  , d_ptr(new QtImageViewerPrivate(*this))
{
  Q_D(QtImageViewer);
  d->setupUi(this);

  this->onDisplayStateChanged(d->OpenGlWindow->displayState());
  d->OpenGlWindow->setFocus();
}


QtImageViewer::~QtImageViewer()
{
}


QtGlSliceView* QtImageViewer::sliceView()const
{
  Q_D(const QtImageViewer);
  return d->OpenGlWindow;
}


void QtImageViewer::setInputImage(ImageType* newImData)
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setInputImage(newImData);
  d->OpenGlWindow->changeSlice((d->OpenGlWindow->maxSliceNum() - 1)/2);

  // Use adjustSize() instead of updateSize() because there is no valid
  // prior size.
  this->layout()->activate();
  this->adjustSize();
  d->updateSize();
}


void QtImageViewer::setOverlayImage(OverlayImageType* newImData)
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setInputOverlay(newImData);
  d->updateSize();
}


bool QtImageViewer::loadInputImage(QString filePathToLoad)
{
  Q_D(QtImageViewer);

  ImageType::Pointer image = d->loadImage<double>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setInputImage( image );
    this->setWindowTitle(filePathToLoad);
    }
  return image.IsNotNull();
}


bool QtImageViewer::loadOverlayImage(QString filePathToLoad)
{
  Q_D(QtImageViewer);

  OverlayImageType::Pointer image = d->loadImage<OverlayPixelType>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setOverlayImage( image );
    }
  return image.IsNotNull();
}


void QtImageViewer::showHelp()
{
  Q_D(QtImageViewer);
  /// \todo don't show help in OpenGlWindow
  d->OpenGlWindow->showHelp();
  d->HelpDialog = d->OpenGlWindow->helpWindow();
}


void QtImageViewer::onDisplayStateChanged(int state)
{
  Q_D(QtImageViewer);
  if (state & QtImageViewerPrivate::ON_COLLAPSE)
    {
    d->OpenGlWindow->setDisplayState(state | QtImageViewerPrivate::ON_SLICEVIEW);
    return;
    }
  // Keep the OpenGlWindow size in the following.
  d->OpenGlWindow->setFixedSize(d->OpenGlWindow->geometry().size());
  /// \todo call releaseFixedSize() at the right time
  QTimer::singleShot(100, this, SLOT(releaseFixedSize()));

  d->Controls->setTextVisible(state & QtImageViewerPrivate::ON_TEXTBOX);
  const bool controlsVisible =
    !(state & QtImageViewerPrivate::OFF_COLLAPSE) && !(state & QtImageViewerPrivate::ON_COLLAPSE);
  this->setControlsVisible(controlsVisible);
}

void QtImageViewer::setControlsVisible(bool controlsVisible)
{
  Q_D(QtImageViewer);

  d->Slider->setVisible(controlsVisible);
  d->Controls->setVisible(controlsVisible);
  d->ButtonBoxWidget->setVisible(controlsVisible);
  qobject_cast<QGridLayout*>(this->layout())->setHorizontalSpacing(
    controlsVisible ? this->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing) : 0);
  qobject_cast<QGridLayout*>(this->layout())->setVerticalSpacing(
    controlsVisible ? this->style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing) : 0);
  d->updateSize();
}


void QtImageViewer::keyPressEvent(QKeyEvent* keyEvent)
{
  Q_D(QtImageViewer);
  if (keyEvent->key() != Qt::Key_Escape &&
      keyEvent->key() != Qt::Key_Enter &&
      keyEvent->key() != Qt::Key_Return)
    {
    if (!d->IsRedirectingEvent)
      {
      d->IsRedirectingEvent = true;
      d->OpenGlWindow->keyPressEvent(keyEvent);
      d->IsRedirectingEvent = false;
      return;
      }
    }
  this->Superclass::keyPressEvent(keyEvent);
}

bool QtImageViewer::eventFilter(QObject *obj, QEvent *filteredEvent)
{
  Q_D(QtImageViewer);
  if (qobject_cast<QDoubleSpinBox*>(obj) &&
      filteredEvent->type() == QEvent::KeyPress)
    {
    QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(filteredEvent);
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
  return this->Superclass::eventFilter(obj, filteredEvent);
}

void QtImageViewer::releaseFixedSize()
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}
