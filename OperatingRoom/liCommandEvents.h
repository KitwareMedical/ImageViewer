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


namespace li {

/**
 *  List of the events expected in this domain
 */
enum EventIds 
{
  RedrawEvent = itk::Command::UserEvent+1,
  FluoroscopyUnitMovedEvent,
  TableMovedEvent,
  PatientMovedEvent,
  VolumeReslicedEvent,
  GLDrawEvent,
  SpineChangedEvent,
  VesselsChangedEvent,
  RequestVesselPointSetGenerationEvent,
  FieldOfViewChangedEvent

};


  
} // end namespace li


#endif
