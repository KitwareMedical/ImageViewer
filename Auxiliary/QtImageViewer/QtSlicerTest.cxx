#include <qapplication.h>
#include "QtGlSliceView.h"
#include "QtSlicer.h"
#include <qfiledialog.h>
#include <qslider.h>

#include "itkImageFileReader.h"
#include "itkImage.h"
#include "itkMetaImageIOFactory.h"
#include "itkImageRegionIteratorWithIndex.h"

#include <qwindowsstyle.h>
#include <qplatinumstyle.h>
#include <qmotifstyle.h>
#include <qmotifplusstyle.h>
#include <qcdestyle.h>

int main( int argc, char* argv[] ) 
{

  QApplication myApp( argc, argv );

  QtSlicer m_GUI( 0, 0, TRUE );
  myApp.setMainWidget(&m_GUI);

  m_GUI.setCaption( "Insight Qt Slicer" );
  myApp.setStyle( new QPlatinumStyle );
  QPalette p( QColor( 239, 239, 239 ) );
  myApp.setPalette( p, TRUE );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  
  QString s = QFileDialog::getOpenFileName(".","Images (*.mha)", 0, "open file dialog","Chose an image filename" );

  reader->SetFileName( s.latin1() );
  
  std::cout << "loading image " << s.latin1() << " ... ";
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception in file reader " << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  std::cout << "Done!" << std::endl;
  m_GUI.SetInputImage( reader->GetOutput() );
  
  try
    {
    m_GUI.exec();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  return 0;

}

