//Qt includes
#include <QDebug>
#include <QLineEdit>
#include <QSlider>

//QtImageViewer includes
#include "QtSlicer.h"
#include "QtGlSliceView.h"

#include <iostream>


/**
 *
 */
QtSlicer::QtSlicer( QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl ) :
  QDialog( parent, fl )
{
  this->setupUi(this);
  QObject::connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(ButtonHelp, SIGNAL(clicked()), OpenGlWindow, SLOT(showHelp()));
  QObject::connect(SliceNumSlider, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(changeSlice(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), SliceNumSlider, SLOT(setValue(int)));
  QObject::connect(OpenGlWindow, SIGNAL(positionChanged(int,int,int,double)), this, SLOT(setDisplayPosition(int,int,int,double)));
  QObject::connect(IntensityMax, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(setMaxIntensity(int)));
  QObject::connect(IntensityMin, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(setMinIntensity(int)));
  QObject::connect(ZoomIn, SIGNAL(clicked()), OpenGlWindow, SLOT(zoomIn()));
  QObject::connect(ZoomOut, SIGNAL(clicked()), OpenGlWindow, SLOT(zoomOut()));
  QObject::connect(SliceNumSlider, SIGNAL(sliderMoved(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(maxIntensityChanged(int)), this, SLOT(setDisplayIMax(int)));
  QObject::connect(OpenGlWindow, SIGNAL(minIntensityChanged(int)), this, SLOT(setDisplayIMin(int)));
  QObject::connect(OpenGlWindow, SIGNAL(maxIntensityChanged(int)), IntensityMax, SLOT(setValue(int)));
  QObject::connect(OpenGlWindow, SIGNAL(minIntensityChanged(int)), IntensityMin, SLOT(setValue(int)));
  QObject::connect(OpenGlWindow, SIGNAL(updateDetails(QString)), Details, SLOT(setText(QString)));
  QObject::connect(OpenGlWindow, SIGNAL(orientationChanged(int)), this, SLOT(setMaximumSlice()));
}

/**  
 *  Destroys the object and frees any allocated resources
 */
QtSlicer::~QtSlicer()
{
}

void QtSlicer::setDisplayPosition(int x,int y ,int z,double value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",x);
  this->PositionX->setText(tr(tempchar));
  sprintf(tempchar,"%d",y);
  this->PositionY->setText(tr(tempchar));
  sprintf(tempchar,"%d",z);
  this->PositionZ->setText(tr(tempchar));
  sprintf(tempchar,"%3.1f",value);
  this->PixelValue->setText(tr(tempchar));
  this->OpenGlWindow->update();
  delete tempchar;
}


void QtSlicer::setInputImage(ImageType * newImData)
{
  this->OpenGlWindow->setInputImage(newImData);
  this->SliceNumSlider->setMaximum( static_cast<int>(this->OpenGlWindow->maxSliceNum() -1));

  this->OpenGlWindow->changeSlice(((this->OpenGlWindow->maxSliceNum() -1)/2));
  this->SliceNumSlider->setValue(static_cast<int>((this->OpenGlWindow->maxSliceNum() -1)/2));
  this->setDisplaySliceNumber(static_cast<int>((this->OpenGlWindow->maxSliceNum() -1)/2));

  this->IntensityMin->setMinimum( static_cast<int>( this->OpenGlWindow->minIntensity() ));
  this->IntensityMin->setMaximum( static_cast<int>( this->OpenGlWindow->maxIntensity() ));
  this->IntensityMin->setValue( static_cast<int>( this->OpenGlWindow->minIntensity() ));
  this->IntensityMax->setMinimum( static_cast<int>( this->OpenGlWindow->minIntensity() ));
  this->IntensityMax->setMaximum( static_cast<int>( this->OpenGlWindow->maxIntensity() ));
  this->IntensityMax->setValue( static_cast<int>( this->OpenGlWindow->maxIntensity() ));
  
  char* tempchar = new char[20];
  sprintf(tempchar,"%.0f",this->OpenGlWindow->minIntensity());
  this->IntensityMinDisplay->setText(tempchar);
  sprintf(tempchar,"%.0f",this->OpenGlWindow->maxIntensity());
  this->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;

  this->OpenGlWindow->show();
  this->OpenGlWindow->update();
}


void QtSlicer::setMaximumSlice()
{
  this->SliceNumSlider->setMaximum(static_cast<int>(this->OpenGlWindow->maxSliceNum() -1));
  this->SliceNumSlider->setValue(static_cast<int>(this->SliceValue->text().toInt()));
}

void QtSlicer::setDisplaySliceNumber(int number)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",number);
  number = this->SliceNumSlider->value();
  this->SliceValue->setText(tempchar);
  delete tempchar;
}

void QtSlicer::setDisplayIMin(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  this->IntensityMinDisplay->setText(tempchar);
  delete tempchar;
}

void QtSlicer::setDisplayIMax(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  this->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;
}
