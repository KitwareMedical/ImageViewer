/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkLightButtonRedrawCommand.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#include <fltkLightButtonRedrawCommand.h>



namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
LightButtonRedrawCommand
::LightButtonRedrawCommand(void)  
{
}


//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
LightButtonRedrawCommand
::Execute( itk::LightObject * , unsigned long event )
{

  Fl_Light_Button * button = GetWidget();

  if( !button )
  {
    return;
  }


  switch( event )
  {
    case StartEvent:
    {
      button->selection_color( FL_YELLOW );
      button->value( 1 );
      break;
    }
    case EndEvent:
    {
      button->selection_color( FL_GREEN );
      button->value( 1 );
      break;
    }
    case ModifiedEvent:
    {
      button->selection_color( FL_RED );
      button->value( 1 );
      break;
    }

  }
  
  button->redraw();
  Fl::check();

}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
LightButtonRedrawCommand
::Execute( const itk::LightObject * , unsigned long event )
{
 
  Fl_Light_Button * button = GetWidget();

  if( !button )
  {
    return;
  }


  switch( event )
  {
    case StartEvent:
    {
      button->selection_color( FL_YELLOW );
      button->value( 1 );
      break;
    }
    case EndEvent:
    {
      button->selection_color( FL_GREEN );
      button->value( 1 );
      break;
    }
    case ModifiedEvent:
    {
      button->selection_color( FL_RED );
      button->value( 1 );
      break;
    }

  }
  
  button->redraw();
  Fl::check();

 
}



} // end namespace li


