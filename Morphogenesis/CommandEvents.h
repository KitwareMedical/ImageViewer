
#ifndef bio_Command_Events_h
#define bio_Command_Events_h

#include <itkCommand.h>


namespace bio {

/**
 *  List of the events expected in this domain
 */
enum EventIds 
{
  TimeStepEvent= itk::Command::UserEvent+1
};


  
} // end namespace bio


#endif
