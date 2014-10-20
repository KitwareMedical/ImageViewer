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

#ifndef QtGlSliceView_cxx
#define QtGlSliceView_cxx

//QtImageViewer include
#include "QtGlSliceView.h"
#include "ui_QtImageViewerHelp.h"

//itk include
#include "itkMinimumMaximumImageCalculator.h"

//std includes
#include <cmath>

// Qt includes
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollArea>

QtGlSliceView::QtGlSliceView(QWidget* widgetParent)
  : QGLWidget(widgetParent)
{
  cDisplayState         = 0x01;
  cMaxDisplayStates     = 2; // Off and On.
  cValidOverlayData     = false;
  cSingleStep           = 1.0;
  cViewOverlayData      = false;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cHelpDialog = 0;
  cViewValuePhysicalUnits = false;
  cPhysicalUnitsName = "mm";
  cViewClickedPoints = false;
  cViewCrosshairs = true;
  cViewValue = true;
  cClickMode = CM_SELECT;

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

  cClickBoxCallBack = NULL;
  cClickBoxArg = NULL;
  cClickBoxArgCallBack = NULL;

  cIWCallBack = NULL;
  cIWArg = NULL;
  cIWArgCallBack = NULL;

  cViewAxisLabel = false;
  sprintf(cAxisLabelX[0], "S");
  sprintf(cAxisLabelX[1], "L");
  sprintf(cAxisLabelX[2], "L");
  sprintf(cAxisLabelY[0], "P");
  sprintf(cAxisLabelY[1], "S");
  sprintf(cAxisLabelY[2], "P");
  cOverlayData = NULL;
  cImData = NULL;
  cClickSelectV = 0;
  cViewImData  = true;
  cValidImData = true;

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
  cW = 0;
  cH = 0;
  for (unsigned int i=0; i < 3; ++i)
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
  cfastMovVal = 1; //fast moving pace: 1 by defaut
  cfastMovThresh = 10; //how many single step moves before fast moving

  QSizePolicy sP = this->sizePolicy();
  sP.setHeightForWidth(true);
  this->setSizePolicy(sP);
  update();
}


void
QtGlSliceView::
setInputImage(ImageType * newImData)
{
  if (!newImData)
    {
    return;
    }

  RegionType region = newImData->GetLargestPossibleRegion();
  if (region.GetNumberOfPixels() == 0)
    {
    return;
    }

  SizeType myImageSize = region.GetSize();
  if (cValidOverlayData)
    {
    RegionType overlayRegion = cOverlayData->GetLargestPossibleRegion();
    SizeType   overlaySize   = overlayRegion.GetSize();

    for (int i=0; i<3; i++)
      {
      if (myImageSize[i] != overlaySize[i])
        {
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

  calculator->SetImage(cImData);
  calculator->Compute();

  cDataMin = calculator->GetMinimum();
  cDataMax = calculator->GetMaximum();
  this->setIWMin(cDataMin);
  this->setIWMax(cDataMax);

  cWinCenter[0] = cDimSize[0]/2;
  cWinCenter[1] = cDimSize[1]/2;
  cWinCenter[2] = 0;

  cWinMinX  = 0;
  cWinSizeX = qMax(cDimSize[0], qMax( cDimSize[1], cDimSize[2]) );
  cWinMaxX = static_cast<int>(cWinSizeX) - 1;

  cWinMinY = 0;
  cWinSizeY = cWinSizeX;
  cWinMaxY = static_cast<int>(cWinSizeY) - 1;

  cWinDataSizeX = cDimSize[0];
  cWinDataSizeY = cDimSize[1];

  if (cWinImData != NULL)
    {
    delete [] cWinImData;
    }

  cWinImData = new unsigned char[ cWinDataSizeX * cWinDataSizeY ];

  if (cWinZBuffer != NULL)
    {
    delete [] cWinZBuffer;
    }

  cWinZBuffer = new unsigned short[ cWinDataSizeX * cWinDataSizeY ];
  this->changeSlice(((this->maxSliceNum() -1)/2));
  this->updateGeometry();
  this->update();
  emit imageChanged();
}


const QtGlSliceView::ImagePointer &
QtGlSliceView
::inputImage(void) const
{
  return cImData;
}


void 
QtGlSliceView
::setInputOverlay(OverlayType * newOverlayData)
{
  RegionType newoverlay_region = newOverlayData->GetLargestPossibleRegion();

  SizeType newoverlay_size = newoverlay_region.GetSize();

  RegionType cImData_region = cImData->GetLargestPossibleRegion();

  SizeType cImData_size = cImData_region.GetSize();

  if (!cValidImData || newoverlay_size[2]==cImData_size[2])
    {
    cOverlayData = newOverlayData;

    cViewOverlayData  = true;
    cValidOverlayData = true;
    cOverlayOpacity   = 1.0;

    if(cWinOverlayData != NULL) 
      {
      delete [] cWinOverlayData;
      }

    cWinOverlayData = new unsigned char[ cWinDataSizeX * cWinDataSizeY * 4 ];

    if (cWinZBuffer != NULL)
      {
      delete [] cWinZBuffer;
      }
    cWinZBuffer = new unsigned short[cWinDataSizeX * cWinDataSizeY * 4];
    emit validOverlayDataChanged(cValidOverlayData);
    update();
    }
  else
    {
    qWarning()<<"Overlay path invalid, make sure both images have the same size.";
    }
}


const QtGlSliceView::OverlayType::Pointer &
QtGlSliceView::inputOverlay(void) const
{
  return cOverlayData;
}


void 
QtGlSliceView::
setViewOverlayData(bool newViewOverlayData)
{
  cViewOverlayData = newViewOverlayData;
  paintGL();
}


bool 
QtGlSliceView::viewOverlayData(void) const
{
  return cViewOverlayData;
}


void 
QtGlSliceView::setOverlayOpacity(double newOverlayOpacity)
{
  cOverlayOpacity = qBound(0., newOverlayOpacity, 1.);
  emit overlayOpacityChanged(cOverlayOpacity);
  update();
}


double
QtGlSliceView::overlayOpacity(void) const
{
  return cOverlayOpacity;
}


QtGlSliceView::ColorTableType* QtGlSliceView::colorTable(void) const
{
  return cColorTable.GetPointer();
}


void QtGlSliceView::saveClickedPointsStored()
{
  QString fileName = QFileDialog::getSaveFileName(this,"Please select a file name","*.*","");
  if(fileName.isNull())
    {
    return;
    }
  QFile fpoints(fileName);
  if(!fpoints.open(QIODevice::ReadWrite))
    {
    return;
    }
  ClickPointListType::const_iterator point;
  QTextStream text(&fpoints);
  for(point = cClickedPoints.begin(); point != cClickedPoints.end(); point++)
    {
    text << (*point).x<<" "<<(*point).y<<" "<<(*point).z<<" "<< (*point).value<<endl;
    }
  fpoints.close();
}


void 
QtGlSliceView::update()
{
  if(!cValidImData)
    {
    return;
    }
  int winWidth = (int)(cDimSize[ cWinOrder[0] ] / cWinZoom);
  cWinSizeX = ((int) winWidth);
  int ti = (int)((int)cWinCenter[ cWinOrder[0] ] - winWidth/2);
  if(ti <= - (int) cDimSize[ cWinOrder[0] ])
    {
    ti = -cDimSize[ cWinOrder[0] ] + 1;
    }
  else if(ti >= (int)cDimSize[ cWinOrder[0] ])
    {
    ti = cDimSize[ cWinOrder[0] ] - 1;
    }
  cWinMinX = ti;
  cWinMaxX = cDimSize[ cWinOrder[0] ] - 1; // here
  if(cWinMaxX >= static_cast<int>(cDimSize[ cWinOrder[0] ]))
    {
    cWinMaxX = cDimSize[ cWinOrder[0] ] - 1;
    }
  
  winWidth = static_cast<int>(cDimSize[ cWinOrder[1] ] / cWinZoom);
  cWinSizeY = (static_cast<int>(winWidth));
  ti = static_cast<int>(static_cast<int>(cWinCenter[ cWinOrder[1] ]) - winWidth/2);
  if(ti <= - static_cast<int>(cDimSize[ cWinOrder[1] ]))
    {
    ti = -cDimSize[ cWinOrder[1] ] + 1;
    }
  else if(ti >= static_cast<int>(cDimSize[ cWinOrder[1] ]))
    {
    ti = cDimSize[ cWinOrder[1] ] - 1;
    }
  cWinMinY = ti;
  cWinMaxY = cDimSize[ cWinOrder[1] ] - 1; // here
  if(cWinMaxY >= static_cast<int>(cDimSize[ cWinOrder[1] ]))
    {
    cWinMaxY = cDimSize[ cWinOrder[1] ] - 1;
    }
  
  memset(cWinImData, 0, cWinDataSizeX*cWinDataSizeY);
  if(cValidOverlayData)
    {
    memset(cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4);
    }
  
  IndexType ind;
  
  int l, m;
  
  double tf;
  
  ind[ cWinOrder[ 2 ] ] = cWinCenter[ cWinOrder[ 2 ] ];
  int startK = cWinMinY;
  if(startK<0)
    startK = 0;
  int startJ = cWinMinX;
  if(startJ<0)
    startJ = 0;
  for(int k=startK; k <= cWinMaxY; k++)
    {
    ind[cWinOrder[1]] = k;
    
    if(k-cWinMinY >= (int)cWinDataSizeY)
      continue;

    for(int j=startJ; j <= cWinMaxX; j++) 
      {
      ind[cWinOrder[0]] = j;
      
      if(j-cWinMinX >= (int)cWinDataSizeX)
         continue;

      switch(cImageMode)
        {
        default:
        case IMG_VAL:
          tf = (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
          break;
        case IMG_INV:
          tf = (double)((cIWMax-cImData->GetPixel(ind))/(cIWMax-cIWMin)*255);
          break;
        case IMG_LOG:
          tf = (double)(log(cImData->GetPixel(ind)-cIWMin+0.00000001)
            /log(cIWMax-cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
            {
            tf = (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]--;
            tf -= (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DY:
          if(ind[1]>0) 
            {
            tf = (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]--;
            tf -= (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DZ:
          if(ind[2]>0) 
            {
            tf = (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]--;
            tf -= (double)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
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
          const int tempval = (int)cWinCenter[cWinOrder[2]]-1;
          int tmpI = ind[cWinOrder[2]];
          ind[cWinOrder[2]] = (tempval < 0) ? 0 : tempval;
          tf = (double)(cImData->GetPixel(ind));
          
          ind[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          tf += (double)(cImData->GetPixel(ind))*2;
          
          const int tempval1 = (int)cDimSize[cWinOrder[2]]-1;
          const int tempval2 = (int)cWinCenter[cWinOrder[2]]+1;
          ind[cWinOrder[2]] = (tempval1 < tempval2) ? tempval1 : tempval2;
          tf += (double)(cImData->GetPixel(ind));
          
          tf = (double)((tf/4-cIWMin)/(cIWMax-cIWMin)*255);
          ind[cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf = cIWMin;
          m = (j-cWinMinX) + (k-cWinMinY)*cWinDataSizeX;
          cWinZBuffer[m] = 0;
          int tmpI = ind[cWinOrder[2]];
          for(l=0; l<(int)cDimSize[cWinOrder[2]]; l++) 
            {
            ind[cWinOrder[2]] = l;        
            if(cImData->GetPixel(ind) > tf) 
              {
              tf = (double)(cImData->GetPixel(ind));
              cWinZBuffer[m] = (unsigned short)l;
              }
            }
          tf = (double)((tf-cIWMin)/(cIWMax-cIWMin)*255);
          ind[cWinOrder[2]] = tmpI;
          break;
          }
      
      if(tf > 255)
        {
        switch(cIWModeMax) 
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
            if(tf<0) 
              {
              tf = 0;
              }
            break;
          }
        }
      else 
        {
        if(tf < 0)
          {
          switch(cIWModeMin) 
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
              if(tf>255)
                {
                tf = 255;
                }
              break;
            }
          }
        }
      
      l = (j-cWinMinX) + (k-cWinMinY)*cWinDataSizeX;
      cWinImData[l] = (unsigned char)tf;
      
      if(cValidOverlayData)
        {
        l = l * 4;
        if(cImageMode == IMG_MIP)
          {
          ind[cWinOrder[2]] = cWinZBuffer[(j-cWinMinX) + 
            (k-cWinMinY)*cWinDataSizeX];
          }
        
        if(sizeof(OverlayPixelType) == 1)
          {
          m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
          if(m > 0)
            {
            m = m - 1;
            cWinOverlayData[l+0] = 
              (unsigned char)(cColorTable->GetColor(m).GetRed()*255);
            cWinOverlayData[l+1] = 
              (unsigned char)(cColorTable->GetColor(m).GetGreen()*255);
            cWinOverlayData[l+2] = 
              (unsigned char)(cColorTable->GetColor(m).GetBlue()*255);
            cWinOverlayData[l+3] = 
              (unsigned char)(cOverlayOpacity*255);
            }
          }
        else 
          {
          if(((unsigned char *)&(cOverlayData->GetPixel(ind)))[0]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2] > 0)
              {
            if(sizeof(OverlayPixelType) == 3)
              {
              cWinOverlayData[l+0] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
              cWinOverlayData[l+1] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
              cWinOverlayData[l+2] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
              cWinOverlayData[l+3] = 
                (unsigned char)(cOverlayOpacity*255);
              }
            else 
              {
              if(sizeof(OverlayPixelType) == 4)
                {
                cWinOverlayData[l+0] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
                cWinOverlayData[l+1] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
                cWinOverlayData[l+2] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
                cWinOverlayData[l+3] = 
                  (unsigned char)(((unsigned char *)
                  &(cOverlayData->GetPixel(ind)))[3]*cOverlayOpacity);
                }
              }
            }
          }
        }
      }
    }
  resizeGL(this->width(), this->height());
  paintGL();
  updateGL();
}


void QtGlSliceView::setValidOverlayData(bool newValidOverlayData)
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


void QtGlSliceView::setSingleStep(double step)
{
  this->cSingleStep = (step != 0. ? step : 1.);
}


double QtGlSliceView::singleStep() const
{
  return this->cSingleStep;
}


int QtGlSliceView::imageSize(int axis) const
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


void QtGlSliceView::setMaxDisplayStates(int newNumberOfStates)
{
  this->setDisplayState(qMin(this->displayState(), static_cast<int>(
                               std::pow(2., newNumberOfStates - 2))));
  this->cMaxDisplayStates = newNumberOfStates;
}


void QtGlSliceView::setDisplayState(int state)
{
  if (state == this->cDisplayState)
    {
    return;
    }
  this->cDisplayState = state;
  emit displayStateChanged(state);
}


int QtGlSliceView::nextDisplayState(int state)const
{
  if (state == 0)
    {
    return 0x01;
    }
  int power = 0;
  while (state >>= 1)
    {
    ++power;
    }
  if (power >= this->maxDisplayStates() - 2)
    {
    return 0x00;
    }
  return std::pow(2., ++power);
}


void QtGlSliceView::showHelp()
{
  if (this->cHelpDialog == 0)
    {
    this->cHelpDialog = new QDialog(this);
    Ui_QtImageViewerHelp helpUi;
    helpUi.setupUi(this->cHelpDialog);
    }
  this->cHelpDialog->show();
}


void QtGlSliceView::setZoom(double newWinZoom)
{
  cWinZoom = newWinZoom;

  if(cWinZoom<1)
    cWinZoom = 1;

  if(cWinZoom>cDimSize[cWinOrder[0]])
    cWinZoom = (double)cDimSize[cWinOrder[0]]/2;

  if(cWinZoom>cDimSize[cWinOrder[1]])
    cWinZoom = (double)cDimSize[cWinOrder[1]]/2;

  emit zoomChanged(cWinZoom);
}


double QtGlSliceView::zoom() const
{
  return cWinZoom;
}


void QtGlSliceView::centerWindow(void)
{
  cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
  cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;

  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
}


void QtGlSliceView::centerWindow(int newWinCenterX,
                                 int newWinCenterY,
                                 int newWinCenterZ)
{
  if(newWinCenterX < 0)
    newWinCenterX = 0;
  if(newWinCenterX >= (int)cDimSize[0])
    newWinCenterX = cDimSize[0] - 1;
  cWinCenter[0] = newWinCenterX;

  if(newWinCenterY < 0)
    newWinCenterY = 0;
  if(newWinCenterY >= (int)cDimSize[1])
    newWinCenterY = cDimSize[1] - 1;
  cWinCenter[1] = newWinCenterY;

  if(newWinCenterZ < 0)
    newWinCenterZ = 0;
  if(newWinCenterZ >= (int)cDimSize[2])
    newWinCenterZ = cDimSize[2] - 1;
  cWinCenter[2] = newWinCenterZ;

  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
}


int QtGlSliceView::windowCenterX(void) const
{
  return cWinCenter[0];
}


int QtGlSliceView::windowCenterY(void) const
{
  return cWinCenter[1];
}


int QtGlSliceView::windowCenterZ(void) const
{
  return cWinCenter[2];
}


void QtGlSliceView::setViewValue(bool value)
{
  cViewValue = value;
}


void QtGlSliceView::setViewCrosshairs(bool crosshairs)
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


void QtGlSliceView::setBoxMin(double minX, double minY, double minZ)
{
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
  cBoxMin[2] = minZ;
}


void QtGlSliceView::setBoxMax(double boxX, double boxY, double boxZ)
{
  double x0 = qMin(cBoxMin[0], boxX);
  double y0 = qMin(cBoxMin[1], boxY);
  double z0 = qMin(cBoxMin[2], boxZ);

  double x1 = qMax(cBoxMin[0], boxX);
  double y1 = qMax(cBoxMin[1], boxY);
  double z1 = qMax(cBoxMin[2], boxZ);

  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
  cBoxMin[2] = z0;

  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
  cBoxMax[2] = z1;

  if (cClickBoxCallBack != NULL)
    {
    cClickBoxCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
                      cBoxMax[0], cBoxMax[1], cBoxMax[2]);
    }
  if (cClickBoxArgCallBack != NULL)
    {
    cClickBoxArgCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
                         cBoxMax[0], cBoxMax[1], cBoxMax[2],
                         cClickBoxArg);
    }
}


int QtGlSliceView::orientation(void) const
{
  return cWinOrientation;
}


void QtGlSliceView::setOrientation(int newOrientation)
{
  cWinOrientation = newOrientation;
  switch(cWinOrientation)
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

  if(cTranspose[cWinOrientation])
    {
    int t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }
  const int newSliceNum = clickedPointsStored() ?(int)cClickSelect[cWinOrder[2]] : cWinCenter[cWinOrder[2]];
  setSliceNum(newSliceNum);

  if(cWinOrientationCallBack != NULL)
    cWinOrientationCallBack();
  if(cWinOrientationArgCallBack != NULL)
    cWinOrientationArgCallBack(cWinOrientationArg);
  centerWindow();
  emit orientationChanged(cDimSize[cWinOrder[2]]);
}


void QtGlSliceView::setImageMode(ImageModeType newImageMode)
{
  cImageMode = newImageMode;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


void QtGlSliceView::setImageMode(const char* newImageMode)
{
  int newImageModeIndex = -1;
  for (int i = 0; i < NUM_ImageModeTypes; ++i)
    {
    if (QString(newImageMode) == QString(ImageModeTypeName[i]))
      {
      newImageModeIndex = i;
      break;
      }
    }
  if (newImageModeIndex >= 0)
    {
    this->setImageMode(static_cast<ImageModeType>(newImageModeIndex));
    }
}


void QtGlSliceView::setIWModeMax(const char* mode)
{
  int newmode = -1;
  for (int i = 0; i < NUM_IWModeTypes; ++i)
    {
    if (QString(mode) == QString(IWModeTypeName[i]))
      {
      newmode = i;
      break;
      }
    }
  if (newmode >= 0)
    {
    this->setIWModeMax(static_cast<IWModeType>(newmode));
    }
}


void QtGlSliceView::setIWModeMin(const char* mode)
{
  int newmode = -1;
  for (int i = 0; i < NUM_IWModeTypes; ++i)
    {
    if (QString(mode) == QString(IWModeTypeName[i]))
      {
      newmode = i;
      break;
      }
    }
  if (newmode >= 0)
    {
    this->setIWModeMin(static_cast<IWModeType>(newmode));
    }
}


void QtGlSliceView::setViewAxisLabel(bool axisLabel)
{
  cViewAxisLabel = axisLabel;
}


ImageModeType QtGlSliceView::imageMode(void) const
{
  return cImageMode;
}


void QtGlSliceView::flipX(bool newFlipX)
{
  cFlipX[cWinOrientation] = newFlipX;
}


bool QtGlSliceView::isXFlipped() const
{
  return cFlipX[cWinOrientation];
}


void QtGlSliceView::flipY(bool newFlipY)
{
  cFlipY[cWinOrientation] = newFlipY;
}


bool QtGlSliceView::isYFlipped() const
{
  return cFlipY[cWinOrientation];
}


void QtGlSliceView::flipZ(bool newFlipZ)
{
  cFlipZ[cWinOrientation] = newFlipZ;
}


bool QtGlSliceView::isZFlipped() const
{
  return cFlipZ[cWinOrientation];
}


void QtGlSliceView::transpose(bool newTranspose)
{
  if(isTransposed() != newTranspose)
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


void QtGlSliceView::setOverlay(bool newOverlay)
{
  cViewOverlayData = newOverlay;
}

void QtGlSliceView::setIWModeMin(IWModeType newIWModeMin)
{
  cIWModeMin = newIWModeMin;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


IWModeType QtGlSliceView::iwModeMin(void) const
{
  return cIWModeMin;
}


void QtGlSliceView::setIWModeMax(IWModeType newIWModeMax)
{
  cIWModeMax = newIWModeMax;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


IWModeType QtGlSliceView::iwModeMax(void) const
{
  return cIWModeMax;
}


void QtGlSliceView::keyPressEvent(QKeyEvent* keyEvent)
{
  static int fastMov = 0;
  int pace;
  int imgShiftSize = (int)(cWinSizeX/10/zoom());
  if(imgShiftSize<1)
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
      //when pressing down ">" or "<" key, scrolling will go faster
      if(fastMov < fastMovThresh())
        {
        fastMov ++;
        pace = 1;
        }
      else
        {
        pace = fastMovVal();
        }
      if((int)cWinCenter[cWinOrder[2]]-pace<0)
        {
        if((int)cWinCenter[cWinOrder[2]] == 0)
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
        setSliceNum((int)cWinCenter[cWinOrder[2]]-pace);
        }
      update();
      break;
    case Qt::Key_Greater: // >
    case Qt::Key_Period:
      //when pressing down ">" or "<" key, scrolling will go faster
      if(fastMov < fastMovThresh())
        {
        fastMov ++;
        pace = 1;
        }
      else
        {
        pace = fastMovVal();
        }
      if((int)cWinCenter[cWinOrder[2]]+pace>(int)cDimSize[cWinOrder[2]]-1)
        {
        if((int)cWinCenter[cWinOrder[2]] == (int)cDimSize[cWinOrder[2]]-1)
          {
          return;
          }
        else
          {
          setSliceNum((int)cDimSize[cWinOrder[2]]-1);
          }
        }
      else
        {
        setSliceNum((int)cWinCenter[cWinOrder[2]]+pace);
        }
      update();
      break;
    case Qt::Key_R:
      setZoom(1.0);
      centerWindow();
      setImageMode(IMG_VAL);
      setIWMin(cDataMin);
      setIWMax(cDataMax);
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
      flipX(!isXFlipped());
      update();
      break;
    case Qt::Key_Y:
      flipY(!isYFlipped());
      update();
      break;
    case Qt::Key_Z:
      flipZ(!isZFlipped());
      update();
      break;
    case Qt::Key_E:
      setIWModeMax(iwModeMax() == IW_FLIP ? IW_MAX : IW_FLIP);
      update();
      break;
    case Qt::Key_L:
      switch(imageMode())
        {
        default:
        case IMG_VAL:
          setImageMode(IMG_INV);
          update();
          break;
        case IMG_INV:
          setImageMode(IMG_LOG);
          update();
          break;
        case IMG_LOG:
          setImageMode(IMG_DX);
          update();
          break;
        case IMG_DX:
          setImageMode(IMG_DY);
          update();
          break;
        case IMG_DY:
          setImageMode(IMG_DZ);
          update();
          break;
        case IMG_DZ:
          setImageMode(IMG_BLEND);
          update();
          break;
        case IMG_BLEND:
          setImageMode(IMG_MIP);
          update();
          break;
        case IMG_MIP:
          setImageMode(IMG_VAL);
          update();
          break;
        }
      break;
    case Qt::Key_Q:
      setIWMax(iwMax()-singleStep());
      update();
      break;
    case Qt::Key_W:
      setIWMax(iwMax()+singleStep());
      update();
      break;
    case (Qt::Key_A):
      if (keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setViewAxisLabel(!viewAxisLabel());
        }
      else
        {
        setIWMin(iwMin()-singleStep());
        }
      update();
      break;
    case Qt::Key_S:
      setIWMin(iwMin()+singleStep());
      update();
      break;
    case (Qt::Key_I):
      if(!(keyEvent->modifiers() & Qt::ShiftModifier))
        {
        int newY;
        if(isYFlipped())
          {
          newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
          }
        else
          {
          newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
          }
        cWinCenter[cWinOrder[1]] = newY;
        centerWindow(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
        }
      update();
      break;
    case Qt::Key_M:
      int nY;
      if(isYFlipped())
        {
        nY = cWinCenter[cWinOrder[1]]+imgShiftSize;
        }
      else
        {
        nY = cWinCenter[cWinOrder[1]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[1]] = nY;
      centerWindow(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      update();
      break;
    case Qt::Key_J:
      int newX;
      if(isXFlipped())
        {
        newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      else
        {
        newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = newX;
      centerWindow(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      update();
      break;
    case Qt::Key_K:
      int nX;
      if(isXFlipped())
        {
        nX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      else
        {
        nX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = nX;
      centerWindow(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      update();
      break;
    case (Qt::Key_T):
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setViewClickedPoints(!viewClickedPoints());
        }
      else
        {
        transpose(!isTransposed());
        }
      update();
      break;
    case (Qt::Key_C):
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setViewCrosshairs(!viewCrosshairs());
        }
      update();
      break;
    case (Qt::Key_V):
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setViewValue(!viewValue());
        }
      update();
      break;
    case Qt::Key_P:
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setViewValuePhysicalUnits(!viewValuePhysicalUnits());
        }
      else
        {
        saveClickedPointsStored();
        }
      update();
      break;
    case Qt::Key_D:
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        int newState = this->nextDisplayState(this->displayState());
        this->setDisplayState(newState);
        }
      else
        {
        setIWModeMin(iwModeMin() == IW_FLIP ? IW_MIN : IW_FLIP);
        }
      update();
      break;
    case (Qt::Key_O):
      if(keyEvent->modifiers() & Qt::ShiftModifier)
        {
        setOverlay(!viewOverlayData());
        }
      update();
      break;
    case Qt::Key_B:
    //decrease opacity overlay
      setOverlayOpacity(overlayOpacity() - 0.025);
      //update();
      break;
    case Qt::Key_N:
    //increase opacity overlay
      setOverlayOpacity(overlayOpacity() + 0.025);
      //update();
      break;
    case Qt::Key_H:
      showHelp();
      break;
    default:
      this->QWidget::keyPressEvent(keyEvent);
      break;
    }
}


void QtGlSliceView::resizeEvent(QResizeEvent* sizeEvent)
{
  cW = sizeEvent->size().width();
  cH = sizeEvent->size().height();
  this->Superclass::resizeEvent(sizeEvent);
}


QSize QtGlSliceView::minimumSizeHint()const
{
  const QSize minSizeHint = this->sizeHint();
  return minSizeHint;
}

QSize QtGlSliceView::sizeHint()const
{
  const QSize winSizeHint(cWinSizeX, cWinSizeY);
  return winSizeHint;
}


bool QtGlSliceView::hasHeightForWidth() const
{
  return cWinSizeX && cWinSizeY;
}


int QtGlSliceView::heightForWidth(int widgetWidth) const
{
  // \todo Currently the view handles only square widgets.
  //return cWinSizeY ? (width * cWinSizeY) / cWinSizeX : width;
  return widgetWidth;
}


/** Set up the OpenGL view port, matrix mode, etc. */
void QtGlSliceView::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLint)w, (GLint)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 200.0);
}

/** Initialize the OpenGL Window */
void QtGlSliceView::initializeGL() 
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
    
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
    //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

/** Draw */
void QtGlSliceView::paintGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
  glLoadIdentity();
    
  glMatrixMode(GL_PROJECTION);
    
  GLint v[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, v);
  glLoadIdentity();
  glViewport(this->width()-v[0], this->height()-v[1], v[0], v[1]);
  glOrtho(this->width()-v[0], this->width(), this->height()-v[1], this->height(), -1, 1);

  int h=7;
#ifdef Q_OS_DARWIN
  h=8;
#endif
  QFont widgetFont = this->font();
  widgetFont.setPointSize(h);
  if (!cImData)
    {
    return;
    }

  double scale0 = this->width()/(double)cDimSize[0] * zoom()
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = this->height()/(double)cDimSize[1] * zoom()
     * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);
  int originX = 0;
  int originY = 0;
  if(this->cWinZoom<=1)
    {
    if(this->cW-scale0*this->cDimSize[this->cWinOrder[0]]>0)
      {
      originX = (int)((this->cW-scale0*this->cDimSize[this->cWinOrder[0]])/2.0);
      }
    if(this->cH-scale1*this->cDimSize[this->cWinOrder[1]]>0)
      {
      originY = (int)((this->cH-scale1*this->cDimSize[this->cWinOrder[1]])/2.0);
      }
    }
  glRasterPos2i((isXFlipped())?cW:0,
     (isYFlipped())?cH:0);
    
  glPixelZoom((isXFlipped())?-scale0:scale0,
     (isYFlipped())?-scale1:scale1);
    
  if(cValidImData && cViewImData)
    {
    glDrawPixels(cWinDataSizeX, cWinDataSizeY,
                  GL_LUMINANCE, GL_UNSIGNED_BYTE, 
                  cWinImData);
    }
    
  if(cValidOverlayData && viewOverlayData())
    {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawPixels(cWinDataSizeX, cWinDataSizeY, GL_RGBA, 
       GL_UNSIGNED_BYTE, cWinOverlayData);
    glDisable(GL_BLEND);
    }

  if(viewClickedPoints())
    {
    glColor3f( 0.8, 0.4, 0.4 );
    glPointSize( 3.0 );
    glBegin(GL_POINTS);
    for( int ii = 0; ii < clickedPointsStored(); ii++ )
      {
      ClickPoint pointClicked(cClickSelect[0], cClickSelect[1], cClickSelect[2], cClickSelectV);
      clickedPoint( ii, pointClicked );
      double pts[3] = { pointClicked.x, pointClicked.y, pointClicked.z };

      if( static_cast<int>( pts[this->cWinOrder[2]] ) ==
          sliceNum() )
        {
        float xx;
        if(isXFlipped())
          {
          xx = this->cW - (pts[this->cWinOrder[0]]
            - this->cWinMinX) * scale0 - originX;
          }
        else
          {
          xx = (pts[this->cWinOrder[0]] - this->cWinMinX) * scale0
            + originX;
          }

        float yy;
        if(isYFlipped())
          {
          yy = this->cH - (pts[this->cWinOrder[1]]
               - this->cWinMinY) * scale1 - originY;
          }
        else
          {
          yy = (pts[this->cWinOrder[1]] - this->cWinMinY) * scale1
               + originY;
          }
        glVertex2f( xx, yy );
        }
      }
    glEnd();
    }
    
  if(viewAxisLabel())
    {
    glLoadIdentity();
    glOrtho( 0.0, (double)width(), 0.0, (double)height(), 0.0, 1.0 );
    glMatrixMode( GL_PROJECTION );
    glViewport( 0,0 , width(), height() );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2, 0.2, 0.78, 0.75);

    if(isXFlipped() == false)
      {
      int posY = static_cast<int>(this->cH/2 - h/2 );
      glRasterPos2i(width(), -posY);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);
      renderText( this->cW - (widgetFont.pointSize())/2 -2 , posY, cAxisLabelX[cWinOrientation], widgetFont);
      }
    else
      {
      int posY = static_cast<int>(this->cH/2 - h/2 );
      glRasterPos2i(width(), -posY);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);
      renderText((widgetFont.pointSize())/2, posY, cAxisLabelX[cWinOrientation], widgetFont);
      }
      
    if(isYFlipped() == false)
      {
      int posY = static_cast<int>(h +10) ;
      glRasterPos2i(this->width()/2, -posY);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);
      renderText(this->cW/2 - (widgetFont.pointSize())/2, posY, cAxisLabelY[cWinOrientation], widgetFont);
      }
    else
      {
      int posY = static_cast<int>(this->cH - h -10);
      glRasterPos2i(this->width()/2, -posY);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);
      renderText(this->cW/2 + (widgetFont.pointSize())/2, posY, cAxisLabelY[cWinOrientation], widgetFont);
      }
    
    glDisable(GL_BLEND);
    }
    
  if(viewValue())
    {
    glLoadIdentity();
    glOrtho( 0.0, (double)width(), 0.0, (double)height(), 0.0, 1.0 );
    glMatrixMode( GL_PROJECTION );
    glViewport( 0,0 , width(), height() );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, (double)0.75);
    char s[80];
    double px, py, pz, val = this->cClickSelectV;
    ClickPoint point(cClickSelect[0], cClickSelect[1], cClickSelect[2], cClickSelectV);
    px = point.x;
    py = point.y;
    pz = point.z;
    const char * suffix = "";
    if(viewValuePhysicalUnits())
      {
      IndexType index;
      index[0] = static_cast< itk::IndexValueType >( this->cClickSelect[0] );
      index[1] = static_cast< itk::IndexValueType >( this->cClickSelect[1] );
      index[2] = static_cast< itk::IndexValueType >( this->cClickSelect[2] );

      suffix = this->cPhysicalUnitsName;
      }
    if((ImagePixelType)1.5==1.5)
      {
      sprintf(s, "(%0.1f%s,  %0.1f%s,  %0.1f%s) = %0.3f",
              px, suffix,
              py, suffix,
              pz, suffix,
              val);
      }
    else
      {
      sprintf(s, "(%0.1f%s,  %0.1f%s,  %0.1f%s) = %d",
              px, suffix,
              py, suffix,
              pz, suffix,
              (int)val);
      }
    renderText(this->cW /2, this->cH, s);
    glDisable(GL_BLEND);    
    }

  QStringList details;
  if(this->orientation() == X_AXIS)
    {
    details << QString("X - Slice: %1").arg(this->windowCenterX());
    }
  else if(orientation() == Y_AXIS)
    {
    details << QString("Y - Slice: %1").arg(this->windowCenterY());
    }
  else if(orientation() == Z_AXIS)
    {
    details << QString("Z - Slice: %1").arg(this->windowCenterZ());
    }
  details << QString("Dims: %1 x %2 x %3")
    .arg(this->cDimSize[0])
    .arg(this->cDimSize[1])
    .arg(this->cDimSize[2]);
  details << QString("Voxel: %1 x %2 x %3")
    .arg(this->cSpacing[0], 0, 'f')
    .arg(this->cSpacing[1], 0, 'f')
    .arg(this->cSpacing[2], 0, 'f');
  details << QString("Int. Range: %1 - %2")
    .arg(this->cDataMin, 0, 'f')
    .arg(this->cDataMax, 0, 'f');
  details << QString("Int. Window: %1(%2) - %3(%4)")
    .arg(this->cIWMin, 0, 'f')
    .arg(IWModeTypeName[this->cIWModeMin])
    .arg(this->cIWMax, 0, 'f')
    .arg(IWModeTypeName[this->cIWModeMax]);
  details << QString("View Mode: %1").arg(ImageModeTypeName[this->cImageMode]);

  if(this->cDisplayState & 0x01)
    {
    glLoadIdentity();
    glOrtho(0.0, (double)width(), 0.0, (double)height(), 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glViewport(0,0 , width(), height());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9, 0.4, 0.1, (double)0.75);

    int i = 5;
    foreach(QString text, details)
      {
      int posX = 2;
      int posY = this->cH - (i--) * (h + 2);
      this->renderText(posX, posY, text);
      }
    glDisable(GL_BLEND);
    }
  QString str = details.join("\n");
  emit detailsChanged(str);

  if(viewCrosshairs()
    && static_cast<int>(cClickSelect[cWinOrder[2]]) ==
       static_cast<int>(sliceNum()))
    {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, 0.75);
    int posX;
    if(isXFlipped())
      {
      posX = (int)(cW - (cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
      }
    else
      {
      posX = (int)((cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
      }
    int posY;
    if(isYFlipped())
      {
      posY = (int)(cH - (cClickSelect[cWinOrder[1]] - cWinMinY) * scale1);
      }
    else
      {
      posY = (int)((cClickSelect[cWinOrder[1]] - cWinMinY) * scale1);
      }
    glBegin(GL_LINES);
    glVertex2d(0, posY);
    glVertex2d(posX-2, posY);
    glVertex2d(posX+2, posY);
    glVertex2d(this->width()-1, posY);
    glVertex2d(posX, 0);
    glVertex2d(posX, posY-2);
    glVertex2d(posX, posY+2);
    glVertex2d(posX, this->height()-1);
    glEnd();
    glDisable(GL_BLEND);
    }
}


void QtGlSliceView::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  if(!cImData)
    {
    return;
    }
  double scale0 = this->width()/(double)cDimSize[0] * zoom()
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = this->height()/(double)cDimSize[1] * zoom()
    * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);

  if(cClickMode == CM_SELECT || cClickMode == CM_BOX) 
    {
    double p[3];
    p[cWinOrder[0]] = cWinMinX + ((1-isXFlipped())*(mouseEvent->x())
                     + (isXFlipped())*(this->width()-mouseEvent->x()))
                     / scale0;
    if(p[cWinOrder[0]]<cWinMinX) 
      {
      p[cWinOrder[0]] = cWinMinX;
      }
    if(p[cWinOrder[0]]>cWinMaxX) 
      {
      p[cWinOrder[0]] = cWinMaxX;
      }
    p[cWinOrder[1]] = cWinMinY + (isYFlipped()*mouseEvent->y()
                     + (1-isYFlipped())*(this->height()-mouseEvent->y()))
                     / scale1;
    if(p[cWinOrder[1]]<cWinMinY) 
      {
      p[cWinOrder[1]] = cWinMinY;
      }
    if(p[cWinOrder[1]]>cWinMaxY) 
      {
      p[cWinOrder[1]] = cWinMaxY;
      }
    if(imageMode() != IMG_MIP)
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
    if(cClickMode == CM_SELECT)
      {
      selectPoint(p[0], p[1], p[2]);
      }
    }
  updateGL();
}

/** catches the mouse press to react appropriate
 *  Overriden to catch mousePressEvents and to start an internal
 *  timer, which calls the appropriate interaction routine */
void QtGlSliceView::mousePressEvent(QMouseEvent* mouseEvent)
{
   if (mouseEvent->button() & Qt::LeftButton)
     {
      if (mouseEvent->button() & Qt::ShiftModifier)
        {
         // left mouse mouse and shift button
         /*this->mouseEventActive = true;
         QObject::connect(this->stepTimer, SIGNAL(timeout()),
                           this->shiftLeftButtonFunction);*/
        }
     }
   else if (mouseEvent->button() & Qt::MidButton)
     {
      // middle mouse button
      //this->mouseEventActive = true;
      //QObject::connect(this->stepTimer, SIGNAL(timeout()),
      //                  this->middleButtonFunction);
     }
   else if (mouseEvent->button() & Qt::RightButton)
     {
      // right mouse button
      //this->mouseEventActive = true;
      //QObject::connect(this->stepTimer, SIGNAL(timeout()),
      //                  this, this->rightButtonFunction);
     }
/*
   if(this->mouseEventActive) {
      this->currentMousePos[0] = event->x();
      this->currentMousePos[1] = event->y();
      this->lastMousePos[0] = event->x();
      this->lastMousePos[1] = event->y();
      this->firstCall = true;
      this->stepTimer->start(this->interactionTime);
   }*/

   this->updateGL();
}


void QtGlSliceView::changeSlice(int value)
{
  this->setSliceNum(value);
  this->update();
  this->paintGL();
}


void QtGlSliceView::setSliceNum(int newSliceNum)
{
  newSliceNum = qMin(newSliceNum, static_cast<int>(cDimSize[cWinOrder[2]]) - 1);
  if (newSliceNum == cWinCenter[cWinOrder[2]])
    {
    return;
    }
  cWinCenter[cWinOrder[2]] = newSliceNum;

  if (cSliceNumCallBack != NULL)
    {
    cSliceNumCallBack();
    }
  if (cSliceNumArgCallBack != NULL)
    {
    cSliceNumArgCallBack(cSliceNumArg);
    }
  emit sliceNumChanged(newSliceNum);
}


int QtGlSliceView::sliceNum() const
{
  return cWinCenter[cWinOrder[2]];
}


int QtGlSliceView::maxSliceNum() const
{
  return cDimSize[cWinOrder[2]];
}


void QtGlSliceView::selectPoint(double newX, double newY, double newZ)
  {    
  cClickSelect[0] = newX;
  if(cClickSelect[0]<0)
    cClickSelect[0] = 0;
  if(cClickSelect[0] >= cDimSize[0])
    cClickSelect[0] = cDimSize[0]-1;
  
  cClickSelect[1] = newY;
  if(cClickSelect[1]<0)
    cClickSelect[1] = 0;
  if(cClickSelect[1] >= cDimSize[1])
    cClickSelect[1] = cDimSize[1]-1;
  
  cClickSelect[2] = newZ;
  if(cClickSelect[2]<0)
    cClickSelect[2] = 0;
  if(cClickSelect[2] >= cDimSize[2])
    cClickSelect[2] = cDimSize[2]-1;
  
  ImageType::IndexType ind;
  
  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
  ind[2] = (unsigned long)cClickSelect[2];
  cClickSelectV = cImData->GetPixel(ind);

  /*if length of list is equal to max, remove the earliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    {
    cClickedPoints.pop_back();
    }
  ClickPoint pointClicked(cClickSelect[0], cClickSelect[1], cClickSelect[2], cClickSelectV);
  cClickedPoints.push_front(pointClicked);

  if(cClickSelectCallBack != NULL)
    {
    cClickSelectCallBack(cClickSelect[0], cClickSelect[1],
    cClickSelect[2], cClickSelectV);
    }
  if(cClickSelectArgCallBack != NULL)
    {
    cClickSelectArgCallBack(cClickSelect[0], cClickSelect[1],
    cClickSelect[2], cClickSelectV,
    cClickSelectArg);
    }

  emit positionChanged(cClickSelect[0], cClickSelect[1], cClickSelect[2], cClickSelectV);

}

void QtGlSliceView::setFastMovThresh(int movThresh)
{
  cfastMovThresh = movThresh;
}

void QtGlSliceView::setFastMovVal(int movVal)
{
  cfastMovVal = movVal;
}


int QtGlSliceView::fastMovThresh() const
{
  return cfastMovThresh;
}


int QtGlSliceView::fastMovVal() const
{
  return cfastMovVal;
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


void QtGlSliceView::setIWMin(double value)
{
  value = qBound(cDataMin, value, cDataMax);
  if (qFuzzyCompare(value, cIWMin))
    {
    return;
    }
  cIWMin = value;
  update();
  emit iwMinChanged(cIWMin);
}


void QtGlSliceView::setIWMax(double value)
{
  value = qBound(cDataMin, value, cDataMax);
  if (qFuzzyCompare(value, cIWMax))
    {
    return;
    }
  cIWMax = value;
  update();
  emit iwMaxChanged(cIWMax);
}


bool QtGlSliceView::clickedPoint(int index, ClickPoint& point)
{
  if(index >= cClickedPoints.size())
    {
    return false;
    }
  ClickPointListType::const_iterator j = cClickedPoints.begin();

  std::advance (j,static_cast<int>(index));
  point = *j;
  return true;

}


int QtGlSliceView::clickedPointsStored() const
{
  return cClickedPoints.size();
}


int QtGlSliceView::maxClickedPointsStored() const
{
  return maxClickPoints;
}


void QtGlSliceView::clearClickedPointsStored()
{
  cClickedPoints.clear();
}


void QtGlSliceView::deleteLastClickedPointsStored()
{
  cClickedPoints.pop_front();
}


void QtGlSliceView::setViewValuePhysicalUnits(bool physicalValueUnits)
{
  cViewValuePhysicalUnits = physicalValueUnits;
}


void QtGlSliceView::setViewClickedPoints(bool pointsClicked)
{
  cViewClickedPoints = pointsClicked;
}


void QtGlSliceView::zoomIn()
{
  setZoom(zoom()+1);
  update();
}


void QtGlSliceView::zoomOut()
{
  setZoom(zoom()-1);
  update();
}


void QtGlSliceView::setMaxClickedPointsStored(int i)
{
  maxClickPoints = i;
}

#endif
