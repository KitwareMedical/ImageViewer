#include "QtSlicer.h"
#include <iostream>
#include "QtGlSliceView.h"
#include <qlineedit.h>
#include <qslider.h>


/**
 *
 */
QtSlicer::QtSlicer( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : Gui( parent, name, modal, fl )
{
}

/**  
 *  Destroys the object and frees any allocated resources
 */
QtSlicer::~QtSlicer()
{
}

void QtSlicer::DisplayPosition(int x,int y ,int z,float value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",x);
  PositionX->setText(tr(tempchar));
  sprintf(tempchar,"%d",y);
  PositionY->setText(tr(tempchar));
  sprintf(tempchar,"%d",z);
  PositionZ->setText(tr(tempchar));
  sprintf(tempchar,"%3.1f",value);
  PixelValue->setText(tr(tempchar));
  delete tempchar;
}

void QtSlicer::Help()
{
  HelpWindow* helpWindow = new HelpWindow(this,"Help ...");
  helpWindow->show();
}

void QtSlicer::SetInputImage(ImageType * newImData)
{
  this->OpenGlWindow->SetInputImage(newImData);
  this->Slider1->setMaxValue(newImData->GetLargestPossibleRegion().GetSize()[2]-1);

  // Set the slice slider at z/2
  this->Slider1->setValue(newImData->GetLargestPossibleRegion().GetSize()[2]/2);
  this->DisplaySliceNumber(newImData->GetLargestPossibleRegion().GetSize()[2]/2);

  this->IntensityMin->setMinValue( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMin->setMaxValue( static_cast<int>( this->OpenGlWindow->GetIntensityMax() ));
  this->IntensityMin->setValue( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMax->setMinValue( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMax->setMaxValue( static_cast<int>( this->OpenGlWindow->GetIntensityMax() ));
  this->IntensityMax->setValue( static_cast<int>( this->OpenGlWindow->GetIntensityMax() ));
  
  char* tempchar = new char[20];
  sprintf(tempchar,"%.0f",this->OpenGlWindow->GetIntensityMin());
  this->IntensityMinDisplay->setText(tempchar);
  sprintf(tempchar,"%.0f",this->OpenGlWindow->GetIntensityMax());
  this->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;

  this->OpenGlWindow->show();
  this->OpenGlWindow->update();
}

void QtSlicer::DisplaySliceNumber(int number)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",number);
  this->SliceValue->setText(tempchar);
  delete tempchar;
}

void QtSlicer::DisplayIMin(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  this->IntensityMinDisplay->setText(tempchar);
  delete tempchar;
}

void QtSlicer::DisplayIMax(int value)
{
  char* tempchar = new char[20];
  sprintf(tempchar,"%d",value);
  this->IntensityMaxDisplay->setText(tempchar);
  delete tempchar;
}
