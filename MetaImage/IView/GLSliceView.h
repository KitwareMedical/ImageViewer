#ifndef GLSLICEVIEW_H
#define GLSLICEVIEW_H

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

#include "itkColorTable.h"

#include "SliceView.h"

#include <cmath>

namespace itk {

// GLSliceView : Derived from abstract class SliceView and Fl_Gl_Window

template <class ImagePixelType, class OverlayPixelType>
class GLSliceView : public SliceView<ImagePixelType>, 
                    public Fl_Gl_Window 
{
public:
  typedef itk::PhysicalImage<ImagePixelType,3>     ImageType;
  typedef itk::PhysicalImage<OverlayPixelType,3>   OverlayType;
 
protected:
  bool        cValidOverlayData;
  bool        cViewOverlayData;
  float       cOverlayOpacity;
  
  OverlayType::Pointer cOverlayData;
  void     (* cViewOverlayCallBack)(void);

  unsigned char * cWinOverlayData;
  
  typedef itk::ColorTable<float> ColorTableType;

  ColorTableType::Pointer      cColorTable;

public:
  /*! FLTK required constructor - must use imData() to complete definition */
  GLSliceView(int x, int y, int w, int h, const char *l);

  /*! Specify the 3D image to view slice by slice */
  void SetInputImage(ImageType * newImData);
  const ImageType::Pointer & GetInputImage(void) const;

  /*! Specify the 3D image to view as an overlay */
  void SetInputOverlay(OverlayType * newOverlayData);

  /*! Return a pointer to the overlay data */
  const OverlayType::Pointer & GetInputOverlay(void) const;

  /*! Turn on/off the viewing of the overlay */
  void	ViewOverlayData(bool newViewOverlayData);
  
  /*! Status of the overlay - viewed /not viewed */
  bool  ViewOverlayData(void);
  
  /*! Specify the opacity of the overlay */
  void  OverlayOpacity(float newOverlayOpacity);
  
  /*! Get the opacity of the overlay */
  float OverlayOpacity(void);
  
  /*! Called when overlay is toggled or opacity is changed */
  void  ViewOverlayCallBack(void (* newOverlayCallBack)(void));

	itk::ColorTable * GetColorTable(void);

  void size(int w, int h);
  void resize(int x, int y, int w, int h);

  void update();
  void draw();

  int  handle(int event);
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
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::
SetInputImage(ImageType * newImData)
{
 
  if( cValidOverlayData )
  {
   ImageType::RegionType region = newImData->GetLargestPossibleRegion();
   ImageType::SizeType   size   = region.GetSize();
   unsigned long *newdimsize = new unsigned long[3] ; 
   newdimsize[0]=size[0];
   newdimsize[1]=size[1];
   newdimsize[2]=size[2];

   ImageType::RegionType overlay_region = cOverlayData->GetLargestPossibleRegion();
   ImageType::SizeType   overlay_size   = overlay_region.GetSize();
   unsigned long *overlaydimsize = new unsigned long [3] ; 
   overlaydimsize[0]=overlay_size[0];
   overlaydimsize[1]=overlay_size[1];
   overlaydimsize[2]=overlay_size[2];

    for( int i=0; i<3; i++ )
    {
      if( newdimsize[i] != overlaydimsize[i] )
      {
        return;
      }
    }
  } 
  cImData = newImData;

  //calculating cDataMax and cDataMin
  ImageType::RegionType cImData_region = cImData->GetLargestPossibleRegion();
  ImageType::SizeType   cImData_size   = cImData_region.GetSize();

   cDimSize = new unsigned long[3];
   cDimSize[0]=cImData_size[0];
   cDimSize[1]=cImData_size[1];
   cDimSize[2]=cImData_size[2];
 
 

  ImageType::IndexType ind;
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
  cWinMaxX  = cWinSizeX - 1;

  cWinMinY  = 0;
  cWinSizeY = cDimSize[1];
  cWinMaxY  = cWinSizeY - 1;

  cWinDataSizeX = 1;
  while( cWinDataSizeX <= cDimSize[0] )
  {
    cWinDataSizeX *= 2;
  }

  cWinDataSizeY = 1;
  while( cWinDataSizeY <= cDimSize[1] )
  {
    cWinDataSizeY *= 2;
  }

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





//
//
template <class ImagePixelType, class OverlayPixelType>
const PhysicalImage<ImagePixelType,3>::Pointer &
GLSliceView<ImagePixelType, OverlayPixelType>
::GetInputImage(void) const
{
    return cImData;
}



//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::SetInputOverlay( 
                              OverlayType * newOverlayData)
{ImageType::RegionType newoverlay_region = newOverlayData->GetLargestPossibleRegion();
 ImageType::SizeType   newoverlay_size   = newoverlay_region.GetSize();
 ImageType::RegionType cImData_region = cImData->GetLargestPossibleRegion();
 ImageType::SizeType   cImData_size   = cImData_region.GetSize();
    if( !cValidImData ||
         newoverlay_size[2]==cImData_size[2] 
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
        update();
    }
}





template <class ImagePixelType, class OverlayPixelType>
const Image<OverlayPixelType,3>::Pointer &
GLSliceView<ImagePixelType, OverlayPixelType>::GetInputOverlay( void ) const
{
  return cOverlayData;
}




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::ViewOverlayData(
                                                bool newViewOverlayData)
{

  cViewOverlayData = newViewOverlayData;

  if( cViewOverlayCallBack != NULL )
  {
    cViewOverlayCallBack();
  }

  Fl_Gl_Window::redraw();

}

template <class ImagePixelType, class OverlayPixelType>
bool 
GLSliceView<ImagePixelType, OverlayPixelType>::ViewOverlayData(void)
{

  return cViewOverlayData;

}


template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::ViewOverlayCallBack(
                                  void (* newViewOverlayCallBack)(void) )
{
  cViewOverlayCallBack = newViewOverlayCallBack;
}




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::OverlayOpacity(
                                              float newOverlayOpacity)
{
  cOverlayOpacity = newOverlayOpacity;
  
  if(cViewOverlayCallBack != NULL) 
  {
    cViewOverlayCallBack();
  }
}




template <class ImagePixelType, class OverlayPixelType>
float 
GLSliceView<ImagePixelType, OverlayPixelType>::OverlayOpacity(void)
{
  return cOverlayOpacity;
}




//
//
//
template <class ImagePixelType, class OverlayPixelType>
itk::ColorTable * 
GLSliceView<ImagePixelType, OverlayPixelType>::GetColorTable(void)
{
  return &cColorTable;
}




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLSliceView<ImagePixelType, OverlayPixelType>::update()
{

  if( !cValidImData ) 
  {
    return;
  }

  int ti = (int)( (int)cWinCenter[ cWinOrder[0] ] 
                - cDimSize[ cWinOrder[0] ]/( 2*cWinZoom ));
  if( ti < 0 ) 
  {
    ti = 0;
    cWinCenter[ cWinOrder[0] ] = (unsigned int)
          ( cDimSize[ cWinOrder[0] ]/( 2*cWinZoom ));
  }
  
  if( ti >= (int)cDimSize[ cWinOrder[0] ] ) 
  {
    ti = cDimSize[ cWinOrder[0] ]-1;
    cWinCenter[ cWinOrder[0] ] = (unsigned int)
      ( cDimSize[cWinOrder[0]]-1-cDimSize[ cWinOrder[0] ]/( 2*cWinZoom ));
  }
  cWinMinX = ti;
  
  ti = (int)((int)cWinCenter[ cWinOrder[1] ] - cDimSize[ cWinOrder[1] ]/( 2*cWinZoom ));
  if( ti < 0 ) {
    ti = 0;
    cWinCenter[ cWinOrder[1] ] = (unsigned int)
      ( cDimSize[ cWinOrder[1] ]/( 2*cWinZoom ));
  }

  if( ti >= (int)cDimSize[ cWinOrder[1] ] ) {
    ti = cDimSize[ cWinOrder[1] ]-1;
    cWinCenter[ cWinOrder[1] ] = (unsigned int)
      ( cDimSize[ cWinOrder[1] ] - 1 - cDimSize[ cWinOrder[1] ]/( 2*cWinZoom ));
  }

  cWinMinY = ti;
          
  cWinSizeX = ( ((int)( cDimSize[ cWinOrder[0] ] / cWinZoom )) / 2 ) * 2;
  cWinSizeY = ( ((int)( cDimSize[ cWinOrder[1] ] / cWinZoom )) / 2 ) * 2;

  cWinMaxX = cWinMinX + cWinSizeX - 1;
  if( cWinMaxX >= cDimSize[ cWinOrder[0] ] )
  {
    cWinMaxX = cDimSize[ cWinOrder[0] ]-1;
  }

  cWinMaxY = cWinMinY + cWinSizeY - 1;
  if( cWinMaxY >= cDimSize[ cWinOrder[1] ] ) 
  {
    cWinMaxY = cDimSize[ cWinOrder[1] ] - 1;
  }
  
  memset( cWinImData, 0, cWinDataSizeX*cWinDataSizeY );
  if( cValidOverlayData ) 
  {
    memset(cWinOverlayData, 0, cWinDataSizeX*cWinDataSizeY*4);
  }

  unsigned int pointX[3];
  ImageType::IndexType ind;

  int l, m;

  float tf;

  for(unsigned int k=cWinMinY; k <= cWinMaxY; k++)
  {
    for(unsigned int j=cWinMinX; j <= cWinMaxX; j++) 
    {
      pointX[ cWinOrder[ 0 ] ] = j;
      pointX[ cWinOrder[ 1 ] ] = k;
      pointX[ cWinOrder[ 2 ] ] = cWinCenter[ cWinOrder[ 2 ] ];
      
      ind[0] = pointX[ 0 ];
      ind[1] = pointX[ 1 ];
      ind[2] = pointX[ 2 ];

      switch( cImageMode ) 
      {
        default:
        case IMG_VAL:
          tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
          break;
        case IMG_LOG:
          tf = (float)(log(cImData->GetPixel(ind)-cIWMin+0.00000001)/log(cIWMax-cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(pointX[0]>0) {
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            tf += 128;
          } else
            tf = 128;
          break;
        case IMG_DY:
          if(pointX[1]>0) {
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            tf += 128;
          } else
            tf = 128;
          break;
        case IMG_DZ:
          if(pointX[2]>0) {
            tf = (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            ind[2]--;
            tf -= (float)((cImData->GetPixel(ind)-cIWMin)/(cIWMax-cIWMin)*255);
            tf += 128;
          } else
            tf = 128;
          break;
        case IMG_H:
          {
          const int tempval = (int)cWinCenter[cWinOrder[2]]-1;
          pointX[cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          ind[0] = pointX[0];
          ind[1] = pointX[1];
          ind[2] = pointX[2];
          tf = (float)(cImData->GetPixel(ind));
          
          pointX[cWinOrder[2]] = cWinCenter[cWinOrder[2]];
          ind[0] = pointX[0];
          ind[1] = pointX[1];
          ind[2] = pointX[2];
          tf += (float)(cImData->GetPixel(ind))*2;
          
          const int tempval1 = (int)cDimSize[cWinOrder[2]]-1;
          const int tempval2 = (int)cWinCenter[cWinOrder[2]]+1;
          pointX[cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          ind[0] = pointX[0];
          ind[1] = pointX[1];
          ind[2] = pointX[2];
          tf += (float)(cImData->GetPixel(ind));
          tf = (float)((tf/4-cIWMin)/(cIWMax-cIWMin)*255);
          break;
          }
        case IMG_MIP:
          tf = cIWMin;
                      m = (j-cWinMinX) + (k-cWinMinY)*cWinDataSizeX;
                      cWinZBuffer[m] = 0;
          for(l=0; l<(int)cDimSize[cWinOrder[2]]; l++) 
          {
            pointX[cWinOrder[2]] = l;				
            ind[0] = pointX[0];
            ind[1] = pointX[1];
            ind[2] = pointX[2];
            if(cImData->GetPixel(ind) > tf) 
            {
              tf = (float)(cImData->GetPixel(ind));
              cWinZBuffer[m] = (unsigned short)l;
            }
          }
          tf = (float)((tf-cIWMin)/(cIWMax-cIWMin)*255);
          break;
      }

      if( tf > 255 )
      {
        switch(cIWModeMax) {
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
          pointX[cWinOrder[2]] = cWinZBuffer[(j-cWinMinX) + (k-cWinMinY)*cWinDataSizeX];
          ind[0] = pointX[0];
          ind[1] = pointX[1];
          ind[2] = pointX[2];
        }

        if( sizeof( OverlayPixelType ) == 1 )
        {
          m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
          if( m > 0 ) {
            m = m - 1;
            cWinOverlayData[l+0] = (unsigned char)(cColorTable->color(m)->GetRed()*255);
            cWinOverlayData[l+1] = (unsigned char)(cColorTable->color(m)->GetGreen()*255);
            cWinOverlayData[l+2] = (unsigned char)(cColorTable->color(m)->GetBlue()*255);
            cWinOverlayData[l+3] = (unsigned char)(cOverlayOpacity*255);
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
              cWinOverlayData[l+0] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
              cWinOverlayData[l+1] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
              cWinOverlayData[l+2] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
              cWinOverlayData[l+3] = (unsigned char)(cOverlayOpacity*255);
            }
            else 
            {
              if( sizeof( OverlayPixelType ) == 4 ) 
              {
                cWinOverlayData[l+0] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
                cWinOverlayData[l+1] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
                cWinOverlayData[l+2] = ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
                cWinOverlayData[l+3] = (unsigned char)(((unsigned char *)
                                        &(cOverlayData->GetPixel(ind)))[3]*cOverlayOpacity);
              }
            }
          }
        }
      }
    }
  }
          
  Fl_Gl_Window::redraw();


}





template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::size(int w, int h)
{
  SliceView<ImagePixelType>::size(w, h);
  Fl_Gl_Window::size(w, h);
  update();
  redraw();
}




template <class ImagePixelType, class OverlayPixelType>
void GLSliceView<ImagePixelType, OverlayPixelType>::resize(int x, int y, int w, int h)
{
  SliceView<ImagePixelType>::resize(x, y, w, h);
  Fl_Gl_Window::resize(x, y, w, h);
  update();
  redraw();
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
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
  }
	else
  {
    glClear(GL_COLOR_BUFFER_BIT);		//this clears and paints to black

    glMatrixMode(GL_MODELVIEW);		//clear previous 3D draw params
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    ortho();

    if( !cImData ) 
    {
      return;
    }

    
    const double *spacing = cImData->GetSpacing();
    
    float scale0 = cW/(float)cDimSize[0]*cWinZoom*spacing[cWinOrder[0]]/spacing[0];
    float scale1 = cW/(float)cDimSize[0]*cWinZoom*spacing[cWinOrder[1]]/spacing[1];

   
    glRasterPos2i((cFlipX)?cW:0, (cFlipY)?cH:0);  
    glPixelZoom((cFlipX)?-scale0:scale0, (cFlipY)?-scale1:scale1);

    if( cValidImData && cViewImData )
    {
      glDrawPixels( cWinDataSizeX, cWinDataSizeY, 
                    GL_LUMINANCE, GL_UNSIGNED_BYTE, cWinImData);

    }
    
    if( cValidOverlayData && cViewOverlayData ) 
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(cWinDataSizeX, cWinDataSizeY, GL_RGBA, GL_UNSIGNED_BYTE, cWinOverlayData);
      glDisable(GL_BLEND);
    }

    if( cViewAxisLabel ) 
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1, 1, 1, (float)0.5);
      gl_font(FL_TIMES_BOLD, 20);

      if( !cFlipX )
      {
        gl_draw( cAxisLabelX[cWinOrientation],
                 cW-(gl_width(cAxisLabelX[cWinOrientation])+10), 
                 cH/2-gl_height()/2 );
      }
      else
      {
        gl_draw( cAxisLabelX[cWinOrientation],
                 (gl_width(cAxisLabelX[cWinOrientation])+10),
                 cH/2-gl_height()/2 );
      }

      if(!cFlipY)
      {
        gl_draw( cAxisLabelY[cWinOrientation],
                 cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
                 cH-gl_height()-10 );
      }
      else
      {
        gl_draw( cAxisLabelY[cWinOrientation], 
                 cW/2-(gl_width(cAxisLabelY[cWinOrientation])/2),
                 gl_height()+10 );
      }

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
  int key;
    
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
    case FL_KEYBOARD:
    case FL_SHORTCUT:
      key = Fl::event_text()[0];
      switch(key)
      {
      case 'o':
        if(cViewOverlayData) 
        {
          ViewOverlayData(false);
        }
        else 
        {
          ViewOverlayData(true);
        }
        return 1;
        break;
      }
    default:
        break;
    }
    
    return SliceView<ImagePixelType>::handle(event);
}

}; //namespace
#endif
