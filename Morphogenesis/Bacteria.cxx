


#include "Bacteria.h"
#include "CellularAggregate.h"




namespace bio {



/**
 *    Constructor
 */ 
Bacteria
::Bacteria()
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
 *    Cell Division
 */ 
Cell *
Bacteria
::CreateNew()
{
  Bacteria * bacteria = new Bacteria;
  bacteria->m_ParentIdentifier = m_SelfIdentifier;
  return bacteria;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
Bacteria
::CreateEgg()
{
  SetGrowthMaximumLatencyTime( 100 );
  SetDivisionMaximumLatencyTime( 100 );
  Cell::GrowthRadiusIncrement = 0.01;
  Cell::GrowthRadiusLimit     = 2.00;
  Bacteria * bacteria = new Bacteria;
  bacteria->m_ParentIdentifier = 0;
  bacteria->m_SelfIdentifier = 1;
  bacteria->m_Generation = 0;
  return bacteria;
}




}  // end namespace bio
