


#include "Bacteria.h"

namespace bio {


  


/**
 *    Constructor
 */ 
Bacteria
::Bacteria()
{
}



/**
 *    Constructor
 */ 
Bacteria
::Bacteria(Cell::CellsListType * cells):Cell( cells )
{
}






/**
 *    Destructor
 */ 
Bacteria
::~Bacteria()
{
}




/**
 *    Growth
 */ 
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



/**
 *    Cell Division
 */ 
void
Bacteria
::Divide(void) 
{


    Bacteria * sibling = new Bacteria( GetAggregate() );

    m_Radius            = m_Radius/sqrt(2.0);
    sibling->m_Radius   = m_Radius;

    // Create a perturbation for separating the daugther cells
    double angle = static_cast<double>( rand() ) / 
                   static_cast<double>( RAND_MAX ) *
                   atan(1) * 4.0;

    Cell::VectorType PerturbationVector;
    double PerturbationLength = m_Radius * 0.75;

    PerturbationVector[0] = PerturbationLength * cos( angle );
    PerturbationVector[1] = PerturbationLength * sin( angle );

    sibling->m_Position = m_Position - PerturbationVector; 
    
    m_Position    	   += PerturbationVector;

}



};  // end namespace bio
