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
#include <QKeyEvent>
#include <QMouseEvent>

QtGlSliceView::QtGlSliceView( QWidget *parent, const char *name)
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
  for(unsigned int i=0;i<3;i++)
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
QtGlSliceView( QGLFormat glf, QWidget *parent, const char *name)
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
SetInputImage(ImageType * newImData)
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

  SizeType   size   = region.GetSize();
  if( cValidOverlayData )
  {
    RegionType overlay_region = cOverlayData->GetLargestPossibleRegion();
    SizeType   overlay_size   = overlay_region.GetSize();
      
    for( int i=0; i<3; i++ )
    {
      if( size[i] != overlay_size[i] )
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

  calculator->SetImage( cImData );
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
  this->update();
}


const QtGlSliceView::ImagePointer &
QtGlSliceView
::GetInputImage(void) const
{
  return cImData;
}


void 
QtGlSliceView
::SetInputOverlay( OverlayType * newOverlayData )
{
  RegionType newoverlay_region = 
            newOverlayData->GetLargestPossibleRegion();
  
  SizeType   newoverlay_size   = newoverlay_region.GetSize();
  
  RegionType cImData_region = 
               cImData->GetLargestPossibleRegion();
  
  SizeType   cImData_size   = cImData_region.GetSize();
  
  if( !cValidImData || newoverlay_size[2]==cImData_size[2] )
  {
    cOverlayData = newOverlayData;
    
    cViewOverlayData  = true;
    cValidOverlayData = true;
    cOverlayOpacity   = (float)1.0;
    
    if(cWinOverlayData != NULL) 
    {
      delete [] cWinOverlayData;
    }
    
    const unsigned long bufferSize = cWinDataSizeX * cWinDataSizeY * 4;
    cWinOverlayData = new unsigned char[ bufferSize ];
    this->update();
  }
}


const QtGlSliceView::OverlayType::Pointer &
QtGlSliceView::GetInputOverlay( void ) 
const
{
  return cOverlayData;
}


void 
QtGlSliceView::
ViewOverlayData( bool newViewOverlayData)
{ 
  cViewOverlayData = newViewOverlayData;
  
  if( cViewOverlayCallBack != NULL )
  {
    cViewOverlayCallBack();
  }
  
  this->paintGL();
}


bool 
QtGlSliceView::ViewOverlayData(void)
{
  return cViewOverlayData;
}



void 
QtGlSliceView::ViewOverlayCallBack(
void (* newViewOverlayCallBack)(void) 
)
{
  cViewOverlayCallBack = newViewOverlayCallBack;
}


void 
QtGlSliceView::OverlayOpacity(float newOverlayOpacity)
{
  cOverlayOpacity = newOverlayOpacity; 
  if(cViewOverlayCallBack != NULL) 
  {
    cViewOverlayCallBack();
  }
}

float 
QtGlSliceView::OverlayOpacity(void)
{
  return cOverlayOpacity;
}


QtGlSliceView::ColorTableType 
* QtGlSliceView::GetColorTable(void)
{
  return cColorTable.GetPointer();
}

void  QtGlSliceView::saveClickedPointsStored()
{
  QFileDialog *dialog;
  QString filename = dialog->getSaveFileName(this,"Please select a file name","*.*","");
  ofstream fpoints;
  fpoints.open(filename.toLatin1().data());
  list< ClickPoint* >::iterator point = cClickedPoints.begin();
  while( point != cClickedPoints.end() )
    {
    fpoints << *(point) <<endl;
    ++point;
    }
  fpoints.close();
}

void 
QtGlSliceView::update()
{
  this->setFocus();
  if( !cValidImData ) 
  {
    return;
  }
  
  int winWidth = (int)( cDimSize[ cWinOrder[0] ] / cWinZoom );
  cWinSizeX = ( (int) winWidth);
  int ti = (int)( (int)cWinCenter[ cWinOrder[0] ] - winWidth/2);
  if( ti <= - (int) cDimSize[ cWinOrder[0] ] ) 
  {
    ti = -cDimSize[ cWinOrder[0] ] + 1;
  }
  else if( ti >= (int)cDimSize[ cWinOrder[0] ]) 
  {
    ti = cDimSize[ cWinOrder[0] ] - 1;
  }
  cWinMinX = ti;
  cWinMaxX = cDimSize[ cWinOrder[0] ] - 1; // here
  if( cWinMaxX >= static_cast<int>( cDimSize[ cWinOrder[0] ] ) )
  {
    cWinMaxX = cDimSize[ cWinOrder[0] ] - 1;
  }
  
  winWidth = static_cast<int>( cDimSize[ cWinOrder[1] ] / cWinZoom );
  cWinSizeY = ( static_cast<int>( winWidth) );
  ti = static_cast<int>( static_cast<int>(cWinCenter[ cWinOrder[1] ]) - winWidth/2);
  if( ti <= - static_cast<int>( cDimSize[ cWinOrder[1] ] ) ) 
  {
    ti = -cDimSize[ cWinOrder[1] ] + 1;
  }
  else if( ti >= static_cast<int>(cDimSize[ cWinOrder[1] ] ) ) 
  {
    ti = cDimSize[ cWinOrder[1] ] - 1;
  } 
  cWinMinY = ti;
  cWinMaxY = cDimSize[ cWinOrder[1] ] - 1; // here
  if( cWinMaxY >= static_cast<int>( cDimSize[ cWinOrder[1] ] ) ) 
  {
    cWinMaxY = cDimSize[ cWinOrder[1] ] - 1;
  }
  
  memset( cWinImData, 0, cWinDataSizeX*cWinDataSizeY );
  if( cValidOverlayData ) 
  {
    memset(cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4);
  }
  
  IndexType ind;
  
  int l, m;
  
  float tf;
  
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

      switch( cImageMode ) 
      {
        default:
        case IMG_VAL:
          tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
          break;
        case IMG_INV:
          tf = (float)((cIWMax-cImData->GetPixel(ind))/(cIWMax-cIWMin)*255);
          break;
        case IMG_LOG:
          tf = (float)(log(cImData->GetPixel(ind)-cIWMin+0.00000001)
            /log(cIWMax-cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
          {
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]--;
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
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
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]--;
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
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
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]--;
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
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
          ind[cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          tf = (float)(cImData->GetPixel(ind));
          
          ind[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          tf += (float)(cImData->GetPixel(ind))*2;
          
          const int tempval1 = (int)cDimSize[cWinOrder[2]]-1;
          const int tempval2 = (int)cWinCenter[cWinOrder[2]]+1;
          ind[cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          tf += (float)(cImData->GetPixel(ind));
          
          tf = (float)((tf/4-cIWMin)/(cIWMax-cIWMin)*255);
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
              tf = (float)(cImData->GetPixel(ind));
              cWinZBuffer[m] = (unsigned short)l;
            }
          }
          tf = (float)((tf-cIWMin)/(cIWMax-cIWMin)*255);
          ind[cWinOrder[2]] = tmpI;
          break;
        }
      
      if( tf > 255 )
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
        if( tf < 0 )
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
      
      if( cValidOverlayData ) 
      {
        l = l * 4;
        if(cImageMode == IMG_MIP)
        {
          ind[cWinOrder[2]] = cWinZBuffer[(j-cWinMinX) + 
            (k-cWinMinY)*cWinDataSizeX];
        }
        
        if( sizeof( OverlayPixelType ) == 1 )
        {
          m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
          if( m > 0 ) 
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
            if( sizeof( OverlayPixelType ) == 3 )
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
              if( sizeof( OverlayPixelType ) == 4 ) 
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
QtGlSliceView::Help()
{
  QDialog* helpDialog = new QDialog(this);
  helpWindow = new Ui::HelpWindow;
  helpWindow->setupUi(helpDialog);
  helpDialog->show();
}


void
QtGlSliceView::winZoom(float newWinZoom)
{
  cWinZoom = newWinZoom;

  if(cWinZoom<1)
    cWinZoom = 1;

  if(cWinZoom>cDimSize[cWinOrder[0]])
    cWinZoom = (float)cDimSize[cWinOrder[0]]/2;

  if(cWinZoom>cDimSize[cWinOrder[1]])
    cWinZoom = (float)cDimSize[cWinOrder[1]]/2;
}


float
QtGlSliceView::winZoom(void)
{
  return cWinZoom;
}


void
QtGlSliceView::winCenter(void)
{
  cWinCenter[cWinOrder[0]] = cDimSize[cWinOrder[0]]/2;
  cWinCenter[cWinOrder[1]] = cDimSize[cWinOrder[1]]/2;

  if(cWinCenterCallBack != NULL)
    cWinCenterCallBack();
  if(cWinCenterArgCallBack != NULL)
    cWinCenterArgCallBack(cWinCenterArg);
}


void QtGlSliceView::winCenter(int newWinCenterX,
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


unsigned int QtGlSliceView::winCenterX(void)
{
  return cWinCenter[0];
}


unsigned int QtGlSliceView::winCenterY(void)
{
  return cWinCenter[1];
}


unsigned int QtGlSliceView::winCenterZ(void)
{
  return cWinCenter[2];
}


void QtGlSliceView::boxMin(float minX, float minY, float minZ)
{
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
  cBoxMin[2] = minZ;
}


void QtGlSliceView::boxMax(float x, float y, float z)
{
  float x0, y0, z0;
  float x1, y1, z1;

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


unsigned int QtGlSliceView::orientation(void)
{
  return cWinOrientation;
}


void QtGlSliceView::orientation(unsigned int newOrientation)
{
  cWinOrientation = newOrientation;
  switch(cWinOrientation) {
    case 0 :
      cWinOrder[0] = 2;
      cWinOrder[1] = 1;
      cWinOrder[2] = 0;
      break;
    case 1 :
      cWinOrder[0] = 0;
      cWinOrder[1] = 2;
      cWinOrder[2] = 1;
      break;
    //default:
    case 2 :
      cWinOrientation = 2;
      cWinOrder[0] = 0;
      cWinOrder[1] = 1;
      cWinOrder[2] = 2;
      break;
    }

  if(cTranspose[cWinOrientation])
    {
    int t = cWinOrder[0];
    cWinOrder[0] = cWinOrder[1];
    cWinOrder[1] = t;
    }

  sliceNum((int)cClickSelect[cWinOrder[2]]);

  if(cWinOrientationCallBack != NULL)
    cWinOrientationCallBack();
  if(cWinOrientationArgCallBack != NULL)
    cWinOrientationArgCallBack(cWinOrientationArg);
}


void QtGlSliceView::iwMin(float newIWMin)
{
  cIWMin = newIWMin;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


float QtGlSliceView::iwMin(void)
{
  return cIWMin;
}


void QtGlSliceView::iwMax(float newIWMax)
{
  cIWMax = newIWMax;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


float QtGlSliceView::iwMax(void)
{
  return cIWMax;
}


void QtGlSliceView::imageMode(ImageModeType newImageMode)
{
  cImageMode = newImageMode;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


ImageModeType QtGlSliceView::imageMode(void)
{
  return cImageMode;
}


void QtGlSliceView::flipX(bool newFlipX)
{
  cFlipX[cWinOrientation] = newFlipX;
}


bool QtGlSliceView::flipX()
{
  return cFlipX[cWinOrientation];
}


void QtGlSliceView::flipY(bool newFlipY)
{
  cFlipY[cWinOrientation] = newFlipY;
}


bool QtGlSliceView::flipY()
{
  return cFlipY[cWinOrientation];
}


void QtGlSliceView::flipZ(bool newFlipZ)
{
  cFlipZ[cWinOrientation] = newFlipZ;
}


bool QtGlSliceView::flipZ()
{
  return cFlipZ[cWinOrientation];
}


void QtGlSliceView::Transpose(bool newTranspose)
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


bool QtGlSliceView::Transpose()
{
  return cTranspose[cWinOrientation];
}


void QtGlSliceView::iwModeMin(IWModeType newIWModeMin)
{
  cIWModeMin = newIWModeMin;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


IWModeType QtGlSliceView::iwModeMin(void)
{
  return cIWModeMin;
}


void QtGlSliceView::iwModeMax(IWModeType newIWModeMax)
{
  cIWModeMax = newIWModeMax;
  if(cIWCallBack != NULL)
    cIWCallBack();
  if(cIWArgCallBack != NULL)
    cIWArgCallBack(cIWArg);
}


IWModeType QtGlSliceView::iwModeMax(void)
{
  return cIWModeMax;
}


void QtGlSliceView::keyPressEvent( QKeyEvent *event)
{
  static int fastMov = 0;
  int pace;
  int imgShiftSize = (int)(cWinSizeX/10/cWinZoom);
  if(imgShiftSize<1)
    {
    imgShiftSize = 1;
    }

  switch(event->key())
    {
    case Qt::Key_0:
      orientation(0);
      cTranspose[cWinOrientation] = true;
      this->update();
      break;
    case Qt::Key_1:
      orientation(1);
      cTranspose[cWinOrientation] = false;
      this->update();
      break;
    case Qt::Key_2:
      orientation(2);
      cTranspose[cWinOrientation] = false;
      this->update();
      break;
    case Qt::Key_Less: // <
    case Qt::Key_Comma:
      //when pressing down ">" or "<" key, scrolling will go faster
      if( fastMov < cfastMovThresh)
        {
        fastMov ++;
        pace = 1;
      }
        else
        {
        pace = cfastMovVal;
        }
      if((int)cWinCenter[cWinOrder[2]]-pace<0)
        {
        if( (int)cWinCenter[cWinOrder[2]] == 0)
          {
          return;
          }
        else
          {
          sliceNum(0);
          emit SliceChanged(0);
          }
        }
      else
        {
        sliceNum((int)cWinCenter[cWinOrder[2]]-pace);
        emit SliceChanged((int)cWinCenter[cWinOrder[2]]-pace);
        }

      this->update();
      break;
    case Qt::Key_Greater: // >
    case Qt::Key_Period:
      //when pressing down ">" or "<" key, scrolling will go faster
      if( fastMov < cfastMovThresh)
        {
        fastMov ++;
        pace = 1;
        }
      else
        {
        pace = cfastMovVal;
        }
      if((int)cWinCenter[cWinOrder[2]]+pace>(int)cDimSize[cWinOrder[2]]-1)
        {
        if((int)cWinCenter[cWinOrder[2]] == (int)cDimSize[cWinOrder[2]]-1)
          {
           return;
          }
        else
          {
          sliceNum((int)cDimSize[cWinOrder[2]]-1);
          emit SliceChanged((int)cDimSize[cWinOrder[2]]-1);
          }
        }
      else
        {
        sliceNum(cWinCenter[cWinOrder[2]]+pace);
        emit SliceChanged(cWinCenter[cWinOrder[2]]+pace);
        }
      this->update();
      break;
    case Qt::Key_R:
      winZoom(1);
      winCenter();
      imageMode(IMG_VAL);
      iwMax(cDataMax);
      iwMin(cDataMin);
      this->update();
      break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
      ZoomIn();
      this->update();
      break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
      ZoomOut();
      this->update();
      break;
    case Qt::Key_X:
      flipX(!cFlipX[cWinOrientation]);
      this->update();
      break;
    case Qt::Key_Y:
      flipY(!cFlipY[cWinOrientation]);
      this->update();
      break;
    case Qt::Key_Z:
      flipZ(!cFlipZ[cWinOrientation]);
      this->update();
      break;
    case Qt::Key_E:
      if(iwModeMax() == IW_FLIP)
        {
        iwModeMax(IW_MAX);
        this->update();
        }
      else
        {
        iwModeMax(IW_FLIP);
        this->update();
        }
      break;
    case Qt::Key_L:
      switch(cImageMode)
        {
        default:
        case IMG_VAL:
          imageMode(IMG_INV);
          this->update();
          break;
        case IMG_INV:
          imageMode(IMG_LOG);
          this->update();
          break;
        case IMG_LOG:
          imageMode(IMG_DX);
          this->update();
          break;
        case IMG_DX:
          imageMode(IMG_DY);
          this->update();
          break;
        case IMG_DY:
          imageMode(IMG_DZ);
          this->update();
          break;
        case IMG_DZ:
          imageMode(IMG_BLEND);
          this->update();
          break;
        case IMG_BLEND:
          imageMode(IMG_MIP);
          this->update();
          break;
        case IMG_MIP:
          imageMode(IMG_VAL);
          this->update();
          break;
        }
      break;
    case Qt::Key_Q:
      IntensityMax(cIWMax-(float)0.02*(cDataMax-cDataMin));
      this->update();
      break;
    case Qt::Key_W:
      IntensityMax(cIWMax+(float)0.02*(cDataMax-cDataMin));
      this->update();
      break;
    case (Qt::Key_A):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        cViewAxisLabel = !cViewAxisLabel;
        }
      else
        {
        IntensityMin(cIWMin-(float)0.02*(cDataMax-cDataMin));
        }
      this->update();
      break;
    case Qt::Key_S:
      IntensityMin(cIWMin+(float)0.02*(cDataMax-cDataMin));
      this->update();
      break;
    case (Qt::Key_I):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        //need to be completed for I
        }
      else
        {
        int newY;
        if(cFlipY[cWinOrientation])
          {
          newY = cWinCenter[cWinOrder[1]]-imgShiftSize;
          }
        else
          {
          newY = cWinCenter[cWinOrder[1]]+imgShiftSize;
          }
        cWinCenter[cWinOrder[1]] = newY;
        winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
        }

      this->update();
      break;
    case Qt::Key_M:
      int nY;
      if(cFlipY[cWinOrientation])
        {
        nY = cWinCenter[cWinOrder[1]]+imgShiftSize;
        }
      else
        {
        nY = cWinCenter[cWinOrder[1]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[1]] = nY;
      winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      this->update();
      break;
    case Qt::Key_J:
      int newX;
      if(cFlipX[cWinOrientation])
        {
        newX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      else
        {
        newX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = newX;
      winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      this->update();
      break;
    case Qt::Key_K:
      int nX;
      if(cFlipX[cWinOrientation])
        {
        nX = cWinCenter[cWinOrder[0]]-imgShiftSize;
        }
      else
        {
        nX = cWinCenter[cWinOrder[0]]+imgShiftSize;
        }
      cWinCenter[cWinOrder[0]] = nX;
      winCenter(cWinCenter[0], cWinCenter[1], cWinCenter[2]);
      this->update();
      break;
    case (Qt::Key_T):
    if(event->modifiers() & Qt::ShiftModifier)
      {
      cViewClickedPoints = !cViewClickedPoints;
      }
    else
      {
      Transpose(!cTranspose[cWinOrientation]);
      }
      this->update();
      break;
    case (Qt::Key_C):
    if(event->modifiers() & Qt::ShiftModifier)
      {
      cViewCrosshairs = !cViewCrosshairs;
      }
    else
      {
      }
      this->update();
      break;
    case (Qt::Key_V):
      if(event->modifiers() & Qt::ShiftModifier)
        {
        cViewValue = !cViewValue;
        }
      this->update();
      break;
    case Qt::Key_P:
    if(event->modifiers() & Qt::ShiftModifier)
      {
      cViewValuePhysicalUnits = !cViewValuePhysicalUnits;
      }
    else
      {
      this->saveClickedPointsStored();
      }
      this->update();
      break;
    case Qt::Key_D:
      if(event->modifiers() & Qt::ShiftModifier)
        {
        cViewDetails = !cViewDetails;
        }
      else
        {
        if(iwModeMin() == IW_FLIP)
          {
          iwModeMin(IW_MIN);
          this->update();
          }
        else
          {
          iwModeMin(IW_FLIP);
          this->update();
          }
        }
      this->update();
      break;
    case (Qt::Key_O):
    if(event->modifiers() & Qt::ShiftModifier)
      {
      cViewOverlayData = !cViewOverlayData;
      }
      this->update();
      break;
    case Qt::Key_H:
      this->Help();
    default:
      QWidget::keyPressEvent(event);
      break;
  }
}


void QtGlSliceView::size(int w, int h)
{
  this->update();
  this->paintGL();
}


/** Set up the OpenGL view port, matrix mode, etc. */
void QtGlSliceView::resizeGL( int w, int h )
{
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 200.0);
}

/** Initialize the OpenGL Window */
void QtGlSliceView::initializeGL() 
{
  glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
  glShadeModel(GL_FLAT);
    
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
    //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

/** Draw */
void QtGlSliceView::paintGL(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
  glLoadIdentity();
    
  glMatrixMode(GL_PROJECTION);
    
  GLint v[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, v);
  glLoadIdentity();
  glViewport(this->width()-v[0], this->height()-v[1], v[0], v[1]);
  glOrtho(this->width()-v[0], this->width(), this->height()-v[1], this->height(), -1, 1);

  if( !cImData ) 
  {
    std::cout << "no cImData !!!" << std::endl;
    return;
  }


  float scale0 = this->width()/(float)cDimSize[0] * cWinZoom
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  float scale1 = this->height()/(float)cDimSize[1] * cWinZoom
     * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);
    
   
  glRasterPos2i((cFlipX[cWinOrientation])?cW:0,
     (cFlipY[cWinOrientation])?cH:0); 
    
  glPixelZoom((cFlipX[cWinOrientation])?-scale0:scale0,
     (cFlipY[cWinOrientation])?-scale1:scale1);
    
  if( cValidImData && cViewImData )
  {
    glDrawPixels( cWinDataSizeX, cWinDataSizeY, 
                  GL_LUMINANCE, GL_UNSIGNED_BYTE, 
                  cWinImData );
  }
    
  if( cValidOverlayData && cViewOverlayData ) 
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawPixels(cWinDataSizeX, cWinDataSizeY, GL_RGBA, 
       GL_UNSIGNED_BYTE, cWinOverlayData);
    glDisable(GL_BLEND);
  }
    
  if( cViewAxisLabel ) 
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2, 0.2, 0.78, (float)0.75);

    if( !cFlipX[cWinOrientation] )
    {
      const int y = static_cast<int>(  this->height()/2-this->height()/2  );
      glRasterPos2i(this->width(), -y);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);
      // gl_draw( cAxisLabelX[cWinOrientation],
      //   cW-(gl_width(cAxisLabelX[cWinOrientation])+10), 
      //   static_cast<float>( y ) );
    }
    else
    {
      const int y = static_cast<int>( this->height()/2-this->height()/2  );
      glRasterPos2i(10, -y);
      glCallLists(strlen(cAxisLabelX[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelX[cWinOrientation]);

      //gl_draw( cAxisLabelX[cWinOrientation],
      // (gl_width(cAxisLabelX[cWinOrientation])+10),
      //  static_cast<float>( y ));
    }
      
    if(!cFlipY[cWinOrientation])
    {
      const int y = static_cast<int>( this->height()-this->height()-10 ) ;
      glRasterPos2i(this->width()/2, -y);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);

      //gl_draw( cAxisLabelY[cWinOrientation],
      //  cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
      //  static_cast<float>(y) );
    }
    else
    {
      const int y = static_cast<int>( this->height()+10 );
      glRasterPos2i(this->width()/2, -y);
      glCallLists(strlen(cAxisLabelY[cWinOrientation]), GL_UNSIGNED_BYTE, cAxisLabelY[cWinOrientation]);

      //gl_draw( cAxisLabelY[cWinOrientation], 
      //  cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
      //  static_cast<float>(y));
    }
    
    glDisable(GL_BLEND);
  }
    
  if( cViewValue ) 
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, (float)0.75);
    //gl_font(FL_TIMES_BOLD, 12);
    char s[80];
    if((ImagePixelType)1.1==1.1)
    {
      sprintf(s, "(%0.1f,  %0.1f,  %0.1f) = %0.3f", 
      cClickSelect[0],
      cClickSelect[1], 
      cClickSelect[2], 
      (float)cClickSelectV);
    }
    else
    {
      sprintf(s, "(%0.1f,  %0.1f,  %0.1f) = %d", 
      cClickSelect[0],
      cClickSelect[1], 
      cClickSelect[2], 
      (int)cClickSelectV);
    }
    //gl_draw( s,(int)(cW-(gl_width(s)+2)), 2);
    glDisable(GL_BLEND);    
  }

  if( cViewDetails )
  {
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9, 0.4, 0.1, (float)0.75);
    //gl_font(FL_TIMES_BOLD, 12);
    char s[80];
    if(cWinOrientation == 0)
        sprintf(s, "X - Slice: %3d", cWinCenter[0]);
      else if(cWinOrientation == 1)
        sprintf(s, "Y - Slice: %3d", cWinCenter[1]);
      else
        sprintf(s, "Z - Slice: %3d", cWinCenter[2]);
      gl_draw( s, 2, 2+5*(gl_height()+2) );
      sprintf(s, "Dims: %3d x %3d x %3d", 
        (int)cDimSize[0], (int)cDimSize[1], (int)cDimSize[2]);
      gl_draw( s, 2, 2+4*(gl_height()+2) );
      sprintf(s, "Voxel: %0.3f x %0.3f x %0.3f", 
        cSpacing[0], cSpacing[1], cSpacing[2]);
      gl_draw( s, 2, 2+3*(gl_height()+2) );
      sprintf(s, "Int. Range: %0.3f - %0.3f", (float)cDataMin, 
              (float)cDataMax);
      gl_draw( s, 2, 2+2*(gl_height()+2) );
      sprintf(s, "Int. Window: %0.3f(%s) - %0.3f(%s)", 
        (float)cIWMin, IWModeTypeName[cIWModeMin], 
        (float)cIWMax, IWModeTypeName[cIWModeMax]);
      gl_draw( s, 2, 2+1*(gl_height()+2) );
      sprintf(s, "View Mode: %s", ImageModeTypeName[cImageMode]);
      gl_draw( s, 2, 2+0*(gl_height()+2) );
      glDisable(GL_BLEND);*/
  }
    
  if( cViewCrosshairs 
    && static_cast<int>(cClickSelect[cWinOrder[2]]) == 
       static_cast<int>( sliceNum() ) )
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1, 0.64, 0.2, (float)0.75);
    int x;
    if(cFlipX[cWinOrientation])
    {
      x = (int)(cW - (cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
    }
    else
    {
      x = (int)((cClickSelect[cWinOrder[0]] - cWinMinX) * scale0);
    }
    int y;
    if(cFlipY[cWinOrientation])
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


void QtGlSliceView::mouseMoveEvent( QMouseEvent *event ) 
{
  float scale0 = this->width()/(float)cDimSize[0] * cWinZoom
    * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  float scale1 = this->height()/(float)cDimSize[1] * cWinZoom
    * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);

  if(cClickMode == CM_SELECT || cClickMode == CM_BOX) 
  {
    float p[3];
    p[cWinOrder[0]] = cWinMinX + ( (1-cFlipX[cWinOrientation])*(event->x()) 
                     + (cFlipX[cWinOrientation])*(this->width()-event->x()) ) 
                     / scale0;
    if(p[cWinOrder[0]]<cWinMinX) 
    {
      p[cWinOrder[0]] = cWinMinX;
    }
    if(p[cWinOrder[0]]>cWinMaxX) 
    {
      p[cWinOrder[0]] = cWinMaxX;
    }
    p[cWinOrder[1]] = cWinMinY + (cFlipY[cWinOrientation]*event->y() 
                     + (1-cFlipY[cWinOrientation])*(this->height()-event->y())) 
                     / scale1;
    if(p[cWinOrder[1]]<cWinMinY) 
    {
      p[cWinOrder[1]] = cWinMinY;
    }
    if(p[cWinOrder[1]]>cWinMaxY) 
    {
      p[cWinOrder[1]] = cWinMaxY;
    }
    if(cImageMode != IMG_MIP)
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
      this->clickSelect(p[0], p[1], p[2]);
    }
   }
   this->updateGL();
}

/** catches the mouse press to react appropriate
 *  Overriden to catch mousePressEvents and to start an internal
 *  timer, which calls the appropriate interaction routine */
void QtGlSliceView::mousePressEvent( QMouseEvent *event ) 
{
   if( event->button() & Qt::LeftButton )
   {
      if( event->button() & Qt::ShiftModifier )
      {
         // left mouse mouse and shift button
         /*this->mouseEventActive = true;
         QObject::connect( this->stepTimer, SIGNAL(timeout()),
                           this->shiftLeftButtonFunction );*/
      }
   }
   else if( event->button() & Qt::MidButton )
   {
      // middle mouse button
      //this->mouseEventActive = true;
      //QObject::connect( this->stepTimer, SIGNAL(timeout()),
      //                  this->middleButtonFunction );
   }
   else if( event->button() & Qt::RightButton )
   {
      // right mouse button
      //this->mouseEventActive = true;
      //QObject::connect( this->stepTimer, SIGNAL(timeout()),
      //                  this, this->rightButtonFunction );
   }
/*
   if( this->mouseEventActive ) {
      this->currentMousePos[0] = event->x();
      this->currentMousePos[1] = event->y();
      this->lastMousePos[0] = event->x();
      this->lastMousePos[1] = event->y();
      this->firstCall = true;
      this->stepTimer->start( this->interactionTime );
   }*/

   this->updateGL();
}


void QtGlSliceView::ChangeSlice(int value)
{
  sliceNum(value);
  update();
  paintGL();
  emit SliceChanged(value);
  emit
}



void QtGlSliceView::sliceNum(unsigned int newSliceNum)
{
  if(newSliceNum>=cDimSize[cWinOrder[2]])
    newSliceNum = cDimSize[cWinOrder[2]]-1;
  cWinCenter[cWinOrder[2]] = newSliceNum;
  
  /*if(cSliceNumCallBack != NULL)
    cSliceNumCallBack();
  if(cSliceNumArgCallBack != NULL)
    cSliceNumArgCallBack(cSliceNumArg);*/
}

unsigned int QtGlSliceView::sliceNum()
{
  return cWinCenter[cWinOrder[2]];
}

void QtGlSliceView::clickSelect(float newX, float newY, float newZ)
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
 
  emit Position(ind[0],ind[1],ind[2],cClickSelectV);

}

void QtGlSliceView::IntensityMax(int value)
{
  cIWMax = value;
  update();
  emit IntensityMaxChanged(cIWMax);
}

void QtGlSliceView::IntensityMin(int value)
{
  cIWMin = value;
  update();
  emit IntensityMinChanged(cIWMin);
}
 
void QtGlSliceView::ZoomIn()
{
  cWinZoom += 1;
  update();
  this->updateGL();
}
 
void QtGlSliceView::ZoomOut()
{
  cWinZoom -= 1;
  update();
  this->updateGL();
}
 

#endif

