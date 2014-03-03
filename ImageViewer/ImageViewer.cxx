//Qt includes
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QPlastiqueStyle>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "QtSlicer.h"

// itk includes
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkMetaImageIOFactory.h"


int main( int argc, char* argv[] ) 
{

  QApplication myApp( argc, argv );

  QtSlicer qtSlicerWindow( 0, 0, TRUE );
  //myApp.setMainWidget(&m_GUI);

  qtSlicerWindow.setWindowTitle("Insight Qt Slicer" );
  myApp.setStyle(new QPlastiqueStyle );
  QPalette p( QColor( 239, 239, 239 ) );
  myApp.setPalette( p );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  
  QString filePathToLoad = QFileDialog::getOpenFileName(&qtSlicerWindow,"", QDir::currentPath());
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
  qtSlicerWindow.setInputImage( reader->GetOutput() );

  qtSlicerWindow.show();
  int execReturn;
  try
    {
    execReturn = myApp.exec();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "Exception during GUI execution" << std::endl;
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
 
  return execReturn;

}

