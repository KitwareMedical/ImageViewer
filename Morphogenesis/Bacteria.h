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
  
  unsigned long m_GrowthLatencyTime;
  unsigned long m_DivisionLatencyTime;

  static unsigned long GrowthMaxLatency;
  static unsigned long DivisionMaxLatency;

};


}; // end namespace bio

#endif
