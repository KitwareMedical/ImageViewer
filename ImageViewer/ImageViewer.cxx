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

#include <itkConnectedThresholdImageFilter.h>
#include <itkStatisticsImageFilter.h>

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

double IMAGE_MIN = 0;
double IMAGE_MAX = 1;

void myCallback(double x, double y, double z, double v, void *d)
{
  static itk::Index<3> V_SEED;
  static double V_MIN = 0;
  static double V_MAX = 1;

  typedef itk::Image< double, 3 >        ImageType;
  typedef itk::Image< unsigned char, 3 > OverlayType;

  typedef itk::ConnectedThresholdImageFilter< ImageType, OverlayType >
                                         ConnCompFilterType;

  QtGlSliceView * sv = (QtGlSliceView *)(d);

  if( sv->clickMode() == CM_SELECT )
    {
    if( sv->selectMovement() == SM_MOVE )
      {
      itk::Index<3> seed;
      seed[0] = int( x );
      seed[1] = int( y );
      seed[2] = int( z );
  
      ImageType::Pointer img = sv->inputImage();
      double v = img->GetPixel( seed );
  
      V_MAX = v;
      if( V_MIN > V_MAX )
        {
        double t = V_MAX;
        V_MAX = V_MIN;
        V_MIN = t;
        }

      double eps = 0.01 * (V_MAX-V_MIN);
      typename ConnCompFilterType::Pointer ccFilter = ConnCompFilterType::New();
      ccFilter->SetInput( sv->inputImage() );
      ccFilter->AddSeed( V_SEED );
      ccFilter->SetLower( V_MIN - eps );
      ccFilter->SetUpper( V_MAX + eps );
      ccFilter->SetReplaceValue(1);
      ccFilter->Update();
    
      sv->setInputOverlay( ccFilter->GetOutput() );
      sv->update();
      }
    else if( sv->selectMovement() == SM_PRESS )
      {
      V_SEED[0] = int( x );
      V_SEED[1] = int( y );
      V_SEED[2] = int( z );
  
      ImageType::Pointer img = sv->inputImage();
      double v = img->GetPixel( V_SEED );

      V_MIN = v;
      }
    }

}

int parseAndExecImageViewer(int argc, char* argv[])
{
  typedef itk::Image< double, 3 >        ImageType;
  typedef itk::Image< unsigned char, 3 > OverlayType;

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
  viewer.sliceView()->setClickSelectArgCallBack( myCallback );
  viewer.sliceView()->setClickSelectArg( (void*)(viewer.sliceView()) );

  ImageType::Pointer img = viewer.sliceView()->inputImage();

  typedef itk::StatisticsImageFilter<ImageType>  StatisticsFilterType;
  StatisticsFilterType::Pointer statsFilter = StatisticsFilterType::New();
  statsFilter->SetInput( img );
  statsFilter->Update();
  IMAGE_MIN = statsFilter->GetMinimum();
  IMAGE_MAX = statsFilter->GetMaximum();

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
