/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWindowInteractive.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/



#ifndef fltkWINDOWINTERCLASS
#define fltkWINDOWINTERCLASS

#include <FL/Fl_Window.H>
#include <fltkDrawer.h>
#include <itkPoint.h>
#include <itkVector.h>
#include <vector>


namespace fltk {


class WindowInteractive: public Fl_Window 
{

public:

  typedef itk::Point<float,2>     OffsetType;
  typedef itk::Vector<float,2>    OffsetVariationType;

  typedef itk::Point<int,2>       ScreenPointType;

public:
  WindowInteractive(int x,int y,int w,int h, const char * label=0);
  virtual ~WindowInteractive();
  virtual void draw(void);
  virtual int  handle(int);

  const float & GetZoom(void) const;
  const OffsetType & GetOffset(void) const;
  const OffsetVariationType & GetDiffOffset(void) const;

  void  ClearOffset(void);
  void  PlugDrawer( Drawer * dr );

private:

  ScreenPointType   m_Mouse_a;
  ScreenPointType   m_Mouse_b;

  float m_Zoom;

  OffsetType            m_Offset;
  OffsetVariationType   m_DiffOffset;

  std::vector< Drawer * > m_Drawers;
  
};


} // end namespace fltk

#endif   
