/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkLightButton.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltk_LightButton_h
#define __fltk_LightButton_h

#include "itkCommand.h"
#include "fltkCommandEvents.h"
#include <FL/Fl_Light_Button.H>


namespace fltk {

/**
 *  GUI object that derives from a FLTK Light Button
 *  and contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class LightButton : public Fl_Light_Button     
{
public:

   typedef itk::ReceptorMemberCommand< LightButton >  RedrawCommandType;


  /** Constructor */
  LightButton(int x, int y, int w, int h, char * label=0);
 
  /** Get Command */
  RedrawCommandType * GetRedrawCommand( void ) const;

  /** Manage a Progress event */
  void ProcessEvent( const itk::EventObject & event );
  
  /** Register as observer for a set of standard events */
  void Observe( itk::Object *caller );


private:

  RedrawCommandType::Pointer m_RedrawCommand;
  
};


} // end namespace fltk


#endif

