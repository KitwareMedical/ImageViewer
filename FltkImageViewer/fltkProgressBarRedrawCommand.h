/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkProgressBarRedrawCommand.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef __fltk_ProgressBarRedrawCommand_h
#define __fltk_ProgressBarRedrawCommand_h

#include <fltkWidgetRedrawCommand.h>
#include <FL/Fl_Slider.H>


namespace fltk {

/**
 *  Implementation of the Command Patter for invoking OpenGL redraw
 */
class ProgressBarRedrawCommand : public WidgetRedrawCommand<Fl_Slider>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef ProgressBarRedrawCommand   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef fltk::WidgetRedrawCommand<Fl_Slider>  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the GL Window
   */
  void Execute(itk::LightObject *caller, unsigned long event);
  void Execute(const itk::LightObject *caller, unsigned long event);

  // all the currently defined events
  // developers can use -- Command::UserEvent + int to
  // specify their own events. 
  enum GLEventIds {
    RedrawEvent = itk::Command::UserEvent+1
  };


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( ProgressBarRedrawCommand, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
   itkNewMacro(Self);

  
protected:
  /**
   * Constructor
   */
  ProgressBarRedrawCommand();


  
};


} // end namespace li


#endif
