/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    GLSliceView.h
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
   This software is distributed WITHOUT ANY WARRANTY; without even 
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
   PURPOSE.  See the above copyright notices for more information.
   
=========================================================================*/
#ifndef GLSLICEVIEW_H
#define GLSLICEVIEW_H

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "itkColorTable.h"

#include "SliceView.h"

#include <math.h>

namespace itk {
  
/**
* GLSliceView : Derived from abstract class SliceView and Fl_Gl_Window
* See SliceView.h for details...
  **/
  template <class ImagePixelType, class OverlayPixelType>
    class GLSliceView : public SliceView<ImagePixelType>, 
    public Fl_Gl_Window 
    {
public:
  
  typedef Image<ImagePixelType,3>     ImageType;
  typedef Image<OverlayPixelType,3>   OverlayType;
  typedef typename ImageType::Pointer      ImagePointer;
  typedef typename OverlayType::Pointer    OverlayPointer;
  typedef typename ImageType::RegionType   RegionType;
  typedef typename ImageType::SizeType     SizeType;
  typedef typename ImageType::IndexType    IndexType;
  
  typedef itk::ColorTable<float>                ColorTableType;
  typedef typename ColorTableType::Pointer      ColorTablePointer;
  
  
protected:
  bool        cValidOverlayData;
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  

  ColorTablePointer      cColorTable;
  
public:
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLSliceView(int x, int y, int w, int h, const char *l);
  
  /*! Specify the 3D image to view slice by slice */
  virtual void SetInputImage(ImageType * newImData);
  virtual const ImagePointer & GetInputImage(void) const;
  
  /*! Specify the 3D image to view as an overlay */
  void SetInputOverlay(OverlayType * newOverlayData);
  
  /*! Return a pointer to the overlay data */
  const OverlayPointer & GetInputOverlay(void) const;
  
  /*! Turn on/off the viewing of the overlay */
  void  ViewOverlayData(bool newViewOverlayData);
  
  /*! Status of the overlay - viewed /not viewed */
  bool  ViewOverlayData(void);
  
  /*! Specify the opacity of the overlay */
  void  OverlayOpacity(float newOverlayOpacity);
  
  /*! Get the opacity of the overlay */
  float OverlayOpacity(void);
  
  /*! Called when overlay is toggled or opacity is changed */
  void  ViewOverlayCallBack(void (* newOverlayCallBack)(void));
  
  ColorTablePointer GetColorTable(void);
  void SetColorTable(ColorTablePointer newColorTable);


  
  virtual void clickSelect(float x, float y, float z);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);
  
  virtual void update();
  virtual void draw();
  
  virtual int  handle(int event);
  };
  
  
  
template <class ImagePixelType, class OverlayPixelType>
GLSliceView<ImagePixelType, OverlayPixelType>::
GLSliceView(int x, int y, int w, int h, const char *l):
SliceView<ImagePixelType>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
  {
  when(FL_WHEN_NOT_CHANGED | FL_WHEN_ENTER_KEY);
  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cColorTable = ColorTableType::New();
  //cColorTable.useGray();
  cColorTable->useDiscrete();
  }
  
  
  
  
//
// Set the input image to be displayed
// Warning: the current overlay is destroyed if the size of the image
// is different from the size of the overlay.
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
SetInputImage(ImageType * newImData)
  {
  RegionType region = newImData->GetLargestPossibleRegion();
  if( region.GetNumberOfPixels() == 0 ) 
    {
    return;
    }

  SizeType   imSize   = region.GetSize();

  // If the overlay has been set and the size is different from the new image,
  // it is removed.
  if( cValidOverlayData)
    {  
    SizeType overlay_size = cOverlayData->GetLargestPossibleRegion().GetSize();
    
    if((overlay_size[0] != imSize[0])
      ||  (overlay_size[1] != imSize[1])
      ||  (overlay_size[2] != imSize[2]))
      {
       if(cWinOverlayData != NULL)
         {
         delete [] cWinOverlayData;
         }
       cWinOverlayData       = NULL;
       cValidOverlayData     = false;
      }       
    }

  cImData = newImData;
  cDimSize[0]=imSize[0];
  cDimSize[1]=imSize[1];
  cDimSize[2]=imSize[2];
  cSpacing[0]=cImData->GetSpacing()[0];
  cSpacing[1]=cImData->GetSpacing()[1];
  cSpacing[2]=cImData->GetSpacing()[2];
  cOrigin[0]=cImData->GetOrigin()[0];
  cOrigin[1]=cImData->GetOrigin()[1];
  cOrigin[2]=cImData->GetOrigin()[2];
    
  //calculating cDataMax and cDataMin    
  IndexType ind;
  ind[0] = 0; 
  ind[1] = 0; 
  ind[2] = 0;
  
  cDataMax = cImData->GetPixel(ind);
  cDataMin = cDataMax;
  ImagePixelType tf;
  
  
  for( unsigned int i=0; i<cDimSize[0]; i++ )
    {
    ind[0] = i;
    for(unsigned int j=0; j<cDimSize[1]; j++ )
      {
      ind[1] = j;
      for( unsigned int k=0; k<cDimSize[2]; k++ )
        {
        ind[2] = k;
        tf = cImData->GetPixel(ind);
        if(tf > cDataMax) 
          {
          cDataMax = tf;
          }
        else 
          {
          if(tf < cDataMin)
            {
            cDataMin = tf;
            }
          }
        }
      }
    }
  
  cIWMin      = cDataMin;
  cIWMax      = cDataMax;
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
  
  cWinDataSizeX = cWinMaxX;//cDimSize[0];
  cWinDataSizeY = cWinMaxY;//cDimSize[1];
  
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
  
  }





//
//
template <class ImagePixelType, class OverlayPixelType>
const typename Image<ImagePixelType,3>::Pointer &
GLSliceView<ImagePixelType, OverlayPixelType>
::GetInputImage(void) const
  {
  return cImData;
  }



//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>
::SetInputOverlay( OverlayType * newOverlayData )
  {
  RegionType newoverlay_region = 
    newOverlayData->GetLargestPossibleRegion();

  SizeType   newoverlay_size   = newoverlay_region.GetSize();

  if( cValidImData 
      &&  (newoverlay_size[0] == cDimSize[0])
      &&  (newoverlay_size[1] == cDimSize[1])
      &&  (newoverlay_size[2] == cDimSize[2])
    )
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
    }
  else // return a warning
    {
      if(!cValidImData)
        {
        std::cout << "GLSliceView: Please set the input image before overlay"  
                  << std::endl;
        std::cout << "GLSliceView: Overlay not set." << std::endl;
        }
      else if((newoverlay_size[0] != cDimSize[0])
      ||  (newoverlay_size[1] != cDimSize[1])
      ||  (newoverlay_size[2] != cDimSize[2])
      )
        {
        std::cout << "GLSliceView: Error: overlay and input images should be the same size" 
                  << std::endl;
        std::cout << "GLSliceView: Overlay not set." << std::endl;
        }
      
    }
  }





template <class ImagePixelType, class OverlayPixelType>
const typename GLSliceView<ImagePixelType, 
OverlayPixelType>::OverlayPointer &
GLSliceView<ImagePixelType, OverlayPixelType>::GetInputOverlay( void ) 
const
  {
  return cOverlayData;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewOverlayData( bool newViewOverlayData)
  {
  
  cViewOverlayData = newViewOverlayData;
  
  if( cViewOverlayCallBack != NULL )
    {
    cViewOverlayCallBack();
    }
  
  this->redraw();
  
  }

template <class ImagePixelType, class OverlayPixelType>
bool 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewOverlayData(void)
  {
  
  return cViewOverlayData;
  
  }


template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewOverlayCallBack( void (* newViewOverlayCallBack)(void) )
  {
  cViewOverlayCallBack = newViewOverlayCallBack;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
OverlayOpacity( float newOverlayOpacity )
  {
  cOverlayOpacity = newOverlayOpacity;
  
  if(cViewOverlayCallBack != NULL) 
    {
    cViewOverlayCallBack();
    }
  }




template <class ImagePixelType, class OverlayPixelType>
float 
GLSliceView<ImagePixelType, OverlayPixelType>::
OverlayOpacity(void)
  {
  return cOverlayOpacity;
  }




//
//
//
template <class ImagePixelType, class OverlayPixelType>
typename GLSliceView<ImagePixelType, OverlayPixelType>::ColorTablePointer 
GLSliceView<ImagePixelType, OverlayPixelType>::
GetColorTable(void)
  {
  return cColorTable;
  }



//
//
//
template <class ImagePixelType, class OverlayPixelType>
void
GLSliceView<ImagePixelType, OverlayPixelType>::
SetColorTable(typename 
              GLSliceView<ImagePixelType, OverlayPixelType>::ColorTablePointer 
              newColorTable)
  {
  cColorTable = newColorTable;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
update()
  {
  
  if( !cValidImData ) 
    {
    return;
    }

  double zoomBase = cW/(cDimSize[cWinOrder[0]]*(fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0])));
  if(zoomBase >
      cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]))))
    {
    zoomBase = cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0])));
    }
  double scale0 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
  double scale1 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);

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
        
        if( sizeof( OverlayPixelType ) == 1  ||
            sizeof( OverlayPixelType ) == 2      )
          {
          if (sizeof( OverlayPixelType ) == 1)
            {
            m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
            }
          else
            {
            m = (int)*((unsigned short *)&(cOverlayData->GetPixel(ind)));
            }
          if( m >= (int)cColorTable->GetNumberOfColors() ) 
            { 
            m = cColorTable->GetNumberOfColors() - 1;
            }
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




template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::
clickSelect(float x, float y, float z)
  {
  SliceView<ImagePixelType>::clickSelect(x, y, z);
  if(cViewValue || cViewCrosshairs)
    {
    this->redraw();
    }
  }

template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::size(int w, int h)
  {
  SliceView<ImagePixelType>::size(w, h);
  Fl_Gl_Window::size(w, h);
  this->update();
  this->redraw();
  }




template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  SliceView<ImagePixelType>::resize(x, y, w, h);
  Fl_Gl_Window::resize(x, y, w, h);
  this->update();
  this->redraw();
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::draw(void)
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
    
    double zoomBase = cW/(cDimSize[cWinOrder[0]]*(fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0])));
    if(zoomBase >
       cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]))))
      {
      zoomBase = cH/(cDimSize[cWinOrder[1]]*(fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0])));
      }

    double scale0 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[0]])/fabs(cSpacing[0]);
    double scale1 = cWinZoom * zoomBase * fabs(cSpacing[cWinOrder[1]])/fabs(cSpacing[0]);
    
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
      float px, py, pz, val = cClickSelectV;
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
        x = (int)(cW - (cClickSelect[cWinOrder[0]] - cWinMinX) * scale0 - originX);
        }
      else
        {
        x = (int)((cClickSelect[cWinOrder[0]] - cWinMinX) * scale0 + originX);
        }
      int y;
      if(cFlipY[cWinOrientation])
        {
        y = (int)(cH - (cClickSelect[cWinOrder[1]] - cWinMinY) * scale1 - originY);
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





//
//
template <class ImagePixelType, class OverlayPixelType>
int GLSliceView<ImagePixelType, OverlayPixelType>::handle(int event)
  {
  int x = Fl::event_x();
  int y = Fl::event_y();
  int button;
  
  static int boxX, boxY;
  
  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
    case FL_RELEASE:
      button = Fl::event_button()-1;       
      if(button <= 0) 
        {
        if(cClickMode == CM_BOX) 
          {
          if(event == FL_PUSH)
            {
            boxX = x;
            boxY = y;
            }
          else
            {
            if(event == FL_DRAG)
              {
              make_current();
              fl_overlay_clear();
              fl_overlay_rect(boxX, boxY, x-boxY, y-boxY);
              }
            else
              {
              make_current();
              fl_overlay_clear();
              }
            }
          }
        }
      break;
    default:
      break;
    }
  
  return SliceView<ImagePixelType>::handle(event);
  }

}; //namespace
#endif

