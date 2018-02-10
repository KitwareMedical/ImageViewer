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
#include "ImageViewerCLP.h"
//Qt includes
#include <QApplication>
#include <QDebug>
#include <QFileInfo>

//QtImageViewer includes
#include "QtGlSliceView.h"
#include "QtImageViewer.h"

int execImageViewer(int argc, char* argv[])
{
  QApplication myApp( argc, argv );

  QtImageViewer viewer( 0 );
  viewer.setWindowTitle("ImageViewer");
  viewer.loadInputImage();
  viewer.show();

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
  PARSE_ARGS;

  QApplication myApp(argc, argv);

  QtImageViewer viewer( 0 );
  viewer.setWindowTitle("ImageViewer");

  QString filePathToLoad;
  filePathToLoad = QString::fromStdString(inputImage);

  viewer.loadInputImage(filePathToLoad);

  if(!overlayImage.empty())
    {
    viewer.loadOverlayImage(QString::fromStdString(overlayImage));
    }
  viewer.sliceView()->setOrientation(orientation);
  if(sliceOffset != -1)
    {
    viewer.sliceView()->setSliceNum(sliceOffset);
    }
  if(minIntensityArg.isSet())
    {
    viewer.sliceView()->setIWMin(minIntensity);
    }
  if(maxIntensityArg.isSet())
    {
    viewer.sliceView()->setIWMax(maxIntensity);
    }

  viewer.sliceView()->setZoom(zoom);
  viewer.sliceView()->transpose(transpose);
  viewer.sliceView()->flipZ(zFlipped);
  viewer.sliceView()->flipY(yFlipped);
  viewer.sliceView()->flipX(xFlipped);
  viewer.sliceView()->setOverlayOpacity(overlayOpacity);
  viewer.sliceView()->setViewCrosshairs(!crosshairs);
  viewer.sliceView()->setDisplayState(details);
  viewer.sliceView()->setViewValuePhysicalUnits(physicalUnits);
  viewer.sliceView()->setViewValue(!overlayValue);
  viewer.sliceView()->setViewAxisLabel(axisLabel);
  viewer.sliceView()->setViewClickedPoints(clickedPoints);
  viewer.sliceView()->setImageMode(imageMode.c_str());
  viewer.sliceView()->setIWModeMax(iwModeMax.c_str());
  viewer.sliceView()->setIWModeMin(iwModeMin.c_str());
  viewer.sliceView()->update();

  viewer.show();
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
  int res = EXIT_FAILURE;
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
