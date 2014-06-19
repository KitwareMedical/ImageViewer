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
  :QDialog( parent, fl )
{
  this->setupUi(this);
  this->Controls->setSliceView(this->OpenGlWindow);
  this->OpenGlWindow->setMaxDisplayStates(5);
  this->HelpDialog = 0;
  this->IsRedirectingEvent = false;
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
  const bool controlsVisible = !(state & OFF_COLLAPSE) && !(state & ON_COLLAPSE);
  this->Buttons->setVisible(controlsVisible);
  this->Slider->setVisible(controlsVisible);
  this->Controls->setVisible(controlsVisible);
  this->Controls->setTextVisible(state & ON_TEXTBOX);
}


void QtSlicer::setInputImage(ImageType * newImData)
{
  this->OpenGlWindow->setInputImage(newImData);
  this->updateSliceMaximum();
  this->OpenGlWindow->changeSlice((this->OpenGlWindow->maxSliceNum() - 1)/2);

  this->Controls->setInputImage();

  this->OpenGlWindow->show();
  this->OpenGlWindow->update();
}


void QtSlicer::setOverlayImage(OverlayType * newImData)
{
  this->OpenGlWindow->setInputOverlay(newImData);
  this->OpenGlWindow->show();
  this->OpenGlWindow->update();
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
