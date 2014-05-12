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
  if( filePathToLoad.isNull() )
    {
    filePathToLoad = QFileDialog::getOpenFileName(
        0,"", QDir::currentPath());
    }

  if(filePathToLoad.isEmpty())
    {
    return 0;
    }
  reader->SetFileName( filePathToLoad.toLatin1().data() );

  qDebug() << "loading image " << filePathToLoad << " ... ";
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    qWarning()<< "Exception during GUI execution"<< e.GetNameOfClass();
    return EXIT_FAILURE;
    }
  this->OpenGlWindow->setInputImage( reader->GetOutput() );
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
    qWarning()<< "Exception during GUI execution"<< e.GetNameOfClass();
    return EXIT_FAILURE;
    }
  this->OpenGlWindow->setInputOverlay(overlayReader->GetOutput());

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
