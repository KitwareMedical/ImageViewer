/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkProgressBar.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltk_ProgressBar_h
#define __fltk_ProgressBar_h

#include "itkCommand.h"
#include "fltkCommandEvents.h"
#include <FL/Fl_Slider.H>


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
  typedef itk::MemberCommand< ProgressBar >  RedrawCommandType;


  /**
   * Constructor
   */
  ProgressBar(int x, int y, int w, int h, char * label=0):
    Fl_Slider( x, y, w, h, label ) {
      m_RedrawCommand = RedrawCommandType::New();
      m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ProcessEvent );
      m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ConstProcessEvent );
    }


  
  /**
   * Get Command
   */
  RedrawCommandType::Pointer GetRedrawCommand( void ) const
  {
    return m_RedrawCommand.GetPointer();
  }

  
  /**
   * Manage a Progress event
   */
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event )
  {
    if( typeid( itk::ProgressEvent )   ==  typeid( event ) )
      {
      itk::ProcessObject::Pointer  process = 
                 dynamic_cast< itk::ProcessObject *>( caller );
      this->value( process->GetProgress() );
      this->redraw();
      }
  }

  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event )
  {
    if( typeid( itk::ProgressEvent )   ==  typeid( event ) ) 
      {
      itk::ProcessObject::ConstPointer  process = 
                 dynamic_cast< const itk::ProcessObject *>( caller );
      this->value( process->GetProgress() );
      this->redraw();
      }
  }
 

  /**
   * Manage a Progress event
   */
  void Observe( itk::Object *caller )
  {
    caller->AddObserver(  itk::ProgressEvent(), m_RedrawCommand.GetPointer() );
  }

private:

  RedrawCommandType::Pointer m_RedrawCommand;
  
};


} // end namespace fltk


#endif
