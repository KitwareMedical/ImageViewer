/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkProgressBar.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef __fltk_ProgressBar_h
#define __fltk_ProgressBar_h

#include <fltkProgressBarRedrawCommand.h>


namespace fltk {

/**
 *  GUI object that derives from a FLTK Slider and
 *  implements a progress barr.
 *
 *  It contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class ProgressBar : public Fl_Slider
{
public:

  /**
   * Command Class invoked for button redraw
   */
  typedef ProgressBarRedrawCommand  RedrawCommandType;


  /**
   * Constructor
   */
  ProgressBar(int x, int y, int w, int h, char * label=0):
    Fl_Slider( x, y, w, h, label ) {
      m_RedrawCommand = RedrawCommandType::New();
      m_RedrawCommand->SetWidget( this );
    }


  
  /**
   * Get Command
   */
  const RedrawCommandType::Pointer & GetRedrawCommand( void ) const
  {
    return m_RedrawCommand;
  }

private:

  RedrawCommandType::Pointer m_RedrawCommand;
  
};


} // end namespace fltk


#endif
