/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWidgetRedrawCommand.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <fltkWidgetRedrawCommand.h>



namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
template <class TWidget>
WidgetRedrawCommand<TWidget>
::WidgetRedrawCommand(void)  
{
  m_Widget  = 0;
}



//--------------------------------------------------
//
//    Set Widget
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::SetWidget( WidgetType * widget )  
{
  m_Widget = widget;
}



//--------------------------------------------------
//
//    Get Widget
//
//--------------------------------------------------
template <class TWidget>
WidgetRedrawCommand<TWidget>::WidgetType *
WidgetRedrawCommand<TWidget>
::GetWidget( void )
{
  return m_Widget;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::Execute( itk::LightObject * , unsigned long event )
{

  if( !m_Widget )
  {
    return;
  }

  m_Widget->redraw();
  Fl::check();

}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::Execute( const itk::LightObject * , unsigned long event )
{

  if( !m_Widget )
  {
    return;
  }

  m_Widget->redraw();
  Fl::check();

}





} // end namespace li


