


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
 *    Growth
 */ 
void
Bacteria
::Grow(void) 
{
  m_Radius += GrowthRadiusIncrement;
  if( m_Radius > GrowthRadiusLimit )
  {
    // Divide is a terminal operation
    // this cell is destroyed in the 
    // process
    this->Divide();
  }
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
 *    Cell Division
 */ 
void
Bacteria
::Divide(void) 
{
  Cell::Divide();

  Bacteria * siblingA = dynamic_cast<Bacteria*>( this->CreateNew() );
  Bacteria * siblingB = dynamic_cast<Bacteria*>( this->CreateNew() );

  siblingA->m_Radius   = m_Radius / sqrt( 2.0f );
  siblingB->m_Radius   = m_Radius / sqrt( 2.0f );

  // Create a perturbation for separating the daugther cells
  double angle = static_cast<double>( rand() ) / 
                 static_cast<double>( RAND_MAX ) *
                 atan(1) * 4.0;

  Cell::VectorType perturbationVector;
  double perturbationLength = m_Radius * 0.75;

  perturbationVector[0] = perturbationLength * cos( angle );
  perturbationVector[1] = perturbationLength * sin( angle );

  CellularAggregate * aggregate = GetCellularAggregate();

  siblingA->m_ParentIdentifier = m_SelfIdentifier;
  siblingB->m_ParentIdentifier = m_SelfIdentifier;

  aggregate->Add( siblingA,  perturbationVector );
  aggregate->Add( siblingB, -perturbationVector );

  aggregate->Remove( this ); // "this" will be destroyed here

}



};  // end namespace bio
