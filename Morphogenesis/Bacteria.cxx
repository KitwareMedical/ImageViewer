


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
  m_Radius += GrowthRadiusIncrement;
  if( m_Radius > GrowthRadiusLimit )
  {
    this->Divide();
  }
}



void
Bacteria
::Divide(void) 
{

    Bacteria * sibling = new Bacteria;

    m_Radius            = GrowthRadiusLimit/sqrt(2.0);
    sibling->m_Radius   = m_Radius;

    // Create a perturbation for separating the daugther cells
    double angle = static_cast<double>( random() ) / 
                   static_cast<double>( RAND_MAX ) *
                   atan(1) * 4.0;

    Cell::VectorType PerturbationVector;
    double PerturbationLength = GrowthRadiusLimit * 0.1;

    PerturbationVector[0] = PerturbationLength * cos( angle );
    PerturbationVector[1] = PerturbationLength * sin( angle );

    m_Position    	+= PerturbationVector;
    sibling->m_Position -= PerturbationVector; 


}

};  // end namespace bio
