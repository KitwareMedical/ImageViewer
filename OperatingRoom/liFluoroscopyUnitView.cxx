/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFluoroscopyUnitView.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project:  Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#include "liFluoroscopyUnitView.h"
#include "itkPoint.h"
#include "itkImageSliceConstIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "liCommandEvents.h"
#include <GL/glu.h>
#include <math.h>


namespace li {


//--------------------------------------------
//
//    Creator
//
//--------------------------------------------
FluoroscopyUnitView
::FluoroscopyUnitView(int x,int y,int w,int h, const char * label)
:fltk::GlWindow(x,y,w,h,label) 
{
  m_Zoom = 1.0;
  m_Notifier = itk::Object::New();
  m_ImageBackground   = 0;
  m_TextureBackground = 0;
  m_TextureScaled     = 0;
  m_TextureState      = TextureNotLoaded;
}



//--------------------------------------------
//
//    Destructor
//
//--------------------------------------------
FluoroscopyUnitView
::~FluoroscopyUnitView()
{

  if( m_TextureBackground )
  {
    delete [] m_TextureBackground;
  }

  if( m_ImageBackground )
  {
    delete [] m_ImageBackground;
  }

}



//--------------------------------------------
//
//    Set position of the X-ray source
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetSourcePosition( const PointType & position )
{
  m_SourcePosition = position;
}



//--------------------------------------------
//
//    Get position of the X-ray source
//
//--------------------------------------------
const FluoroscopyUnitView::PointType &
FluoroscopyUnitView
::GetSourcePosition( void ) const
{
  return m_SourcePosition;
}



//--------------------------------------------
//
//    Set radius of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorRadius( double radius )
{
  m_DetectorRadius = radius;
}




//--------------------------------------------
//
//    Set position of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorPosition( const PointType & position )
{
  m_DetectorPosition = position;
}



//--------------------------------------------
//
//    Get position of the Image Detector
//
//--------------------------------------------
const FluoroscopyUnitView::PointType &
FluoroscopyUnitView
::GetDetectorPosition( void ) const
{
  return m_DetectorPosition;
}



//--------------------------------------------
//
//    Set vertical direction of the Image Detector
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetDetectorVerticalDirection( const VectorType & direction )
{
  m_DetectorVerticalDirection = direction;
}



//--------------------------------------------
//
//    Get vertical direction of the image detector
//
//--------------------------------------------
const FluoroscopyUnitView::VectorType &
FluoroscopyUnitView
::GetDetectorVerticalDirection( void ) const
{
  return m_DetectorVerticalDirection;
}





//--------------------------------------------
//
//    Set the zoom factor
//
//--------------------------------------------
void
FluoroscopyUnitView
::SetZoom( double zoom )
{
  m_Zoom = zoom;
}



//--------------------------------------------
//
//    Get position of the Image Detector
//
//--------------------------------------------
double
FluoroscopyUnitView
::GetZoom( void ) const
{
  return m_Zoom;
}



//--------------------------------------------
//
//    Get the projection matrix
//
//--------------------------------------------
const GLfloat *
FluoroscopyUnitView
::GetProjectionMatrix( void ) const
{
  return m_ProjectionMatrix;
}



//--------------------------------------------
//
//    Get the model matrix
//
//--------------------------------------------
const GLfloat *
FluoroscopyUnitView
::GetModelMatrix( void ) const
{
  return m_ModelMatrix;
}





//--------------------------------------------
//
//    Get the Notifier
//
//--------------------------------------------
itk::Object::Pointer
FluoroscopyUnitView::GetNotifier(void) 
{
  return m_Notifier;
}



//--------------------------------------------
//
//    Set fluoroscopy image
//
//--------------------------------------------
void
FluoroscopyUnitView::SetFluoroscopyImage( const ImageType * image )
{
  if( !image ) 
  {
    return;
  }

  m_Image = image;

  if( m_ImageBackground )
  {
    delete [] m_ImageBackground;
  }
  
  if( m_TextureBackground )
  {
    delete [] m_TextureBackground;
  }
  
  ImageType::SizeType size = m_Image->GetBufferedRegion().GetSize();
  
  m_ImageBackgroundWidth  = size[0];
  m_ImageBackgroundHeight = size[1];

  const unsigned long totalSize = 
              m_ImageBackgroundHeight * m_ImageBackgroundWidth ;

  if( !totalSize ) 
  {
    return;
  }

  itk::ImageRegionConstIteratorWithIndex< ImageType > 
                                        at( m_Image,
                                            m_Image->GetBufferedRegion() );

  ImageType::PixelType max = at.Get();
  ImageType::PixelType min = at.Get();

  while( !at.IsAtEnd() )
  {
    ImageType::PixelType value = at.Get();
    if( value > max )
    {
      max = value;
    }
    else if( value < min )
    {
      min = value;
    }
    ++at;
  }
  

  m_ImageBackground   = new unsigned char [ totalSize     ];
  m_TextureBackground = new unsigned char [ totalSize * 4 ];  // RGB + A
  
  itk::ImageSliceConstIteratorWithIndex< ImageType > 
                                        it( m_Image,
                                            m_Image->GetBufferedRegion() );

  it.SetFirstDirection( 0 );
  it.SetSecondDirection( 1 );

  unsigned char * dest = m_ImageBackground + totalSize - m_ImageBackgroundWidth;

  unsigned char * texture = m_TextureBackground;

  const float factor = 255.0 / (max - min );

  while( !it.IsAtEnd() )  // Should only have one slice...but anyway.
  {
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() ) 
      {
        const unsigned char value = 
                (unsigned char)( ( it.Get() - min ) * factor );
        *dest = value;
        *texture++ = value;
        *texture++ = value;
        *texture++ = value;
        *texture++ = 255;
        ++it;
        ++dest;
      }
      it.NextLine();
      dest -= 2 * m_ImageBackgroundWidth;
    }
    it.NextSlice();
  }

  m_TextureState = TextureLoaded;

}






//--------------------------------------------
//
//    Draw the model
//
//--------------------------------------------
void FluoroscopyUnitView::draw(void) 
{
  
  glViewport(0,0,w(),h());

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_NORMALIZE );
  glEnable( GL_DEPTH_TEST );
  
  glShadeModel( GL_SMOOTH );
  
  glClearColor((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0,(GLfloat)1.0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if( m_TextureState == TextureLoaded ) 
  {

    const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
    const GLint border = 0;  // Texture image border

    GLint textureNx = (GLint)pow(2, floor( 
          log(static_cast<float>(m_ImageBackgroundWidth) ) / 
          log(2.0f)+1 ) );

    GLint textureNy = (GLint)pow(2, floor( 
          log(static_cast<float>(m_ImageBackgroundHeight)) / 
          log(2.0f)+1 ) );
    
    if( m_TextureScaled )
    {
      delete [] m_TextureScaled;
    }
    m_TextureScaled = new unsigned char [ 4 * textureNx * textureNy ];

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, m_TextureName );

    gluScaleImage( GL_RGBA, 
                   m_ImageBackgroundWidth, 
                   m_ImageBackgroundHeight,
                   GL_UNSIGNED_BYTE, 
                   m_TextureBackground,
                   textureNx, 
                   textureNy,
                   GL_UNSIGNED_BYTE, m_TextureScaled);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 textureNx, textureNy,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureScaled);

    m_TextureState = TextureBinded;

  }



  const GLdouble rad2degree = 45.0f / atan( 1.0f );

  const GLdouble distance = 
            m_DetectorPosition.EuclideanDistanceTo( m_SourcePosition );

  const GLdouble fieldOfView = static_cast<GLdouble>( 
                rad2degree * 2.0 * atan2( m_DetectorRadius, distance ) );

  const GLdouble aspect     = 
    static_cast<GLdouble>( w() ) / static_cast<GLdouble>( h() );

  const GLdouble nearPlane  = 10.0f;
  const GLdouble farPlane   = distance;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( fieldOfView, aspect, nearPlane, farPlane );

  VectorType lineOfSight = m_SourcePosition - m_DetectorPosition;

  GLfloat lightPosition[4];
  lightPosition[0] = 0.0;
  lightPosition[1] = 0.0;
  lightPosition[2] = 1.0;   // directional light along Z
  lightPosition[3] = 0.0;

  GLfloat specular[]      = {   0.0, 0.0, 0.0, 0.0 };
  GLfloat diffuse[]       = {   0.9, 0.9, 0.9, 0.0 };

  glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
  glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

  glEnable( GL_LIGHT0 );

  gluLookAt( m_SourcePosition[0],
             m_SourcePosition[1],
             m_SourcePosition[2],
             m_DetectorPosition[0],
             m_DetectorPosition[1],
             m_DetectorPosition[2],
             m_DetectorVerticalDirection[0],
             m_DetectorVerticalDirection[1],
             m_DetectorVerticalDirection[2]  );


  glGetFloatv( GL_PROJECTION_MATRIX, m_ProjectionMatrix );

  lightPosition[0] = lineOfSight[0];
  lightPosition[1] = lineOfSight[1];
  lightPosition[2] = lineOfSight[2];
  glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glGetFloatv( GL_MODELVIEW_MATRIX, m_ModelMatrix );



  // Draw the object as viewed by the Fluoroscopy Unit
  m_Notifier->InvokeEvent( GLDrawEvent()  );

  

  if( m_TextureState == TextureBinded )
  {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fieldOfView, aspect, nearPlane, farPlane );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
 
    lightPosition[0] = 0.0;
    lightPosition[1] = 0.0;
    lightPosition[2] = 1.0;   // directional light along Z
    lightPosition[3] = 0.0;

    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

    glColor3f( (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0 );
    
    GLfloat color[] = { (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0 };
    glMaterialfv(GL_FRONT,GL_DIFFUSE,color);

    GLfloat focalDistance = lineOfSight.GetNorm();
    GLfloat pnx =   m_DetectorRadius;
    GLfloat pny =   m_DetectorRadius;
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, m_TextureName );
    glBegin( GL_QUADS );
      glNormal3f( 0.0,0.0,1.0); glTexCoord2f( 0.0, 0.0 ); glVertex3f( -pnx,  pny, -focalDistance );
      glNormal3f( 0.0,0.0,1.0); glTexCoord2f( 0.0, 1.0 ); glVertex3f( -pnx, -pny, -focalDistance );
      glNormal3f( 0.0,0.0,1.0); glTexCoord2f( 1.0, 1.0 ); glVertex3f(  pnx, -pny, -focalDistance );
      glNormal3f( 0.0,0.0,1.0); glTexCoord2f( 1.0, 0.0 ); glVertex3f(  pnx,  pny, -focalDistance );
    glEnd();
    glDisable( GL_TEXTURE_2D );

  }


}


}  // end namespace li


