/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWidgetRedrawCommand.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/


#ifndef __fltkWidgetRedrawCommand_h
#define __fltkWidgetRedrawCommand_h

#include "itkCommand.h"
#include <FL/Fl.H>


namespace fltk {


/**
 *  Implementation of the Command Pattern 
 *  for redrawing a fltk Widget 
 *
 *  This command will be registered as observer
 *  of other objects. When one of these objects
 *  change, it can invoke a redraw event, and 
 *  hence this command will be notified. The command
 *  will call the redraw method of the fltk Widget.
 *
 */
template<class TWidget>  
class WidgetRedrawCommand : public itk::Command 
{
public:

  /**
   * Type of the widget for which the command is directed
   */
  typedef TWidget    WidgetType;

  
  /**
   * Standard "Self" typedef.
   */
  typedef WidgetRedrawCommand<WidgetType>   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the Widget
   */
  void Execute(itk::Object *caller, unsigned long event);
  void Execute(const itk::Object *caller, unsigned long event);

  // all the currently defined events
  // developers can use -- Command::UserEvent + int to
  // specify their own events. 
  enum GLEventIds {
    RedrawEvent = itk::Command::UserEvent+1,
  };


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( WidgetRedrawCommand, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);


  /**
   * Set Widget to update
   */
  void SetWidget( WidgetType * widget );

  /**
   * Get Widget to update
   */
  WidgetType * GetWidget( void );



protected:
  /**
   * Constructor
   */
  WidgetRedrawCommand();

private:

  /**
   * Widget to redraw
   */
  WidgetType     *  m_Widget;
 

  
};


} // end namespace  fltk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "fltkWidgetRedrawCommand.txx"
#endif



#endif
