/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#include "ImageViewerConfigure.h"
#ifdef USE_CLI
#include "ImageViewerCLP.h"
#endif
//Qt includes
#include <QApplication>
#include <QDebug>
#include <QFileInfo>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "QtSlicer.h"

int execImageViewer(int argc, char* argv[])
{
  QApplication myApp( argc, argv );

  QtSlicer qtSlicerWindow( 0 );
  qtSlicerWindow.setWindowTitle("ImageViewer");
  qtSlicerWindow.loadInputImage();
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

int parseAndExecImageViewer(int argc, char* argv[])
{
#ifdef USE_CLI
  PARSE_ARGS;
#endif

  QApplication myApp(argc, argv);

  QtSlicer qtSlicerWindow( 0 );
  qtSlicerWindow.setWindowTitle("ImageViewer");

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
  qtSlicerWindow.sliceView()->setOrientation(orientation);
  if(sliceOffset != -1)
    {
    qtSlicerWindow.sliceView()->setSliceNum(sliceOffset);
    }
  if(minIntensityArg.isSet())
    {
    qtSlicerWindow.sliceView()->setMinIntensity(minIntensity);
    }
  if(maxIntensityArg.isSet())
    {
    qtSlicerWindow.sliceView()->setMaxIntensity(maxIntensity);
    }

  qtSlicerWindow.sliceView()->setZoom(zoom);
  qtSlicerWindow.sliceView()->transpose(transpose);
  qtSlicerWindow.sliceView()->flipZ(zFlipped);
  qtSlicerWindow.sliceView()->flipY(yFlipped);
  qtSlicerWindow.sliceView()->flipX(xFlipped);
  qtSlicerWindow.sliceView()->setOverlayOpacity(overlayOpacity);
  qtSlicerWindow.sliceView()->setViewCrosshairs(crosshairs);
  qtSlicerWindow.sliceView()->setDisplayState(details);
  qtSlicerWindow.sliceView()->setViewValuePhysicalUnits(physicalUnits);
  qtSlicerWindow.sliceView()->setViewValue(value);
  qtSlicerWindow.sliceView()->setViewAxisLabel(axisLabel);
  qtSlicerWindow.sliceView()->setViewClickedPoints(clickedPoints);
  qtSlicerWindow.sliceView()->setImageMode(imageMode.c_str());
  qtSlicerWindow.sliceView()->setIWModeMax(iwModeMax.c_str());
  qtSlicerWindow.sliceView()->setIWModeMin(iwModeMin.c_str());
  qtSlicerWindow.sliceView()->update();
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
#if !defined(BUILD_SHARED_LIBS)
  Q_INIT_RESOURCE(qtImageViewerResources);
#endif
  int res;
  if(argc == 1)
    {
    res = execImageViewer(argc, argv);
    }
  else
    {
    res = parseAndExecImageViewer(argc, argv);
    }
  return res;
}
