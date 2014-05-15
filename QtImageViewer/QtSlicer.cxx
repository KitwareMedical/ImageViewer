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
//Qt includes
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QSlider>

//QtImageViewer includes
#include "QtSlicer.h"
#include "QtSliceControlsWidget.h"
#include "QtGlSliceView.h"

#include <iostream>


QtSlicer::QtSlicer(QWidget* parent, Qt::WindowFlags fl ) :
  QDialog( parent, fl )
{
  this->setupUi(this);
  this->Controls->setSliceView(this->OpenGlWindow);
  this->HelpDialog = 0;
  QObject::connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(ButtonHelp, SIGNAL(toggled(bool)), this, SLOT(showHelp(bool)));
  QObject::connect(SliceNumSlider, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(changeSlice(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), SliceNumSlider, SLOT(setValue(int)));
  QObject::connect(SliceNumSlider, SIGNAL(sliderMoved(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(orientationChanged(int)), this, SLOT(updateSliceMaximum()));
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


void QtSlicer::setInputImage(ImageType * newImData)
{
  this->OpenGlWindow->setInputImage(newImData);
  updateSliceMaximum();
  this->setDisplaySliceNumber(static_cast<int>((this->OpenGlWindow->maxSliceNum() -1)/2));

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
  reader->SetFileName( filePathToLoad.toLatin1().data() );

  QFileInfo filePath(filePathToLoad);
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
    return EXIT_FAILURE;
    }
  this->setInputImage( reader->GetOutput() );
  show();
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
    return EXIT_FAILURE;
    }
  this->setOverlayImage(overlayReader->GetOutput());

  show();
  return true;
}
void QtSlicer::updateSliceMaximum()
{
  this->SliceNumSlider->setMaximum(static_cast<int>(this->OpenGlWindow->maxSliceNum() -1));
  this->SliceNumSlider->setValue(static_cast<int>(this->SliceValue->text().toInt()));
}

void QtSlicer::setDisplaySliceNumber(int number)
{
  QString tempchar = QString::number(number);
  this->SliceValue->setText(tempchar);
}
