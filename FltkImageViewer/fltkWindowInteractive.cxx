/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWindowInteractive.cxx
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


#include <fltkWindowInteractive.h>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <cmath>


namespace fltk {


//------------------------------------------
//
//    Creator
//
//------------------------------------------
WindowInteractive
::WindowInteractive(int x,int y,int w,int h, const char * label)
  :Fl_Window(x,y,w,h,label) 
{

  m_Zoom     = 1.0;

  m_Offset.Fill( 0.0 );
  m_OffsetVariation.Fill( 0.0 );

  m_Mouse_a.Fill( 0.0 );
  m_Mouse_b.Fill( 0.0 );

}






//------------------------------------------
//
//    Destructor
//
//------------------------------------------
WindowInteractive
::~WindowInteractive() 
{
  
}



//------------------------------------------
//
//    Plug Drawer Object
//
//------------------------------------------
void 
WindowInteractive
::AddDrawer( Drawer *dr) 
{
  if( !dr ) 
  {
    return;
  }
  m_Drawers.push_back(dr);
}

  


//------------------------------------------
//
//    Return actual Zoom factor
//
//------------------------------------------
const float &
WindowInteractive
::GetZoom(void) const 
{
  return m_Zoom;
}


//------------------------------------------
//
//    Return actual offset
//
//------------------------------------------
const WindowInteractive::OffsetType &  
WindowInteractive
::GetOffset(void) const 
{
  return m_Offset;
}




//------------------------------------------
//
//    Return variation in offset
//
//------------------------------------------
const WindowInteractive::OffsetVariationType &  
WindowInteractive
::GetOffsetVariation(void) const 
{
  return m_OffsetVariation;
}




//------------------------------------------
//
//    Clear cumulated offset values
//
//------------------------------------------
void 
WindowInteractive
::ClearOffset(void) 
{
  m_Offset.Fill( 0.0 );  
  m_OffsetVariation.Fill( 0.0 );
}



//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void 
WindowInteractive
::draw(void) 
{

  if( !visible_r() ) return;

  std::vector<Drawer *>::iterator it = m_Drawers.begin();

  while( it != m_Drawers.end() )
  {
    Drawer * drawer = *it;
    if( drawer ) 
    {
      drawer->Draw();
    }
    ++it;
  }

}

  



//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
int 
WindowInteractive
::handle(int event) 
{

  m_OffsetVariation.Fill( 0.0 );

  switch(event) 
  {
    case FL_PUSH:
      m_Mouse_a[0] = Fl::event_x();
      m_Mouse_a[1] = Fl::event_y();
      return 1;
    case FL_RELEASE:
      m_Mouse_a = m_Mouse_b;
      return 1;
    case FL_DRAG:
    {
      m_Mouse_b[0] = Fl::event_x();
      m_Mouse_b[1] = Fl::event_y();
      if( Fl::event_key(FL_Control_L) ) 
      {
        m_Zoom *= (float)exp(2.0*(m_Mouse_b[1] - m_Mouse_a[1])/h());
      }
      else
      {
        m_OffsetVariation[0] = 90.0f*(m_Mouse_b[0] - m_Mouse_a[0])/w();
        m_OffsetVariation[1] = 90.0f*(m_Mouse_b[1] - m_Mouse_a[1])/h();
        m_Offset += m_OffsetVariation;
      }
      m_Mouse_a = m_Mouse_b;
      redraw();
      return 1;
    }
   }

  return 0;

}




} // end namespace fltk


