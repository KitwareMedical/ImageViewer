/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindow.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#include <fltkGlWindow.h>
#include <FL/fl_ask.H>
#include <FL/fl_file_chooser.H>
#include <fstream>
#include <iostream>


namespace fltk {


//--------------------------------------------------
//
//    Creator
//
//--------------------------------------------------
GlWindow::GlWindow(int x,int y,int w,int h, const char * label)
  :Fl_Gl_Window(x,y,w,h,label) 
{
  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetWidget( this );
}


//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
GlWindow::~GlWindow() 
{

}



//--------------------------------------------------
//
//    Grab Image and Store in a PPM file 
//    This is the user interface part.
//
//--------------------------------------------------
void GlWindow::SaveImage(void) 
{

  const char * filename = 
        fl_file_chooser("Please select a file name","*.ppm","");

  if( !filename ) return;

  SaveImage( filename );

}



//------------------------------------------
//
//    Return Redraw Command
//
//------------------------------------------
const GlWindow::RedrawCommandType::Pointer & 
GlWindow
::GetRedrawCommand(  void )
{
  
  return m_RedrawCommand;

}

 

//------------------------------------------
//
//    Add Drawer Object
//
//------------------------------------------
void GlWindow::AddDrawer(  GlDrawerPtr dr) 
{
  
  if( !dr ) return;
  drawer.push_back(dr);

}

  

//------------------------------------------
//
//    Call Model Drawers
//
//------------------------------------------
void GlWindow::CallDrawers(void) const 
{

  if( !drawer.empty() ) 
  {
    std::vector<GlDrawerPtr>::const_iterator dr = drawer.begin();
    while( dr != drawer.end() ) 
    {
      (*dr)->glDraw();
      dr++;
    }
  }

}






//--------------------------------------------------
//
//    Grab Image and Store in a PPM file
//
//--------------------------------------------------
void GlWindow::SaveImage(const char * filename) 
{

  if( !filename ) 
  {
    return;
  }

  if( strlen(filename) == 0 ) 
  {
    return;
  }

  const int wnx = w();
  const int wny = h();

  std::ofstream  of;
  of.open(filename);
  if( of.fail() ) 
  {
    fl_alert("Error opening file %s",filename);
    return;
  }

  unsigned char *image = new unsigned char[ wnx * wny * 8 ];

  if( !image ) 
  {
    fl_alert("Problem","Allocating memory for image grabbing buffer");
    of.close();
    return;
  }


  of << "P6" << std::endl;      // Magic number for PPM files
  of << wnx << " " << wny << std::endl;

  of << 255 << std::endl;

  make_current();
  draw();

  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,wnx,wny,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid *)image);

  bool firstPixel = true;

  for(int y=wny-1; y>=0; y--)  
  {
    unsigned char *p = image + 4*y*wnx;
    for(int x=0; x<wnx; x++) 
    {
      const unsigned char red   = *p++;
      const unsigned char green = *p++;
      const unsigned char blue  = *p++;
                                   p++; // alpha channel
      if( firstPixel )       
      {
        of << green << blue;
        firstPixel = false;
      }
      else 
      {
        of << red << green << blue;
      }
    }
  }


  of.close();

  delete [] image;

}



} // end namespace fltk


