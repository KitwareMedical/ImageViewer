
#include <qapplication.h>
#include <qpushbutton.h>

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

  QPushButton  bb( "Start", 0 );
  bb.resize( 100, 30 );

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
//  QObject::connect( &signalAdaptor, SIGNAL(Signal()), &bb, SLOT(Slot()) );





  app.setMainWidget( &bb );
  bb.show();

  return app.exec();

}



