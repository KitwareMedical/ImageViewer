/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindow.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#ifndef fltkGLWINDOW
#define fltkGLWINDOW

#include <FL/gl.h> 
#include <FL/Fl_Gl_Window.H>
#include <fltkGlDrawer.h>
#include <fltkWidgetRedrawCommand.h>
#include <vector>

namespace fltk {


/**
 *  This class implements an OpenGl Window.
 *  it derives from Fl_Gl_Window and add
 *  the elements needed to communicate with
 *  ITK via Command objects
 *
 */
class GlWindow : public Fl_Gl_Window 
{

  std::vector< GlDrawerPtr > drawer;

public:

  typedef WidgetRedrawCommand< Fl_Gl_Window >  RedrawCommandType;
    
protected:
  void CallDrawers(void) const;

public:

  GlWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindow();
  void SaveImage( void );
  void SaveImage( const char * filename );
  void AddDrawer( GlDrawerPtr dr );
  const RedrawCommandType::Pointer & GetRedrawCommand(void);

private:

  RedrawCommandType::Pointer   m_RedrawCommand;
    

};

} // end namespace fltk


#endif



