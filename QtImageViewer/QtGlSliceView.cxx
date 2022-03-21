/*=========================================================================

Library:   TubeTK

Copyright Kitware Inc.

All rights reserved.

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#ifndef QtGlSliceView_cxx
#define QtGlSliceView_cxx

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif

//QtImageViewer include
#include "QtGlSliceView.h"

//itk include
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"

//std includes
#include <cmath>

// Qt includes
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollArea>
#include <QBoxLayout>
#include <QTextEdit>
#include <memory>
#include <QGuiApplication>
#include <QPainterPath>

QtGlSliceView::QtGlSliceView( QWidget* widgetParent )
  : QOpenGLWidget( widgetParent )
{
  cDisplayState         = 0x01;
  cMaxDisplayStates     = 2;             // Off and On.

  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cOverlayOpacity       = 0.75;
  cOverlayPaintRadius   = 2;
  cOverlayPaintColor    = 1;
  cWinOverlayData       = NULL;
  cOverlayImageExtension = "mha";

  cWorkflowIndex        = 0;

  cHelpDialog             = 0;

  cViewValuePhysicalUnits = false;
  cPhysicalUnitsName      = "mm";

  cClickMode              = CM_SELECT;
  cSelectMovement         = SM_PRESS;
  cViewClickedPoints      = false;
  cViewCrosshairs         = true;
  cViewValue              = true;

  cWinOrientationCallBack = NULL;
  cWinOrientationArg = NULL;
  cWinOrientationArgCallBack = NULL;

  cWinCenterCallBack = NULL;
  cWinCenterArg = NULL;
  cWinCenterArgCallBack = NULL;

  cSliceNumCallBack = NULL;
  cSliceNumArg = NULL;
  cSliceNumArgCallBack = NULL;

  cClickSelectCallBack = NULL;
  cClickSelectArg = NULL;
  cClickSelectArgCallBack = NULL;

  cKeyEventArg = NULL;
  cKeyEventArgCallBack = NULL;

  cClickBoxCallBack = NULL;
  cClickBoxArg = NULL;
  cClickBoxArgCallBack = NULL;

  cIWCallBack = NULL;
  cIWArg = NULL;
  cIWArgCallBack = NULL;

  cViewAxisLabel = false;
  sprintf( cAxisLabelX[0], "S" );
  sprintf( cAxisLabelX[1], "L" );
  sprintf( cAxisLabelX[2], "L" );
  sprintf( cAxisLabelY[0], "P" );
  sprintf( cAxisLabelY[1], "S" );
  sprintf( cAxisLabelY[2], "P" );
  cOverlayData = NULL;
  cImData = NULL;
  cClickSelectV = 0;
  cViewImData  = true;
  cValidImData = false;

  cIWMax = 0;
  cIWMin = 0;

  cIWModeMax = IW_MAX;
  cIWModeMin = IW_MIN;

  cImageMode = IMG_VAL;

  cWinZoom = 1;
  cWinOrientation = 2;

  cViewAxisLabel = 0;
  cColorTable = ColorTableType::New();
  cColorTable->UseDiscreteColors();
  for ( unsigned int i=0; i < 3; ++i )
    {
    cFlipX[i] = false;
    cFlipY[i] = true;
    cFlipZ[i] = false;
    cBoxMax[i] = 0;
    cBoxMin[i] = 0;
    cTranspose[i] = 0;
    cDimSize[i] = 0;
    cSpacing[i] = 0;
    cWinOrder[i] = 0;
    cWinCenter[i] = 0;
    cClickSelect[i] = 0;
    }
  cWinOrder[0] = 0;
  cWinOrder[1] = 1;
  cWinOrder[2] = 2;
  cClickSelectV = 0;
  cSpanMax = 0;
  cWinMinX = 0;
  cWinMaxX = 0;
  cWinSizeX = 0;
  cWinMinY = 0;
  cWinMaxY = 0;
  cWinSizeY = 0;
  cWinDataSizeX = 0;
  cWinDataSizeY = 0;
  inDataSizeX = 0;
  inDataSizeY = 0;
  cWinImData = NULL;
  cWinZBuffer = NULL;

  cMessage = "";

  cSaveOnExitPrefix = "";

  cFastPace = 1;
  cFastMoveValue[0] = 0.5; //fast moving pace: 1 by defaut
  cFastMoveValue[1] = 1; //fast moving pace: 1 by defaut
  cFastMoveValue[2] = 2; //fast moving pace: 1 by defaut
  cFastIWValue[0] = 0.5; //fast window and level pace: 1 by defaut
  cFastIWValue[1] = 1; //fast window and level pace: 1 by defaut
  cFastIWValue[2] = 2; //fast window and level pace: 1 by defaut

  QSizePolicy sP = this->sizePolicy();
  sP.setHeightForWidth( true );
  this->setSizePolicy( sP );
  
  cONSDMetaFactory = std::shared_ptr< RulerToolMetaDataFactory >(new RulerToolMetaDataFactory(std::unique_ptr< ONSDRulerMetaDataGenerator >(new ONSDRulerMetaDataGenerator())));
  cRainbowMetaFactory = std::shared_ptr< RulerToolMetaDataFactory >(new RulerToolMetaDataFactory(std::unique_ptr< RainbowRulerMetaDataGenerator >(new RainbowRulerMetaDataGenerator())));

  isONSDRuler = false;

  cCurrentRulerMetaFactory = cRainbowMetaFactory;

  cCurrentBoxMetaFactory = std::shared_ptr< BoxToolMetaDataFactory >(new BoxToolMetaDataFactory(std::unique_ptr< ConstantBoxMetaDataGenerator >(new ConstantBoxMetaDataGenerator())));

  update();
}

QtGlSliceView::~QtGlSliceView()
{
  if( cSaveOnExitPrefix.size() > 0 ) {
    auto overlayFileName = cSaveOnExitPrefix + ".overlay." + cOverlayImageExtension;
    saveOverlay( overlayFileName.toStdString() );
    auto rulersFileName = cSaveOnExitPrefix + ".rulers.json";
    saveRulers( rulersFileName.toStdString() );
    auto boxesFileName = cSaveOnExitPrefix + ".boxes.json";
    saveBoxes( boxesFileName.toStdString() );
  }
}


void
QtGlSliceView::
setInputImage( ImageType * newImData )
{
  if( !newImData )
    {
    return;
    }

  RegionType region = newImData->GetLargestPossibleRegion();
  if( region.GetNumberOfPixels() == 0 )
    {
    return;
    }

  SizeType myImageSize = region.GetSize();
  if( cValidOverlayData )
    {
    RegionType overlayRegion = cOverlayData->GetLargestPossibleRegion();
    SizeType   overlaySize   = overlayRegion.GetSize();

    for ( int i=0; i<3; i++ )
      {
      if( myImageSize[i] != overlaySize[i] )
        {
        qWarning() << "ImageSize != OverlaySize.  Aborting SetImage().";
        return;
        }
      }
    }

  cImData = newImData;
  cDimSize[0] = myImageSize[0];
  cDimSize[1] = myImageSize[1];
  cDimSize[2] = myImageSize[2];
  cSpacing[0] = cImData->GetSpacing()[0];
  cSpacing[1] = cImData->GetSpacing()[1];
  cSpacing[2] = cImData->GetSpacing()[2];

  typedef MinimumMaximumImageCalculator<ImageType> CalculatorType;
  CalculatorType::Pointer calculator = CalculatorType::New();

  calculator->SetImage( cImData );
  calculator->Compute();

  cDataMin = calculator->GetMinimum();
  cDataMax = calculator->GetMaximum();
  this->setIWMin( cDataMin );
  this->setIWMax( cDataMax );

  cWinCenter[0] = cDimSize[0]/2;
  cWinCenter[1] = cDimSize[1]/2;
  cWinCenter[2] = 0;

  cWinMinX  = 0;
  cWinSizeX = qMax( cDimSize[0], qMax( cDimSize[1], cDimSize[2] ) );
  cWinMaxX = static_cast<int>( cWinSizeX ) - 1;

  cSpanMax = qMax( cDimSize[0]*cSpacing[0], qMax( cDimSize[1]*cSpacing[1],
      cDimSize[2]*cSpacing[2] ) );

  cWinMinY = 0;
  cWinSizeY = cWinSizeX;
  cWinMaxY = static_cast<int>( cWinSizeY ) - 1;

  cWinDataSizeX = cWinSizeX;
  cWinDataSizeY = cWinSizeX;

  cFastMoveValue[0] = 1;
  cFastMoveValue[2] = (int)(cWinSizeX / 10);
  if( cFastMoveValue[2] < 5 )
    {
    cFastMoveValue[2] = 5;
    }
  cFastMoveValue[1] = cFastMoveValue[2] / 2;

  cFastIWValue[0] = (double)((cDataMax-cDataMin) / 10240);
  cFastIWValue[1] = (double)((cDataMax-cDataMin) / 1024);
  cFastIWValue[2] = (double)((cDataMax-cDataMin) / 20);

  if( cWinImData != NULL )
    {
    delete [] cWinImData;
    }

  cWinImData = new unsigned char[ cWinDataSizeX * cWinDataSizeY ];

  if( cWinZBuffer != NULL )
    {
    delete [] cWinZBuffer;
    }

  cWinZBuffer = new unsigned short[ cWinDataSizeX * cWinDataSizeY ];
  this->changeSlice( ( ( this->maxSliceNum() -1 )/2 ) );
  this->updateGeometry();

  cValidImData = true;

  this->update();
  emit imageChanged();
}


const QtGlSliceView::ImagePointer &
QtGlSliceView
::inputImage( void ) const
{
  return cImData;
}


void
QtGlSliceView
::setInputOverlay( OverlayType * newOverlayData )
{
  RegionType newoverlay_region = newOverlayData->GetLargestPossibleRegion();

  SizeType newoverlay_size = newoverlay_region.GetSize();

  RegionType cImData_region = cImData->GetLargestPossibleRegion();

  SizeType cImData_size = cImData_region.GetSize();

  if( !cValidImData || newoverlay_size[2]==cImData_size[2] )
    {
    cOverlayData = newOverlayData;

    cViewOverlayData  = true;
    cValidOverlayData = true;
    cOverlayOpacity   = 0.75;

    if( cWinOverlayData != NULL )
      {
      delete [] cWinOverlayData;
      }

    cWinOverlayData = new unsigned char[ cWinDataSizeX * cWinDataSizeY * 4 ];

    if( cWinZBuffer != NULL )
      {
      delete [] cWinZBuffer;
      }
    cWinZBuffer = new unsigned short[cWinDataSizeX * cWinDataSizeY * 4];
    emit validOverlayDataChanged( cValidOverlayData );
    update();
    }
  else
    {
    qWarning() << "Overlay invalid. Must be the same size as base image.";
    }
}


const QtGlSliceView::OverlayType::Pointer &
QtGlSliceView::inputOverlay( void ) const
{
  return cOverlayData;
}


void
QtGlSliceView::
setViewOverlayData( bool newViewOverlayData )
{
  cViewOverlayData = newViewOverlayData;
}


bool
QtGlSliceView::viewOverlayData( void ) const
{
  return cViewOverlayData;
}


void
QtGlSliceView::setOverlayOpacity( double newOverlayOpacity )
{
  cOverlayOpacity = qBound( 0., newOverlayOpacity, 1. );
  emit overlayOpacityChanged( cOverlayOpacity );
  update();
}


double
QtGlSliceView::overlayOpacity( void ) const
{
  return cOverlayOpacity;
}

void QtGlSliceView::setOverlayImageExtension(const char * ext )
{
  cOverlayImageExtension = ext;
}


QtGlSliceView::ColorTableType* QtGlSliceView::colorTable( void ) const
{
  return cColorTable.GetPointer();
}


void QtGlSliceView::saveClickedPointsStored()
{
  QString fileName = QFileDialog::getSaveFileName( this,
    "Please select a file name", "*.*", "" );
  if( fileName.isNull() )
    {
    return;
    }
  QFile fpoints( fileName );
  if( !fpoints.open( QIODevice::ReadWrite ) )
    {
    return;
    }
  ClickPointListType::const_iterator point;
  QTextStream text( &fpoints );
  for( point = cClickedPoints.begin(); point != cClickedPoints.end();
    point++ )
    {
    text << ( *point ).x << " " << ( *point ).y << " " << ( *point ).z
      << " : " << ( *point ).value << endl;
    }
  fpoints.close();
}

void QtGlSliceView::setSaveOnExitPrefix(const char * prefix )
{
  cSaveOnExitPrefix = prefix;
}

void QtGlSliceView::saveRulersWithPrompt()
{
    QFileInfo fileInfo(this->inputImageFilepath);
    QString newFilepath = fileInfo.path() + "/" + fileInfo.completeBaseName() + ".json";    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save ruler measurements", newFilepath, "*.*");
    if (fileName.isNull())
    {
        return;
    }

    saveRulers(fileName.toStdString());
}

void QtGlSliceView::saveRulers( std::string fileName )
{
    auto hasRulers = false;
    for (auto& node : cRulerCollections)
    {
      if (node.second->rulers.size() > 0)
      {
        hasRulers = true;
        break;
      }
    }

    if (!hasRulers)
    {
      return;
    }

    QFile fpoints(QString::fromStdString(fileName));
    if (!fpoints.open(QIODevice::ReadWrite | QIODevice::Truncate)) // write and overwrite
    {
        return;
    }
    QTextStream text(&fpoints);
    text << "{ \"rulers\" : [";
    bool isFirst = true;
    for (auto& node : cRulerCollections) 
    {
            auto& rc = node.second;
            std::string sep = isFirst ? "" : ",";
            if (node.second->rulers.size() > 0) {
                text << sep.c_str() << rc->toJson().c_str();
                isFirst = false;
            }
    }
    text << "]}";
    fpoints.close();
}

void QtGlSliceView::saveBoxesWithPrompt()
{
    QFileInfo fileInfo(this->inputImageFilepath);
    QString newFilepath = fileInfo.path() + "/" + fileInfo.completeBaseName() + ".boxes.json";
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save box measurements", newFilepath, "*.*");
    if (fileName.isNull())
    {
        return;
    }

    saveBoxes(fileName.toStdString());
}

void QtGlSliceView::saveBoxes( std::string fileName )
{
    auto hasBoxes = false;
    for (auto& node : cBoxCollections)
    {
      if (node.second->boxes.size() > 0)
      {
        hasBoxes = true;
        break;
      }
    }

    if (!hasBoxes)
    {
      return;
    }

    QFile fpoints(QString::fromStdString(fileName));
    if (!fpoints.open(QIODevice::ReadWrite | QIODevice::Truncate)) // write and overwrite
    {
        return;
    }
    QTextStream text(&fpoints);
    text << "{ \"boxes\" : [";
    bool isFirst = true;
    for (auto& node : cBoxCollections)
    {
            auto& rc = node.second;
            std::string sep = isFirst ? "" : ",";
            if (node.second->boxes.size() > 0) {
                text << sep.c_str() << rc->toJson().c_str();
                isFirst = false;
            }
    }
    text << "]}";
    fpoints.close();
}

void
QtGlSliceView::update()
{
  if( !cValidImData )
    {
    return;
    }
  cWinSizeX = ( int )( (cSpanMax / cWinZoom)
    / (cDimSize[cWinOrder[0]]*cSpacing[cWinOrder[0]])
    * cDimSize[cWinOrder[0]] );
  int ti = ( int )( ( int )cWinCenter[ cWinOrder[0] ] - cWinSizeX/2 );
  if( ti < -(int)cWinSizeX )
    {
    ti = -(int)cWinSizeX;
    }
  else if( ti >= static_cast<int>( cDimSize[ cWinOrder[0] ] + cWinSizeX ) )
    {
    ti = cDimSize[ cWinOrder[0] ] + cWinSizeX;
    }
  cWinMinX = ti;
  cWinMaxX = cWinMinX + cWinSizeX - 1;
  if( cWinMaxX >= static_cast<int>( cDimSize[ cWinOrder[0] ] + cWinSizeX ) )
    {
    cWinMaxX = cDimSize[ cWinOrder[0] ] + cWinSizeX;
    }

  cWinSizeY = ( int )( (cSpanMax / cWinZoom)
    / (cDimSize[cWinOrder[1]]*cSpacing[cWinOrder[1]])
    * cDimSize[cWinOrder[1]] );
  ti = ( int )( ( int )cWinCenter[ cWinOrder[1] ] - cWinSizeY/2 );
  if( ti < -(int)cWinSizeY )
    {
    ti = -(int)cWinSizeY;
    }
  else if( ti >= static_cast<int>( cDimSize[ cWinOrder[1] ] + cWinSizeY ) )
    {
    ti = cDimSize[ cWinOrder[1] ] + cWinSizeY ;
    }
  cWinMinY = ti;
  cWinMaxY = cWinMinY + cWinSizeY - 1;
  if( cWinMaxY >= static_cast<int>( cDimSize[ cWinOrder[1] ] + cWinSizeY ) )
    {
    cWinMaxY = cDimSize[ cWinOrder[1] ] + cWinSizeY;
    }

  memset( cWinImData, 0, cWinDataSizeX*cWinDataSizeY );
  if( cValidOverlayData )
    {
    memset( cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4 );
    }

  IndexType ind;

  int l, m;

  double tf;

  ind[ cWinOrder[ 2 ] ] = cWinCenter[ cWinOrder[ 2 ] ];
  int startK = cWinMinY;
  if( startK<0 )
    startK = 0;
  int endK = cWinMaxY;
  if( endK >= (int)(cDimSize[cWinOrder[1]]) )
    {
    endK = (int)(cDimSize[cWinOrder[1]])-1;
    }
  int startJ = cWinMinX;
  if( startJ<0 )
    startJ = 0;
  int endJ = cWinMaxX;
  if( endJ >= (int)(cDimSize[cWinOrder[0]]) )
    {
    endJ = (int)(cDimSize[cWinOrder[0]])-1;
    }

  for( int k=startK; k <= endK; k++ )
    {
    ind[cWinOrder[1]] = k;

    if( k-startK >= ( int )cWinDataSizeY )
      {
      continue;
      }

    for( int j=startJ; j <= endJ; j++ )
      {
      ind[cWinOrder[0]] = j;

      if( j-startJ >= ( int )cWinDataSizeX )
        {
        continue;
        }

      switch( cImageMode )
        {
        default:
        case IMG_VAL:
          tf = ( double )( ( cImData->GetPixel( ind )-cIWMin )
            / ( cIWMax-cIWMin )*255 );
          break;
        case IMG_INV:
          tf = ( double )( ( cIWMax-cImData->GetPixel( ind ) )
            / ( cIWMax-cIWMin )*255 );
          break;
        case IMG_LOG:
          tf = ( double )( log(
              cImData->GetPixel( ind ) - cIWMin + 0.00000001 )
            / log( cIWMax-cIWMin+0.00000001 )*255 );
          break;
        case IMG_DX:
          if( ind[0]>0 )
            {
            tf = ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[0]--;
            tf -= ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[0]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DY:
          if( ind[1]>0 )
            {
            tf = ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[1]--;
            tf -= ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[1]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DZ:
          if( ind[2]>0 )
            {
            tf = ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[2]--;
            tf -= ( double )( ( cImData->GetPixel( ind )-cIWMin )
              / ( cIWMax-cIWMin )*255 );
            ind[2]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_BLEND:
          {
          const int tempval = ( int )cWinCenter[cWinOrder[2]]-1;
          int tmpI = ind[cWinOrder[2]];
          ind[cWinOrder[2]] = ( tempval < 0 ) ? 0 : tempval;
          tf = ( double )( cImData->GetPixel( ind ) );

          ind[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          tf += ( double )( cImData->GetPixel( ind ) )*2;

          const int tempval1 = ( int )cDimSize[cWinOrder[2]]-1;
          const int tempval2 = ( int )cWinCenter[cWinOrder[2]]+1;
          ind[cWinOrder[2]] = ( tempval1 < tempval2 ) ? tempval1 : tempval2;
          tf += ( double )( cImData->GetPixel( ind ) );

          tf = ( double )( ( tf/4-cIWMin )/( cIWMax-cIWMin )*255 );
          ind[cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf = cIWMin;
          m = ( j-startJ ) + ( k-startK )*cWinDataSizeX;
          cWinZBuffer[m] = 0;
          int tmpI = ind[cWinOrder[2]];
          for( l=0; l<( int )cDimSize[cWinOrder[2]]; l++ )
            {
            ind[cWinOrder[2]] = l;
            if( cImData->GetPixel( ind ) > tf )
              {
              tf = ( double )( cImData->GetPixel( ind ) );
              cWinZBuffer[m] = ( unsigned short )l;
              }
            }
          tf = ( double )( ( tf-cIWMin )/( cIWMax-cIWMin )*255 );
          ind[cWinOrder[2]] = tmpI;
          break;
          }

      if( tf > 255 )
        {
        switch( cIWModeMax )
          {
          case IW_MIN:
            tf = 0;
            break;
          default:
          case IW_MAX:
            tf = 255;
            break;
          case IW_FLIP:
            tf = 512-tf;
            if( tf<0 )
              {
              tf = 0;
              }
            break;
          }
        }
      else
        {
        if( tf < 0 )
          {
          switch( cIWModeMin )
            {
            default:
            case IW_MIN:
              tf = 0;
              break;
            case IW_MAX:
              tf = 255;
              break;
            case IW_FLIP:
              tf = -tf;
              if( tf>255 )
                {
                tf = 255;
                }
              break;
            }
          }
        }

      l = ( j-startJ ) + ( k-startK )*cWinDataSizeX;
      cWinImData[l] = ( unsigned char )tf;

      if( cValidOverlayData )
        {
        l = l * 4;
        if( cImageMode == IMG_MIP )
          {
          ind[cWinOrder[2]] = cWinZBuffer[( j-startJ ) +
            ( k-startK )*cWinDataSizeX];
          }

        if( sizeof( OverlayPixelType ) == 1 )
          {
          m = ( int )*( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) );
          if( m > 0 )
            {
            m = m - 1;
            cWinOverlayData[l+0] =
              ( unsigned char )( cColorTable->GetColor( m ).GetRed()*255 );
            cWinOverlayData[l+1] =
              ( unsigned char )( cColorTable->GetColor( m ).GetGreen()*255 );
            cWinOverlayData[l+2] =
              ( unsigned char )( cColorTable->GetColor( m ).GetBlue()*255 );
            cWinOverlayData[l+3] =
              ( unsigned char )( cOverlayOpacity*255 );
            }
          }
        else
          {
          if( ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[0]
            + ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[1]
            + ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[2] > 0 )
              {
            if( sizeof( OverlayPixelType ) == 3 )
              {
              cWinOverlayData[l+0] =
                ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[0];
              cWinOverlayData[l+1] =
                ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[1];
              cWinOverlayData[l+2] =
                ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[2];
              cWinOverlayData[l+3] =
                ( unsigned char )( cOverlayOpacity*255 );
              }
            else
              {
              if( sizeof( OverlayPixelType ) == 4 )
                {
                cWinOverlayData[l+0] =
                  ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[0];
                cWinOverlayData[l+1] =
                  ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[1];
                cWinOverlayData[l+2] =
                  ( ( unsigned char * )&( cOverlayData->GetPixel( ind ) ) )[2];
                cWinOverlayData[l+3] =
                  ( unsigned char )( ( ( unsigned char * )
                  &( cOverlayData->GetPixel( ind ) ) )[3]*cOverlayOpacity );
                }
              }
            }
          }
        }
      }
    }
  Superclass::update();
}


void QtGlSliceView::setValidOverlayData( bool newValidOverlayData )
{
  this->cValidOverlayData = newValidOverlayData;
}


bool QtGlSliceView::validOverlayData() const
{
  return this->cValidOverlayData;
}


QDialog* QtGlSliceView::helpWindow() const
{
  return this->cHelpDialog;
}


int QtGlSliceView::imageSize( int axis ) const
{
  return this->cDimSize[axis];
}


int QtGlSliceView::displayState() const
{
  return this->cDisplayState;
}


int QtGlSliceView::maxDisplayStates() const
{
  return this->cMaxDisplayStates;
}


void QtGlSliceView::setMaxDisplayStates( int newNumberOfStates )
{
  this->setDisplayState( qMin( this->displayState(), static_cast<int>(
                               std::pow( 2., newNumberOfStates - 2 ) ) ) );
  this->cMaxDisplayStates = newNumberOfStates;
}


void QtGlSliceView::setDisplayState( int state )
{
  if( state == this->cDisplayState )
    {
    return;
    }
  this->cDisplayState = state;
  emit displayStateChanged( state );
}


int QtGlSliceView::nextDisplayState( int state )const
{
  if( state == 0 )
    {
    return 0x01;
    }
  int power = 0;
  while ( state >>= 1 )
    {
    ++power;
    }
  if( power >= this->maxDisplayStates() - 2 )
    {
    return 0x00;
    }
  return std::pow( 2., ++power );
}


void QtGlSliceView::showHelp()
{
  if( this->cHelpDialog == 0 )
    {
    this->cHelpDialog = new QDialog( this );
    this->cHelpDialog->resize( 1000, 1500 );

    QVBoxLayout *layout = new QVBoxLayout;
    this->cHelpDialog->setLayout( layout );

    QTextEdit *help = new QTextEdit();
    QStringList str;
    str << QString("Image Viewer");
    str << QString("");
    str << QString("This is a multi-dimensional image viewer. It displays one 2D slice or a MIP view of that data. A variety of viewing options exist...");
    str << QString("");
    str << QString("Options: (press a key in the window)");
    str << QString("");
    str << QString("   0 - View slices along the x-axis");
    str << QString("   1 - View slices along the y-axis");
    str << QString("   2 - View slices along the z-axis (default)");
    str << QString("");
    str << QString("   > , - View the next slice");
    str << QString("   < , - View the previous slice");
    str << QString("");
    str << QString("   r -reset all options");
    str << QString("   h - help (this document)");
    str << QString("");
    str << QString("   x - Flip the x-axis");
    str << QString("   y - Flip the y-axis");
    str << QString("   z - Flip the z-axis");
    str << QString("");
    str << QString("   f - Toggle fast-pace mode: quicker movement and intensity windowing");
    str << QString("");
    str << QString("   q w - Decrease, Increase the upper limit of the intensity windowing");
    str << QString("   e - Toggle between clipping and setting-to-black values above IW upper limit");
    str << QString("   ");
    str << QString("   a s - Decrease, Increase the lower limit of the intensity windowing");
    str << QString("   d - Toggle between clipping and setting-to-white values below IW lower limit");
    str << QString("   ");
    str << QString("   + = - Zoom-in by a factor of 2");
    str << QString("   - _ - Zoom-out by a factor of 2");
    str << QString("   ");
    str << QString("   i j k m - Shift the image in the corresponding direction");
    str << QString("   t - Transpose the axis of the slice being viewed");
    str << QString("   ");
    str << QString("   A - View axis labels: P=posterior, L=left, S=superior");
    str << QString("   C - View crosshairs that illustrate last user-click in the window");
    str << QString("   V - View image values as the user clicks in the window");
    str << QString("   T - Toggle display of clicked points");
    str << QString("   P - Toggle coordinates display between index and physical units");
    str << QString("   D - View image details as an overlay on the image");
    str << QString("   O - View a color overlay (application dependent)");
    str << QString("   p - Save the clicked points in a file");
    str << QString("   l - Toggle how the data is the window is viewed:");
    str << QString("       Modes cycle between the following views:");
    str << QString("         - Intensity");
    str << QString("         - Inverse");
    str << QString("         - Log");
    str << QString("         - Derivative wrt x");
    str << QString("         - Derivative wrt y");
    str << QString("         - Derivative wrt z");
    str << QString("         - Blend with previous and next slice");
    str << QString("         - MIP");
    str << QString("    ");
    str << QString("   \\ - cycle between mouse Modes: Select Points, Custom, Ruler, Box, Paint");
    str << QString("        - Default Custom is threshold connected components");
    str << QString("    ");
    str << QString("   Paint mode: ");
    str << QString("   [ ] - increase / decrease paint sphere radius");
    str << QString("   { } - increase / decrease paint color (0 erases)");
    str << QString("   \" - save the overlay to a file");
    str << QString("    ");
    str << QString("   Image processing ");
    str << QString("   \' - Perform median filtering with radius=1");
    str << QString("    ");
    str << QString("   Ruler/Box Measurements ");
    str << QString("   Left click to set ruler.  Right-click over X to move or delete");
    str << QString("   u - Toggle between rainbow ruler and ONSD measurements");
    str << QString("   ctrl-U - Save measurements to JSON");
    for( auto &data : str )
      {
      help->append( data );
      }
    layout->addWidget(help);
    }
  this->cHelpDialog->show();
}


void QtGlSliceView::setZoom( double newWinZoom )
{
  cWinZoom = newWinZoom;

  if( cWinZoom > 10 )
    {
    cWinZoom = 10;
    }
  else
    if( cWinZoom < 0.5 )
      {
      cWinZoom = 0.5;
      }

  emit zoomChanged( cWinZoom );
}


double QtGlSliceView::zoom() const
{
  return cWinZoom;
}


void QtGlSliceView::centerWindow( void )
{
  cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
  cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;

  if( cWinCenterCallBack != NULL )
    cWinCenterCallBack();
  if( cWinCenterArgCallBack != NULL )
    cWinCenterArgCallBack( cWinCenterArg );
}


void QtGlSliceView::centerWindow( int newWinCenterX,
                                 int newWinCenterY,
                                 int newWinCenterZ )
{
  if( newWinCenterX < 0 )
    newWinCenterX = 0;
  if( newWinCenterX >= ( int )cDimSize[0] )
    newWinCenterX = cDimSize[0] - 1;
  cWinCenter[0] = newWinCenterX;

  if( newWinCenterY < 0 )
    newWinCenterY = 0;
  if( newWinCenterY >= ( int )cDimSize[1] )
    newWinCenterY = cDimSize[1] - 1;
  cWinCenter[1] = newWinCenterY;

  if( newWinCenterZ < 0 )
    newWinCenterZ = 0;
  if( newWinCenterZ >= ( int )cDimSize[2] )
    newWinCenterZ = cDimSize[2] - 1;
  cWinCenter[2] = newWinCenterZ;

  if( cWinCenterCallBack != NULL )
    cWinCenterCallBack();
  if( cWinCenterArgCallBack != NULL )
    cWinCenterArgCallBack( cWinCenterArg );
}


int QtGlSliceView::windowCenterX( void ) const
{
  return cWinCenter[0];
}


int QtGlSliceView::windowCenterY( void ) const
{
  return cWinCenter[1];
}


int QtGlSliceView::windowCenterZ( void ) const
{
  return cWinCenter[2];
}


void QtGlSliceView::setViewValue( bool value )
{
  cViewValue = value;
}


void QtGlSliceView::setViewCrosshairs( bool crosshairs )
{
  cViewCrosshairs = crosshairs;
}


bool QtGlSliceView::viewCrosshairs() const
{
  return cViewCrosshairs;
}


bool QtGlSliceView::viewValue() const
{
  return cViewValue;
}


void QtGlSliceView::setBoxMin( double minX, double minY, double minZ )
{
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
  cBoxMin[2] = minZ;
}


void QtGlSliceView::setBoxMax( double boxX, double boxY, double boxZ )
{
  double x0 = qMin( cBoxMin[0], boxX );
  double y0 = qMin( cBoxMin[1], boxY );
  double z0 = qMin( cBoxMin[2], boxZ );

  double x1 = qMax( cBoxMin[0], boxX );
  double y1 = qMax( cBoxMin[1], boxY );
  double z1 = qMax( cBoxMin[2], boxZ );

  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
  cBoxMin[2] = z0;

  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
  cBoxMax[2] = z1;

  if( cClickBoxCallBack != NULL )
    {
    cClickBoxCallBack( cBoxMin[0], cBoxMin[1], cBoxMin[2],
                      cBoxMax[0], cBoxMax[1], cBoxMax[2] );
    }
  if( cClickBoxArgCallBack != NULL )
    {
    cClickBoxArgCallBack( cBoxMin[0], cBoxMin[1], cBoxMin[2],
                         cBoxMax[0], cBoxMax[1], cBoxMax[2],
                         cClickBoxArg );
    }
}


int QtGlSliceView::orientation( void ) const
{
  return cWinOrientation;
}


void QtGlSliceView::setOrientation( int newOrientation )
{
  cWinOrientation = newOrientation;
  switch( cWinOrientation )
    {
    case X_AXIS :
      cWinOrder[0] = Z_AXIS;
      cWinOrder[1] = Y_AXIS;
      cWinOrder[2] = X_AXIS;
      break;
    case Y_AXIS :
      cWinOrder[0] = X_AXIS;
      cWinOrder[1] = Z_AXIS;
      cWinOrder[2] = Y_AXIS;
      break;
    default:
    case Z_AXIS :
      cWinOrder[0] = X_AXIS;
      cWinOrder[1] = Y_AXIS;
      cWinOrder[2] = Z_AXIS;
      break;
    }

  if( cTranspose[cWinOrientation] )
    {
    int t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }
  const int newSliceNum = clickedPointsStored() ?
    ( int )cClickSelect[cWinOrder[2]] : cWinCenter[cWinOrder[2]];
  setSliceNum( newSliceNum );

  if( cWinOrientationCallBack != NULL )
    cWinOrientationCallBack();
  if( cWinOrientationArgCallBack != NULL )
    cWinOrientationArgCallBack( cWinOrientationArg );
  centerWindow();
  emit orientationChanged( cDimSize[cWinOrder[2]] );
}


void QtGlSliceView::setImageMode( ImageModeType newImageMode )
{
  cImageMode = newImageMode;
  if( cIWCallBack != NULL )
    cIWCallBack();
  if( cIWArgCallBack != NULL )
    cIWArgCallBack( cIWArg );
}


void QtGlSliceView::setImageMode( const char* newImageMode )
{
  int newImageModeIndex = -1;
  for ( int i = 0; i < NUM_ImageModeTypes; ++i )
    {
    if( QString( newImageMode ) == QString( ImageModeTypeName[i] ) )
      {
      newImageModeIndex = i;
      break;
      }
    }
  if( newImageModeIndex >= 0 )
    {
    this->setImageMode( static_cast<ImageModeType>( newImageModeIndex ) );
    }
}


void QtGlSliceView::setIWModeMax( const char* mode )
{
  int newmode = -1;
  for ( int i = 0; i < NUM_IWModeTypes; ++i )
    {
    if( QString( mode ) == QString( IWModeTypeName[i] ) )
      {
      newmode = i;
      break;
      }
    }
  if( newmode >= 0 )
    {
    this->setIWModeMax( static_cast<IWModeType>( newmode ) );
    }
}


void QtGlSliceView::setIWModeMin( const char* mode )
{
  int newmode = -1;
  for ( int i = 0; i < NUM_IWModeTypes; ++i )
    {
    if( QString( mode ) == QString( IWModeTypeName[i] ) )
      {
      newmode = i;
      break;
      }
    }
  if( newmode >= 0 )
    {
    this->setIWModeMin( static_cast<IWModeType>( newmode ) );
    }
}


void QtGlSliceView::setViewAxisLabel( bool axisLabel )
{
  cViewAxisLabel = axisLabel;
}


ImageModeType QtGlSliceView::imageMode( void ) const
{
  return cImageMode;
}


void QtGlSliceView::flipX( bool newFlipX )
{
  cFlipX[cWinOrientation] = newFlipX;
}


bool QtGlSliceView::isXFlipped() const
{
  return cFlipX[cWinOrientation];
}


void QtGlSliceView::flipY( bool newFlipY )
{
  cFlipY[cWinOrientation] = newFlipY;
}


bool QtGlSliceView::isYFlipped() const
{
  return cFlipY[cWinOrientation];
}


void QtGlSliceView::flipZ( bool newFlipZ )
{
  cFlipZ[cWinOrientation] = newFlipZ;
}


bool QtGlSliceView::isZFlipped() const
{
  return cFlipZ[cWinOrientation];
}


void QtGlSliceView::transpose( bool newTranspose )
{
  if( isTransposed() != newTranspose )
    {
    int t;
    t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }

  cTranspose[cWinOrientation] = newTranspose;
}


bool QtGlSliceView::isTransposed() const
{
  return cTranspose[cWinOrientation];
}


void QtGlSliceView::setOverlay( bool newOverlay )
{
  cViewOverlayData = newOverlay;
}

void QtGlSliceView::createOverlay( void )
{
  cOverlayData = OverlayType::New();

  cOverlayData->CopyInformation( cImData );
  cOverlayData->SetRegions( cImData->GetLargestPossibleRegion() );
  cOverlayData->Allocate();
  cOverlayData->FillBuffer( 0 );

  this->setInputOverlay( cOverlayData );
}


void QtGlSliceView::switchWorkflowStep( int index ) 
{

  auto& step = cWorkflowSteps[index];

  if (step->type == CM_PAINT) {
    setClickMode(CM_PAINT);
    auto paintStep = static_cast<PaintStep*>(step.get());
    cOverlayPaintColor = paintStep->label;
    cOverlayPaintRadius = paintStep->radius;
  } else if (step->type == CM_BOX) {
    setClickMode(CM_BOX);
    auto boxStep = static_cast<BoxStep*>(step.get());
    auto collection = getBoxToolCollection();
    collection->setMetaDataFactory(boxStep->factory);
  } else if (step->type == CM_RULER) {
    setClickMode(CM_RULER);
    auto rulerStep = static_cast<RulerStep *>(step.get());
    auto collection = getRulerToolCollection();
    collection->setMetaDataFactory(rulerStep->factory);
  }
}

void QtGlSliceView::paintOverlayPoint( double x, double y, double z )
{
  if( !cValidOverlayData )
    {
    return;
    }

  int r = cOverlayPaintRadius;
  int c = cOverlayPaintColor;

  if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
    c = 0;
  }

  int minX = x-r+1;
  if( minX < 0 )
    {
    minX = 0;
    }
  int maxX = x+r-1;
  if( maxX >= static_cast<int>(cDimSize[0]) )
    {
    maxX = cDimSize[0]-1;
    }

  int minY = y-r+1;
  if( minY < 0 )
    {
    minY = 0;
    }
  int maxY = y+r-1;
  if( maxY >= static_cast<int>(cDimSize[1]) )
    {
    maxY = cDimSize[1]-1;
    }

  int minZ = z-r+1;
  if( minZ < 0 )
    {
    minZ = 0;
    }
  int maxZ = z+r-1;
  if( maxZ >= static_cast<int>(cDimSize[2]) )
    {
    maxZ = cDimSize[2]-1;
    }

  double r2 = r * r;
  ImageType::IndexType idx;
  for( int iz=minZ; iz<=maxZ; ++iz )
    {
    double z2 = iz - z;
    z2 *= z2;
    idx[2] = iz;
    for( int iy=minY; iy<=maxY; ++iy )
      {
      double y2 = iy - y;
      y2 *= y2;
      idx[1] = iy;
      for( int ix=minX; ix<=maxX; ++ix )
        {
        double x2 = ix - x;
        x2 *= x2;
        if( z2 + y2 + x2 <= r2 )
          {
          idx[0] = ix;
          cOverlayData->SetPixel( idx, c );
          }
        }
      }
    }
  update();
}

void QtGlSliceView::saveOverlayWithPrompt( void )
{
  if( !cValidOverlayData )
    {
    return;
    }

  QString fileName = QFileDialog::getSaveFileName( this,
    "Please select a file name", "*.*", "" );
  if( fileName.isNull() )
    {
    return;
    }

  saveOverlay( fileName.toStdString() );
}

void QtGlSliceView::saveOverlay( std::string fileName )
{
  if( !cValidOverlayData )
    {
    return;
    }

  if( cOverlayData->GetLargestPossibleRegion().GetSize()[2] == 1 )
    {
    typedef itk::Image<unsigned char, 2> Overlay2DType;

    typedef itk::ExtractImageFilter< OverlayType, Overlay2DType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(cOverlayData);
    filter->SetDirectionCollapseToSubmatrix();
    OverlayType::RegionType region = cOverlayData->GetLargestPossibleRegion();
    OverlayType::RegionType::SizeType size = region.GetSize();
    size[2] = 0;
    region.SetSize(size);
    filter->SetExtractionRegion(region);
    filter->Update();
    typedef itk::ImageFileWriter< Overlay2DType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( fileName );
    writer->SetInput( filter->GetOutput() );
    writer->SetUseCompression( true );
    writer->Update();
    }
  else
    {
    typedef itk::ImageFileWriter< OverlayType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( fileName );
    writer->SetInput( cOverlayData );
    writer->SetUseCompression( true );
    writer->Update();
    }
}

void QtGlSliceView::setIWModeMin( IWModeType newIWModeMin )
{
  cIWModeMin = newIWModeMin;
  if( cIWCallBack != NULL )
    cIWCallBack();
  if( cIWArgCallBack != NULL )
    cIWArgCallBack( cIWArg );
}


IWModeType QtGlSliceView::iwModeMin( void ) const
{
  return cIWModeMin;
}


void QtGlSliceView::setIWModeMax( IWModeType newIWModeMax )
{
  cIWModeMax = newIWModeMax;
  if( cIWCallBack != NULL )
    cIWCallBack();
  if( cIWArgCallBack != NULL )
    cIWArgCallBack( cIWArg );
}


IWModeType QtGlSliceView::iwModeMax(void) const
{
    return cIWModeMax;
}


void QtGlSliceView::keyPressEvent(QKeyEvent* keyEvent)
{
    int movePace;
    double iwPace;
    int imgShiftSize = static_cast<int>((cWinSizeX / 10) / zoom());
    if (imgShiftSize < 1)
    {
        imgShiftSize = 1;
    }

    switch (keyEvent->key())
    {
    case Qt::Key_0:
        setOrientation(X_AXIS);
        transpose(true);
        update();
        break;
    case Qt::Key_1:
        setOrientation(Y_AXIS);
        transpose(false);
        update();
        break;
    case Qt::Key_2:
        setOrientation(Z_AXIS);
        transpose(false);
        update();
        break;
    case Qt::Key_Less: // <
    case Qt::Key_Comma:
        movePace = cFastMoveValue[cFastPace];
        if ((int)cWinCenter[cWinOrder[2]] - movePace < 0)
        {
            if ((int)cWinCenter[cWinOrder[2]] == 0)
            {
                return;
            }
            else
            {
                setSliceNum(0);
            }
        }
        else
        {
            setSliceNum((int)cWinCenter[cWinOrder[2]] - movePace);
        }
        update();
        break;
    case Qt::Key_BracketRight:
        ++cOverlayPaintRadius;
        update();
        break;
    case Qt::Key_BracketLeft:
        if (cOverlayPaintRadius > 1)
        {
            --cOverlayPaintRadius;
            update();
        }
        break;
    case Qt::Key_BraceRight:
        if (cOverlayPaintColor < 10)
        {
            ++cOverlayPaintColor;
            update();
        }
        break;
    case Qt::Key_BraceLeft:
        if (cOverlayPaintColor > 0)
        {
            --cOverlayPaintColor;
            update();
        }
        break;
    case Qt::Key_Space:
        if (cWorkflowSteps.size() > 0 && cWorkflowIndex < cWorkflowSteps.size() - 1)
        {
          cWorkflowIndex++;
          switchWorkflowStep(cWorkflowIndex);
          update();
        }
        break;
    case Qt::Key_Backspace:
        if (cWorkflowSteps.size() > 0 && cWorkflowIndex > 0)
        {
          cWorkflowIndex--;
          switchWorkflowStep(cWorkflowIndex);
          update();
        }
        break;
    case Qt::Key_QuoteDbl:
        saveOverlayWithPrompt();
        break;
    case Qt::Key_Backslash:
        if (cClickMode == CM_SELECT)
        {
            if (!cValidOverlayData)
            {
                createOverlay();
            }
            cClickMode = CM_CUSTOM;
            update();
        }
        else if (cClickMode == CM_CUSTOM)
        {
            if (!cValidOverlayData)
            {
                createOverlay();
            }
            cClickMode = CM_RULER;
            update();
        }
        else if ( cClickMode == CM_RULER ) {
            if (!cValidOverlayData)
            {
                createOverlay();
            }
            cClickMode = CM_BOX;
            update();
        }
        else if ( cClickMode == CM_BOX ) {
            if (!cValidOverlayData)
            {
                createOverlay();
            }
            cClickMode = CM_PAINT;
            update();
        }
      else
        {
        cClickMode = CM_SELECT;
        update();
        }
      break;
    case Qt::Key_Greater: // >
    case Qt::Key_Period:
      //when pressing down ">" or "<" key, scrolling will go faster
      movePace = cFastMoveValue[ cFastPace ];
      if( ( int )cWinCenter[cWinOrder[2]]+movePace >=
          ( int )cDimSize[cWinOrder[2]]-1 )
        {
        setSliceNum( ( int )cDimSize[cWinOrder[2]]-1 );
        }
      else
        {
        setSliceNum( ( int )cWinCenter[cWinOrder[2]]+movePace );
        }
      update();
      break;
    case Qt::Key_R:
      setZoom( 1.0 );
      centerWindow();
      setImageMode( IMG_VAL );
      setIWMin( cDataMin );
      setIWMax( cDataMax );
      update();
      break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
      zoomIn();
      update();
      break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
      zoomOut();
      update();
      break;
    case Qt::Key_X:
      flipX( !isXFlipped() );
      update();
      break;
    case Qt::Key_Y:
      flipY( !isYFlipped() );
      update();
      break;
    case Qt::Key_Z:
      flipZ( !isZFlipped() );
      update();
      break;
    case Qt::Key_U:
        if (cClickMode == CM_RULER) {
            if (keyEvent->modifiers() & Qt::CTRL) {
                saveRulersWithPrompt();
            }
            else {
                setIsONSDRuler(!isONSDRuler);
            }
        }
        else if (cClickMode == CM_BOX && keyEvent->modifiers() & Qt::CTRL) {
          saveBoxesWithPrompt();
        }
        break;
    case Qt::Key_E:
      setIWModeMax( iwModeMax() == IW_FLIP ? IW_MAX : IW_FLIP );
      update();
      break;
    case Qt::Key_L:
      switch( imageMode() )
        {
        default:
        case IMG_VAL:
          setImageMode( IMG_INV );
          update();
          break;
        case IMG_INV:
          setImageMode( IMG_LOG );
          update();
          break;
        case IMG_LOG:
          setImageMode( IMG_DX );
          update();
          break;
        case IMG_DX:
          setImageMode( IMG_DY );
          update();
          break;
        case IMG_DY:
          setImageMode( IMG_DZ );
          update();
          break;
        case IMG_DZ:
          setImageMode( IMG_BLEND );
          update();
          break;
        case IMG_BLEND:
          setImageMode( IMG_MIP );
          update();
          break;
        case IMG_MIP:
          setImageMode( IMG_VAL );
          update();
          break;
        }
      break;
    case Qt::Key_F:
      if( ++cFastPace > 2 )
        {
        cFastPace = 0;
        }
      break;
    case Qt::Key_Q:
      iwPace = cFastIWValue[ cFastPace ];
      setIWMax( iwMax()-iwPace );
      update();
      break;
    case Qt::Key_W:
      iwPace = cFastIWValue[ cFastPace ];
      setIWMax( iwMax()+iwPace );
      update();
      break;
    case ( Qt::Key_A ):
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setViewAxisLabel( !viewAxisLabel() );
        }
      else
        {
        iwPace = cFastIWValue[ cFastPace ];
        setIWMin( iwMin()-iwPace );
        }
      update();
      break;
    case Qt::Key_S:
      iwPace = cFastIWValue[ cFastPace ];
      setIWMin( iwMin()+iwPace );
      update();
      break;
    case Qt::Key_I:
      int newY;
      if( isYFlipped() )
        {
        newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
        }
      else
        {
        newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
        }
      cWinCenter[cWinOrder[1]] = newY;
      centerWindow( cWinCenter[0], cWinCenter[1], cWinCenter[2] );
      update();
      break;
    case Qt::Key_M:
      int nY;
      if( isYFlipped() )
        {
        nY = cWinCenter[cWinOrder[1]]+imgShiftSize;
        }
      else
        {
        nY = cWinCenter[cWinOrder[1]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[1]] = nY;
      centerWindow( cWinCenter[0], cWinCenter[1], cWinCenter[2] );
      update();
      break;
    case Qt::Key_J:
      int newX;
      if( isXFlipped() )
        {
        newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      else
        {
        newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = newX;
      centerWindow( cWinCenter[0], cWinCenter[1], cWinCenter[2] );
      update();
      break;
    case Qt::Key_K:
      int nX;
      if( isXFlipped() )
        {
        nX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      else
        {
        nX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = nX;
      centerWindow( cWinCenter[0], cWinCenter[1], cWinCenter[2] );
      update();
      break;
    case ( Qt::Key_T ):
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setViewClickedPoints( !viewClickedPoints() );
        }
      else
        {
        transpose( !isTransposed() );
        }
      update();
      break;
    case ( Qt::Key_C ):
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setViewCrosshairs( !viewCrosshairs() );
        }
      update();
      break;
    case ( Qt::Key_V ):
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setViewValue( !viewValue() );
        }
      update();
      break;
    case Qt::Key_P:
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setViewValuePhysicalUnits( !viewValuePhysicalUnits() );
        }
      else
        {
        saveClickedPointsStored();
        }
      update();
      break;
    case Qt::Key_D:
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        int newState = this->nextDisplayState( this->displayState() );
        this->setDisplayState( newState );
        }
      else
        {
        setIWModeMin( iwModeMin() == IW_FLIP ? IW_MIN : IW_FLIP );
        }
      update();
      break;
    case ( Qt::Key_O ):
      if( keyEvent->modifiers() & Qt::ShiftModifier )
        {
        setOverlay( !viewOverlayData() );
        update();
        }
      break;
    case Qt::Key_B:
      //decrease opacity overlay
      setOverlayOpacity( overlayOpacity() - 0.025 );
      //update();
      break;
    case Qt::Key_N:
      //increase opacity overlay
      setOverlayOpacity( overlayOpacity() + 0.025 );
      //update();
      break;
    case Qt::Key_H:
      showHelp();
      break;
    default:
      this->QWidget::keyPressEvent( keyEvent );
      break;
    }
  if( cKeyEventArgCallBack != NULL )
    {
    cKeyEventArgCallBack( keyEvent, cKeyEventArg );
    }
}


QSize QtGlSliceView::minimumSizeHint() const
{
  int winWidth = cWinSizeX;
  int winHeight = cWinSizeY;
  if( winWidth == 0 || winHeight == 0 )
    {
    winWidth = 100;
    winHeight = 100;
    }
  while( winWidth > 200 || winHeight > 200 )
    {
    winWidth /= 2;
    winHeight /= 2;
    }
  const QSize minSizeHint( winWidth, winHeight );
  return minSizeHint;
}

QSize QtGlSliceView::sizeHint()const
{
  int winWidth = cWinSizeX;
  int winHeight = cWinSizeY;
  if( winWidth == 0 || winHeight == 0 )
    {
    winWidth = 100;
    winHeight = 100;
    }
  while( winWidth > 1000 || winHeight > 1000 )
    {
    winWidth /= 2;
    winHeight /= 2;
    }
  while( winWidth < 200 && winHeight < 200 )
    {
    winWidth *= 2;
    winHeight *= 2;
    }
  const QSize winSizeHint( winWidth, winHeight );
  return winSizeHint;
}


bool QtGlSliceView::hasHeightForWidth() const
{
  return width() && height();
}


int QtGlSliceView::heightForWidth( int widgetWidth ) const
{
  // \todo Currently the view handles only square widgets.
  //return cWinSizeY ? ( width * cWinSizeY ) / cWinSizeX : width;
  return widgetWidth;
}


/** Initialize the OpenGL Window */
void QtGlSliceView::initializeGL()
{
  glClearColor( 0.0, 0.0, 0.0, 0.0 );
  glShadeModel( GL_FLAT );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
}

/** Draw */
void QtGlSliceView::paintGL( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );    //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0, width(), 0, height(), 0, 1 );

  glViewport( 0, 0, width(), height() );

  QFont widgetFont = this->font();
  widgetFont.setStyleHint(QFont::Helvetica, QFont::PreferAntialias);
  widgetFont.setBold(true);
  widgetFont.setPointSize(10);
  QFontMetrics widgetFontMetric( widgetFont );

  if( !cImData )
    {
    return;
    }

  double scale0 = this->width() / (double) cWinSizeX;
  double scale1 = this->height() / (double) cWinSizeY;
  int originX = (int)( -cWinMinX * scale0 );
  if( originX < 0 )
    {
    originX = 0;
    }
  int originY = (int)( -cWinMinY * scale1 );
  if( originY < 0 )
    {
    originY = 0;
    }

  glRasterPos2i( ( isXFlipped() )?width()-originX:originX,
     ( isYFlipped() )?height()-originY:originY );

  glPixelZoom( ( isXFlipped() )?-scale0:scale0,
     ( isYFlipped() )?-scale1:scale1 );

  if( cValidImData && cViewImData )
    {
    glDrawPixels( cWinDataSizeX, cWinDataSizeY,
                  GL_LUMINANCE, GL_UNSIGNED_BYTE,
                  cWinImData );
    }

  if( cValidOverlayData && viewOverlayData() )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDrawPixels( cWinDataSizeX, cWinDataSizeY, GL_RGBA,
       GL_UNSIGNED_BYTE, cWinOverlayData );
    glDisable( GL_BLEND );
    }

  if( viewClickedPoints() )
    {
    glColor3f( 0.8, 0.4, 0.4 );
    glPointSize( 3.0 );
    glBegin( GL_POINTS );
    for( int ii = 0; ii < clickedPointsStored(); ii++ )
      {
      ClickPoint pointClicked( cClickSelect[0], cClickSelect[1],
        cClickSelect[2], cClickSelectV );
      clickedPoint( ii, pointClicked );
      double pts[3] = { pointClicked.x, pointClicked.y, pointClicked.z };

      if( static_cast<int>( pts[this->cWinOrder[2]] ) ==
          sliceNum() )
        {
        float xx;
        if( isXFlipped() )
          {
          xx = width() - ( pts[this->cWinOrder[0]]
            - this->cWinMinX ) * scale0 - originX;
          }
        else
          {
          xx = ( pts[this->cWinOrder[0]] - this->cWinMinX ) * scale0
            + originX;
          }

        float yy;
        if( isYFlipped() )
          {
          yy = height() - ( pts[this->cWinOrder[1]]
               - this->cWinMinY ) * scale1 - originY;
          }
        else
          {
          yy = ( pts[this->cWinOrder[1]] - this->cWinMinY ) * scale1
               + originY;
          }
        glVertex2f( xx, yy );
        }
      }
    glEnd();
    }

  if( viewAxisLabel() )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.2, 0.2, 0.78, 0.75 );

    if( isXFlipped() == false )
      {
      int posY = static_cast<int>( height()/2 - widgetFontMetric.height()/2 );
      glRasterPos2i( width(), -posY );
      glCallLists( strlen( cAxisLabelX[cWinOrientation] ),
        GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation] );
      this->renderText(
        (width()-widgetFontMetric.horizontalAdvance(cAxisLabelX[cWinOrientation]))/2 - 2,
        posY, cAxisLabelX[cWinOrientation], widgetFont );
      }
    else
      {
      int posY = static_cast<int>( height()/2 - widgetFontMetric.height()/2 );
      glRasterPos2i( width(), -posY );
      glCallLists( strlen( cAxisLabelX[cWinOrientation] ),
        GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation] );
      this->renderText(
        (widgetFontMetric.horizontalAdvance(cAxisLabelX[cWinOrientation]))/2 - 2,
        posY, cAxisLabelX[cWinOrientation], widgetFont );
      }

    if( isYFlipped() == false )
      {
      int posY = static_cast<int>( widgetFontMetric.height() * 2 ) ;
      glRasterPos2i( this->width()/2, -posY );
      glCallLists( strlen( cAxisLabelY[cWinOrientation] ),
        GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation] );
      this->renderText(
        (width()-widgetFontMetric.horizontalAdvance(cAxisLabelY[cWinOrientation]))/2 - 2,
        posY,
        cAxisLabelY[cWinOrientation], widgetFont );
      }
    else
      {
      int posY = static_cast<int>( height() - widgetFont.pointSize() * 2 );
      glRasterPos2i( this->width()/2, -posY );
      glCallLists( strlen( cAxisLabelY[cWinOrientation] ),
        GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation] );
      this->renderText(
        width()/2  + widgetFontMetric.horizontalAdvance(cAxisLabelY[cWinOrientation])/2,
        posY,
        cAxisLabelY[cWinOrientation], widgetFont );
      }

    glDisable( GL_BLEND );
    }

  if( cClickMode == CM_SELECT )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, ( double )0.75 );
    char s[80];
    if( viewValuePhysicalUnits() )
      {
      sprintf( s, "SELECT: Physical points" );
      }
    else
      {
      sprintf( s, "SELECT: Index points" );
      }
    int posX = width() - widgetFontMetric.horizontalAdvance(s)
      - widgetFontMetric.horizontalAdvance("00");
    int posY = height() - 2 * ( widgetFontMetric.height() + 1 );
    this->renderText( posX, posY, s, widgetFont );
    glDisable( GL_BLEND );
    }
  else if( cClickMode == CM_CUSTOM )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, ( double )0.75 );
    char s[80];
    sprintf( s, "CUSTOM: Thresh Conn. Comp." );
    int posX = width() - widgetFontMetric.horizontalAdvance(s)
      - widgetFontMetric.horizontalAdvance("00");
    int posY = height() - 2 * ( widgetFontMetric.height() + 1 );
    this->renderText( posX, posY, s, widgetFont );
    glDisable( GL_BLEND );
    }
  else if (cClickMode == CM_RULER)
  {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (double)0.75);
      char s[80];
      sprintf(s, "RULER: Ruler Widget");
      int posX = width() - widgetFontMetric.horizontalAdvance(s)
          - widgetFontMetric.horizontalAdvance("00");
      int posY = height() - 2 * (widgetFontMetric.height() + 1);
      this->renderText(posX, posY, s, widgetFont);

      RulerTool* r = getRulerToolCollection()->getActive();
      if (r != nullptr) {
          auto d = r->length();
          sprintf(s, "%s: Length (mm) : %7.2f", r->metaData->name.c_str(), d);
          posX = width() - widgetFontMetric.horizontalAdvance(s)
              - widgetFontMetric.horizontalAdvance("00");
          posY = height() - 4 * (widgetFontMetric.height() + 1);
          this->renderText(posX, posY, s, widgetFont);
      }
      //QPainter painter(this);
      //painter.setPen(Qt::blue);
      //painter.drawLine(QLine(0, 0, 100, 100));
      // if there is a selected/active ruler, then displace its length


      getRulerToolCollection()->paint();
      glDisable(GL_BLEND);
  }
  else if (cClickMode == CM_BOX)
  {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (double)0.75);
      char s[80];
      sprintf(s, "BOX: Box Widget");
      int posX = width() - widgetFontMetric.horizontalAdvance(s)
          - widgetFontMetric.horizontalAdvance("00");
      int posY = height() - 2 * (widgetFontMetric.height() + 1);
      this->renderText(posX, posY, s, widgetFont);

      BoxTool* b = getBoxToolCollection()->getActive();
      if (b != nullptr) {
          auto a = b->area(cWinOrder[2]);
          sprintf(s, "%s: Area (mm^3) : %7.2f", b->metaData->name.c_str(), a);
          posX = width() - widgetFontMetric.horizontalAdvance(s)
              - widgetFontMetric.horizontalAdvance("00");
          posY = height() - 4 * (widgetFontMetric.height() + 1);
          this->renderText(posX, posY, s, widgetFont);
      }
      glDisable(GL_BLEND);
  }
  else if( cValidOverlayData && cClickMode == CM_PAINT )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, ( double )0.75 );
    char s[80];
    sprintf( s, "PAINT: R = %d  C = %d", cOverlayPaintRadius,
      cOverlayPaintColor );
    int posX = width() - widgetFontMetric.horizontalAdvance(s)
      - widgetFontMetric.horizontalAdvance("00");
    int posY = height() - 2 * ( widgetFontMetric.height() + 1 );
    this->renderText( posX, posY, s, widgetFont );
    glDisable( GL_BLEND );
    }

  if (this->viewOverlayData()) {
    glEnable(GL_BLEND);
    getBoxToolCollection()->paint();
    glDisable(GL_BLEND);
  }

  if( cWorkflowSteps.size() != 0 ) {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, ( double )0.75 );
    char s[80];

    auto& step = cWorkflowSteps[cWorkflowIndex];

      if (step->type == CM_PAINT) {

        auto paintStep = static_cast<PaintStep*>(step.get());

        sprintf( s, "STEP: %d (PAINT), L: %s", cWorkflowIndex, paintStep->name.c_str());

      } else if (step->type == CM_BOX) {

        auto boxStep = static_cast<BoxStep*>(step.get());

        sprintf( s, "STEP: %d (BOX), L: %s", cWorkflowIndex, boxStep->name.c_str());
      } else if (step->type == CM_RULER)
      {

        auto rulerStep = static_cast<RulerStep*>(step.get());

        sprintf(s, "STEP: %d (RULER)", cWorkflowIndex);
      }

    int posX = width() - widgetFontMetric.horizontalAdvance(s)
      - widgetFontMetric.horizontalAdvance("00");
    int posY = height() - 3 * ( widgetFontMetric.height() + 1 );
    this->renderText( posX, posY, s, widgetFont );
    glDisable( GL_BLEND );
  }

  if( viewValue() )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, ( double )0.75 );
    char s[80];
    double px, py, pz, val = this->cClickSelectV;
    ClickPoint point( cClickSelect[0], cClickSelect[1], cClickSelect[2],
      cClickSelectV );
    px = point.x;
    py = point.y;
    pz = point.z;
    const char * suffix = "";
    if( viewValuePhysicalUnits() )
      {
      IndexType index;
      index[0] = static_cast< itk::IndexValueType >(
        this->cClickSelect[0] );
      index[1] = static_cast< itk::IndexValueType >(
        this->cClickSelect[1] );
      index[2] = static_cast< itk::IndexValueType >(
        this->cClickSelect[2] );

      ImageType::PointType pnt;
      cImData->TransformIndexToPhysicalPoint( index, pnt );
      px = pnt[0];
      py = pnt[1];
      pz = pnt[2];

      suffix = this->cPhysicalUnitsName;
      }
    if( ( ImagePixelType )1.5==1.5 )
      {
      sprintf( s, "( %0.1f%s,  %0.1f%s,  %0.1f%s ) = %0.3f",
              px, suffix,
              py, suffix,
              pz, suffix,
              val );
      }
    else
      {
      sprintf( s, "( %0.1f%s,  %0.1f%s,  %0.1f%s ) = %d",
              px, suffix,
              py, suffix,
              pz, suffix,
              ( int )val );
      }
    int posX = width() - widgetFontMetric.horizontalAdvance(s)
      - widgetFontMetric.horizontalAdvance("00");
    int posY = height() - ( widgetFontMetric.height() + 1 );
    this->renderText( posX, posY, s, widgetFont );
    glDisable( GL_BLEND );
    }

  QStringList details;
  if( this->orientation() == X_AXIS )
    {
    details << QString( "X - Slice: %1" ).arg( this->windowCenterX() );
    }
  else if( orientation() == Y_AXIS )
    {
    details << QString( "Y - Slice: %1" ).arg( this->windowCenterY() );
    }
  else if( orientation() == Z_AXIS )
    {
    details << QString( "Z - Slice: %1" ).arg( this->windowCenterZ() );
    }
  details << QString( "Dims: %1 x %2 x %3" )
    .arg( this->cDimSize[0] )
    .arg( this->cDimSize[1] )
    .arg( this->cDimSize[2] );
  details << QString( "Voxel: %1 x %2 x %3" )
    .arg( this->cSpacing[0], 0, 'f' )
    .arg( this->cSpacing[1], 0, 'f' )
    .arg( this->cSpacing[2], 0, 'f' );
  details << QString( "Int. Range: %1 - %2" )
    .arg( this->cDataMin, 0, 'f' )
    .arg( this->cDataMax, 0, 'f' );
  details << QString( "Int. Window: %1( %2 ) - %3( %4 )" )
    .arg( this->cIWMin, 0, 'f' )
    .arg( IWModeTypeName[this->cIWModeMin] )
    .arg( this->cIWMax, 0, 'f' )
    .arg( IWModeTypeName[this->cIWModeMax] );
  details << QString( "View Mode: %1" ).arg(
    ImageModeTypeName[this->cImageMode] );

  if( this->cDisplayState & 0x01 )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.9, 0.4, 0.1, ( double )0.75 );

    int i = 6;
    foreach( QString text, details )
      {
      int posX = widgetFontMetric.horizontalAdvance("00");
      int posY = height() - ( i * ( widgetFontMetric.height() + 1 ) );
      this->renderText( posX, posY, text, widgetFont );
      --i;
      }
    glDisable( GL_BLEND );
    }
  QString str = details.join( "\n" );
  emit detailsChanged( str );

  if( cMessage.size() != 0 )
    {
    int posX = (width()/2) - (widgetFontMetric.horizontalAdvance(cMessage)/2);
    int posY = ( 2 * ( widgetFontMetric.height() + 1 ) );
    this->renderText( posX, posY, cMessage, widgetFont );
    }

  if( viewCrosshairs()
    && static_cast<int>( cClickSelect[cWinOrder[2]] ) ==
       static_cast<int>( sliceNum() ) )
    {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 0.1, 0.64, 0.2, 0.75 );
    int posX;
    if( isXFlipped() )
      {
      posX = ( int )( width() - ( cClickSelect[cWinOrder[0]] - cWinMinX )
        * scale0 );
      }
    else
      {
      posX = ( int )( ( cClickSelect[cWinOrder[0]] - cWinMinX ) * scale0 );
      }
    int posY;
    if( isYFlipped() )
      {
      posY = ( int )( height() - ( cClickSelect[cWinOrder[1]] - cWinMinY )
        * scale1 );
      }
    else
      {
      posY = ( int )( ( cClickSelect[cWinOrder[1]] - cWinMinY ) * scale1 );
      }
    glBegin( GL_LINES );
    glVertex2d( 0, posY );
    glVertex2d( posX-2, posY );
    glVertex2d( posX+2, posY );
    glVertex2d( this->width()-1, posY );
    glVertex2d( posX, 0 );
    glVertex2d( posX, posY-2 );
    glVertex2d( posX, posY+2 );
    glVertex2d( posX, this->height()-1 );
    glEnd();
    glDisable( GL_BLEND );
    }
}

QtGlSliceView::PointType2D QtGlSliceView::indexToScreenPoint(const PointType3D& index) {
    double scale0 = this->width() / (double)cWinSizeX;
    double scale1 = this->height() / (double)cWinSizeY;

    double x, y;
    if (isXFlipped()) {
        x = -((index[cWinOrder[0]] - cWinMinX) * scale0 - width() + 1);
    }
    else {
        x = (index[cWinOrder[0]] - cWinMinX) * scale0;
    }


    if (isYFlipped())
    {
        y = (index[cWinOrder[1]] - cWinMinY) * scale1;
    }
    else
    {
        y = -((index[cWinOrder[1]] - cWinMinY) * scale1 - height() + 1);
    }

    double p[2] = { x, y };
    return PointType2D(p);
}

QtGlSliceView::PointType3D QtGlSliceView::screenPointToIndex(double x, double y) {
    double scale0 = this->width() / (double)cWinSizeX;
    double scale1 = this->height() / (double)cWinSizeY;
    int originX = (int)(-cWinMinX * scale0);
    if (originX < 0)
    {
        originX = 0;
    }
    int originY = (int)(-cWinMinY * scale1);
    if (originY < 0)
    {
        originY = 0;
    }
 
    double p[3];
    if (isXFlipped())
    {
        p[cWinOrder[0]] =
            ((width() - 1 - x)) / scale0 + cWinMinX;
    }
    else
    {
        p[cWinOrder[0]] =
            (x) / scale0 + cWinMinX;
    }
    if (p[cWinOrder[0]] < cWinMinX)
    {
        p[cWinOrder[0]] = cWinMinX;
    }
    if (p[cWinOrder[0]] > cWinMaxX)
    {
        p[cWinOrder[0]] = cWinMaxX;
    }
    if (isYFlipped())
    {
        p[cWinOrder[1]] =
            (y) / scale1 + cWinMinY;
    }
    else
    {
        p[cWinOrder[1]] =
            ((height() - 1 - y)) / scale1 + cWinMinY;
    }
    if (p[cWinOrder[1]] < cWinMinY)
    {
        p[cWinOrder[1]] = cWinMinY;
    }
    if (p[cWinOrder[1]] > cWinMaxY)
    {
        p[cWinOrder[1]] = cWinMaxY;
    }
    if (imageMode() != IMG_MIP)
    {
        p[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
    }
    else
    {
        p[cWinOrder[2]] = cWinZBuffer[(int)p[cWinOrder[0]]
            - cWinMinX
            + ((int)p[cWinOrder[1]]
                - cWinMinY)
            * cWinDataSizeX];
    }

    return PointType3D(p);
}

QtGlSliceView::PointType3D QtGlSliceView::indexToPhysicalPoint(const PointType3D& indexPoint) {
    itk::ContinuousIndex<double, 3> idx{ };
    idx.SetElement(0, indexPoint[0]);
    idx.SetElement(1, indexPoint[1]);
    idx.SetElement(2, indexPoint[2]);
    PointType3D ans{ };
    this->cImData->TransformContinuousIndexToPhysicalPoint(idx, ans);
    return ans;
}

void QtGlSliceView::mouseSelectEvent( QMouseEvent* mouseEvent )
{
  if( !cImData )
    {
    return;
    }

  if (cClickMode == CM_SELECT || cClickMode == CM_PAINT ||
      cClickMode == CM_CUSTOM || cClickMode == CM_RULER ||
      cClickMode == CM_BOX)
  {
      auto p = screenPointToIndex(mouseEvent->x(), mouseEvent->y()).GetDataPointer();

      if (cClickMode == CM_SELECT || cClickMode == CM_CUSTOM)
      {
          selectPoint(p[0], p[1], p[2]);
      }
      if (cClickMode == CM_PAINT)
      {
          selectPoint(p[0], p[1], p[2]);
          paintOverlayPoint(p[0], p[1], p[2]);
      }
      if (cClickMode == CM_RULER) {
          getRulerToolCollection()->handleMouseEvent(mouseEvent, p);
      }
      if (cClickMode == CM_BOX) {
          getBoxToolCollection()->handleMouseEvent(mouseEvent, p);
      }
  }
  this->update();
}

/** catches the mouse press to react appropriate
 *  Overriden to catch mousePressEvents and to start an internal
 *  timer, which calls the appropriate interaction routine */
void QtGlSliceView::mousePressEvent( QMouseEvent* mouseEvent )
{
  if( mouseEvent->button() & Qt::LeftButton )
    {
    cSelectMovement = SM_PRESS;
    this->mouseSelectEvent( mouseEvent );
    }
}

void QtGlSliceView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
  cSelectMovement = SM_MOVE;
  //printf("mouseMoveEvent");

  // previous code didn't have mouse tracking enabled, but could handle mouse move events (i.e. when a button was held down)
  if (
    cClickMode == CM_RULER ||
    cClickMode == CM_BOX ||
    (mouseEvent->buttons() != Qt::NoButton)
  ) { // other modes don't handle moveEvents well
      this->mouseSelectEvent(mouseEvent);
  }
}

void QtGlSliceView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
  // pass all ReleaseEvents if CM_RULER enabled
  if( mouseEvent->button() & Qt::LeftButton || cClickMode == CM_RULER || cClickMode == CM_BOX)
    {
    cSelectMovement = SM_RELEASE;
    this->mouseSelectEvent( mouseEvent );
    }
}

void QtGlSliceView::changeSlice( int value )
{
  this->setSliceNum( value );
  this->update();
}


void QtGlSliceView::setSliceNum( int newSliceNum )
{
  newSliceNum = qMin( newSliceNum, static_cast<int>( cDimSize[cWinOrder[2]] ) - 1 );
  if( newSliceNum == cWinCenter[cWinOrder[2]] )
    {
    return;
    }
  cWinCenter[cWinOrder[2]] = newSliceNum;

  if( cSliceNumCallBack != NULL )
    {
    cSliceNumCallBack();
    }
  if( cSliceNumArgCallBack != NULL )
    {
    cSliceNumArgCallBack( cSliceNumArg );
    }
  emit sliceNumChanged( newSliceNum );
}


int QtGlSliceView::sliceNum() const
{
  return cWinCenter[cWinOrder[2]];
}


int QtGlSliceView::maxSliceNum() const
{
  return cDimSize[cWinOrder[2]];
}


void QtGlSliceView::selectPoint( double newX, double newY, double newZ )
  {
  cClickSelect[0] = newX;
  if( cClickSelect[0]<0 )
    cClickSelect[0] = 0;
  if( cClickSelect[0] >= cDimSize[0] )
    cClickSelect[0] = cDimSize[0]-1;

  cClickSelect[1] = newY;
  if( cClickSelect[1]<0 )
    cClickSelect[1] = 0;
  if( cClickSelect[1] >= cDimSize[1] )
    cClickSelect[1] = cDimSize[1]-1;

  cClickSelect[2] = newZ;
  if( cClickSelect[2]<0 )
    cClickSelect[2] = 0;
  if( cClickSelect[2] >= cDimSize[2] )
    cClickSelect[2] = cDimSize[2]-1;

  ImageType::IndexType ind;

  ind[0] = ( unsigned long )cClickSelect[0];
  ind[1] = ( unsigned long )cClickSelect[1];
  ind[2] = ( unsigned long )cClickSelect[2];
  cClickSelectV = cImData->GetPixel( ind );

  /*if length of list is equal to max, remove the earliest point stored */
  if( ( cMaxClickPoints>0 )&&( cClickedPoints.size() == cMaxClickPoints ) )
    {
    cClickedPoints.pop_back();
    }
  ClickPoint pointClicked( cClickSelect[0], cClickSelect[1], cClickSelect[2],
    cClickSelectV );
  cClickedPoints.push_front( pointClicked );

  if( cClickMode == CM_CUSTOM && cClickSelectCallBack != NULL )
    {
    cClickSelectCallBack( cClickSelect[0], cClickSelect[1],
      cClickSelect[2], cClickSelectV );
    }
  if( cClickMode == CM_CUSTOM && cClickSelectArgCallBack != NULL )
    {
    cClickSelectArgCallBack( cClickSelect[0], cClickSelect[1],
      cClickSelect[2], cClickSelectV, cClickSelectArg );
    }

  emit positionChanged( cClickSelect[0], cClickSelect[1], cClickSelect[2],
    cClickSelectV );

}

void QtGlSliceView::setFastMoveValue( int movVal )
{
  cFastMoveValue[ 2 ] = movVal;
}


int QtGlSliceView::fastMoveValue() const
{
  return cFastMoveValue[ 2 ];
}


void QtGlSliceView::setFastIWValue( double movVal )
{
  cFastIWValue[ 2 ] = movVal;
}


double QtGlSliceView::fastIWValue() const
{
  return cFastIWValue[ 2 ];
}


bool QtGlSliceView::viewAxisLabel() const
{
  return cViewAxisLabel;
}


bool QtGlSliceView::viewClickedPoints() const
{
  return cViewClickedPoints;
}


bool QtGlSliceView::viewValuePhysicalUnits() const
{
  return cViewValuePhysicalUnits;
}


double QtGlSliceView::intensityRange() const
{
  return cDataMax - cDataMin;
}


double QtGlSliceView::minIntensity() const
{
  return cDataMin;
}


double QtGlSliceView::maxIntensity() const
{
  return cDataMax;
}


double QtGlSliceView::iwMin() const
{
  return cIWMin;
}


double QtGlSliceView::iwMax() const
{
  return cIWMax;
}


void QtGlSliceView::setIWMin( double value )
{
  value = qBound( cDataMin, value, cDataMax );
  if( qFuzzyCompare( value, cIWMin ) )
    {
    return;
    }
  cIWMin = value;
  update();
  emit iwMinChanged( cIWMin );
}


void QtGlSliceView::setIWMax( double value )
{
  value = qBound( cDataMin, value, cDataMax );
  if( qFuzzyCompare( value, cIWMax ) )
    {
    return;
    }
  cIWMax = value;
  update();
  emit iwMaxChanged( cIWMax );
}


bool QtGlSliceView::clickedPoint( int index, ClickPoint& point )
{
  if( index >= cClickedPoints.size() )
    {
    return false;
    }
  ClickPointListType::const_iterator j = cClickedPoints.begin();

  std::advance( j, static_cast<int>( index ) );
  point = *j;
  return true;

}


int QtGlSliceView::clickedPointsStored() const
{
  return cClickedPoints.size();
}


int QtGlSliceView::maxClickedPointsStored() const
{
  return cMaxClickPoints;
}


void QtGlSliceView::clearClickedPointsStored()
{
  cClickedPoints.clear();
}


void QtGlSliceView::deleteLastClickedPointsStored()
{
  cClickedPoints.pop_front();
}


void QtGlSliceView::setViewValuePhysicalUnits( bool physicalValueUnits )
{
  cViewValuePhysicalUnits = physicalValueUnits;
}


void QtGlSliceView::setViewClickedPoints( bool pointsClicked )
{
  cViewClickedPoints = pointsClicked;
}


void QtGlSliceView::zoomIn()
{
  if( zoom() < 1 )
    {
    setZoom( zoom() * 2);
    }
  else
    {
    setZoom( static_cast<int>( zoom() + 1 ) );
    }
  update();
}

void QtGlSliceView::setInputImageFilepath(QString filepath) {
    this->inputImageFilepath = filepath;
}

void QtGlSliceView::zoomOut()
{
  if( zoom() <= 1 )
    {
    setZoom( zoom() / 2);
    }
  else
    {
    setZoom( static_cast<int>( zoom() - 1 ) );
    }
  update();
}


void QtGlSliceView::setMaxClickedPointsStored( int i )
{
  cMaxClickPoints = i;
}

void QtGlSliceView::setMessage( const std::string & str )
{
  cMessage = QString::fromUtf8(str.c_str());
}

void QtGlSliceView::renderText( double x, double y, const QString & str,
  const QFont & font )
{
  if( this->isVisible() )
    {
    QPainterPath textPath;
    textPath.addText( x, y, font, str );

    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing |
      QPainter::TextAntialiasing );
    painter.setBrush( Qt::red );
    painter.setPen( Qt::NoPen );
    painter.drawPath( textPath );
    painter.end();
    }
}

RulerToolCollection* QtGlSliceView::getRulerToolCollection() {
    auto axis_slice = std::pair<int, int>(cWinOrder[2], this->sliceNum());
    if (cRulerCollections.find(axis_slice) == cRulerCollections.end()) {
        std::unique_ptr< RulerToolCollection > rc(new RulerToolCollection(this, cCurrentRulerMetaFactory, cWinOrder[2], this->sliceNum())); // TODO: figure out which axis we're talking about (remove 2)
        cRulerCollections[axis_slice] = std::move(rc);
    }
    return cRulerCollections[axis_slice].get();
}

BoxToolCollection* QtGlSliceView::getBoxToolCollection() {
    return this->getBoxToolCollection(cWinOrder[2], this->sliceNum());
}

BoxToolCollection* QtGlSliceView::getBoxToolCollection(int axis, int sliceNum) {
    auto axis_slice = std::pair<int, int>(axis, sliceNum);
    if (cBoxCollections.find(axis_slice) == cBoxCollections.end()) {
        std::unique_ptr< BoxToolCollection > rc(new BoxToolCollection(this, cCurrentBoxMetaFactory, axis, sliceNum)); // TODO: figure out which axis we're talking about (remove 2)
        cBoxCollections[axis_slice] = std::move(rc);
    }
    return cBoxCollections[axis_slice].get();
}

void QtGlSliceView::setIsONSDRuler(bool flag) {
    isONSDRuler = !isONSDRuler;
    cCurrentRulerMetaFactory = isONSDRuler ? cONSDMetaFactory : cRainbowMetaFactory;

    this->getRulerToolCollection()->setMetaDataFactory(cCurrentRulerMetaFactory);
}

void QtGlSliceView::addBox(
    std::string name,
    int axis,
    int slice,
    double point1[],
    double point2[]
) {
  auto collection = this->getBoxToolCollection(axis, slice);
  BoxTool* box = collection->createBox(point1, point2);
  box->metaData.get()->name = name;
}
#endif
