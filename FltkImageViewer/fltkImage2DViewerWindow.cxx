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
#include <vcl_cmath.h>
#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>


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

  m_ShiftX =   0;
  m_ShiftY =   0;
  m_Zoom   = 1.0;

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

  this->size(nx,ny);

  m_Buffer = new unsigned char[ nx * ny * 4 ]; // *4 for RGBA
  
  this->SetWidth( nx );
  this->SetHeight( ny );

}



//------------------------------------------
//
//    Update the display 
//
//------------------------------------------
void
Image2DViewerWindow
::Update(void) 
{
  this->redraw();
  Fl::check();
}




//------------------------------------------
//
//    Set Height
//
//------------------------------------------
void Image2DViewerWindow::SetHeight(unsigned int height) 
{
  m_Height = height;
  m_ShiftY = -m_Height;
}




//------------------------------------------
//
//    Set Width
//
//------------------------------------------
void Image2DViewerWindow::SetWidth(unsigned int width) 
{
  m_Width = width;
  m_ShiftX = -m_Width;
}



//------------------------------------------
//
//    Get Width
//
//------------------------------------------
unsigned int
Image2DViewerWindow::GetWidth( void ) const
{
  return m_Width;
}




//------------------------------------------
//
//    Get Height
//
//------------------------------------------
unsigned int
Image2DViewerWindow::GetHeight( void ) const
{
  return m_Height;
}



//------------------------------------------
//
//    Fit Image to Window
//
//------------------------------------------
void 
Image2DViewerWindow::FitImageToWindow(void) 
{
  m_Zoom = static_cast<GLdouble>( h() ) /
           static_cast<GLdouble>( m_Height ); 
  m_ShiftY = -m_Height;
  m_ShiftX =  m_Width;
  this->redraw();
  Fl::check();

}



//------------------------------------------
//
//    Fit Window to Image
//
//------------------------------------------
void 
Image2DViewerWindow::FitWindowToImage(void) 
{
  m_Zoom = 1.0;
  m_ShiftX = -m_Width;
  m_ShiftY = -m_Height;
  m_ParentWindow->size( m_Width, m_Height );
}



//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void Image2DViewerWindow::draw(void) 
{

  if( !m_Buffer)
  {
    return;
  }
  
  
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
  
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
  }

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  const GLdouble width  = static_cast<GLdouble>( m_Width );
  const GLdouble height = static_cast<GLdouble>( m_Height );

  glOrtho( -width, width, -height, height, -20000, 10000 );
  
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  

  glRasterPos2i( m_ShiftX, m_ShiftY );

  glPixelZoom( m_Zoom, m_Zoom );

  glDrawPixels( m_Width, m_Height, 
                GL_RGBA, GL_UNSIGNED_BYTE, 
                static_cast<void *>(m_Buffer) );
}

  

//------------------------------------------
//
//    Set Intensity Window
//
//------------------------------------------
void 
Image2DViewerWindow
::SetIntensityWindow( Fl_Window * window )
{
  m_IntensityWindow = window;
}



//------------------------------------------
//
//    Set Parent Window
//
//------------------------------------------
void 
Image2DViewerWindow
::SetParentWindow( Fl_Window * window )
{
  m_ParentWindow = window;
}




//------------------------------------------
//
//    Intensity Windowing
//
//------------------------------------------
void 
Image2DViewerWindow
::IntensityWindowing(void) 
{
  m_IntensityWindow->show();
}


//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::handlePopUpMenu(void) 
{
  static Fl_Menu_Button * popupMenu = 0;
  if( !popupMenu )
    {
      popupMenu = new Fl_Menu_Button( 
          Fl::event_x(), Fl::event_y(),100,200 );
      popupMenu->type( Fl_Menu_Button::POPUP3);
      popupMenu->add("Fit Image To Window");
      popupMenu->add("Fit Window To Image");
      popupMenu->add("Intensity Windowing");
    }

  typedef enum 
    {
    FIT_IMAGE_TO_WINDOW,
    FIT_WINDOW_TO_IMAGE,
    INTENSITY_WINDOWING
    };

  popupMenu->popup();

  switch( popupMenu->value() )
  {
    case FIT_WINDOW_TO_IMAGE:
      this->FitWindowToImage();
      break;
    case FIT_IMAGE_TO_WINDOW:
      this->FitImageToWindow();
      break;
    case INTENSITY_WINDOWING:
      this->IntensityWindowing();
      break;
  }
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

  static int p1x  = 0;
  static int p1y  = 0;

  switch( event ) 
  {
    case FL_PUSH:
      {
      const int state = Fl::event_state();
      if( state == FL_BUTTON1 )
        {
        p1x = Fl::event_x();
        p1y = Fl::event_y();
        }
      else if( state == FL_BUTTON2 )
        {
          std::cout << "Button2 " << std::endl;
        } 
      else if( state == FL_BUTTON3 )
        {
          handlePopUpMenu();
        }
      return 1;
      }
    case FL_RELEASE:
      return 1;
    case FL_DRAG:
      {
      const int state = Fl::event_state();
      if( state == FL_BUTTON1 )
        {
          PanningEventHandling( p1x, p1y );
        }
      else if (state == FL_BUTTON2 )
        {
          ZoomingEventHandling( p1x, p1y );
        }
      return 1;
      }
  }

  return 0;
}



//------------------------------------------
//
//    Panning Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::PanningEventHandling(int & p1x, int & p1y)
{
  int p2x = Fl::event_x();
  int p2y = Fl::event_y();
  const int dx = p2x - p1x;
  const int dy = p2y - p1y;
  m_ShiftX += dx;
  m_ShiftY -= dy; // Mouse Y -> - OpenGl Y
  p1x = p2x;
  p1y = p2y;
  redraw();
  Fl::check();
}


//------------------------------------------
//
//    Zooming Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::ZoomingEventHandling(int & p1x, int & p1y)
{
  int p2x = Fl::event_x();
  int p2y = Fl::event_y();
  const int dx = p2x - p1x;
  const int dy = p2y - p1y;
  m_ShiftX += dx;
  m_ShiftY -= dy; // Mouse Y -> - OpenGl Y
  p1x = p2x;
  p1y = p2y;
  redraw();
  Fl::check();
}



} // end namespace fltk



