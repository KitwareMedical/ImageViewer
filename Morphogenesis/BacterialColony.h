
#ifndef __BacterialColony_H
#define __BacterialColony_H

#include "Bacteria.h"



namespace bio {

/**
 * \class BacterialColony
 * \brief This class represent an aggregation of bio::Bacteria.
 * The interactions between the bacteria are managed at this 
 * level. Global coordination is also performed for growing and
 * spreading
 */
class BacterialColony 
{

public:

  typedef Bacteria                    BacteriaType;
  typedef CellType::VectorType        VectorType;
  typedef CellType::PointType         PointType;
	typedef std::list<BacteriaType *>   BacteriaListType;
	

	BacterialColony();
  ~BacterialColony();

  void Draw(void) const;
  void Grow(void);
  void Spread(void);
  void KillAll(void);
  
  VectorType WellPotentialGradient( const VectorType & relativePosition) const;
  
  unsigned int GetNumberOfCells(void) const;
 
  void SetGrowthRadiusLimit( double value );
  void SetGrowthRadiusIncrement( double value );

private:

	BacteriaListType   m_Bacteria;


};

}; // end namespace bio

#endif
