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

#include <cstring>
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
  typedef typename ImageType::PointType    PointType;
  
  typedef itk::ColorTable<float>                ColorTableType;
  typedef typename ColorTableType::Pointer      ColorTablePointer;
  
  
protected:
  bool        cValidOverlayData;
  float       cOverlayOpacity;
  
  OverlayPointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);
  
  unsigned char * cWinOverlayData;
  

  ColorTablePointer      cColorTable;

  unsigned int           cOverlayColorIndex;
  
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


  /*! Turn on/off the display of clicked points */
  void ViewClickedPoints( bool newViewClickedPoints );

  /*! Status of clicked points display - on/off */
  bool ViewClickedPoints();

  virtual void clickSelect(float x, float y, float z);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);
  
  virtual void update();
  virtual void draw();
  
  virtual int  handle(int event);
  
  /*! Display Overlay in Color 'c'. You must ensure that the color-table specified
   * contains color 'c'. For example with the default useDiscrete() color table,
   * SetOverlayColorIndex( 0 ) will display the overlay in red. 
   * SetOverlayColorIndex( 1 ) purple etc.... */
  void SetOverlayColorIndex( unsigned int c)
    {
    cOverlayColorIndex = c;
    }
  };
  
  
  
template <class ImagePixelType, class OverlayPixelType>
GLSliceView<ImagePixelType, OverlayPixelType>::
GLSliceView(int lx, int ly, int lw, int lh, const char *ll):
SliceView<ImagePixelType>(lx, ly, lw, lh, ll), Fl_Gl_Window(lx, ly, lw, lh, ll)
  {
  when(FL_WHEN_NOT_CHANGED | FL_WHEN_ENTER_KEY);
  cValidOverlayData     = false;
  this->cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cColorTable = ColorTableType::New();
  //cColorTable.useGray();
  cColorTable->UseDiscreteColors();
  cOverlayColorIndex = 7;  //default white
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

  this->cImData = newImData;
  this->cDimSize[0]=imSize[0];
  this->cDimSize[1]=imSize[1];
  this->cDimSize[2]=imSize[2];
  this->cSpacing[0]=this->cImData->GetSpacing()[0];
  this->cSpacing[1]=this->cImData->GetSpacing()[1];
  this->cSpacing[2]=this->cImData->GetSpacing()[2];
  this->cOrigin[0]=this->cImData->GetOrigin()[0];
  this->cOrigin[1]=this->cImData->GetOrigin()[1];
  this->cOrigin[2]=this->cImData->GetOrigin()[2];
    
  //calculating cDataMax and cDataMin    
  IndexType ind;
  ind[0] = 0; 
  ind[1] = 0; 
  ind[2] = 0;
  
  this->cDataMax = this->cImData->GetPixel(ind);
  this->cDataMin = this->cDataMax;
  ImagePixelType tf;
  
  
  for( unsigned int ii=0; ii<this->cDimSize[0]; ii++ )
    {
    ind[0] = ii;
    for(unsigned int jj=0; jj<this->cDimSize[1]; jj++ )
      {
      ind[1] = jj;
      for( unsigned int kk=0; kk<this->cDimSize[2]; kk++ )
        {
        ind[2] = kk;
        tf = this->cImData->GetPixel(ind);
        if(tf > this->cDataMax) 
          {
          this->cDataMax = tf;
          }
        else 
          {
          if(tf < this->cDataMin)
            {
            this->cDataMin = tf;
            }
          }
        }
      }
    }
  
  this->cIWMin      = this->cDataMin;
  this->cIWMax      = this->cDataMax;
  this->cIWModeMin  = IW_MIN;
  this->cIWModeMax  = IW_MAX;
  
  this->cImageMode = IMG_VAL;
  
  this->cWinZoom = 1;
  
  this->cWinOrientation = 2;
  this->cWinOrder[0] = 0;
  this->cWinOrder[1] = 1;
  this->cWinOrder[2] = 2;
  
  this->cWinCenter[0] = this->cDimSize[0]/2;
  this->cWinCenter[1] = this->cDimSize[1]/2;
  this->cWinCenter[2] = 0;
  
  this->cWinMinX  = 0;
  this->cWinSizeX = this->cDimSize[0];
  if(this->cWinSizeX<this->cDimSize[1])
    {
    this->cWinSizeX = this->cDimSize[1];
    }
  if(this->cWinSizeX<this->cDimSize[2])
    {
    this->cWinSizeX = this->cDimSize[2];
    }
  this->cWinMaxX  = this->cWinSizeX - 1;
  
  this->cWinMinY  = 0;
  this->cWinSizeY = this->cWinSizeX;
  this->cWinMaxY  = this->cWinSizeY - 1;
  
  this->cWinDataSizeX = this->cWinMaxX;//this->cDimSize[0];
  this->cWinDataSizeY = this->cWinMaxY;//this->cDimSize[1];
  
  if(this->cWinImData != NULL)
    {
    delete [] this->cWinImData;
    }
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY ];

  if(this->cWinZBuffer != NULL) 
    {
    delete [] this->cWinZBuffer;
    }
  this->cWinZBuffer = new unsigned short[ this->cWinDataSizeX * this->cWinDataSizeY ];
  
  this->cViewImData  = true;
  this->cValidImData = true;
  
  }





//
//
template <class ImagePixelType, class OverlayPixelType>
const typename GLSliceView<ImagePixelType,OverlayPixelType>::ImagePointer &
GLSliceView<ImagePixelType, OverlayPixelType>
::GetInputImage(void) const
  {
  return this->cImData;
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

  if( this->cValidImData 
      &&  (newoverlay_size[0] == this->cDimSize[0])
      &&  (newoverlay_size[1] == this->cDimSize[1])
      &&  (newoverlay_size[2] == this->cDimSize[2])
    )
    {
    this->cOverlayData = newOverlayData;
    
    this->cViewOverlayData  = true;
    this->cValidOverlayData = true;
    this->cOverlayOpacity   = (float)1.0;
    
    if(this->cWinOverlayData != NULL) 
      {
      delete [] this->cWinOverlayData;
      }
    

    const unsigned long bufferSize = this->cWinDataSizeX * this->cWinDataSizeY * 4;
    this->cWinOverlayData = new unsigned char[ bufferSize ];
    }
  else // return a warning
    {
      if(!this->cValidImData)
        {
        std::cout << "GLSliceView: Please set the input image before overlay"  
                  << std::endl;
        std::cout << "GLSliceView: Overlay not set." << std::endl;
        }
      else if((newoverlay_size[0] != this->cDimSize[0])
      ||  (newoverlay_size[1] != this->cDimSize[1])
      ||  (newoverlay_size[2] != this->cDimSize[2])
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
  return this->cOverlayData;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewOverlayData( bool newViewOverlayData)
  {
  
  this->cViewOverlayData = newViewOverlayData;
  
  if( this->cViewOverlayCallBack != NULL )
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
  
  return this->cViewOverlayData;
  
  }


template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewOverlayCallBack( void (* newViewOverlayCallBack)(void) )
  {
  this->cViewOverlayCallBack = newViewOverlayCallBack;
  }


template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewClickedPoints( bool newViewClickedPoints )
{
    this->cViewClickedPoints = newViewClickedPoints;

    this->redraw();
}

template <class ImagePixelType, class OverlayPixelType>
bool
GLSliceView<ImagePixelType, OverlayPixelType>::
ViewClickedPoints()
{
    return this->cViewClickedPoints;
}


//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
OverlayOpacity( float newOverlayOpacity )
  {
  this->cOverlayOpacity = newOverlayOpacity;
  
  if(this->cViewOverlayCallBack != NULL) 
    {
    this->cViewOverlayCallBack();
    }
  }




template <class ImagePixelType, class OverlayPixelType>
float 
GLSliceView<ImagePixelType, OverlayPixelType>::
OverlayOpacity(void)
  {
  return this->cOverlayOpacity;
  }




//
//
//
template <class ImagePixelType, class OverlayPixelType>
typename GLSliceView<ImagePixelType, OverlayPixelType>::ColorTablePointer 
GLSliceView<ImagePixelType, OverlayPixelType>::
GetColorTable(void)
  {
  return this->cColorTable;
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
  
  if( !this->cValidImData ) 
    {
    return;
    }

  double zoomBase = this->cW / (this->cDimSize[this->cWinOrder[0]]
                                * (fabs(this->cSpacing[this->cWinOrder[0]])
                                   /fabs(this->cSpacing[0])));
  if(zoomBase > this->cH / (this->cDimSize[this->cWinOrder[1]]
                            * (fabs(this->cSpacing[this->cWinOrder[1]])
                              / fabs(this->cSpacing[0]))))
    {
    zoomBase = this->cH / (this->cDimSize[this->cWinOrder[1]]
                           * (fabs(this->cSpacing[this->cWinOrder[1]])
                              / fabs(this->cSpacing[0])));
    }
  double scale0 = this->cWinZoom * zoomBase 
                                 * fabs(this->cSpacing[this->cWinOrder[0]])
                                 / fabs(this->cSpacing[0]);
  double scale1 = this->cWinZoom * zoomBase 
                                 * fabs(this->cSpacing[this->cWinOrder[1]]) 
                                 / fabs(this->cSpacing[0]);

  if(this->cWinZoom>1)
    {
    this->cWinSizeX = (int)( this->cW / scale0 );
    this->cWinMinX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] 
                            - this->cWinSizeX/2 );
    this->cWinMaxX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] 
                            + this->cWinSizeX/2 );
    }
  else
    {
    this->cWinSizeX = (int)(this->cDimSize[ this->cWinOrder[0] ]);
    this->cWinMinX = 0;
    this->cWinMaxX = (int)( (int)(this->cDimSize[ this->cWinOrder[0] ]) - 1 );
    this->cWinCenter[this->cWinOrder[0]] = 
                     (int)( this->cDimSize[ this->cWinOrder[0] ] / 2);
    }
  if( this->cWinMinX <= - (int) this->cDimSize[ this->cWinOrder[0] ] ) 
    {
    this->cWinMinX = -(int)this->cDimSize[ this->cWinOrder[0] ] + 1;
    }
  else if(this->cWinMinX >= (int)this->cDimSize[ this->cWinOrder[0] ]) 
    {
    this->cWinMinX = (int)this->cDimSize[ this->cWinOrder[0] ] - 1;
    }
  if( this->cWinMaxX >= (int)( this->cDimSize[ this->cWinOrder[0] ] ) )
    {
    this->cWinMaxX = (int)this->cDimSize[ this->cWinOrder[0] ] - 1;
    }
  
  if(this->cWinZoom>1)
    {
    this->cWinSizeY = (int)( this->cH / scale1 );
    this->cWinMinY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) 
                             - this->cWinSizeY/2 );
    this->cWinMaxY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) 
                             + this->cWinSizeY/2 );
    }
  else
    {
    this->cWinSizeY = (int)(this->cDimSize[ this->cWinOrder[1] ]);
    this->cWinMinY = 0;
    this->cWinMaxY = (int)( (int)(this->cDimSize[ this->cWinOrder[1] ]) - 1 );
    this->cWinCenter[this->cWinOrder[1]] = 
                     (int)( this->cDimSize[ this->cWinOrder[1] ] / 2);
    }
  if( this->cWinMinY <= - (int)( this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMinY = -(int)this->cDimSize[ this->cWinOrder[1] ] + 1;
    }
  else if( this->cWinMinY >= (int)(this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMinY = this->cDimSize[ this->cWinOrder[1] ] - 1;
    } 
  if( this->cWinMaxY >= (int)( this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMaxY = this->cDimSize[ this->cWinOrder[1] ] - 1;
    }
  
  memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY );
  if( this->cValidOverlayData ) 
    {
    memset(this->cWinOverlayData, 0, this->cWinDataSizeX*this->cWinDataSizeY*4);
    }
  
  IndexType ind;
  
  int l, m;
  
  float tf;
  
  ind[ this->cWinOrder[ 2 ] ] = this->cWinCenter[ this->cWinOrder[ 2 ] ];
  int startK = this->cWinMinY;
  if(startK<0)
    startK = 0;
  int startJ = this->cWinMinX;
  if(startJ<0)
    startJ = 0;
  for(int k=startK; k <= this->cWinMaxY; k++)
    {
    ind[this->cWinOrder[1]] = k;
    
    if(k-this->cWinMinY >= (int)this->cWinDataSizeY)
      continue;

    for(int j=startJ; j <= this->cWinMaxX; j++) 
      {
      ind[this->cWinOrder[0]] = j;
      
      if(j-this->cWinMinX >= (int)this->cWinDataSizeX)
         continue;

      switch( this->cImageMode ) 
        {
        default:
        case IMG_VAL:
          tf = (float)((this->cImData->GetPixel(ind)-this->cIWMin) 
                       / (this->cIWMax-this->cIWMin)*255);
          break;
        case IMG_INV:
          tf = (float)((this->cIWMax-this->cImData->GetPixel(ind)) 
                       / (this->cIWMax-this->cIWMin)*255);
          break;
        case IMG_LOG:
          tf = (float)(log(this->cImData->GetPixel(ind)-this->cIWMin+0.00000001)
                       /log(this->cIWMax-this->cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
            {
            tf = (float)((this->cImData->GetPixel(ind)-this->cIWMin)
                         / (this->cIWMax-this->cIWMin)*255);
            ind[0]--;
            tf -= (float)((this->cImData->GetPixel(ind)-this->cIWMin) 
                         / (this->cIWMax-this->cIWMin)*255);
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
            tf = (float)((this->cImData->GetPixel(ind)-this->cIWMin) 
                         / (this->cIWMax-this->cIWMin)*255);
            ind[1]--;
            tf -= (float)((this->cImData->GetPixel(ind)-this->cIWMin) 
                          / (this->cIWMax-this->cIWMin)*255);
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
            tf = (float)((this->cImData->GetPixel(ind)-this->cIWMin)
                         / (this->cIWMax-this->cIWMin)*255);
            ind[2]--;
            tf -= (float)((this->cImData->GetPixel(ind)-this->cIWMin)
                          / (this->cIWMax-this->cIWMin)*255);
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
          const int tempval = (int)this->cWinCenter[this->cWinOrder[2]]-1;
          int tmpI = ind[this->cWinOrder[2]];
          ind[this->cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          tf = (float)(this->cImData->GetPixel(ind));
          
          ind[this->cWinOrder[2]] = this->cWinCenter[this->cWinOrder[2]];
          tf += (float)(this->cImData->GetPixel(ind))*2;
          
          const int tempval1 = (int)this->cDimSize[this->cWinOrder[2]]-1;
          const int tempval2 = (int)this->cWinCenter[this->cWinOrder[2]]+1;
          ind[this->cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          tf += (float)(this->cImData->GetPixel(ind));
          
          tf = (float)((tf/4-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf = this->cIWMin;
          m = (j-this->cWinMinX) + (k-this->cWinMinY)*this->cWinDataSizeX;
          this->cWinZBuffer[m] = 0;
          int tmpI = ind[this->cWinOrder[2]];
          for(l=0; l<(int)this->cDimSize[this->cWinOrder[2]]; l++) 
            {
            ind[this->cWinOrder[2]] = l;        
            if(this->cImData->GetPixel(ind) > tf) 
              {
              tf = (float)(this->cImData->GetPixel(ind));
              this->cWinZBuffer[m] = (unsigned short)l;
              }
            }
          tf = (float)((tf-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
        }
      
      if( tf > 255 )
        {
        switch(this->cIWModeMax) 
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
          switch(this->cIWModeMin) 
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
      
      l = (j-this->cWinMinX) + (k-this->cWinMinY)*this->cWinDataSizeX;
      this->cWinImData[l] = (unsigned char)tf;
      unsigned int overlayColorIndex = 0;
      
      if( this->cValidOverlayData ) 
        {
        l = l * 4;
        if(this->cImageMode == IMG_MIP)
          {
          ind[this->cWinOrder[2]] = this->cWinZBuffer[(j-this->cWinMinX) + 
            (k-this->cWinMinY)*this->cWinDataSizeX];
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
            overlayColorIndex = m-1;
            if( static_cast<unsigned int>(m) > cOverlayColorIndex )
              {
              overlayColorIndex = cOverlayColorIndex;
              }
            cWinOverlayData[l+0] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'r') * 255);
            cWinOverlayData[l+1] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'g') * 255);
            cWinOverlayData[l+2] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'b') * 255);
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
clickSelect(float lx, float ly, float lz)
  {
  SliceView<ImagePixelType>::clickSelect(lx, ly, lz);
  if(this->cViewValue || this->cViewCrosshairs)
    {
    this->redraw();
    }
  }

template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::size(int lw, int lh)
  {
  SliceView<ImagePixelType>::size(lw, lh);
  Fl_Gl_Window::size(lw, lh);
  this->update();
  this->redraw();
  }




template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
resize(int lx, int ly, int lw, int lh)
  {
  SliceView<ImagePixelType>::resize(lx, ly, lw, lh);
  Fl_Gl_Window::resize(lx, ly, lw, lh);
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
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
                                            //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }
  else
    {
    glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
    glShadeModel(GL_FLAT);
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    
    glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    ortho();
    
    if( !this->cImData ) 
      {
      return;
      }
    
    double zoomBase = this->cW / (this->cDimSize[this->cWinOrder[0]]
                                  * (fabs(this->cSpacing[this->cWinOrder[0]])
                                     / fabs(this->cSpacing[0])));
    if(zoomBase > this->cH / (this->cDimSize[this->cWinOrder[1]]
                              * (fabs(this->cSpacing[this->cWinOrder[1]])
                                 / fabs(this->cSpacing[0]))))
      {
      zoomBase = this->cH / (this->cDimSize[this->cWinOrder[1]]
                             * (fabs(this->cSpacing[this->cWinOrder[1]])
                                / fabs(this->cSpacing[0])));
      }

    double scale0 = this->cWinZoom * zoomBase 
                                   * fabs(this->cSpacing[this->cWinOrder[0]])
                                   / fabs(this->cSpacing[0]);
    double scale1 = this->cWinZoom * zoomBase 
                                   * fabs(this->cSpacing[this->cWinOrder[1]])
                                   / fabs(this->cSpacing[0]);
    
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
    glRasterPos2i((this->cFlipX[this->cWinOrientation])?this->cW-originX:originX,
      (this->cFlipY[this->cWinOrientation])?this->cH-originY:originY);  
    glPixelZoom((this->cFlipX[this->cWinOrientation])?-scale0:scale0,
      (this->cFlipY[this->cWinOrientation])?-scale1:scale1);
    
    if( this->cValidImData && this->cViewImData )
      {
      glDrawPixels( this->cWinDataSizeX, this->cWinDataSizeY, 
                    GL_LUMINANCE, GL_UNSIGNED_BYTE, 
                    this->cWinImData );
      }
    
    if( this->cValidOverlayData && this->cViewOverlayData ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(this->cWinDataSizeX, this->cWinDataSizeY, GL_RGBA, 
        GL_UNSIGNED_BYTE, this->cWinOverlayData);
      glDisable(GL_BLEND);
      }

    if( this->cViewClickedPoints )
      {
      glColor3f( 0.8, 0.4, 0.4 );
      glPointSize( 3.0 );
      glBegin(GL_POINTS);
      for( int ii = 0; ii < this->numClickedPointsStored(); ii++ )
        {
        ClickPoint p;
        this->getClickedPoint( ii, p );
        float pts[3] = { p.x, p.y, p.z };

        if( static_cast<int>( pts[this->cWinOrder[2]] ) ==
            (int)this->sliceNum() )
          {
          float xx;
          if(this->cFlipX[this->cWinOrientation])
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
          if(this->cFlipY[this->cWinOrientation])
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

    if( this->cViewAxisLabel ) 
      {
      glLoadIdentity();
      glOrtho( 0.0, (double)w(), 0.0, (double)h(), 0.0, 1.0 );
      glMatrixMode( GL_PROJECTION );
      glViewport( 0,0 , w(), h() );

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.2, 0.2, 0.78, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      
      if( !this->cFlipX[this->cWinOrientation] )
        {
        const int yy = static_cast<int>(  this->cH/2-gl_height()/2  );
        gl_draw( this->cAxisLabelX[this->cWinOrientation],
          this->cW-(gl_width(this->cAxisLabelX[this->cWinOrientation])+10), 
          static_cast<float>( yy ) );
        }
      else
        {
        const int yy = static_cast<int>( this->cH/2-gl_height()/2  );
        gl_draw( this->cAxisLabelX[this->cWinOrientation],
          (gl_width(this->cAxisLabelX[this->cWinOrientation])+10),
          static_cast<float>( yy ));
        }
      
      if(!this->cFlipY[this->cWinOrientation])
        {
        const int yy = static_cast<int>( this->cH-gl_height()-10 ) ;
        gl_draw( this->cAxisLabelY[this->cWinOrientation],
          this->cW/2-(gl_width(this->cAxisLabelY[this->cWinOrientation])/2),
          static_cast<float>(yy) );
        }
      else
        {
        const int yy = static_cast<int>( gl_height()+10 );
        gl_draw( this->cAxisLabelY[this->cWinOrientation], 
          this->cW/2-(gl_width(this->cAxisLabelY[this->cWinOrientation])/2),
          static_cast<float>(yy));
        }
      
      glDisable(GL_BLEND);
      }
    if( this->cViewValue ) 
      {
      glLoadIdentity();
      glOrtho( 0.0, (double)w(), 0.0, (double)h(), 0.0, 1.0 );
      glMatrixMode( GL_PROJECTION );
      glViewport( 0,0 , w(), h() );

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      char s[80];
      float px, py, pz, val = this->cClickSelectV;
      const char * suffix = "";
      if( this->cViewValuePhysicalUnits )
        {
        IndexType index;
        typedef typename IndexType::IndexValueType indexValueType;
        index[0] = static_cast< indexValueType >( this->cClickSelect[0] );
        index[1] = static_cast< indexValueType >( this->cClickSelect[1] );
        index[2] = static_cast< indexValueType >( this->cClickSelect[2] );
        PointType point;
        this->cImData->TransformIndexToPhysicalPoint( index, point );
        px = point[0];
        py = point[1];
        pz = point[2];
        suffix = this->cPhysicalUnitsName;
        }
       else
        {
        px = this->cClickSelect[0];
        py = this->cClickSelect[1];
        pz = this->cClickSelect[2];
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
        (int)(this->cW-(gl_width(s)+2)), 2);
      glDisable(GL_BLEND);
      
      }
    if( this->cViewDetails )
      {
      glLoadIdentity();
      glOrtho( 0.0, (double)w(), 0.0, (double)h(), 0.0, 1.0 );
      glMatrixMode( GL_PROJECTION );
      glViewport( 0,0 , w(), h() );

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.9, 0.4, 0.1, (float)0.75);

      gl_font(FL_TIMES_BOLD, 12);
      char s[80];
      if(this->cWinOrientation == 0)
        sprintf(s, "X - Slice: %3d", this->cWinCenter[0]);
      else if(this->cWinOrientation == 1)
        sprintf(s, "Y - Slice: %3d", this->cWinCenter[1]);
      else
        sprintf(s, "Z - Slice: %3d", this->cWinCenter[2]);
      gl_draw( s, 2, 2+5*(gl_height()+2) );
      sprintf(s, "Dims: %3d x %3d x %3d", 
        (int)this->cDimSize[0], (int)this->cDimSize[1], (int)this->cDimSize[2]);
      gl_draw( s, 2, 2+4*(gl_height()+2) );
      sprintf(s, "Voxel: %0.3f x %0.3f x %0.3f", 
        this->cSpacing[0], this->cSpacing[1], this->cSpacing[2]);
      gl_draw( s, 2, 2+3*(gl_height()+2) );
      sprintf(s, "Int. Range: %0.3f - %0.3f", (float)this->cDataMin, 
              (float)this->cDataMax);
      gl_draw( s, 2, 2+2*(gl_height()+2) );
      sprintf(s, "Int. Window: %0.3f(%s) - %0.3f(%s)", 
        (float)this->cIWMin, IWModeTypeName[this->cIWModeMin], 
        (float)this->cIWMax, IWModeTypeName[this->cIWModeMax]);
      gl_draw( s, 2, 2+1*(gl_height()+2) );
      sprintf(s, "View Mode: %s", ImageModeTypeName[this->cImageMode]);
      gl_draw( s, 2, 2+0*(gl_height()+2) );
      glDisable(GL_BLEND);
      }
    if( this->cViewCrosshairs 
      && static_cast<int>(this->cClickSelect[this->cWinOrder[2]]) == 
         static_cast<int>( this->sliceNum() ) )
      {
      // Set the model view matrix first
      glLoadIdentity();
      glOrtho( 0.0, (double)w(), 0.0, (double)h(), 0.0, 1.0 );
      glMatrixMode( GL_PROJECTION );
      glViewport( 0,0 , w(), h() );

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (float)0.75);

      int xx;
      if(this->cFlipX[this->cWinOrientation])
        {
        xx = (int)(this->cW - (this->cClickSelect[this->cWinOrder[0]] 
                           - this->cWinMinX) * scale0 - originX);
        }
      else
        {
        xx = (int)((this->cClickSelect[this->cWinOrder[0]] 
                   - this->cWinMinX) * scale0 + originX);
        }
      int yy;
      if(this->cFlipY[this->cWinOrientation])
        {
        yy = (int)(this->cH - (this->cClickSelect[this->cWinOrder[1]] 
                           - this->cWinMinY) * scale1 - originY);
        }
      else
        {
        yy = (int)((this->cClickSelect[this->cWinOrder[1]] 
                   - this->cWinMinY) * scale1 + originY);
        }
      glBegin(GL_LINES);
      glVertex2d(0, yy);
      glVertex2d(xx-2, yy);
      glVertex2d(xx+2, yy);
      glVertex2d(this->cW-1, yy);
      glVertex2d(xx, 0);
      glVertex2d(xx, yy-2);
      glVertex2d(xx, yy+2);
      glVertex2d(xx, this->cH-1);
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
  int xx = Fl::event_x();
  int yy = Fl::event_y();
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
        if(this->cClickMode == CM_BOX) 
          {
          if(event == FL_PUSH)
            {
            boxX = xx;
            boxY = yy;
            }
          else
            {
            if(event == FL_DRAG)
              {
              make_current();
              fl_overlay_clear();
              fl_overlay_rect(boxX, boxY, xx-boxY, yy-boxY);
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

