#ifndef Tissue_H
#define Tissue_H

#include "Cell.h"
#include <list>


class Tissue 
{


public:

	typedef std::list<Cell>  CellListType;
	
	Tissue();
  ~Tissue();
  void Draw(void) const;
  void Grow(void);

private:

	CellListType    cells;


};



#endif
