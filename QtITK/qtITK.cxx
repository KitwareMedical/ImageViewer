
#include <qapplication.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qvbox.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"

#include "itkQtAdaptor.h"


int main(int argc, char **argv)
{

  typedef itk::Image< float, 2 > ImageType;

  typedef itk::DiscreteGaussianImageFilter< 
                    ImageType,
                    ImageType >  FilterType;

  typedef itk::ImageFileReader< ImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  
  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );

  // Create Qt Application to let Qt get its 
  // parameters from the command line
  QApplication app( argc, argv );

  reader->SetFileName( argv[1] );

  QVBox qb;
  qb.resize(120,200);

  QPushButton  bb( "Start", &qb );
  bb.resize( 100, 30 );

  QPushButton  cc( "State", &qb );
  cc.resize( 100, 30 );

  QPushButton  qt( "Quit", &qb );
  qt.resize( 100, 30 );

  QSlider qs( QSlider::Horizontal, &qb, "Progress");
  qs.resize( 100, 30 );
  qs.setRange(0,100);
  qs.setValue(0);
  

  typedef itk::QtSlotAdaptor<FilterType> SlotAdaptorType;
  SlotAdaptorType slotAdaptor;

  // Connect the adaptor to a method of the ITK filter
  slotAdaptor.SetCallbackFunction( filter, & FilterType::Update );

  // Connect the adaptor's Slot to the Qt Widget Signal
  QObject::connect( &bb, SIGNAL(clicked()), &slotAdaptor, SLOT(Slot()) );


  typedef itk::QtSignalAdaptor SignalAdaptorType;
  SignalAdaptorType signalAdaptor;

  // Connect the adaptor as an observer of a Filter's event
  filter->AddObserver( itk::StartEvent(),  signalAdaptor.GetCommand() );

  // Connect the adaptor's Signal to the Qt Widget Slot
  QObject::connect( &signalAdaptor, SIGNAL(Signal()), &cc, SLOT(toggle()) );



  // Connect the Quit button signal to the quit slot of the application
  QObject::connect( &qt, SIGNAL(clicked()), &app, SLOT(quit()) );




  app.setMainWidget( &qb );
  qb.show();

  return app.exec();

}



