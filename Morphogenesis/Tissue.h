#ifndef Tissue_H
#define Tissue_H

#include "Cell.h"
#include <list>

namespace bio {

class Tissue 
{

  typedef Cell                  CellType;
  typedef CellType::VectorType  VectorType;
  typedef CellType::PointType   PointType;

public:

	typedef std::list<CellType *>  CellsListType;
	
	Tissue();
  ~Tissue();
  void Draw(void) const;
  void Grow(void);
  void Spread(void);
  void Restart(void);
  VectorType WellPotentialGradient( const VectorType & relativePosition) const;
  unsigned int GetNumberOfCells(void) const;
  void SetCells( CellsListType * );
  void SetGrowthRadiusLimit( double value );
  void SetGrowthRadiusIncrement( double value );

private:

	CellsListType   * m_Cells;


};

}; // end namespace bio

#endif
