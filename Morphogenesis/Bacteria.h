#ifndef Bacteria_H
#define Bacteria_H

#include "Cell.h"


namespace bio {


/**
 * \class Bacteria 
 * \brief This class implements the basic behavior of Prokariote cells.
 */
class Bacteria  : public Cell
{


public:
  Bacteria();
  ~Bacteria();
  virtual void Grow(void);
  virtual void Divide(void);
  virtual Cell * CreateNew(void);

private:

};


}; // end namespace bio

#endif
