#ifndef Prokariote_H
#define Prokariote_H

#include "Cell.h"


namespace bio {


/**
 * \class Prokariote 
 * \brief This class implements the basic behavior of Prokariote cells.
 */
class Prokariote  : public Cell
{


public:

  typedef  Cell  SuperClass;

  Prokariote();
  virtual ~Prokariote();

  virtual void Grow(void);
  virtual void Divide(void);
  virtual void Apoptosis(void);

  virtual bool CheckPointGrowth(void);
  virtual bool CheckPointDivision(void);
  virtual bool CheckPointApoptosis(void);

  static  Cell * CreateEgg(void);

  static void SetGrowthMaximumLatencyTime( unsigned long );
  static void SetDivisionMaximumLatencyTime( unsigned long );
  static unsigned long GetGrowthMaximumLatencyTime(void);
  static unsigned long GetDivisionMaximumLatencyTime(void);

  virtual void SetGrowthLatencyTime( unsigned long );
  virtual void SetDivisionLatencyTime( unsigned long );
  virtual unsigned long GetGrowthLatencyTime(void);
  virtual unsigned long GetDivisionLatencyTime(void);

  static const char * GetSpeciesName(void) 
                          { return "Primitive Prokariote"; }
private:
  
  virtual Cell * CreateNew(void);
  unsigned long m_GrowthLatencyTime;
  unsigned long m_DivisionLatencyTime;

  static unsigned long GrowthMaximumLatencyTime;
  static unsigned long DivisionMaximumLatencyTime;

};


} // end namespace bio

#endif
