/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CommandEvents.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef bio_Command_Events_h
#define bio_Command_Events_h

#include "itkCommand.h"
#include "itkEventObject.h"


namespace bio {

/**
 *  List of the events expected in this domain
 */
itkEventMacro( TimeStepEvent, itk::UserEvent );


  
} // end namespace bio


#endif
