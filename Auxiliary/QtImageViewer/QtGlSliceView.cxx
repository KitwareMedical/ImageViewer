/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    QtGlSliceView.cxx
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
   This software is distributed WITHOUT ANY WARRANTY; without even 
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
   PURPOSE.  See the above copyright notices for more information.
   
=========================================================================*/
#ifndef QtGlSliceView_cxx
#define QtGlSliceView_cxx

#include <QScrollArea>

#include "QtGlSliceView.h"
#include "itkMinimumMaximumImageCalculator.h"

#include <iostream>
#include <fstream>
#include <list>
using namespace std;

// Qt include
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>

QtGlSliceView::QtGlSliceView(QWidget *parent, const char *name)
: QGLWidget(parent)
{
  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cViewAxisLabel = true;
  cViewOverlayData = true;
  cViewValuePhysicalUnits = false;
  cViewDetails = true;
  cViewClickedPoints = false;
  cViewCrosshairs = true;
  cViewValue = true;
  cClickMode = CM_SELECT;
  
  cColorTable = ColorTableType::New();
  cColorTable->UseDiscreteColors();
  cW = 0;
  cH = 0;
  for(int i=0;i<3;i++)
  {
    cFlipX[i]=false;
    cFlipY[i]=false;
    cFlipZ[i]=false;
  }
  cWinImData = NULL;
  cWinZBuffer = NULL;
  cfastMovVal = 1; //fast moving pace: 1 by defaut
  cfastMovThresh = 10; //how many single step moves before fast moving
  update();
}
  
  
QtGlSliceView::
QtGlSliceView(QGLFormat glf, QWidget *parent, const char *name)
: QGLWidget(glf,parent)
{    
  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cViewClickedPoints = false;
  cViewValuePhysicalUnits = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cColorTable = ColorTableType::New();
  cColorTable->UseDiscreteColors();
  cfastMovVal = 1; //fast moving pace: 1 by defaut
  cfastMovThresh = 10; //how many single step moves before fast moving
  update();
}
  

  
void 
QtGlSliceView::
setInputImage(ImageType * newImData)
{
  if(!newImData)
  {
    return;
  }

  RegionType region = newImData->GetLargestPossibleRegion();
  if(region.GetNumberOfPixels() == 0)
  {
    return;
  }

  SizeType   size   = region.GetSize();
  if(cValidOverlayData)
  {
    RegionType overlay_region = cOverlayData->GetLargestPossibleRegion();
    SizeType   overlay_size   = overlay_region.GetSize();
      
    for(int i=0; i<3; i++)
    {
      if(size[i] != overlay_size[i])
      {
        return;
      }
    }
  } 

  cImData = newImData;
  cDimSize[0]=size[0];
  cDimSize[1]=size[1];
  cDimSize[2]=size[2];
  cSpacing[0]=cImData->GetSpacing()[0];
  cSpacing[1]=cImData->GetSpacing()[1];
  cSpacing[2]=cImData->GetSpacing()[2];
      
    
  typedef MinimumMaximumImageCalculator<ImageType> CalculatorType;
  CalculatorType::Pointer calculator = CalculatorType::New();

  calculator->SetImage(cImData);
  calculator->Compute();
        
  cIWMin      = calculator->GetMinimum();
  cIWMax      = calculator->GetMaximum();

  cIWModeMin  = IW_MIN;
  cIWModeMax  = IW_MAX;
    
  cImageMode = IMG_VAL;
    
  cWinZoom = 1;
  cWinOrientation = 2;
  cWinOrder[0] = 0;
  cWinOrder[1] = 1;
  cWinOrder[2] = 2;
    
  cWinCenter[0] = cDimSize[0]/2;
  cWinCenter[1] = cDimSize[1]/2;
  cWinCenter[2] = 0;
    
  cWinMinX  = 0;
  cWinSizeX = cDimSize[0];
  if(cWinSizeX<cDimSize[1])
  {
    cWinSizeX = cDimSize[1];
  }
  if(cWinSizeX<cDimSize[2])
  {
    cWinSizeX = cDimSize[2];
  }
  cWinMaxX  = cWinSizeX - 1;
    
  cWinMinY  = 0;
  cWinSizeY = cWinSizeX;
  cWinMaxY  = cWinSizeY - 1;
    
  cWinDataSizeX = cDimSize[0];
  cWinDataSizeY = cDimSize[1];
  
  if(cWinImData != NULL)
  {
    delete [] cWinImData;
  }
    
  cWinImData = new unsigned char[ cWinDataSizeX * cWinDataSizeY ];
    
  if(cWinZBuffer != NULL) 
  {
    delete [] cWinZBuffer;
  }
    
  cWinZBuffer = new unsigned short[ cWinDataSizeX * cWinDataSizeY ];
    
  cViewImData  = true;
  cValidImData = true;
  update();
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
  RegionType newoverlay_region = 
            newOverlayData->GetLargestPossibleRegion();
  
  SizeType   newoverlay_size   = newoverlay_region.GetSize();
  
  RegionType cImData_region = 
               cImData->GetLargestPossibleRegion();
  
  SizeType   cImData_size   = cImData_region.GetSize();
  
  if(!cValidImData || newoverlay_size[2]==cImData_size[2])
  {
    cOverlayData = newOverlayData;
    
    cViewOverlayData  = true;
    cValidOverlayData = true;
    cOverlayOpacity   = 1.0;
    
    if(cWinOverlayData != NULL) 
    {
      delete [] cWinOverlayData;
    }
    
    const unsigned long bufferSize = cWinDataSizeX * cWinDataSizeY * 4;
    cWinOverlayData = new unsigned char[ bufferSize ];
    update();
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
  
  if(cViewOverlayCallBack != NULL)
  {
    cViewOverlayCallBack();
  }
  
  paintGL();
}


bool 
QtGlSliceView::viewOverlayData(void) const
{
  return cViewOverlayData;
}


void 
QtGlSliceView::viewOverlayCallBack(
void (* newViewOverlayCallBack)(void) 
)
{
  cViewOverlayCallBack = newViewOverlayCallBack;
}


void 
QtGlSliceView::setOverlayOpacity(double newOverlayOpacity)
{
  cOverlayOpacity = newOverlayOpacity; 
  if(cViewOverlayCallBack != NULL) 
  {
    cViewOverlayCallBack();
  }
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

void  QtGlSliceView::saveClickedPointsStored()
{
  QString fileName = QFileDialog::getSaveFileName(this,"Please select a file name","*.*","");
  if(fileName.isEmpty() || fileName.isNull())
    {
    return;
    }
  QFile fpoints(fileName);
  fpoints.open(QIODevice::ReadWrite);
  std::list< ClickPoint* >::iterator point;
  QTextStream text(&fpoints);
  for(point = cClickedPoints.begin(); point != cClickedPoints.end(); point++)
    {
    text << *point <<endl;
    }
  fpoints.close();
}

void 
QtGlSliceView::update()
{
  setFocus();
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



void
QtGlSliceView::showHelp()
{
  QDialog* helpDialog = new QDialog(this);
  cHelpUi = new Ui::HelpWindow;
  cHelpUi->setupUi(helpDialog);
  helpDialog->show();
}


void
QtGlSliceView::setZoom(double newWinZoom)
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


void
QtGlSliceView::centerWindow(void)
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


double QtGlSliceView::minIntensity() const
{
  return cIWMin;
}


double QtGlSliceView::maxIntensity() const
{
  return cIWMax;
}


void QtGlSliceView::setViewValue(bool value)
{
  cViewValue = value;
}


void QtGlSliceView::setViewCrosshairs(bool crosshairs)
{
  cViewCrosshairs = crosshairs;
}


void QtGlSliceView::setViewDetails(bool detail)
{
  cViewDetails = detail;
}


bool QtGlSliceView::viewCrosshairs() const
{
  return cViewCrosshairs;
}


bool QtGlSliceView::viewDetails() const
{
  return cViewDetails;
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


void QtGlSliceView::setBoxMax(double x, double y, double z)
{
  double x0, y0, z0;
  double x1, y1, z1;

  x0 = (cBoxMin[0]<x) ? cBoxMin[0] : x;
  y0 = (cBoxMin[1]<y) ? cBoxMin[1] : y;
  z0 = (cBoxMin[2]<z) ? cBoxMin[2] : z;

  x1 = (cBoxMin[0]<x) ? x : cBoxMin[0];
  y1 = (cBoxMin[1]<y) ? y : cBoxMin[1];
  z1 = (cBoxMin[2]<z) ? z : cBoxMin[2];

  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
  cBoxMin[2] = z0;

  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
  cBoxMax[2] = z1;

  if(cClickBoxCallBack != NULL)
    cClickBoxCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2]);
  if(cClickBoxArgCallBack != NULL)
    cClickBoxArgCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2],
    cClickBoxArg);
}


int QtGlSliceView::orientation(void) const
{
  return cWinOrientation;
}


void QtGlSliceView::setOrientation(int newOrientation)
{
  cWinOrientation = newOrientation;
  switch(cWinOrientation) {
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

  setSliceNum((int)cClickSelect[cWinOrder[2]]);

  if(cWinOrientationCallBack != NULL)
    cWinOrientationCallBack();
  if(cWinOrientationArgCallBack != NULL)
    cWinOrientationArgCallBack(cWinOrientationArg);
}


void QtGlSliceView::setImageMode(ImageModeType newImageMode)
{
  cImageMode = newImageMode;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
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
  if(cTranspose[cWinOrientation] != newTranspose)
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


void QtGlSliceView::keyPressEvent(QKeyEvent *event)
{
  static int fastMov = 0;
  int pace;
  int imgShiftSize = (int)(cWinSizeX/10/zoom());
  if(imgShiftSize<1)
    {
    imgShiftSize = 1;
    }

  switch(event->key())
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
        setSliceNum(cWinCenter[cWinOrder[2]]+pace);
        }
      update();
      break;
    case Qt::Key_R:
      setZoom(1.0);
      centerWindow();
      setImageMode(IMG_VAL);
      setMaxIntensity(cDataMax);
      setMinIntensity(cDataMin);
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
      iwModeMax() == IW_FLIP ? setIWModeMax(IW_MAX) : setIWModeMax(IW_FLIP);
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
      setMaxIntensity(maxIntensity()-0.02*intensityRange());
      update();
      break;
    case Qt::Key_W:
      setMaxIntensity(maxIntensity()+0.02*intensityRange());
      update();
      break;
    case (Qt::Key_A):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        setViewAxisLabel(!viewAxisLabel());
        }
      else
        {
        setMinIntensity(minIntensity()-0.02*intensityRange());
        }
      update();
      break;
    case Qt::Key_S:
      setMinIntensity(minIntensity()+0.02*intensityRange());
      update();
      break;
    case (Qt::Key_I):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        }
      else
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
      if(event->modifiers() & Qt::ShiftModifier)
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
      if(event->modifiers() & Qt::ShiftModifier)
        {
        setViewCrosshairs(!viewCrosshairs());
        }
      else
        {
        }
      update();
      break;
    case (Qt::Key_V):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        setViewValue(!viewValue());
        }
      update();
      break;
    case Qt::Key_P:
      if(event->modifiers() & Qt::ShiftModifier)
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
      if(event->modifiers() & Qt::ShiftModifier)
        {
        setViewDetails(!viewDetails());
        }
      else
        {
        if(iwModeMin() == IW_FLIP)
          {
          setIWModeMin(IW_MIN);
          update();
          }
        else
          {
          setIWModeMin(IW_FLIP);
          update();
          }
        }
      update();
      break;
    case (Qt::Key_O):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        setOverlay(!viewOverlayData());
        }
      update();
      break;
    case Qt::Key_H:
      showHelp();
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
}


void QtGlSliceView::size(int w, int h)
{
  update();
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

  if(!cImData)
  {
    std::cout << "no cImData !!!" << std::endl;
    return;
  }


  double scale0 = this->width()/(double)cDimSize[0] * zoom()
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = this->height()/(double)cDimSize[1] * zoom()
     * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);
    
   
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
    
  if(viewAxisLabel())
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2, 0.2, 0.78, 0.75);

    if(isXFlipped() == false)
    {
      const int y = static_cast<int>(this->height()/2-this->height()/2 );
      glRasterPos2i(this->width(), -y);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);
      // gl_draw(cAxisLabelX[cWinOrientation],
      //   cW-(gl_width(cAxisLabelX[cWinOrientation])+10), 
      //   static_cast<double>(y));
    }
    else
    {
      const int y = static_cast<int>(this->height()/2-this->height()/2 );
      glRasterPos2i(10, -y);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);

      //gl_draw(cAxisLabelX[cWinOrientation],
      // (gl_width(cAxisLabelX[cWinOrientation])+10),
      //  static_cast<double>(y));
    }
      
    if(isYFlipped() == false)
    {
      const int y = static_cast<int>(this->height()-this->height()-10) ;
      glRasterPos2i(this->width()/2, -y);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);

      //gl_draw(cAxisLabelY[cWinOrientation],
      //  cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
      //  static_cast<double>(y));
    }
    else
    {
      const int y = static_cast<int>(this->height()+10);
      glRasterPos2i(this->width()/2, -y);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);

      //gl_draw(cAxisLabelY[cWinOrientation],
      //  cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
      //  static_cast<double>(y));
    }
    
    glDisable(GL_BLEND);
  }
    
  if(viewValue())
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, (double)0.75);
    //gl_font(FL_TIMES_BOLD, 12);
    char s[80];
    if((ImagePixelType)1.1==1.1)
    {
      sprintf(s, "(%0.1f,  %0.1f,  %0.1f) = %0.3f", 
      cClickSelect[0],
      cClickSelect[1], 
      cClickSelect[2], 
      (double)cClickSelectV);
    }
    else
    {
      sprintf(s, "(%0.1f,  %0.1f,  %0.1f) = %d", 
      cClickSelect[0],
      cClickSelect[1], 
      cClickSelect[2], 
      (int)cClickSelectV);
    }
    //gl_draw(s,(int)(cW-(gl_width(s)+2)), 2);
    glDisable(GL_BLEND);    
  }

  if(viewDetails())
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9, 0.4, 0.1, (double)0.75);
    //gl_font(FL_TIMES_BOLD, 12);
    char s[80];
    if(orientation() == 0)
        sprintf(s, "X - Slice: %3d", cWinCenter[0]);
      else if(orientation() == 1)
        sprintf(s, "Y - Slice: %3d", cWinCenter[1]);
      else
        sprintf(s, "Z - Slice: %3d", cWinCenter[2]);
//      gl_draw(s, 2, 2+5*(gl_height()+2));
//      sprintf(s, "Dims: %3d x %3d x %3d",
//        (int)cDimSize[0], (int)cDimSize[1], (int)cDimSize[2]);
//      gl_draw(s, 2, 2+4*(gl_height()+2));
//      sprintf(s, "Voxel: %0.3f x %0.3f x %0.3f",
//        cSpacing[0], cSpacing[1], cSpacing[2]);
//      gl_draw(s, 2, 2+3*(gl_height()+2));
//      sprintf(s, "Int. Range: %0.3f - %0.3f", (double)cDataMin,
//              (double)cDataMax);
//      gl_draw(s, 2, 2+2*(gl_height()+2));
//      sprintf(s, "Int. Window: %0.3f(%s) - %0.3f(%s)",
//        (double)cIWMin, IWModeTypeName[cIWModeMin],
//        (double)cIWMax, IWModeTypeName[cIWModeMax]);
//      gl_draw(s, 2, 2+1*(gl_height()+2));
//      sprintf(s, "View Mode: %s", ImageModeTypeName[cImageMode]);
//      gl_draw(s, 2, 2+0*(gl_height()+2));
      glDisable(GL_BLEND);
  }
    
  if(viewCrosshairs()
    && static_cast<int>(cClickSelect[cWinOrder[2]]) == 
       static_cast<int>(sliceNum()))
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, 0.75);
    int x;
    if(isXFlipped())
    {
      x = (int)(cW - (cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
    }
    else
    {
      x = (int)((cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
    }
    int y;
    if(isYFlipped())
    {
      y = (int)(cH - (cClickSelect[cWinOrder[1]] - cWinMinY) * scale1);
    }
    else
    {
      y = (int)((cClickSelect[cWinOrder[1]] - cWinMinY) * scale1);
    }
    glBegin(GL_LINES);
    glVertex2d(0, y);
    glVertex2d(x-2, y);
    glVertex2d(x+2, y);
    glVertex2d(this->width()-1, y);
    glVertex2d(x, 0);
    glVertex2d(x, y-2);
    glVertex2d(x, y+2);
    glVertex2d(x, this->height()-1);
    glEnd();
    glDisable(GL_BLEND);
  }
}


void QtGlSliceView::mouseMoveEvent(QMouseEvent *event)
{
  double scale0 = this->width()/(double)cDimSize[0] * zoom()
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = this->height()/(double)cDimSize[1] * zoom()
    * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);

  if(cClickMode == CM_SELECT || cClickMode == CM_BOX) 
  {
    double p[3];
    p[cWinOrder[0]] = cWinMinX + ((1-isXFlipped())*(event->x())
                     + (isXFlipped())*(this->width()-event->x()))
                     / scale0;
    if(p[cWinOrder[0]]<cWinMinX) 
    {
      p[cWinOrder[0]] = cWinMinX;
    }
    if(p[cWinOrder[0]]>cWinMaxX) 
    {
      p[cWinOrder[0]] = cWinMaxX;
    }
    p[cWinOrder[1]] = cWinMinY + (isYFlipped()*event->y()
                     + (1-isYFlipped())*(this->height()-event->y()))
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
void QtGlSliceView::mousePressEvent(QMouseEvent *event)
{
   if(event->button() & Qt::LeftButton)
   {
      if(event->button() & Qt::ShiftModifier)
      {
         // left mouse mouse and shift button
         /*this->mouseEventActive = true;
         QObject::connect(this->stepTimer, SIGNAL(timeout()),
                           this->shiftLeftButtonFunction);*/
      }
   }
   else if(event->button() & Qt::MidButton)
   {
      // middle mouse button
      //this->mouseEventActive = true;
      //QObject::connect(this->stepTimer, SIGNAL(timeout()),
      //                  this->middleButtonFunction);
   }
   else if(event->button() & Qt::RightButton)
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

   updateGL();
}


void QtGlSliceView::changeSlice(int value)
{
  setSliceNum(value);
  update();
  paintGL();
}



void QtGlSliceView::setSliceNum(int newSliceNum)
{
  if(newSliceNum>=cDimSize[cWinOrder[2]])
    newSliceNum = cDimSize[cWinOrder[2]]-1;
  cWinCenter[cWinOrder[2]] = newSliceNum;

  emit sliceNumChanged(newSliceNum);
  
  /*if(cSliceNumCallBack != NULL)
    cSliceNumCallBack();
  if(cSliceNumArgCallBack != NULL)
    cSliceNumArgCallBack(cSliceNumArg);*/
}

int QtGlSliceView::sliceNum() const
{
  return cWinCenter[cWinOrder[2]];
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
 
  emit positionChanged(ind[0],ind[1],ind[2],cClickSelectV);

}

void QtGlSliceView::setMaxIntensity(int value)
{
  cIWMax = value;
  update();
  emit maxIntensityChanged(cIWMax);
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


void QtGlSliceView::setViewValuePhysicalUnits(bool physicalValueUnits)
{
  cViewValuePhysicalUnits = physicalValueUnits;
}


void QtGlSliceView::setViewClickedPoints(bool pointsClicked)
{
  cViewClickedPoints = pointsClicked;
}


void QtGlSliceView::setMinIntensity(int value)
{
  cIWMin = value;
  update();
  emit minIntensityChanged(cIWMin);
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
 
#endif
