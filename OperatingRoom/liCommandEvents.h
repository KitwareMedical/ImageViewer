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

/**
 *  List of the events expected in this domain
 */

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
