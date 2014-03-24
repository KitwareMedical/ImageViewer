//Qt includes
#include <QDebug>
#include <QLineEdit>
#include <QSlider>

//QtImageViewer includes
#include "QtSlicer.h"
#include "QtSliceControlsWidget.h"
#include "QtGlSliceView.h"

#include <iostream>


QtSlicer::QtSlicer( QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl ) :
  QDialog( parent, fl )
{
  this->setupUi(this);
  this->Controls->setSliceView(this->OpenGlWindow);
  QObject::connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(ButtonHelp, SIGNAL(clicked()), OpenGlWindow, SLOT(showHelp()));
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


void QtSlicer::setInputImage(ImageType * newImData)
{
  this->OpenGlWindow->setInputImage(newImData);
  updateSliceMaximum();
  this->setDisplaySliceNumber(static_cast<int>((this->OpenGlWindow->maxSliceNum() -1)/2));

  this->Controls->setInputImage();

  this->OpenGlWindow->show();
  this->OpenGlWindow->update();
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
