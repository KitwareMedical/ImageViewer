


#include "Bacteria.h"

namespace bio {


  


Bacteria
::Bacteria()
{
}





Bacteria
::~Bacteria()
{
}


void
Bacteria
::Draw(void) const
{
	fl_color( m_Color );
	fl_circle( m_Position[0], m_Position[1], m_Radius );
}


void
Bacteria
::Grow(void) 
{
  m_Radius += RadiusIncrement;
  if( m_Radius > RadiusLimit )
  {
    m_Radius = RadiusLimit;
    Bacteria * bacteria = new Bacteria;
    bacteria->m_Position = m_Position;
  }
}



};  // end namespace bio
