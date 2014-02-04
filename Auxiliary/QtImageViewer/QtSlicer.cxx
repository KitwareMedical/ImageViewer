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
  QObject::connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(buttonHelp, SIGNAL(clicked()), OpenGlWindow, SLOT(Help()));
  QObject::connect(Slider1, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(ChangeSlice(int)));
  QObject::connect(OpenGlWindow, SIGNAL(Position(int,int,int,float)), this, SLOT(DisplayPosition(int,int,int,float)));
  QObject::connect(IntensityMax, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(IntensityMax(int)));
  QObject::connect(IntensityMin, SIGNAL(sliderMoved(int)), OpenGlWindow, SLOT(IntensityMin(int)));
  QObject::connect(ZoomIn, SIGNAL(clicked()), OpenGlWindow, SLOT(ZoomIn()));
  QObject::connect(ZoomOut, SIGNAL(clicked()), OpenGlWindow, SLOT(ZoomOut()));
  QObject::connect(Slider1, SIGNAL(sliderMoved(int)), this, SLOT(DisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(SliceChanged(int)), this, SLOT(DisplaySliceNumber(int)));
  QObject::connect(OpenGlWindow, SIGNAL(IntensityMaxChanged(int)), this, SLOT(DisplayIMax(int)));
  QObject::connect(OpenGlWindow, SIGNAL(IntensityMinChanged(int)), this, SLOT(DisplayIMin(int)));
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

//void QtSlicer::Help()
//{
//  QDialog* helpDialog = new QDialog(this);
//  helpWindow = new Ui::HelpWindow;
//  helpWindow->setupUi(helpDialog);
//  helpDialog->show();
//}

void QtSlicer::SetInputImage(ImageType * newImData)
{
  this->OpenGlWindow->SetInputImage(newImData);
  this->Slider1->setMaximum(newImData->GetLargestPossibleRegion().GetSize()[2]-1);

  // Set the slice slider at z/2
  this->Slider1->setValue(newImData->GetLargestPossibleRegion().GetSize()[2]/2);
  this->DisplaySliceNumber(newImData->GetLargestPossibleRegion().GetSize()[2]/2);

  this->IntensityMin->setMinimum( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMin->setMaximum( static_cast<int>( this->OpenGlWindow->GetIntensityMax() ));
  this->IntensityMin->setValue( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMax->setMinimum( static_cast<int>( this->OpenGlWindow->GetIntensityMin() ));
  this->IntensityMax->setMaximum( static_cast<int>( this->OpenGlWindow->GetIntensityMax() ));
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
  number = this->Slider1->value();
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
