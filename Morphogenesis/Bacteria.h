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

  typedef  Cell  SuperClass;

  Bacteria();
  ~Bacteria();

  virtual void Grow(void);
  virtual void Divide(void);
  virtual void Apoptosis(void);

  virtual bool CheckPointGrowth(void);
  virtual bool CheckPointDivision(void);
  virtual bool CheckPointApoptosis(void);

  virtual Cell * CreateNew(void);

  static void SetGrowthLatencyTime( unsigned long );
  static void SetDivisionLatencyTime( unsigned long );

private:
  
  unsigned long m_GrowthLatencyTime;
  unsigned long m_DivisionLatencyTime;

  static unsigned long GrowthMaxLatency;
  static unsigned long DivisionMaxLatency;

};


} // end namespace bio

#endif
