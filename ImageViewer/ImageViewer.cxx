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
#include <QKeyEvent>

#include <itkMedianImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkInvertIntensityImageFilter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
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

void myKeyCallback(QKeyEvent *event, void *d)
{
  if( event->key() == Qt::Key_Apostrophe )
    {
    QtGlSliceView * sv = (QtGlSliceView *)(d);

    typedef itk::Image< double, 3 >        ImageType;
    ImageType::Pointer img = sv->inputImage();

    typedef itk::MedianImageFilter< ImageType, ImageType >
                                           FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput( img );
    filter->SetRadius( 2 );
    filter->Update();
    sv->setInputImage( filter->GetOutput() );
    sv->update();
    }
}

void myMouseCallback(double x, double y, double z, double v, void *d)
{
  static itk::Index<3> V_SEED;
  static double V_MIN = 0;
  static double V_MAX = 1;

  typedef itk::Image< double, 3 >        ImageType;
  typedef itk::Image< unsigned char, 3 > OverlayType;

  typedef itk::ConnectedThresholdImageFilter< ImageType, OverlayType >
                                         ConnCompFilterType;
  typedef itk::ExtractImageFilter< OverlayType, OverlayType >
                                         ExtractFilterType;
  typedef itk::InvertIntensityImageFilter< OverlayType, OverlayType >
                                         InvertFilterType;
  typedef itk::SignedMaurerDistanceMapImageFilter< OverlayType, ImageType >
                                         DistanceFilterType;
  typedef itk::MinimumMaximumImageCalculator< ImageType >
                                         MinMaxFilterType;

  QtGlSliceView * sv = (QtGlSliceView *)(d);

  if( sv->clickMode() == CM_CUSTOM )
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
      ConnCompFilterType::Pointer ccFilter = ConnCompFilterType::New();
      ccFilter->SetInput( sv->inputImage() );
      ccFilter->AddSeed( V_SEED );
      ccFilter->SetLower( V_MIN - eps );
      ccFilter->SetUpper( V_MAX + eps );
      ccFilter->SetReplaceValue(1);
      ccFilter->Update();
    
      OverlayType::Pointer overlay = ccFilter->GetOutput();

      ImageType::RegionType croppedRegion = img->GetLargestPossibleRegion();
      ImageType::IndexType croppedIndex = croppedRegion.GetIndex();
      ImageType::SizeType croppedSize = croppedRegion.GetSize();
      double cropMaxSize = 0;
      for( unsigned int d=0; d<3; ++d )
        {
        double tf = std::fabs(seed[d] - V_SEED[d]) * 6 * img->GetSpacing()[d];
        if( tf > cropMaxSize )
          {
          cropMaxSize = tf;
          }
        }
      for( unsigned int d=0; d<3; ++d )
        {
        int sizeD = (int)( cropMaxSize / img->GetSpacing()[d] + 0.5 );
        if( sizeD < 1 )
          {
          sizeD = 1;
          }
        int indxD = V_SEED[d] - (sizeD/2);
        if( indxD < croppedIndex[d] )
          {
          sizeD -= (croppedIndex[d] - indxD);
          indxD = croppedIndex[d];
          }
        if( indxD + sizeD > croppedIndex[d] + croppedSize[d] )
          {
          sizeD = ( croppedIndex[d] + croppedSize[d] ) - indxD;
          }
        croppedSize[d] = sizeD;
        croppedIndex[d] = indxD;
        }
      croppedRegion.SetIndex( croppedIndex );
      croppedRegion.SetSize( croppedSize );
      ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
      extractFilter->SetInput( overlay );
      extractFilter->SetExtractionRegion( croppedRegion );
      extractFilter->Update();
      OverlayType::Pointer croppedImg = extractFilter->GetOutput();

      InvertFilterType::Pointer invertFilter = InvertFilterType::New();
      invertFilter->SetInput( croppedImg );
      invertFilter->SetMaximum( 1 );
      invertFilter->Update();
      croppedImg = invertFilter->GetOutput();

      DistanceFilterType::Pointer distanceFilter =
        DistanceFilterType::New();
      distanceFilter->SetInput( croppedImg );
      distanceFilter->SetUseImageSpacing( true );
      distanceFilter->SetSquaredDistance( false );
      distanceFilter->Update();
      
      MinMaxFilterType::Pointer minmaxFilter = MinMaxFilterType::New();
      minmaxFilter->SetImage( distanceFilter->GetOutput() );
      minmaxFilter->Compute();

      std::cout << "At x = " << minmaxFilter->GetIndexOfMaximum()
        << "  Radius = " << minmaxFilter->GetMaximum()
        << std::endl << std::endl;

      ImageType::IndexType indx;
      indx = minmaxFilter->GetIndexOfMaximum();

      double radius = minmaxFilter->GetMaximum();
      double zri = radius / img->GetSpacing()[2];
      int zMin = indx[2] - zri;
      if( zMin < croppedIndex[2] )
        {
        zMin = croppedIndex[2];
        }
      int zMax = indx[2] + zri;
      if( zMax >= croppedIndex[2] + croppedSize[2] )
        {
        zMax = croppedIndex[2] + croppedSize[2] - 1;
        }
      double yri = radius / img->GetSpacing()[1];
      int yMin = indx[1] - yri;
      if( yMin < croppedIndex[1] )
        {
        yMin = croppedIndex[1];
        }
      int yMax = indx[1] + yri;
      if( yMax >= croppedIndex[1] + croppedSize[1] )
        {
        yMax = croppedIndex[1] + croppedSize[1] - 1;
        }
      double xri = radius / img->GetSpacing()[0];
      int xMin = indx[0] - xri;
      if( xMin < croppedIndex[0] )
        {
        xMin = croppedIndex[0];
        }
      int xMax = indx[0] + xri;
      if( xMax >= croppedIndex[0] + croppedSize[0] )
        {
        xMax = croppedIndex[0] + croppedSize[0] - 1;
        }
      ImageType::IndexType indxR;
      for( indxR[2]=zMin; indxR[2]<=zMax; ++indxR[2] )
        {
        double tf = (indxR[2]-indx[2])*img->GetSpacing()[2];
        double dz = (tf * tf);
        for( indxR[1]=yMin; indxR[1]<=yMax; ++indxR[1] )
          {
          tf = (indxR[1]-indx[1])*img->GetSpacing()[1];
          double dy = (tf * tf);
          for( indxR[0]=xMin; indxR[0]<=xMax; ++indxR[0] )
            {
            tf = (indxR[0]-indx[0])*img->GetSpacing()[0];
            double dx = (tf * tf);
            if( dz + dy + dx <= radius * radius )
              {
              overlay->SetPixel( indxR, 3 );
              }
            }
          }
        }

      sv->setInputOverlay( overlay );
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
  viewer.sliceView()->setClickSelectArgCallBack( myMouseCallback );
  viewer.sliceView()->setClickSelectArg( (void*)(viewer.sliceView()) );
  viewer.sliceView()->setKeyEventArgCallBack( myKeyCallback );
  viewer.sliceView()->setKeyEventArg( (void*)(viewer.sliceView()) );

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
