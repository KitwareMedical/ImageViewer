#include "Tissue.h"


Tissue
::Tissue()
{
}

Tissue
::~Tissue()
{
}

void
Tissue
::Draw(void) const
{
	CellListType::const_iterator cell = cells.begin();
	while( cell != cells.end() )
	{
		cell->Draw();
	}
}


void
Tissue
::Grow(void)
{

}



