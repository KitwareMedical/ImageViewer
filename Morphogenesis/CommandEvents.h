
#ifndef bio_Command_Events_h
#define bio_Command_Events_h

#include <itkCommand.h>


namespace bio {

/**
 *  List of the events expected in this domain
 */
enum EventIds 
{
  RedrawEvent = itk::Command::UserEvent+1,
  GLDrawEvent,
};


  
} // end namespace bio


#endif
