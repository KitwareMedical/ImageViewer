#include "Cell.h"


Point      Cell::DefaultPosition = Point(0.0, 0.0);
Fl_Color   Cell::DefaultColor    = FL_BLUE;
double      Cell::DefaultRadius   = 20; // microns


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
	fl_circle( m_Position.X(), m_Position.Y(), m_Radius );
}
