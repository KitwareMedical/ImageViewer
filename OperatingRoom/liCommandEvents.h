/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandEvents.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------
#ifndef liCOMMANDEVENTS
#define liCOMMANDEVENTS

#include "itkCommand.h"
#include "itkEventObject.h"

namespace li {

/** List of the events expected in this domain. */
itkEventMacro( RedrawEvent,                 itk::UserEvent );
itkEventMacro( FluoroscopyUnitMovedEvent,   itk::UserEvent );
itkEventMacro( TableMovedEvent,             itk::UserEvent );
itkEventMacro( PatientMovedEvent,           itk::UserEvent );
itkEventMacro( VolumeReslicedEvent,         itk::UserEvent );
itkEventMacro( GLDrawEvent,                 itk::UserEvent );
itkEventMacro( SpineChangedEvent,           itk::UserEvent );
itkEventMacro( VesselsChangedEvent,         itk::UserEvent );
itkEventMacro( FieldOfViewChangedEvent,     itk::UserEvent );
itkEventMacro( RequestVesselPointSetGenerationEvent, itk::UserEvent );

} // end namespace li


#endif
