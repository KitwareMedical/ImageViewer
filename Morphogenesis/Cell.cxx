#include "Cell.h"

namespace bio {


Cell::PointType  Cell::DefaultPosition;
Fl_Color         Cell::DefaultColor    = FL_BLUE;
double           Cell::DefaultRadius   = 20; // microns


Cell
::Cell()
{
	m_Radius   = DefaultRadius;
	m_Position = DefaultPosition;
  m_Color    = DefaultColor;
}





Cell
::~Cell()
{
}


void
Cell
::Draw(void) const
{
	fl_color( m_Color );
	fl_circle( m_Position[0], m_Position[1], m_Radius );
}



};  // end namespace bio
