/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateLightButton.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/





#include <liCommandUpdateLightButton.h>
#include <FL/Fl.H>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
CommandUpdateLightButton::CommandUpdateLightButton(void)  
{
  m_Button  = 0;
}


//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
CommandUpdateLightButton::Pointer 
CommandUpdateLightButton::New( void ) 
{
  Self * ret = ::itk::ObjectFactory< Self >::Create();
  if( ret )
  {
    return ret;
  }
  return new Self;

}



//--------------------------------------------------
//
//    Set Window
//
//--------------------------------------------------
void
CommandUpdateLightButton::SetButton( Fl_Light_Button * button )  
{
  m_Button = button;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateLightButton::Execute( itk::LightObject * , unsigned long event )
{

  if( !m_Button )
  {
    return;
  }

  switch( event )
  {
    case StartEvent:
    {
      m_Button->selection_color( FL_YELLOW );
      m_Button->value( 1 );
      m_Button->redraw();
      Fl::check();
      break;
    }
    case EndEvent:
    {
      m_Button->selection_color( FL_GREEN );
      m_Button->value( 1 );
      m_Button->redraw();
      Fl::check();
      break;
    }
    case ModifiedEvent:
    {
      m_Button->selection_color( FL_RED );
      m_Button->value( 1 );
      m_Button->redraw();
      Fl::check();
      break;
    }

  }

}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
void
CommandUpdateLightButton::Execute( const itk::LightObject * , unsigned long event )
{

  if( !m_Button )
  {
    return;
  }

  switch( event )
  {
    case StartEvent:
    {
      m_Button->selection_color( FL_YELLOW );
      m_Button->value( 1 );
      Fl::check();
      break;
    }
    case EndEvent:
    {
      m_Button->selection_color( FL_GREEN );
      m_Button->value( 1 );
      Fl::check();
      break;
    }
    case ModifiedEvent:
    {
      m_Button->selection_color( FL_RED );
      m_Button->value( 1 );
      m_Button->redraw();
      Fl::check();
      break;
    }

  }

}





} // end namespace li


