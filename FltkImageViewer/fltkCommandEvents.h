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

#include "itkEventObject.h"


namespace fltk {

/**
 *  List of the events expected in this domain
 */
itkEventMacro( RedrawEvent,            itk::UserEvent );
itkEventMacro( GlDrawEvent,            itk::UserEvent );
itkEventMacro( VolumeReslicedEvent,    itk::UserEvent );


  
} // end namespace fltk


#endif
