/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSlice3DDrawer.txx
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

#include <fltkSlice3DDrawer.h>
#include <GL/glu.h>


namespace fltk {


/**
 * Default constructor
 */
template <class TImage>
Slice3DDrawer<TImage>
::Slice3DDrawer()
{

  m_TextureX = 0;
  m_TextureY = 0;
  m_TextureZ = 0;

  m_SliceX = 0;
  m_SliceY = 0;
  m_SliceZ = 0;

  texturesGenerated = false;
}


/**
 * Destructor
 */
template <class TImage>
Slice3DDrawer<TImage>
::~Slice3DDrawer()
{


  if( m_TextureX ) 
  {
    delete m_TextureX;
    m_TextureX = 0;
  }

  if( m_TextureY ) 
  {
    delete m_TextureY;
    m_TextureY = 0;
  }

  if( m_TextureZ ) 
  {
    delete m_TextureZ;
    m_TextureZ = 0;
  }

  if( m_SliceX ) 
  {
    delete m_SliceX;
    m_SliceX = 0;
  }

  if( m_SliceY ) 
  {
    delete m_SliceY;
    m_SliceY = 0;
  }

  if( m_SliceZ ) 
  {
    delete m_SliceZ;
    m_SliceZ = 0;
  }




}



/**
 * SetInput
 */
template <class TImage>
void
Slice3DDrawer<TImage>
::SetInput( ImageType * image )
{

  m_Image = image;

  RefreshGUI();

}





/**
 * Refresh GUI
 */
template <class TImage>
void
Slice3DDrawer<TImage>
::RefreshGUI( void )
{

  if( ! m_Image ) 
  {
    VolumeWindow->deactivate();
    return;
  }

  SizeType size =
    m_Image->GetRequestedRegion().GetSize();

  xScrollBar->value( size[0]/2, 1, 0, size[0] );
  yScrollBar->value( size[1]/2, 1, 0, size[1] );
  zScrollBar->value( size[2]/2, 1, 0, size[2] );

  xScrollBar->linesize( 1 );
  yScrollBar->linesize( 1 );
  zScrollBar->linesize( 1 );

  typedef  itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;

  IteratorType it( m_Image, m_Image->GetRequestedRegion() );

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


  RegionType region = m_Image->GetRequestedRegion();

  m_Nx = region.GetSize()[0];
  m_Ny = region.GetSize()[1];
  m_Nz = region.GetSize()[2];

  m_Nxr = (int)pow(2, floor( log(m_Nx)/log(2.0)+1 ) );
  m_Nyr = (int)pow(2, floor( log(m_Ny)/log(2.0)+1 ) );
  m_Nzr = (int)pow(2, floor( log(m_Nz)/log(2.0)+1 ) );

  m_Dx = m_Image->GetSpacing()[0];
  m_Dy = m_Image->GetSpacing()[1];
  m_Dz = m_Image->GetSpacing()[2];

  RegionType totalRegion = m_Image->GetLargestPossibleRegion();

  m_Cx = totalRegion.GetSize()[0] * m_Dx / 2.0;
  m_Cy = totalRegion.GetSize()[1] * m_Dy / 2.0;
  m_Cz = totalRegion.GetSize()[2] * m_Dz / 2.0;

  m_Ox = m_Image->GetOrigin()[0];
  m_Oy = m_Image->GetOrigin()[1];
  m_Oz = m_Image->GetOrigin()[2];


  if( m_SliceX ) 
  {
    delete m_SliceX;
  }

  if( m_SliceY ) 
  {
    delete m_SliceY;
  }

  if( m_SliceZ ) 
  {
    delete m_SliceZ;
  }

  m_SliceX = new unsigned char [ 4 * m_Ny * m_Nz ];
  m_SliceY = new unsigned char [ 4 * m_Nz * m_Nx ];
  m_SliceZ = new unsigned char [ 4 * m_Nx * m_Ny ];

  if( m_TextureX ) 
  {
    delete m_TextureX;
  }

  if( m_TextureY ) 
  {
    delete m_TextureY;
  }

  if( m_TextureZ ) 
  {
    delete m_TextureZ;
  }

  m_TextureX = new unsigned char [ 4 * m_Nyr * m_Nzr ];
  m_TextureY = new unsigned char [ 4 * m_Nzr * m_Nxr ];
  m_TextureZ = new unsigned char [ 4 * m_Nxr * m_Nyr ];

  VolumeWindow->activate();
}





/**
 * Show
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::Show(void)
{

  VolumeWindow->show();

}






/**
 * Hide
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::Hide(void)
{

  VolumeWindow->hide();

}








/**
* Select slice X to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceX(void)
{
    xValueOutput->value( xScrollBar->value() );
    BindTextureX();
    InvokeEvent( li::VolumeReslicedEvent );
}






/**
* Select slice Y to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceY(void)
{
    yValueOutput->value( yScrollBar->value() );
    BindTextureY();
    InvokeEvent( li::VolumeReslicedEvent );
}





/**
* Select slice Z to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceZ(void)
{
    zValueOutput->value( zScrollBar->value() );
    BindTextureZ();
    InvokeEvent( li::VolumeReslicedEvent );
}






/**
 * Draw the target as textures in OpenGL
 */
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::glDraw(void) const
{


  if( !texturesGenerated )
  {
    glGenTextures(3, m_TextureName);
    texturesGenerated = true;
  }
  
  if( !(m_Image) )
  {
    return;
  }

  glDisable( GL_LIGHTING );

  glPushMatrix();

  glRotatef( 90.0, 0, 0, 1 );
  glRotatef( 90.0, 1, 0, 0 );

  glTranslatef( -m_Ox, -m_Oy, -m_Oy );

  const GLfloat qx = -m_Cx; 
  const GLfloat qy = -m_Cy; 
  const GLfloat qz = -m_Cz; 
    

  glColor3f(0.7,0.5,0.5);

  glBegin(GL_LINE_LOOP);
  glVertex3f(-qx, -qy, -qz);
  glVertex3f(-qx,  qy, -qz);
  glVertex3f( qx,  qy, -qz);
  glVertex3f( qx, -qy, -qz);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(-qx, -qy,  qz);
  glVertex3f(-qx,  qy,  qz);
  glVertex3f( qx,  qy,  qz);
  glVertex3f( qx, -qy,  qz);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(-qx, -qy, -qz);
  glVertex3f(-qx, -qy,  qz);
  glVertex3f(-qx,  qy, -qz);
  glVertex3f(-qx,  qy,  qz);
  glVertex3f( qx,  qy, -qz);
  glVertex3f( qx,  qy,  qz);
  glVertex3f( qx, -qy, -qz);
  glVertex3f( qx, -qy,  qz);
  glEnd();

  glTranslatef( qx, qy, qz );

  glGetError();        // Clearing Error buffer
  glColor3f(1.0,1.0,1.0);

  if( xCheckButton->value() ) 
  {
    int px = xScrollBar->value();
    GLfloat pnx =   px    * m_Dx;
    GLfloat pny =   m_Ny  * m_Dy;
    GLfloat pnz =   m_Nz  * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[0]);
    glBegin(GL_QUADS);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 0.0, 0.0 ); glVertex3f( pnx, 0.0, 0.0);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 0.0, 1.0 ); glVertex3f( pnx, 0.0, pnz);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, pny, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }



  if( yCheckButton->value() ) 
  {
    int py = yScrollBar->value();
    GLfloat pnx =   m_Nx   * m_Dx;
    GLfloat pny =   py     * m_Dy;
    GLfloat pnz =   m_Nz   * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[1]);
    glBegin(GL_QUADS);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0.0, pny, 0.0);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0.0, pny, pnz);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, pny, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }



  if( zCheckButton->value() ) 
  {
    int pz = zScrollBar->value();
    GLfloat pnx =   m_Nx   * m_Dx;
    GLfloat pny =   m_Ny   * m_Dy;
    GLfloat pnz =   pz     * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[2]);
    glBegin(GL_QUADS);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0.0, 0.0, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0.0, pny, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, 0.0, pnz);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

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
Slice3DDrawer<ImagePixelType>
::BindTextureX(void) 
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( xCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int px = xScrollBar->value();
    int pix = 0;
    index[0] = px;
    for(int z=0; z<m_Nz; z++) {
      index[2] = z;
      for(int y=0; y<m_Ny; y++) {
        index[1] = y;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[0]);

    gluScaleImage( GL_RGBA, m_Ny, m_Nz,
                   GL_UNSIGNED_BYTE, m_SliceX,
                   m_Nyr, m_Nzr,
                   GL_UNSIGNED_BYTE, m_TextureX);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nyr,m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureX);

    }

}



//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::BindTextureY(void) 
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( yCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int py = yScrollBar->value();
    int piy = 0;
    index[1] = py;
    for(int z=0; z<m_Nz; z++) {
      index[2] = z;
      for(int x=0; x<m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[1]);

    gluScaleImage( GL_RGBA, m_Nx, m_Nz,
                   GL_UNSIGNED_BYTE, m_SliceY,
                   m_Nxr, m_Nzr,
                   GL_UNSIGNED_BYTE, m_TextureY);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nxr,m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureY);

    }



}


//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::BindTextureZ(void) 
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( zCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int pz = zScrollBar->value();
    int piz = 0;
    index[2] = pz;
    for(int y=0; y<m_Ny; y++) {
      index[1] = y;
      for(int x=0; x<m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[2]);

    gluScaleImage( GL_RGBA, m_Nx, m_Ny,
                   GL_UNSIGNED_BYTE, m_SliceZ,
                   m_Nxr, m_Nyr,
                   GL_UNSIGNED_BYTE, m_TextureZ);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nxr,m_Nyr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureZ);

    }


}



} // end namespace fltk



