/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkLightButton.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkLightButton.h"
#include <FL/Fl.H>


namespace fltk {


/** Constructor */
LightButton
::LightButton(int x, int y, int w, int h, char * label=0):
    Fl_Light_Button( x, y, w, h, label ) 
{
  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &LightButton::ProcessEvent );
}


  
/** Get Command */
LightButton::RedrawCommandType::Pointer 
LightButton::GetRedrawCommand( void ) const
{
  return m_RedrawCommand.GetPointer();
}


/** Manage a Progress event */
void 
LightButton::ProcessEvent( const itk::EventObject & event )
{
  if( typeid( itk::StartEvent )  ==  typeid( event ) ) 
    {
    this->selection_color( FL_YELLOW );
    this->value( 1 );
    }
  else if ( typeid( itk::EndEvent )   ==   typeid( event ) ) 
    {
    this->selection_color( FL_GREEN );
    this->value( 1 );
    }
  else if  ( typeid( itk::ModifiedEvent )  ==   typeid( event ) ) 
    {
    this->selection_color( FL_RED );
    this->value( 1 );
    }

  this->redraw();
  Fl::check();
}
  



/** Register as observer for a set of standard events */
void 
LightButton::Observe( itk::Object *caller )
{
  itk::Object::Pointer observed( caller );
  observed->AddObserver(  itk::StartEvent(),    m_RedrawCommand.GetPointer() );
  observed->AddObserver(  itk::ModifiedEvent(), m_RedrawCommand.GetPointer() );
  observed->AddObserver(  itk::EndEvent(),      m_RedrawCommand.GetPointer() );
}


  

} // end namespace fltk



