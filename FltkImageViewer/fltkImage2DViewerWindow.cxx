/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImage2DViewerWindow.cxx
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



#include <fltkImage2DViewerWindow.h>
#include <GL/glu.h>
#include <cmath>
#include <FL/Fl.H>


namespace fltk {

//------------------------------------------
//
//    Creator
//
//------------------------------------------
Image2DViewerWindow::
Image2DViewerWindow(int x,int y,int w,int h, const char * label)
:GlWindow(x,y,w,h,label) 
{

  m_Background.SetRed( 0.5 );
  m_Background.SetGreen( 0.5 );
  m_Background.SetBlue( 0.5 );
  
  m_Buffer = 0;

}




//------------------------------------------
//
//    Destructor
//
//------------------------------------------
Image2DViewerWindow
::~Image2DViewerWindow() 
{
  if( m_Buffer )
    {
    delete [] m_Buffer;
    m_Buffer = 0;
    }
}




//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
Image2DViewerWindow
::SetBackground( GLfloat r, GLfloat g, GLfloat b ) 
{
  m_Background.SetRed( r );
  m_Background.SetGreen( g );
  m_Background.SetBlue( b );
}





//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
Image2DViewerWindow
::SetBackground( const ColorType & newcolor ) 
{
  m_Background = newcolor;
}




//------------------------------------------
//
//    Get Background Color
//
//------------------------------------------
const Image2DViewerWindow::ColorType & 
Image2DViewerWindow
::GetBackground(void) const 
{
  return m_Background;
}



//------------------------------------------
//
//    Allocate
//
//------------------------------------------
void
Image2DViewerWindow::
Allocate(unsigned int nx,unsigned int ny)
{

  if( m_Buffer )
    {
    delete [] m_Buffer;
    }

  size(nx,ny);
  m_Buffer = new unsigned char[ nx * ny * 4 ]; // *4 for RGBA

}




//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void Image2DViewerWindow::draw(void) 
{

  if( !visible_r() ) 
  {
    return;
  }
  if(!valid())
  {
    
    glViewport( 0, 0, w(), h() );

    glClearColor(
      m_Background.GetRed(),
      m_Background.GetGreen(),
      m_Background.GetBlue(),
      1.0);
  
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
  }
  const GLdouble width = w();
  const GLdouble height = h();
  glOrtho( -width, width, -height, height, -20000, 10000 );
  
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  
  glDrawPixels( width, height, 
                GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer);
}

  



//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
int 
Image2DViewerWindow
::handle(int event) 
{

  switch( event ) 
  {
    case FL_PUSH:
      return 1;
    case FL_RELEASE:
      return 1;
    case FL_DRAG:
      return 1;
  }

  return 0;
}




} // end namespace fltk



