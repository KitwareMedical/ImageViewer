/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    qtITK.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include <qapplication.h>
#include <qpushbutton.h>
#include <qfiledialog.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"

#include "itkQtAdaptor.h"
#include "itkQtLightIndicator.h"
#include "itkQtProgressBar.h"


int main(int argc, char **argv)
{

  typedef itk::Image< float, 2 > ImageType;

  typedef itk::AddImageFilter< 
                    ImageType,
                    ImageType,
                    ImageType >  FilterType;

  typedef itk::ImageFileReader< ImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput1( reader->GetOutput() );
  filter->SetInput2( reader->GetOutput() );

  // Create Qt Application to let Qt get its 
  // parameters from the command line
  QApplication app( argc, argv );

  QWidget qb;
  qb.resize(620,100);

  const int buttonHeight =  30;
  const int buttonWidth  = 100;
  const int buttonSpace  = 100;

  int horizontalPosition =  60;

  QPushButton  bb( "Start", &qb );
  bb.setGeometry( horizontalPosition, 20, buttonWidth, buttonHeight );

  horizontalPosition += buttonWidth + buttonSpace;

  itk::QtLightIndicator  cc( &qb, "State" );
  cc.setGeometry( horizontalPosition, 20, buttonWidth, buttonHeight );
  cc.Modified();

  horizontalPosition += buttonWidth + buttonSpace;

  QPushButton  qt( "Quit", &qb );
  qt.setGeometry( horizontalPosition, 20, buttonWidth, buttonHeight );

  itk::QtProgressBar qs( &qb, "Progress");
  qs.setGeometry( 10, 60, 600, 30 );
  


  // Connect the progress bar to the ITK processObject
  qs.Observe( filter.GetPointer() );
  qs.Observe( reader.GetPointer() );

  typedef itk::QtSlotAdaptor<FilterType> SlotAdaptorType;
  SlotAdaptorType slotAdaptor;

  // Connect the adaptor to a method of the ITK filter
  slotAdaptor.SetCallbackFunction( filter, & FilterType::Update );

  // Connect the adaptor's Slot to the Qt Widget Signal
  QObject::connect( &bb, SIGNAL(clicked()), &slotAdaptor, SLOT(Slot()) );






  // One signal adaptor for observing the StartEvent()
  typedef itk::QtSignalAdaptor SignalAdaptorType;
  SignalAdaptorType signalAdaptor1;

  // Connect the adaptor as an observer of a Filter's event
  filter->AddObserver( itk::StartEvent(),  signalAdaptor1.GetCommand() );

  // Connect the adaptor's Signal to the Qt Widget Slot
  QObject::connect( &signalAdaptor1, SIGNAL(Signal()), &cc, SLOT(Start()) );






  // One signal adaptor for observing the ModifiedEvent()
  typedef itk::QtSignalAdaptor SignalAdaptorType;
  SignalAdaptorType signalAdaptor2;

  // Connect the adaptor as an observer of a Filter's event
  filter->AddObserver( itk::ModifiedEvent(),  signalAdaptor2.GetCommand() );

  // Connect the adaptor's Signal to the Qt Widget Slot
  QObject::connect( &signalAdaptor2, SIGNAL(Signal()), &cc, SLOT(Modified()) );





  // One signal adaptor for observing the EndEvent()
  typedef itk::QtSignalAdaptor SignalAdaptorType;
  SignalAdaptorType signalAdaptor3;

  // Connect the adaptor as an observer of a Filter's event
  filter->AddObserver( itk::EndEvent(),  signalAdaptor3.GetCommand() );

  // Connect the adaptor's Signal to the Qt Widget Slot
  QObject::connect( &signalAdaptor3, SIGNAL(Signal()), &cc, SLOT(End()) );





  // Connect the Quit button signal to the quit slot of the application
  QObject::connect( &qt, SIGNAL(clicked()), &app, SLOT(quit()) );


  app.setMainWidget( &qb );
  qb.show();

  QString s = QFileDialog::getOpenFileName(".","Images (*.png *.mha)", 0, "open file dialog","Chose an image filename" );

  reader->SetFileName( s.latin1() );

  return app.exec();

}



