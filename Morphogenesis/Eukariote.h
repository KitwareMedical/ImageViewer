#ifndef Eukariote_H
#define Eukariote_H

#include "Cell.h"


namespace bio {


/**
 * \class Eukariote 
 * \brief This class implements the basic behavior of Prokariote cells.
 */
class Eukariote  : public Cell
{


public:

  typedef  Cell  SuperClass;

  Eukariote();
  virtual ~Eukariote();

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

  static const char * GetSpeciesName(void) 
                          { return "Primitive Eukariote"; }
private:
  
  virtual Cell * CreateNew(void);
  unsigned long m_GrowthLatencyTime;
  unsigned long m_DivisionLatencyTime;

  static unsigned long GrowthMaximumLatencyTime;
  static unsigned long DivisionMaximumLatencyTime;

};


} // end namespace bio

#endif
