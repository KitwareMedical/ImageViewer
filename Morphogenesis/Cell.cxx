


#include "Cell.h"

namespace bio {


  
Cell::PointType    Cell::DefaultPosition;
Fl_Color           Cell::DefaultColor    = FL_BLUE;
double             Cell::DefaultRadius   = 10; // microns

double             Cell::RadiusIncrement =  1; // microns
double             Cell::RadiusLimit     = 20; // microns

std::list<Cell *>  Cell::m_Aggregate;       


Cell
::Cell()
{

	m_Radius   = DefaultRadius;
	m_Position = DefaultPosition;
  m_Color    = DefaultColor;
  
  m_Aggregate.push_back( this );

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


void
Cell
::Grow(void) 
{
  m_Radius += RadiusIncrement;
  if( m_Radius > RadiusLimit )
  {

  }
}



void
Cell
::ClearForce(void) 
{
  m_Force.Fill( 0.0 );
}



void
Cell
::AddForce( const VectorType & force )
{
  m_Force += force;
}



Cell::CellsListType *
Cell
::GetAggregate(void) 
{
  return & m_Aggregate;
}


const Cell::PointType &
Cell
::GetPosition(void) const
{
  return m_Position;
}


void
Cell
::UpdatePosition(void) 
{
  m_Position += m_Force / 2.0;
}



};  // end namespace bio
