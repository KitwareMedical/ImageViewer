/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkWidgetRedrawCommand.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef __fltkWidgetRedrawCommand_h
#define __fltkWidgetRedrawCommand_h

#include <itkCommand.h>
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
  void Execute(itk::LightObject *caller, unsigned long event);
  void Execute(const itk::LightObject *caller, unsigned long event);

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


} // end namespace li
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "fltkWidgetRedrawCommand.txx"
#endif



#endif
