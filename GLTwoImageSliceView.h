/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    GLTwoImageSliceView.h
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
   This software is distributed WITHOUT ANY WARRANTY; without even 
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
   PURPOSE.  See the above copyright notices for more information.
   
=========================================================================*/
#ifndef GLTWOIMAGESLICEVIEW_H
#define GLTWOIMAGESLICEVIEW_H

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "SliceView.h"
#include "GLSliceView.h"

#include <math.h>

namespace itk {
  
/**
* GLSliceView : Derived from abstract class SliceView and Fl_Gl_Window
* See SliceView.h for details...
  **/
template <class ImagePixelType, class OverlayPixelType>
class GLTwoImageSliceView 
  : public GLSliceView<ImagePixelType, OverlayPixelType> 
  {
  public:


    typedef itk::Image< ImagePixelType, 3> ImageType;
    typedef typename ImageType::Pointer ImagePointer;
    typedef typename ImageType::ConstPointer ImageConstPointer;
    typedef GLSliceView<ImagePixelType, OverlayPixelType> Superclass;
    typedef typename Superclass::RegionType RegionType;
    typedef typename Superclass::SizeType SizeType;
    typedef typename Superclass::IndexType IndexType;

    /*! FLTK required constructor - must use imData() to complete 
     *  definition */
    GLTwoImageSliceView(int x, int y, int w, int h, const char *l);
    
    /*! Specify the 3D image to view slice by slice */
    virtual void SetInputImage(ImageType * newImData);
    virtual void SetSecondInputImage(ImageType * newImData);

    virtual void clickSelect(float newX, float newY, float newZ);
    virtual void size(int w, int h);
    
    virtual void resize(int x, int y, int w, int h);

    virtual void update();

    virtual void draw();
    
    virtual int handle(int event);

  protected:

    float         cClickSelectR;
    float         cClickSelectG;
    float         cClickSelectB;

    ImagePointer  cIm2Data;
    bool          cValidIm2Data;

    unsigned long cIm2DimSize[3];

    double        cIm2DataMin;
    double        cIm2DataMax;
  };
  
template <class ImagePixelType, class OverlayPixelType>
GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
GLTwoImageSliceView(int x, int y, int w, int h, const char *l)
:GLSliceView<ImagePixelType, OverlayPixelType>(x, y, w, h, l)
  {
  cClickSelectR = 0;
  cClickSelectG = 0;
  cClickSelectB = 0;
  cIm2Data = NULL;
  cIm2DataMin = 0;
  cIm2DataMax = 0;
  cValidIm2Data = false;
  }
  
//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
SetInputImage(ImageType * newImData)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::SetInputImage(newImData);

  if(cWinImData != NULL)
    {
    delete [] cWinImData;
    }
  cWinImData = new unsigned char[ cWinDataSizeX * cWinDataSizeY * 3 ];
  
  cValidIm2Data = false;
  }

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
SetSecondInputImage(ImageType * newImData)
  {
  cIm2Data = newImData;
  cValidIm2Data = true;

  SizeType imSize = cIm2Data->GetLargestPossibleRegion().GetSize();
  cIm2DimSize[0] = imSize[0];
  cIm2DimSize[1] = imSize[1];
  cIm2DimSize[2] = imSize[2];

  IndexType ind;
  ind[0] = 0; 
  ind[1] = 0; 
  ind[2] = 0;
  
  cIm2DataMax = cIm2Data->GetPixel(ind);
  cIm2DataMin = cIm2DataMax;
  ImagePixelType tf;
  
  for( unsigned int i=0; i<cIm2DimSize[0]; i++ )
    {
    ind[0] = i;
    for(unsigned int j=0; j<cIm2DimSize[1]; j++ )
      {
      ind[1] = j;
      for( unsigned int k=0; k<cIm2DimSize[2]; k++ )
        {
        ind[2] = k;
        tf = cIm2Data->GetPixel(ind);
        if(tf > cIm2DataMax) 
          {
          cIm2DataMax = tf;
          }
        else 
          {
          if(tf < cIm2DataMin)
            {
            cIm2DataMin = tf;
            }
          }
        }
      }
    }

  }

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
clickSelect(float newX, float newY, float newZ)
  {    
  cClickSelect[0] = newX;
  if(cClickSelect[0]<0) 
    {
    cClickSelect[0] = 0;
    }
  if(cClickSelect[0] >= cDimSize[0])
    {
    cClickSelect[0] = cDimSize[0]-1;
    }

  cClickSelect[1] = newY;
  if(cClickSelect[1]<0)
    {
    cClickSelect[1] = 0;
    }
  if(cClickSelect[1] >= cDimSize[1])
    {
    cClickSelect[1] = cDimSize[1]-1;
    }

  cClickSelect[2] = newZ;

  if(cClickSelect[2]<0)
    {
    cClickSelect[2] = 0;
    }
  if(cClickSelect[2] >= cDimSize[2])
    {
    cClickSelect[2] = cDimSize[2]-1;
    }

  IndexType ind;

  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
  ind[2] = (unsigned long)cClickSelect[2];
  cClickSelectV = cImData->GetPixel(ind);
  cClickSelectR = cImData->GetPixel(ind);
  itk::Point<double, 3> pnt;
  IndexType ind2;
  cImData->TransformIndexToPhysicalPoint(ind, pnt);
  if(cIm2Data)
    {
    if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
      {
      cClickSelectG = cIm2Data->GetPixel(ind2);
      }
    }
      
  /*if length of list is equal to max, remove the earliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    {
    cClickedPoints.pop_back();
    }
  
  ClickPoint point( cClickSelect[0], 
                    cClickSelect[1], 
                    cClickSelect[2], 
                    cClickSelectV     );

  cClickedPoints.push_front( point );

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

  if(cViewValue || cViewCrosshairs)
    {
    this->redraw();
    }
  }


template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
size(int w, int h)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::size(w, h);
  }
    
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::resize(x, y, w, h);
  }

//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
update()
  {
  if( !cValidImData ) 
    {
    return;
    }
  
  double zoomBase = cW/(cDimSize[cWinOrder[0]]*(fabs(cSpacing[cWinOrder[0]])
                        /fabs(cSpacing[0])));
  if(zoomBase >
      cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])
          /fabs(cSpacing[0]))))
    {
    zoomBase = cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])
                   /fabs(cSpacing[0])));
    }
  double scale0 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[0]])
                                             /fabs(cSpacing[0]);
  double scale1 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[1]])
                                             /fabs(cSpacing[0]);

  if(cWinZoom>1)
    {
    cWinSizeX = (int)( cW / scale0 );
    cWinMinX = (int)( (int)cWinCenter[ cWinOrder[0] ] - cWinSizeX/2);
    cWinMaxX = (int)( (int)cWinCenter[ cWinOrder[0] ] + cWinSizeX/2);
    }
  else
    {
    cWinSizeX = (int)(cDimSize[ cWinOrder[0] ]);
    cWinMinX = 0;
    cWinMaxX = (int)( (int)(cDimSize[ cWinOrder[0] ]) - 1 );
    cWinCenter[cWinOrder[0]] = (int)( cDimSize[ cWinOrder[0] ] / 2);
    }
  if( cWinMinX <= - (int) cDimSize[ cWinOrder[0] ] ) 
    {
    cWinMinX = -(int)cDimSize[ cWinOrder[0] ] + 1;
    }
  else if(cWinMinX >= (int)cDimSize[ cWinOrder[0] ]) 
    {
    cWinMinX = (int)cDimSize[ cWinOrder[0] ] - 1;
    }
  if( cWinMaxX >= (int)( cDimSize[ cWinOrder[0] ] ) )
    {
    cWinMaxX = (int)cDimSize[ cWinOrder[0] ] - 1;
    }
  
  if(cWinZoom>1)
    {
    cWinSizeY = (int)( cH / scale1 );
    cWinMinY = (int)( (int)(cWinCenter[ cWinOrder[1] ]) - cWinSizeY/2 );
    cWinMaxY = (int)( (int)(cWinCenter[ cWinOrder[1] ]) + cWinSizeY/2 );
    }
  else
    {
    cWinSizeY = (int)(cDimSize[ cWinOrder[1] ]);
    cWinMinY = 0;
    cWinMaxY = (int)( (int)(cDimSize[ cWinOrder[1] ]) - 1 );
    cWinCenter[cWinOrder[1]] = (int)( cDimSize[ cWinOrder[1] ] / 2);
    }
  if( cWinMinY <= - (int)( cDimSize[ cWinOrder[1] ] ) ) 
    {
    cWinMinY = -(int)cDimSize[ cWinOrder[1] ] + 1;
    }
  else if( cWinMinY >= (int)(cDimSize[ cWinOrder[1] ] ) ) 
    {
    cWinMinY = cDimSize[ cWinOrder[1] ] - 1;
    } 
  if( cWinMaxY >= (int)( cDimSize[ cWinOrder[1] ] ) ) 
    {
    cWinMaxY = cDimSize[ cWinOrder[1] ] - 1;
    }

  memset( cWinImData, 0, cWinDataSizeX*cWinDataSizeY*3 );
  if( cValidOverlayData ) 
    {
    memset(cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4);
    }
  
  IndexType ind;
  IndexType ind2;
  itk::Point<double, 3> pnt;
  
  int l, m;
  
  float tf[3];
  float tfv[3];
  
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

      tfv[0] = cImData->GetPixel(ind);
      if(cValidIm2Data)
        {
        cImData->TransformIndexToPhysicalPoint(ind, pnt);
        if(cIm2Data->TransformPhysicalPointToIndex(pnt, ind2))
          {
          tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                   / (cIm2DataMax-cIm2DataMin);
          tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
          }
        else
          {
          tfv[1] = cDataMin;
          }
        }
      else
        {
        tfv[1] = 0;
        }
      tfv[2] = 0;
      switch( cImageMode ) 
        {
        default:
        case IMG_VAL:
          tf[0] = (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
          tf[1] = (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
          break;
        case IMG_INV:
          tf[0] = (float)((cIWMax-(float)tfv[0])/(cIWMax-cIWMin)*255);
          tf[1] = (float)((cIWMax-(float)tfv[1])/(cIWMax-cIWMin)*255);
          break;
        case IMG_LOG:
          tf[0] = (float)(log((float)tfv[0]-cIWMin+0.00000001)
                          /log(cIWMax-cIWMin+0.00000001)*255);
          tf[1] = (float)(log((float)tfv[1]-cIWMin+0.00000001)
                          /log(cIWMax-cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]--;
            tfv[0] = cImData->GetPixel(ind);
            if(cValidIm2Data )
              {
              cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                         / (cIm2DataMax-cIm2DataMin);
                tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
                }
               else
                 {
                 tfv[1] = cDataMin;
                 }
              }
            tf[0] -= (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[0]++;
            tf[0] += 128;
            tf[1] += 128;
            } 
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_DY:
          if(ind[1]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]--;
            tfv[0] = cImData->GetPixel(ind);
            if(cValidIm2Data)
              {
              cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                         / (cIm2DataMax-cIm2DataMin);
                tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
                }
               else
                 {
                 tfv[1] = cDataMin;
                 }
              }
            tf[0] -= (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[1]++;
            tf[0] += 128;
            tf[1] += 128;
            }
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_DZ:
          if(ind[2]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]--;
            tfv[0] = cImData->GetPixel(ind);
            if(cValidIm2Data)
              {
              cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                         / (cIm2DataMax-cIm2DataMin);
                tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
                }
              else
                {
                tfv[1] = cDataMin;
                }
              }
            tf[0] -= (float)(((float)tfv[0]-cIWMin)/(cIWMax-cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]++;
            tf[0] += 128;
            tf[1] += 128;
            }
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_BLEND:
          {
          const int tempval = (int)cWinCenter[cWinOrder[2]]-1;
          int tmpI = ind[cWinOrder[2]];
          ind[cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          tfv[0] = cImData->GetPixel(ind);
          if(cValidIm2Data )
            {
            cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                       / (cIm2DataMax-cIm2DataMin);
              tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
              }
            else
              {
              tfv[1] = cDataMin;
              }
            }
          tf[0] = (float)(tfv[0]);
          tf[1] = (float)(tfv[1]);
          
          ind[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          tfv[0] = cImData->GetPixel(ind);
          if(cValidIm2Data)
            {
            cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                       / (cIm2DataMax-cIm2DataMin);
              tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
              }
            else
              {
              tfv[1] = cDataMin;
              }
            }
          tf[0] += (float)(tfv[0])*2;
          tf[1] += (float)(tfv[1])*2;
          
          const int tempval1 = (int)cDimSize[cWinOrder[2]]-1;
          const int tempval2 = (int)cWinCenter[cWinOrder[2]]+1;
          ind[cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          tfv[0] = cImData->GetPixel(ind);
          if(cValidIm2Data )
            {
            cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                       / (cIm2DataMax-cIm2DataMin);
              tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
              }
            else
              {
              tfv[1] = cDataMin;
              }
            }
          tf[0] += (float)(tfv[0]);
          tf[1] += (float)(tfv[1]);
          
          tf[0] = (float)((tf[0]/4-cIWMin)/(cIWMax-cIWMin)*255);
          tf[1] = (float)((tf[1]/4-cIWMin)/(cIWMax-cIWMin)*255);
          ind[cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf[0] = cIWMin;
          tf[1] = cIWMin;
          m = (j-cWinMinX) + (k-cWinMinY)*cWinDataSizeX;
          cWinZBuffer[m] = 0;
          int tmpI = ind[cWinOrder[2]];
          float tfp = 0;
          float tft = tf[0]+tf[1];
          for(l=0; l<(int)cDimSize[cWinOrder[2]]; l++) 
            {
            ind[cWinOrder[2]] = l;        
            tfv[0] = cImData->GetPixel(ind);
            if(cValidIm2Data )
              {
              cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (cIm2Data->GetPixel(ind2)-cIm2DataMin)
                         / (cIm2DataMax-cIm2DataMin);
                tfv[1] = tfv[1] * (cDataMax-cDataMin) + cDataMin;
                }
              else
                {
                tfv[1] = cDataMin;
                }
              }
            tfp = (float)tfv[0] + tfv[1] ;
            if(tfp > tft) 
              {
              tf[0] = (float)(tfv[0]);
              tf[1] = (float)(tfv[1]);
              cWinZBuffer[m] = (unsigned short)l;
              }
            }
          tf[0] = (float)((tf[0]-cIWMin)/(cIWMax-cIWMin)*255);
          tf[1] = (float)((tf[1]-cIWMin)/(cIWMax-cIWMin)*255);
          ind[cWinOrder[2]] = tmpI;
          break;
        }
      
      int c;
      for(c=0; c<2; c++)
        {
        if( tf[c] > 255 )
          {
          switch(cIWModeMax) 
            {
            case IW_MIN:
              tf[c] = 0;
              break;
            default:
            case IW_MAX:
              tf[c] = 255;
              break;
            case IW_FLIP:
              tf[c] = 512-tf[c];
              if(tf[c]<0) 
                {
                tf[c] = 0;
                }
              break;
            }
          }
        else 
          {
          if( tf[c] < 0 )
            {
            switch(cIWModeMin) 
              {
              default:
              case IW_MIN:
                tf[c] = 0;
                break;
              case IW_MAX:
                tf[c] = 255;
                break;
              case IW_FLIP:
                tf[c] = -tf[c];
                if(tf[c]>255)
                  {
                  tf[c] = 255;
                  }
                break;
              }
            }
          }
        }
        
      l = (j-cWinMinX)*3 + (k-cWinMinY)*cWinDataSizeX*3;
      cWinImData[l+0] = (unsigned char)tf[0];
      cWinImData[l+1] = (unsigned char)tf[1];
      cWinImData[l+2] = (unsigned char)tf[2];
      
      if( cValidOverlayData ) 
        {
        l = (j-cWinMinX)*4 + (k-cWinMinY)*cWinDataSizeX*4;
        if(cImageMode == IMG_MIP)
          {
          ind[cWinOrder[2]] = cWinZBuffer[(j-cWinMinX) + 
                              (k-cWinMinY)*cWinDataSizeX];
          }
        
        if( sizeof( OverlayPixelType ) == 1 )
          {
          m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
          if( m > 0 ) {
            m = m - 1;
            cWinOverlayData[l+0] = 
              (unsigned char)(cColorTable->GetColorComponent(m, 'r')*255);
            cWinOverlayData[l+1] = 
              (unsigned char)(cColorTable->GetColorComponent(m, 'g')*255);
            cWinOverlayData[l+2] = 
              (unsigned char)(cColorTable->GetColorComponent(m, 'b')*255);
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
  
  this->redraw();
  
  }

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
draw()
  {
  if( !valid() )
    {
    glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
    glShadeModel(GL_FLAT);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
    //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }
  else
    {
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    
    glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    ortho();
    
    if( !cImData ) 
      {
      return;
      }
    
    double zoomBase = cW/(cDimSize[cWinOrder[0]]*(fabs(cSpacing[cWinOrder[0]])
                                                  /fabs(cSpacing[0])));
    if(zoomBase >
       cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])
                                   /fabs(cSpacing[0]))))
      {
      zoomBase = cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])
                                             /fabs(cSpacing[0])));
      }

    double scale0 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[0]])
                             / fabs(cSpacing[0]);
    double scale1 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[1]]) 
                             / fabs(cSpacing[0]);

    int originX = 0;
    int originY = 0;
    if(cWinZoom<=1)
      {
      if(cW-scale0*cDimSize[cWinOrder[0]]>0)
        {
        originX = (int)((cW-scale0*cDimSize[cWinOrder[0]])/2.0);
        }
      if(cH-scale1*cDimSize[cWinOrder[1]]>0)
        {
        originY = (int)((cH-scale1*cDimSize[cWinOrder[1]])/2.0);
        }
      }
    glRasterPos2i((cFlipX[cWinOrientation])?cW-originX:originX,
      (cFlipY[cWinOrientation])?cH-originY:originY);  
    glPixelZoom((cFlipX[cWinOrientation])?-scale0:scale0,
      (cFlipY[cWinOrientation])?-scale1:scale1);
    
    if( cValidImData && cViewImData )
      {
      glDrawPixels( cWinDataSizeX, cWinDataSizeY, 
                    GL_RGB, GL_UNSIGNED_BYTE, 
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
      gl_font(FL_TIMES_BOLD, 12);
      
      if( !cFlipX[cWinOrientation] )
        {
        const int y = static_cast<int>(  cH/2-gl_height()/2  );
        gl_draw( cAxisLabelX[cWinOrientation],
          cW-(gl_width(cAxisLabelX[cWinOrientation])+10), 
          static_cast<float>( y ) );
        }
      else
        {
        const int y = static_cast<int>( cH/2-gl_height()/2  );
        gl_draw( cAxisLabelX[cWinOrientation],
          (gl_width(cAxisLabelX[cWinOrientation])+10),
          static_cast<float>( y ));
        }
      
      if(!cFlipY[cWinOrientation])
        {
        const int y = static_cast<int>( cH-gl_height()-10 ) ;
        gl_draw( cAxisLabelY[cWinOrientation],
          cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
          static_cast<float>(y) );
        }
      else
        {
        const int y = static_cast<int>( gl_height()+10 );
        gl_draw( cAxisLabelY[cWinOrientation], 
          cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
          static_cast<float>(y));
        }
      
      glDisable(GL_BLEND);
      }
    if( cViewValue ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      char s[80];
      float px, py, pz;
      float val = cClickSelectV;
      char * suffix = "";
      if( cViewValuePhysicalUnits )
        {
        px = cOrigin[0]+cSpacing[0]*cClickSelect[0];
        py = cOrigin[1]+cSpacing[1]*cClickSelect[1];
        pz = cOrigin[2]+cSpacing[2]*cClickSelect[2];
        suffix = cPhysicalUnitsName;
        }
       else
        {
        px = cClickSelect[0];
        py = cClickSelect[1];
        pz = cClickSelect[2];
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
      gl_draw( s,
        (int)(cW-(gl_width(s)+2)), 2);
      glDisable(GL_BLEND);
      
      }
    if( cViewDetails )
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.9, 0.4, 0.1, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
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
      glDisable(GL_BLEND);
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
        x = (int)(cW - (cClickSelect[cWinOrder[0]] - cWinMinX) * scale0 
                     - originX);
        }
      else
        {
        x = (int)((cClickSelect[cWinOrder[0]] - cWinMinX) * scale0 + originX);
        }
      int y;
      if(cFlipY[cWinOrientation])
        {
        y = (int)(cH - (cClickSelect[cWinOrder[1]] - cWinMinY) * scale1
                     - originY);
        }
      else
        {
        y = (int)((cClickSelect[cWinOrder[1]] - cWinMinY) * scale1 + originY);
        }
      glBegin(GL_LINES);
      glVertex2d(0, y);
      glVertex2d(x-2, y);
      glVertex2d(x+2, y);
      glVertex2d(cW-1, y);
      glVertex2d(x, 0);
      glVertex2d(x, y-2);
      glVertex2d(x, y+2);
      glVertex2d(x, cH-1);
      glEnd();
      glDisable(GL_BLEND);
      }
    
    }
  }

template <class ImagePixelType, class OverlayPixelType>
int GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
handle(int event)
  {
  return GLSliceView<ImagePixelType, OverlayPixelType>::handle(event);
  }

}; //namespace
#endif

