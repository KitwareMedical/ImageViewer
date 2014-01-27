#include <QApplication>
#include "QtGlSliceView.h"
#include "QtSlicer.h"
#include <QFileDialog>
//#include <qslider.h>

#include "itkImageFileReader.h"
#include "itkImage.h"
#include "itkMetaImageIOFactory.h"
#include "itkImageRegionIteratorWithIndex.h"

#include <QDebug>
#include <QPlastiqueStyle>
#include <qwindowsstyle.h>
//#include <qplatinumstyle.h>
#include <qmotifstyle.h>
//#include <qmotifplusstyle.h>
#include <qcdestyle.h>

int main( int argc, char* argv[] ) 
{

  QApplication myApp( argc, argv );

  QtSlicer m_GUI( 0, 0, TRUE );
  //myApp.setMainWidget(&m_GUI);

  m_GUI.setWindowTitle("Insight Qt Slicer" );
  myApp.setStyle(new QPlastiqueStyle );
  QPalette p( QColor( 239, 239, 239 ) );
  myApp.setPalette( p );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  
  QString filePathToLoad = QFileDialog::getOpenFileName(&m_GUI,"", QDir::currentPath());
  //"Images (*.mha)", 0, "open file dialog","Chose an image filename" );

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
    std::cerr << "Exception in file reader " << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  std::cout << "Done!" << std::endl;
  m_GUI.SetInputImage( reader->GetOutput() );

  m_GUI.show();
  
  try
    {
    myApp.exec();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  return 0;

}

