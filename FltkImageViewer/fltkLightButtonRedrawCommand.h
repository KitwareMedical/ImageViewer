/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkLightButtonRedrawCommand.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef __fltkLightButtonRedrawCommand_h
#define __fltkLightButtonRedrawCommand_h

#include <fltkWidgetRedrawCommand.h>
#include <FL/Fl_Light_Button.H>


namespace fltk {


/**
 *  Implementation of the Command Pattern 
 *  for redrawing a fltk Light Button 
 *
 *  This command will change the color of 
 *  the light button in response to events
 *  ocurring on the observed object
 *
 */
class LightButtonRedrawCommand : 
  public WidgetRedrawCommand< Fl_Light_Button >
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef LightButtonRedrawCommand   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef fltk::WidgetRedrawCommand  Superclass;


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


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( LightButtonRedrawCommand, WidgetRedrawCommand );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

 
protected:
  /**
   * Constructor
   */
  LightButtonRedrawCommand();


  
};


} // end namespace li


#endif
