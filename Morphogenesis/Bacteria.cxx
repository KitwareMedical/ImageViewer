


#include "Bacteria.h"
#include "CellularAggregate.h"




namespace bio {


unsigned long Bacteria::GrowthMaxLatency    = 100; 
unsigned long Bacteria::DivisionMaxLatency  = 500; 


/**
 *    Constructor
 */ 
Bacteria
::Bacteria()
{
  // add a random time before starting to grow
  m_GrowthLatencyTime   = rand() % GrowthMaxLatency;
  m_DivisionLatencyTime = rand() % DivisionMaxLatency;
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
  SuperClass::Grow();
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
 *    This is a terminal operation. This cell will
 *    be destroyed in the process, and two new cell
 *    will be created.
 */ 
void
Bacteria
::Divide(void) 
{
  SuperClass::Divide();

  Bacteria * siblingA = dynamic_cast<Bacteria*>( this->CreateNew() );
  Bacteria * siblingB = dynamic_cast<Bacteria*>( this->CreateNew() );

  siblingA->m_Radius   = m_Radius / sqrt( 2.0f );
  siblingB->m_Radius   = m_Radius / sqrt( 2.0f );

  siblingA->m_Generation = m_Generation + 1;
  siblingB->m_Generation = m_Generation + 1;

  // Create a perturbation for separating the daugther cells
  double angle = static_cast<double>( rand() ) / 
                 static_cast<double>( RAND_MAX ) *
                 atan(1.0f) * 4.0;

  Cell::VectorType perturbationVector;
  double perturbationLength = m_Radius * 0.75;

  perturbationVector[0] = perturbationLength * cos( angle );
  perturbationVector[1] = perturbationLength * sin( angle );

  CellularAggregate * aggregate = GetCellularAggregate();

  siblingA->m_ParentIdentifier = m_SelfIdentifier;
  siblingB->m_ParentIdentifier = m_SelfIdentifier;

//  aggregate->Add( siblingA,  perturbationVector );
//  aggregate->Add( siblingB, -perturbationVector );

  aggregate->Add( siblingA, siblingB, perturbationVector );

  aggregate->Remove( this ); // "this" will be destroyed here

}



/**
 *    Programmed Cell Death 
 *    This is the cellular equivalent of suicide.
 */ 
void
Bacteria
::Apoptosis(void) 
{

  // Because this is a terminal method,
  // the superclass behavior is the last
  // to be called
  SuperClass::Apoptosis();
}



/**
 *    Check point after division
 *    This check point will control
 *    the entrance in the growth stage.
 *    It returns true when conditions
 *    required for growth are satisfied.
 */ 
bool
Bacteria
::CheckPointGrowth(void) 
{

  bool super = SuperClass::CheckPointGrowth();
  bool here  = false;

  if( m_GrowthLatencyTime )
    {
    m_GrowthLatencyTime--;
    }
  else
    {
    here = true;
    }

  return ( super && here );
}


/**
 *    Check point before division
 *    This check point will control
 *    the entrance in the division stage.
 *    It returns true when conditions
 *    required for division are satisfied.
 */ 
bool
Bacteria
::CheckPointDivision(void) 
{
  bool super = SuperClass::CheckPointDivision();

  // Stop dividing after 10th generation
  bool fatality  = (m_Generation < 10); 

  bool here = false;

  if( m_Radius >= GrowthRadiusLimit )
    {
    if( m_DivisionLatencyTime )
      {
      m_DivisionLatencyTime--;
      }
    else
      {
      here = true;
      }
    }

  return ( super && here && fatality );

}



/**
 *    Check point before apoptosis
 *    This check point will control
 *    the entrance in the apoptosis stage.
 *    It returns true when conditions
 *    required for apoptosis are satisfied.
 *    The cell will die in apoptosis.
 */ 
bool
Bacteria
::CheckPointApoptosis(void) 
{
  bool super = SuperClass::CheckPointApoptosis();
  bool here  = false;
  return ( super || here );
}





};  // end namespace bio
