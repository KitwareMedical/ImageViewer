
#include <qapplication.h>
#include <qpushbutton.h>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"

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

//  QObject::connect( &bb, SIGNAL(clicked()), &a, SLOT(quit()) );

  app.setMainWidget( &bb );
  bb.show();

  return app.exec();

}
