/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkLightButton.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef __fltk_LightButton_h
#define __fltk_LightButton_h

#include <fltkLightButtonRedrawCommand.h>


namespace fltk {

/**
 *  GUI object that derives from a FLTK Light Button
 *  and contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class LightButton : public Fl_Light_Button     
{
public:

  /**
   * Command Class invoked for button redraw
   */
  typedef LightButtonRedrawCommand  RedrawCommandType;


  /**
   * Constructor
   */
  LightButton(int x, int y, int w, int h, char * label=0):
    Fl_Light_Button( x, y, w, h, label ) {
      m_Command = RedrawCommandType::New();
      m_Command->SetWidget( this );
    }


  
  /**
   * Get Command
   */
  const RedrawCommandType::Pointer & GetRedrawCommand( void ) const
  {
    return m_Command;
  }

private:

  RedrawCommandType::Pointer m_Command;
  
};


} // end namespace fltk


#endif
