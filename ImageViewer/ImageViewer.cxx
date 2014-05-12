#include "ImageViewerConfigure.h"
#ifdef USE_CLI
#include "ImageViewerCLP.h"
#endif
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


int execImageViewer(int argc, char* argv[])
{
  QApplication myApp( argc, argv );

  QtSlicer qtSlicerWindow( 0 );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;
  typedef itk::ImageFileReader<ImageType>   ReaderType;
  QString filePathToLoad;
  qtSlicerWindow.loadInputImage(filePathToLoad);

  int execReturn;
  try
    {
    execReturn = myApp.exec();
    }
  catch (itk::ExceptionObject & e)
    {
    qWarning()<< "Exception during GUI execution"<< e.GetNameOfClass();
    return EXIT_FAILURE;
    }
  return execReturn;
}

int parseAndExecImageViewer(int argc, char* argv[])
{
#ifdef USE_CLI
  PARSE_ARGS;
#endif

  QApplication myApp(argc, argv);

  QtSlicer qtSlicerWindow( 0 );

  typedef double                            PixelType;
  typedef itk::Image<PixelType, 3>          ImageType;

  QString filePathToLoad;
#ifdef USE_CLI
  filePathToLoad = QString::fromStdString(inputImage);
#endif

  qtSlicerWindow.loadInputImage(filePathToLoad);

#ifdef USE_CLI
  if(!overlayImage.empty())
    {
    qtSlicerWindow.loadOverlayImage(QString::fromStdString(overlayImage));
    }
  qtSlicerWindow.OpenGlWindow->setOrientation(orientation);
  qtSlicerWindow.OpenGlWindow->setSliceNum(sliceOffset);
  qtSlicerWindow.OpenGlWindow->setMaxIntensity(maxIntensity);
  qtSlicerWindow.OpenGlWindow->setMinIntensity(minIntensity);
  qtSlicerWindow.OpenGlWindow->setZoom(zoom);
  qtSlicerWindow.OpenGlWindow->transpose(transpose);
  qtSlicerWindow.OpenGlWindow->flipZ(zFlipped);
  qtSlicerWindow.OpenGlWindow->flipY(yFlipped);
  qtSlicerWindow.OpenGlWindow->flipX(xFlipped);
  qtSlicerWindow.OpenGlWindow->setOverlayOpacity(overlayOpacity);
  qtSlicerWindow.OpenGlWindow->setViewCrosshairs(crosshairs);
  qtSlicerWindow.OpenGlWindow->setViewDetails(details);
  qtSlicerWindow.OpenGlWindow->setViewValuePhysicalUnits(physicalUnits);
  qtSlicerWindow.OpenGlWindow->setViewValue(value);
  qtSlicerWindow.OpenGlWindow->setViewAxisLabel(axisLabel);
  qtSlicerWindow.OpenGlWindow->setViewClickedPoints(clickedPoints);
  qtSlicerWindow.OpenGlWindow->setImageMode(imageMode.c_str());
  qtSlicerWindow.OpenGlWindow->setIWModeMax(iwModeMax.c_str());
  qtSlicerWindow.OpenGlWindow->setIWModeMin(iwModeMin.c_str());

  qtSlicerWindow.OpenGlWindow->update();
#endif

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

int main( int argc, char* argv[] ) 
{
  QFileInfo info(argv[0]);
  if(argc == 1 || !info.isFile() )
    {
    return execImageViewer(argc, argv);
    }
  if(argc >1 && info.isFile())
    {
    return parseAndExecImageViewer(argc, argv);
    }
  return 0;
}
