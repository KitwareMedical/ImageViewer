#include "QtSlicer.h"
#include <iostream>
#include "QtGlSliceView.h"
#include <qlineedit.h>
#include <qslider.h>


/**
 *
 */
QtSlicer::QtSlicer( QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl ) :
  QDialog( parent, fl )
{
  this->setupUi(this);
  QObject::connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(ButtonHelp, SIGNAL(clicked()), OpenGlWindow, SLOT(showHelp()));
  QObject::connect(Slider1, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(changeSlice(int)));
  QObject::connect(OpenGlWindow, SIGNAL(positionChanged(int,int,int,double)), this, SLOT(setDisplayPosition(int,int,int,double)));
  QObject::connect(IntensityMax, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(setMaxIntensity(int)));
  QObject::connect(IntensityMin, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(setMinIntensity(int)));
  QObject::connect(ZoomIn, SIGNAL(clicked()), OpenGlWindow, SLOT(zoomIn()));
  QObject::connect(ZoomOut, SIGNAL(clicked()), OpenGlWindow, SLOT(zoomOut()));
  QObject::connect(Slider1, SIGNAL(sliderMoved(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(sliceNumChanged(int)), this, SLOT(setDisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(maxIntensityChanged(int)), this, SLOT(setDisplayIMax(int)));
  QObject::connect(OpenGlWindow, SIGNAL(maxIntensityChanged(int)), this, SLOT(setDisplayIMin(int)));
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
  this->Slider1->setMaximum(newImData->GetLargestPossibleRegion().GetSize()[2]-1);

  // Set the slice slider at z/2
  this->Slider1->setValue(newImData->GetLargestPossibleRegion().GetSize()[2]/2);
  this->setDisplaySliceNumber(newImData->GetLargestPossibleRegion().GetSize()[2]/2);

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

void QtSlicer::setDisplaySliceNumber(int number)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",number);
  number = this->Slider1->value();
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
