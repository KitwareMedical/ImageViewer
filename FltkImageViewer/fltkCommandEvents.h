/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkCommandEvents.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/



#ifndef fltkCOMMANDEVENTS
#define fltkCOMMANDEVENTS

#include <itkCommand.h>


namespace fltk {

/**
 *  List of the events expected in this domain
 */
enum EventIds 
{
  RedrawEvent = itk::Command::UserEvent+1,
  GlDrawEvent
};


  
} // end namespace fltk


#endif
