/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWindowInteractive.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

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
  m_DiffOffset.Fill( 0.0 );

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
::PlugDrawer( Drawer *dr) 
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
::GetDiffOffset(void) const 
{
  return m_DiffOffset;
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
  m_DiffOffset.Fill( 0.0 );
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

  m_DiffOffset.Fill( 0.0 );

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
        m_DiffOffset[0] = 90.0f*(m_Mouse_b[0] - m_Mouse_a[0])/w();
        m_DiffOffset[1] = 90.0f*(m_Mouse_b[1] - m_Mouse_a[1])/h();
        m_Offset += m_DiffOffset;
      }
      m_Mouse_a = m_Mouse_b;
      redraw();
      return 1;
    }
   }

  return 0;

}




} // end namespace fltk


