#include "Tissue.h"


namespace bio {


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


}; // end namespace bio

