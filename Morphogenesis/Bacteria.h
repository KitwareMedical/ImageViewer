#ifndef Bacteria_H
#define Bacteria_H

#include "Prokariote.h"


namespace bio {


/**
 * \class Bacteria 
 * \brief This class implements the basic behavior of Prokariote cells.
 */
class Bacteria  : public Prokariote
{


public:

  typedef  Prokariote  SuperClass;

  Bacteria();
  virtual ~Bacteria();

  static  Cell * CreateEgg(void);

  static const char * GetSpeciesName(void) 
                          { return "Primitive Bacteria"; }
private:
  
  virtual Cell * CreateNew(void);

};


} // end namespace bio

#endif
