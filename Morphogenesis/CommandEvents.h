
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
