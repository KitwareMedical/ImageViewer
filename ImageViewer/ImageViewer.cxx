/*=========================================================================

Library:   TubeTK

Copyright Kitware Inc.

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
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkNeighborhoodIterator.h>
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
    filter->SetRadius( 1 );
    filter->Update();
    sv->setInputImage( filter->GetOutput() );
    sv->update();
    }
}

void myMouseCallback(double x, double y, double z, double v, void *d)
{
  static itk::Index<3> PRESS_SEED;
  static double PRESS_SEED_V = 0;

  typedef itk::Image< double, 3 >        ImageType;
  typedef itk::Image< unsigned char, 3 > OverlayType;

  typedef itk::ConnectedThresholdImageFilter< ImageType, OverlayType >
                                         ConnCompFilterType;
  typedef itk::BinaryBallStructuringElement< unsigned char, 3 >
                                         StructuringElementType;
  typedef itk::BinaryErodeImageFilter< OverlayType, OverlayType,
                                         StructuringElementType >
                                         ErodeFilterType;
  typedef itk::BinaryDilateImageFilter< OverlayType, OverlayType,
                                         StructuringElementType >
                                         DilateFilterType;
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
    if( sv->selectMovement() == SM_MOVE || sv->selectMovement() == SM_RELEASE )
      {
      itk::Index<3> seed;
      seed[0] = int( x );
      seed[1] = int( y );
      seed[2] = int( z );

      ImageType::Pointer img = sv->inputImage();

      double seedDistance = 0;
      for(unsigned int d=0; d<3; ++d)
        {
        double tf = seed[d] - PRESS_SEED[d];
        seedDistance += tf * tf;
        }
      seedDistance = std::sqrt(seedDistance);

      itk::NeighborhoodIterator<ImageType>::RadiusType seedRadius;
      for(unsigned int d=0; d<3; ++d)
        {
        seedRadius[d] = seedDistance/2;
        int limit = img->GetLargestPossibleRegion().GetSize()[d]/2-1;
        if( limit < 0 )
          {
          limit = 0;
          }
        if( seedRadius[d] > limit )
          {
          seedRadius[d] = limit;
          }
        }
      ImageType::IndexType indx = PRESS_SEED;

      double threshMin = PRESS_SEED_V;
      double threshMax = PRESS_SEED_V;
      itk::NeighborhoodIterator<ImageType> it( seedRadius, img,
        img->GetLargestPossibleRegion() );
      it.SetLocation( indx );
      for( unsigned int i=0; i<it.Size(); ++i )
        {
        double tf = it.GetPixel(i);
        if( tf < threshMin )
          {
          threshMin = tf;
          }
        else if( tf > threshMax )
          {
          threshMax = tf;
          }
        }

      double v = img->GetPixel(seed);
      if( v < threshMin )
        {
        threshMin = v;
        }
      else if( v > threshMax )
        {
        threshMax = v;
        }

      ConnCompFilterType::Pointer ccFilter = ConnCompFilterType::New();
      ccFilter->SetInput( sv->inputImage() );
      ccFilter->AddSeed( PRESS_SEED );
      ccFilter->SetLower( threshMin );
      ccFilter->SetUpper( threshMax );
      ccFilter->SetReplaceValue(1);
      ccFilter->Update();

      OverlayType::Pointer overlay = ccFilter->GetOutput();

      ImageType::RegionType croppedRegion = img->GetLargestPossibleRegion();
      ImageType::IndexType croppedIndex = croppedRegion.GetIndex();
      ImageType::SizeType croppedSize = croppedRegion.GetSize();
      double cropMaxSize = 0;
      for( unsigned int d=0; d<3; ++d )
        {
        double tf = seed[d] - PRESS_SEED[d];
        cropMaxSize += tf * tf;
        }
      int morphRadius = 2; //cropMaxSize / 5;
      if( morphRadius > 8 )
        {
        morphRadius = 8;
        }
      cropMaxSize = std::sqrt(cropMaxSize) * 6;
      for( unsigned int d=0; d<3; ++d )
        {
        int sizeD = (int)( cropMaxSize / img->GetSpacing()[d] + 0.5 );
        if( sizeD < 1 )
          {
          sizeD = 1;
          }
        int indxD = PRESS_SEED[d] - (sizeD/2);
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

      if( morphRadius > 2 )
        {
        StructuringElementType dilateKernel;
        dilateKernel.SetRadius(morphRadius/2);
        dilateKernel.CreateStructuringElement();
        DilateFilterType::Pointer dilateFilter = DilateFilterType::New();
        dilateFilter->SetInput( overlay );
        dilateFilter->SetKernel( dilateKernel );
        dilateFilter->SetForegroundValue( 1 );
        dilateFilter->Update();
  
        StructuringElementType erodeKernel;
        erodeKernel.SetRadius(morphRadius);
        erodeKernel.CreateStructuringElement();
        ErodeFilterType::Pointer erodeFilter = ErodeFilterType::New();
        erodeFilter->SetInput( dilateFilter->GetOutput() );
        erodeFilter->SetKernel( erodeKernel );
        erodeFilter->SetForegroundValue( 1 );
        erodeFilter->Update();
  
        dilateFilter->SetInput( erodeFilter->GetOutput() );
        dilateFilter->SetKernel( dilateKernel );
        dilateFilter->SetForegroundValue( 1 );
        dilateFilter->Update();
  
        overlay = dilateFilter->GetOutput();
        }

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
      img = distanceFilter->GetOutput();

      double dMax = img->GetPixel(indx);
      ImageType::IndexType dMaxIndx = PRESS_SEED;
      itk::NeighborhoodIterator<ImageType> itD( seedRadius, img,
        img->GetLargestPossibleRegion() );
      itD.SetLocation( dMaxIndx );
      for( unsigned int i=0; i<itD.Size(); ++i )
        {
        double tf = itD.GetPixel(i);
        if( tf > dMax )
          {
          dMax = tf;
          dMaxIndx = itD.GetIndex(i);
          }
        }

      double zri = dMax / img->GetSpacing()[2];
      int zMin = dMaxIndx[2] - zri;
      if( zMin < croppedIndex[2] )
        {
        zMin = croppedIndex[2];
        }
      int zMax = dMaxIndx[2] + zri;
      if( zMax >= croppedIndex[2] + croppedSize[2] )
        {
        zMax = croppedIndex[2] + croppedSize[2] - 1;
        }
      double yri = dMax / img->GetSpacing()[1];
      int yMin = dMaxIndx[1] - yri;
      if( yMin < croppedIndex[1] )
        {
        yMin = croppedIndex[1];
        }
      int yMax = dMaxIndx[1] + yri;
      if( yMax >= croppedIndex[1] + croppedSize[1] )
        {
        yMax = croppedIndex[1] + croppedSize[1] - 1;
        }
      double xri = dMax / img->GetSpacing()[0];
      int xMin = dMaxIndx[0] - xri;
      if( xMin < croppedIndex[0] )
        {
        xMin = croppedIndex[0];
        }
      int xMax = dMaxIndx[0] + xri;
      if( xMax >= croppedIndex[0] + croppedSize[0] )
        {
        xMax = croppedIndex[0] + croppedSize[0] - 1;
        }
      for( indx[2]=zMin; indx[2]<=zMax; ++indx[2] )
        {
        double tf = (indx[2]-dMaxIndx[2])*img->GetSpacing()[2];
        double dz = (tf * tf);
        for( indx[1]=yMin; indx[1]<=yMax; ++indx[1] )
          {
          tf = (indx[1]-dMaxIndx[1])*img->GetSpacing()[1];
          double dy = (tf * tf);
          for( indx[0]=xMin; indx[0]<=xMax; ++indx[0] )
            {
            tf = (indx[0]-dMaxIndx[0])*img->GetSpacing()[0];
            double dx = (tf * tf);
            if( dz + dy + dx <= dMax * dMax )
              {
              overlay->SetPixel( indx, 3 );
              }
            }
          }
        }

      std::ostringstream sstr;
      sstr << "R = " << dMax;
      sv->setMessage( sstr.str() );

      sv->setInputOverlay( overlay );
      sv->update();
      }
    else if( sv->selectMovement() == SM_PRESS )
      {
      PRESS_SEED[0] = int( x );
      PRESS_SEED[1] = int( y );
      PRESS_SEED[2] = int( z );

      ImageType::Pointer img = sv->inputImage();

      double v = img->GetPixel( PRESS_SEED );

      PRESS_SEED_V = v;
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
  viewer.sliceView()->flipY(!yFlipped);
  viewer.sliceView()->flipX(xFlipped);
  viewer.sliceView()->setOverlayOpacity(overlayOpacity);
  viewer.sliceView()->setViewCrosshairs(!crosshairs);
  viewer.sliceView()->setDisplayState(details);
  viewer.sliceView()->setViewValuePhysicalUnits(physicalUnits);
  viewer.sliceView()->setViewValue(!overlayValue);
  viewer.sliceView()->setViewAxisLabel(axisLabel);
  viewer.sliceView()->setViewClickedPoints(clickedPoints);
  viewer.sliceView()->setImageMode(imageMode.c_str());
  viewer.sliceView()->setSaveOverlayOnExit(saveOverlayOnExit.c_str());
  viewer.sliceView()->setIWModeMax(iwModeMax.c_str());
  viewer.sliceView()->setIWModeMin(iwModeMin.c_str());
  viewer.sliceView()->setClickSelectArgCallBack( myMouseCallback );
  viewer.sliceView()->setClickSelectArg( (void*)(viewer.sliceView()) );
  viewer.sliceView()->setKeyEventArgCallBack( myKeyCallback );
  viewer.sliceView()->setKeyEventArg( (void*)(viewer.sliceView()) );

  viewer.sliceView()->setIsONSDRuler(ONSDRuler);

  viewer.sliceView()->setPaintColor( paintColor );
  viewer.sliceView()->setPaintRadius( paintRadius );

  if( !strcmp(mouseMode.c_str(),"ConnComp") )
    {
    viewer.sliceView()->setClickMode( CM_CUSTOM );
    }
  else if( !strcmp(mouseMode.c_str(),"Paint") )
    {
    viewer.sliceView()->setClickMode( CM_PAINT );
    }
  else if( !strcmp(mouseMode.c_str(),"Ruler") )
    {
    viewer.sliceView()->setClickMode( CM_RULER );
    }
  else // if( !strcmp(mouseMode.c_str(),"Select") )
    {
    viewer.sliceView()->setClickMode( CM_SELECT );
    }


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
