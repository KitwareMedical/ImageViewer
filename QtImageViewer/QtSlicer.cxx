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
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QSlider>

// QtImageViewer includes
#include "QtSlicer.h"
#include "QtSliceControlsWidget.h"
#include "QtGlSliceView.h"
#include "ui_QtSlicerHelpGUI.h"

// STD includes
#include <iostream>

QtSlicer::QtSlicer(QWidget* parent, Qt::WindowFlags fl )
  : QDialog( parent, fl )
{
  this->HelpDialog = 0;
  this->IsRedirectingEvent = false;

  this->setupUi(this);
  this->Controls->setSliceView(this->OpenGlWindow);
  this->OpenGlWindow->setMaxDisplayStates(5);
  QObject::connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(ButtonHelp, SIGNAL(toggled(bool)), this, SLOT(showHelp(bool)));
  QObject::connect(SliceNumSlider, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(changeSlice(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), SliceNumSlider, SLOT(setValue(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), SliceValue, SLOT(setValue(int)));
  QObject::connect(OpenGlWindow, SIGNAL(orientationChanged(int)), this, SLOT(updateSliceMaximum()));
  QObject::connect(OpenGlWindow, SIGNAL(displayStateChanged(int)), this, SLOT(setDisplayState(int)));

  // Install event filter on all the double spinboxes. They eat letter key
  // events for no reason
  foreach(QDoubleSpinBox* spinBox, this->findChildren<QDoubleSpinBox*>())
    {
    spinBox->installEventFilter(this);
    }

  this->setDisplayState(this->OpenGlWindow->displayState());
  this->OpenGlWindow->setFocus();
}


QtSlicer::~QtSlicer()
{
}


void QtSlicer::showHelp(bool checked)
{
  if(!checked && this->HelpDialog != 0)
    {
    this->HelpDialog->reject();
    }
  else
    {
    this->OpenGlWindow->showHelp();
    this->HelpDialog = this->OpenGlWindow->helpWindow();
    if(this->HelpDialog != 0)
      {
      QObject::connect(HelpDialog, SIGNAL(rejected()), this,
                       SLOT(hideHelp()),Qt::UniqueConnection);
      }
    }
}


void QtSlicer::hideHelp()
{
  this->ButtonHelp->setChecked(false);
}


void QtSlicer::setDisplayState(int state)
{
  if (state & ON_COLLAPSE)
    {
    this->OpenGlWindow->setDisplayState(state | ON_SLICEVIEW);
    return;
    }
  // Keep the OpenGlWindow size in the following.
  this->OpenGlWindow->setFixedSize(this->OpenGlWindow->geometry().size());
  const bool controlsVisible = !(state & OFF_COLLAPSE) && !(state & ON_COLLAPSE);
  this->Buttons->setVisible(controlsVisible);
  this->Slider->setVisible(controlsVisible);
  this->Controls->setVisible(controlsVisible);
  this->Controls->setTextVisible(state & ON_TEXTBOX);
  this->updateSize();
  this->OpenGlWindow->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}


void QtSlicer::setInputImage(ImageType * newImData)
{
  this->OpenGlWindow->setInputImage(newImData);
  this->updateSliceMaximum();
  this->OpenGlWindow->changeSlice((this->OpenGlWindow->maxSliceNum() - 1)/2);

  this->Controls->setInputImage();

  this->OpenGlWindow->update();
  // Use adjustSize() instead of updateSize() because there is no prior size
  // that is valid.
  this->adjustSize();
}


void QtSlicer::setOverlayImage(OverlayType * newImData)
{
  this->OpenGlWindow->setInputOverlay(newImData);
  this->OpenGlWindow->update();
  this->updateSize();
}


bool QtSlicer::loadInputImage(QString filePathToLoad)
{
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  if( filePathToLoad.isEmpty() )
    {
    filePathToLoad = QFileDialog::getOpenFileName(
        0,"", QDir::currentPath());
    }

  if(filePathToLoad.isEmpty())
    {
    return false;
    }
  QFileInfo filePath(filePathToLoad);
  if(!filePath.exists())
    {
    const QString title("Failed loading");
    QString str;
    str = QString("The file you have selected does not exist. %1").arg(filePathToLoad);
    QMessageBox::warning(this, title, str);
    return false;
    }
  reader->SetFileName( filePathToLoad.toLatin1().data() );
  setWindowTitle(filePath.fileName());
  qDebug() << "loading image " << filePathToLoad << " ... ";
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    const QString title("Failed to read image.");
    QString str;
    str = QString("%1").arg(filePathToLoad);
    QMessageBox::warning(this, title, str);
    return false;
    }
  this->setInputImage( reader->GetOutput() );
  return true;
}

bool QtSlicer::loadOverlayImage(QString overlayImagePath)
{
  typedef itk::ImageFileReader<OverlayType> OverlayReaderType;
  OverlayReaderType::Pointer overlayReader = OverlayReaderType::New();

  if(overlayImagePath.isEmpty())
    {
    overlayImagePath = QFileDialog::getOpenFileName(
            0,"", QDir::currentPath());
    }
  if(overlayImagePath.isEmpty())
    {
    return false;
    }
  QFileInfo filePath(overlayImagePath);
  if(!filePath.exists())
    {
    const QString title("Failed loading");
    QString str;
    str = QString("The file you have selected does not exist. %1").arg(overlayImagePath);
    QMessageBox::warning(this, title, str);
    return false;
    }
  overlayReader->SetFileName( overlayImagePath.toLatin1().data() );

  qDebug() << "loading image " << overlayImagePath << " ... ";
  try
    {
    overlayReader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    const QString title("Failed to read image.");
    QString str;
    str = QString("%1").arg(overlayImagePath);
    QMessageBox::warning(this, title, str);
    return false;
    }
  this->setOverlayImage(overlayReader->GetOutput());
  return true;
}

QWidgetList layoutWidgets(QLayout* layout, int level = -1)
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


void QtSlicer::updateSize()
{
  this->updateGeometry();

  QWidgetList childWidgets = layoutWidgets(this->layout(), 2);
  // Find the true width of the widget. Discard the children that are hidden.
  // We do not need the height information.
  QRect geom;
  foreach(QWidget* child, childWidgets)
    {
    if (child->isVisibleTo(this))
      {
      QPoint childPos = child->mapTo(this, QPoint(0,0));
      int width = child->width() ? child->width() : child->sizeHint().width();
      int height = child->height() ? child->height() : child->sizeHint().height();
      geom = geom.united(QRect(pos, QSize(width, height)));
      }
    }
  int width = geom.width();
  int height = geom.height();
  if (this->Controls->isVisibleTo(this))
    {
    height = this->heightForWidth(width);
    }
  else
    {
    height = this->OpenGlWindow->heightForWidth(width);
    }
  QSize newSize = QSize(width, height);
  // Can't use QWidget::adjustSize() here because it resizes to the sizeHint of
  // the widget. Instead we want to resize using the current size, just update the height.
  this->resize(newSize);
}


void QtSlicer::updateSliceMaximum()
{
  const int maximum = this->OpenGlWindow->maxSliceNum() - 1;
  this->SliceNumSlider->setMaximum(maximum);
  this->SliceValue->setMaximum(maximum);
  this->SliceNumSlider->setValue(this->SliceValue->value());
}

void QtSlicer::keyPressEvent(QKeyEvent* event)
{
  if (!this->IsRedirectingEvent)
    {
    this->IsRedirectingEvent = true;
    this->OpenGlWindow->keyPressEvent(event);
    this->IsRedirectingEvent = false;
    return;
    }
  this->Superclass::keyPressEvent(event);
}

bool QtSlicer::eventFilter(QObject *obj, QEvent *event)
{
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
      this->OpenGlWindow->keyPressEvent(keyEvent);
      return true;
      }
    }
  return this->Superclass::eventFilter(obj, event);
}
