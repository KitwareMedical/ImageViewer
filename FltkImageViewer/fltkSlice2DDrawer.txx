/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSlice2DDrawer.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef _itkSlice2DDrawer_txx
#define _itkSlice2DDrawer_txx

#include "fltkSlice2DDrawer.h"
#include "itkImageRegionIteratorWithIndex.h"
#include <GL/glu.h>
#include "fltkCommandEvents.h"


namespace fltk {


/**
 * Default constructor
 */
template <class TImage>
Slice2DDrawer<TImage>
::Slice2DDrawer()
{

  m_Texture = 0;
  m_Slice   = 0;

  m_TexturesGenerated = false;

  m_DrawCommand = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

  m_Enabled = false;

}


/**
 * Destructor
 */
template <class TImage>
Slice2DDrawer<TImage>
::~Slice2DDrawer()
{


  if( m_Texture ) 
  {
    delete m_Texture;
    m_Texture = 0;
  }

  if( m_Slice ) 
  {
    delete m_Slice;
    m_Slice = 0;
  }

}



/**
 * Show
 */
template <class ImagePixelType>
void
Slice2DDrawer<ImagePixelType>
::Show(void)
{

  VolumeWindow->show();

}






/**
 * Hide
 */
template <class ImagePixelType>
void
Slice2DDrawer<ImagePixelType>
::Hide(void)
{

  VolumeWindow->hide();

}




/**
 * Destructor
 */
template <class TImage>
void
Slice2DDrawer<TImage>
::SetEnabled( bool enabled )
{
  m_Enabled = enabled;
  this->InvokeEvent( fltk::VolumeReslicedEvent() );
}





/**
 * Get Draw Command
 */
template <class TImage>
Slice2DDrawer<TImage>::DrawCommandPointer
Slice2DDrawer<TImage>
::GetDrawCommand(void) 
{
  return m_DrawCommand.GetPointer();
}



/**
 * SetInput
 */
template <class TImage>
void
Slice2DDrawer<TImage>
::SetInput( ImageType * image )
{

  m_Image = image;

  RegionType region = m_Image->GetRequestedRegion();
  if( region.GetNumberOfPixels() == 0 )
  {
    return;
  }

  SizeType size = region.GetSize();

  typedef  itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;

  IteratorType it( m_Image, region );

  PixelType  max = it.Get();
  while( ! it.IsAtEnd() )
    {
    const PixelType val = it.Get();
    if( max < val )
      {
      max = val;
      }
    ++it;
    }
  m_Max_Value = max;


  m_Nx = size[0];
  m_Ny = size[1];

  m_Nxr = (int)pow(2, floor( log(static_cast<float>(m_Nx))/log(2.0f)+1 ) );
  m_Nyr = (int)pow(2, floor( log(static_cast<float>(m_Ny))/log(2.0f)+1 ) );

  m_Dx = m_Image->GetSpacing()[0];
  m_Dy = m_Image->GetSpacing()[1];

  RegionType totalRegion = m_Image->GetLargestPossibleRegion();

  m_Cx = totalRegion.GetSize()[0] * m_Dx / 2.0;
  m_Cy = totalRegion.GetSize()[1] * m_Dy / 2.0;

  m_Ox = m_Image->GetOrigin()[0];
  m_Oy = m_Image->GetOrigin()[1];


  if( m_Slice ) 
  {
    delete m_Slice;
  }

  m_Slice = new unsigned char [ m_Nx * m_Ny ];

  if( m_Texture ) 
  {
    delete m_Texture;
  }

  m_Texture = new unsigned char [ m_Nxr * m_Nyr ];

  VolumeWindow->activate();


}






/**
 * Draw the target as textures in OpenGL
 */
template <class ImagePixelType>
void 
Slice2DDrawer<ImagePixelType>
::glDraw(void) const
{

  if( !m_Enabled )
    {
    return;
    }
  
  if( !(m_Image) )
  {
    return;
  }

  if( !m_TexturesGenerated )
  {
    this->BindTexture();
    m_TexturesGenerated = true;
  }


  RegionType region = m_Image->GetRequestedRegion();
  if( region.GetNumberOfPixels() == 0 )
  {
    return;
  }


  glDisable( GL_LIGHTING );
  glEnable( GL_DEPTH_TEST );

  glPushMatrix();

  glTranslatef( -m_Ox, -m_Oy, 0.0 );

  const GLfloat qx = -m_Cx; 
  const GLfloat qy = -m_Cy; 
  const GLfloat qz = -1.0;
    

  glColor3f(0.7,0.5,0.5);

  glBegin(GL_LINE_LOOP);
  glVertex3f(-qx, -qy,  qz);
  glVertex3f(-qx,  qy,  qz);
  glVertex3f( qx,  qy,  qz);
  glVertex3f( qx, -qy,  qz);
  glEnd();

  glTranslatef( qx, qy, qz );

  glGetError();        // Clearing Error buffer
  glColor3f(1.0,1.0,1.0);

  GLfloat pnx =   m_Nx   * m_Dx;
  GLfloat pny =   m_Ny   * m_Dy;
  GLfloat pnz =   qz;
  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, m_TextureName );
  glBegin(GL_QUADS);
    glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0.0, 0.0, pnz);
    glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0.0, pny, pnz);
    glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
    glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, 0.0, pnz);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glEnable( GL_LIGHTING );

}





//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice2DDrawer<ImagePixelType>
::BindTexture(void) const
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border


  float factor = 255.0 / m_Max_Value;

  IndexType index;
  int pix = 0;
  for(int y=0; y<m_Ny; y++) {
    index[1] = y;
    for(int x=0; x<m_Nx; x++) {
      index[0] = x;
      GLubyte val = (GLubyte)( m_Image->GetPixel(index) * factor );
      m_Slice[pix++] = val;
      }
    }


  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  glGenTextures(1, &m_TextureName);
  glBindTexture(GL_TEXTURE_2D, m_TextureName);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


  gluScaleImage( GL_LUMINANCE, m_Nx, m_Ny,
                 GL_UNSIGNED_BYTE, m_Slice,
                 m_Nxr, m_Nyr,
                 GL_UNSIGNED_BYTE, m_Texture);

  glTexImage2D(GL_TEXTURE_2D,level,GL_LUMINANCE,
               m_Nxr,m_Nyr,
               border,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_Texture);

}





} // end namespace fltk


#endif
