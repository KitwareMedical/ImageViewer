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

#include <itkSlice3DDrawer.h>
#include <GL/glu.h>


namespace fltk {


/**
 * Default constructor
 */
template <class ImagePixelType>
Slice3DDrawer<ImagePixelType>
::Slice3DDrawer()
{

  this->m_TextureX = 0;
  this->m_TextureY = 0;
  this->m_TextureZ = 0;

  this->m_SliceX = 0;
  this->m_SliceY = 0;
  this->m_SliceZ = 0;

  texturesGenerated = false;
}


/**
 * Destructor
 */
template <class ImagePixelType>
Slice3DDrawer<ImagePixelType>
::~Slice3DDrawer()
{


  if( this->m_TextureX ) 
  {
    delete this->m_TextureX;
    this->m_TextureX = 0;
  }

  if( this->m_TextureY ) 
  {
    delete this->m_TextureY;
    this->m_TextureY = 0;
  }

  if( this->m_TextureZ ) 
  {
    delete this->m_TextureZ;
    this->m_TextureZ = 0;
  }

  if( this->m_SliceX ) 
  {
    delete this->m_SliceX;
    this->m_SliceX = 0;
  }

  if( this->m_SliceY ) 
  {
    delete this->m_SliceY;
    this->m_SliceY = 0;
  }

  if( this->m_SliceZ ) 
  {
    delete this->m_SliceZ;
    this->m_SliceZ = 0;
  }




}



/**
 * SetInput
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::SetInput( ImageType * volume )
{

  this->m_Volume = volume;

  RefreshGUI();

}





/**
 * Refresh GUI
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::RefreshGUI( void )
{

  if( ! this->m_Volume ) 
  {
    VolumeWindow->deactivate();
    return;
  }

  itk::Size<3> size =
    this->m_Volume->GetRequestedRegion().GetSize();

  xScrollBar->value( size[0]/2, 1, 0, size[0] );
  yScrollBar->value( size[1]/2, 1, 0, size[1] );
  zScrollBar->value( size[2]/2, 1, 0, size[2] );

  xScrollBar->linesize( 1 );
  yScrollBar->linesize( 1 );
  zScrollBar->linesize( 1 );

  typedef  itk::SimpleImageRegionIterator< ImageType > IteratorType;

  IteratorType it( this->m_Volume,
   this->m_Volume->GetRequestedRegion() );

  it.Begin();
  ImagePixelType  max = it.Get();
  while( ! it.IsAtEnd() )
  {
	 const ImagePixelType val = it.Get();
     if( max < val )
     {
       max = val;
     }
     ++it;
  }
  this->m_Max_Value = max;


  itk::ImageRegion<3> region = this->m_Volume->GetRequestedRegion();

  this->m_Nx = region.GetSize()[0];
  this->m_Ny = region.GetSize()[1];
  this->m_Nz = region.GetSize()[2];

  this->m_Nxr = (int)pow(2, floor( log(this->m_Nx)/log(2.0)+1 ) );
  this->m_Nyr = (int)pow(2, floor( log(this->m_Ny)/log(2.0)+1 ) );
  this->m_Nzr = (int)pow(2, floor( log(this->m_Nz)/log(2.0)+1 ) );

  this->m_Dx = this->m_Volume->GetSpacing()[0];
  this->m_Dy = this->m_Volume->GetSpacing()[1];
  this->m_Dz = this->m_Volume->GetSpacing()[2];

  itk::ImageRegion<3> totalRegion = this->m_Volume->GetLargestPossibleRegion();

  this->m_Cx = totalRegion.GetSize()[0] * this->m_Dx / 2.0;
  this->m_Cy = totalRegion.GetSize()[1] * this->m_Dx / 2.0;
  this->m_Cz = totalRegion.GetSize()[2] * this->m_Dz / 2.0;


  if( this->m_SliceX ) 
  {
    delete this->m_SliceX;
  }

  if( this->m_SliceY ) 
  {
    delete this->m_SliceY;
  }

  if( this->m_SliceZ ) 
  {
    delete this->m_SliceZ;
  }

  this->m_SliceX = new unsigned char [ 4 * this->m_Ny * this->m_Nz ];
  this->m_SliceY = new unsigned char [ 4 * this->m_Nz * this->m_Nx ];
  this->m_SliceZ = new unsigned char [ 4 * this->m_Nx * this->m_Ny ];

  if( this->m_TextureX ) 
  {
    delete this->m_TextureX;
  }

  if( this->m_TextureY ) 
  {
    delete this->m_TextureY;
  }

  if( this->m_TextureZ ) 
  {
    delete this->m_TextureZ;
  }

  this->m_TextureX = new unsigned char [ 4 * this->m_Nyr * this->m_Nzr ];
  this->m_TextureY = new unsigned char [ 4 * this->m_Nzr * this->m_Nxr ];
  this->m_TextureZ = new unsigned char [ 4 * this->m_Nxr * this->m_Nyr ];

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
    UpdateLinks();
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
    UpdateLinks();
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
    UpdateLinks();
}






/**
 * Draw the target as textures in OpenGL
 */
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::glDraw(void)
{


  if( !texturesGenerated )
  {
    glGenTextures(3, m_TextureName);
    texturesGenerated = true;
  }
  
  if( !(this->m_Volume) )
  {
    return;
  }

  glDisable( GL_LIGHTING );

  glPushMatrix();


  const GLfloat qx = -this->m_Cx; 
  const GLfloat qy = -this->m_Cy; 
  const GLfloat qz = -this->m_Cz; 
    

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
    GLfloat pnx =   px               * this->m_Dx;
    GLfloat pny =   this->m_Ny       * this->m_Dy;
    GLfloat pnz =   this->m_Nz       * this->m_Dz;
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
    GLfloat pnx =   this->m_Nx       * this->m_Dx;
    GLfloat pny =   py               * this->m_Dy;
    GLfloat pnz =   this->m_Nz       * this->m_Dz;
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
    GLfloat pnx =   this->m_Nx       * this->m_Dx;
    GLfloat pny =   this->m_Ny       * this->m_Dy;
    GLfloat pnz =   pz               * this->m_Dz;
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

    float factor = 255.0 / this->m_Max_Value;

    itk::Index<3> index;
    const int px = xScrollBar->value();
    int pix = 0;
    index[0] = px;
    for(int z=0; z<this->m_Nz; z++) {
      index[2] = z;
      for(int y=0; y<this->m_Ny; y++) {
        index[1] = y;
        GLubyte val = (GLubyte)(
          this->m_Volume->GetPixel(index) * factor );
        this->m_SliceX[pix++] = val;
        this->m_SliceX[pix++] = val;
        this->m_SliceX[pix++] = val;
        this->m_SliceX[pix++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, this->m_TextureName[0]);

    gluScaleImage( GL_RGBA, this->m_Ny, this->m_Nz,
                   GL_UNSIGNED_BYTE, this->m_SliceX,
                   this->m_Nyr, this->m_Nzr,
                   GL_UNSIGNED_BYTE, this->m_TextureX);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 this->m_Nyr,this->m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,this->m_TextureX);

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

    float factor = 255.0 / this->m_Max_Value;

    itk::Index<3> index;
    const int py = yScrollBar->value();
    int piy = 0;
    index[1] = py;
    for(int z=0; z<this->m_Nz; z++) {
      index[2] = z;
      for(int x=0; x<this->m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          this->m_Volume->GetPixel(index) * factor );
        this->m_SliceY[piy++] = val;
        this->m_SliceY[piy++] = val;
        this->m_SliceY[piy++] = val;
        this->m_SliceY[piy++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, this->m_TextureName[1]);

    gluScaleImage( GL_RGBA, this->m_Nx, this->m_Nz,
                   GL_UNSIGNED_BYTE, this->m_SliceY,
                   this->m_Nxr, this->m_Nzr,
                   GL_UNSIGNED_BYTE, this->m_TextureY);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 this->m_Nxr,this->m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,this->m_TextureY);

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

    float factor = 255.0 / this->m_Max_Value;

    itk::Index<3> index;
    const int pz = zScrollBar->value();
    int piz = 0;
    index[2] = pz;
    for(int y=0; y<this->m_Ny; y++) {
      index[1] = y;
      for(int x=0; x<this->m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          this->m_Volume->GetPixel(index) * factor );
        this->m_SliceZ[piz++] = val;
        this->m_SliceZ[piz++] = val;
        this->m_SliceZ[piz++] = val;
        this->m_SliceZ[piz++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, this->m_TextureName[2]);

    gluScaleImage( GL_RGBA, this->m_Nx, this->m_Ny,
                   GL_UNSIGNED_BYTE, this->m_SliceZ,
                   this->m_Nxr, this->m_Nyr,
                   GL_UNSIGNED_BYTE, this->m_TextureZ);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 this->m_Nxr,this->m_Nyr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,this->m_TextureZ);

    }


}



} // end namespace fltk



