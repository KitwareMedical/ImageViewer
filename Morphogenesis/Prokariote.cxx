


#include "Prokariote.h"
#include "CellularAggregate.h"




namespace bio {


unsigned long Prokariote::GrowthMaximumLatencyTime    = 50; 
unsigned long Prokariote::DivisionMaximumLatencyTime  = 50; 


/**
 *    Constructor
 */ 
Prokariote
::Prokariote()
{
  // add a random time before starting to grow
  m_GrowthLatencyTime   = rand() % this->GetGrowthMaximumLatencyTime();
  m_DivisionLatencyTime = rand() % this->GetDivisionMaximumLatencyTime();
}







/**
 *    Destructor
 */ 
Prokariote
::~Prokariote()
{
}




/**
 *    Growth
 */ 
void
Prokariote
::Grow(void) 
{
  SuperClass::Grow();
}



/**
 *    Cell Division
 */ 
Cell *
Prokariote
::CreateNew()
{
  Prokariote * prokariote = new Prokariote;
  prokariote->m_ParentIdentifier = m_SelfIdentifier;
  return prokariote;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
Prokariote
::CreateEgg()
{
  Prokariote * prokariote = new Prokariote;
  prokariote->m_ParentIdentifier = 0;
  prokariote->m_SelfIdentifier = 1;
  prokariote->m_Generation = 0;
  return prokariote;
}



/**
 *    Cell Division
 *    This is a terminal operation. This cell will
 *    be destroyed in the process, and two new cell
 *    will be created.
 */ 
void
Prokariote
::Divide(void) 
{
  SuperClass::Divide();

  Prokariote * siblingA = dynamic_cast<Prokariote*>( this->CreateNew() );
  Prokariote * siblingB = dynamic_cast<Prokariote*>( this->CreateNew() );

  siblingA->m_Radius   = m_Radius / sqrt( 2.0f );
  siblingB->m_Radius   = m_Radius / sqrt( 2.0f );

  siblingA->m_Generation = m_Generation + 1;
  siblingB->m_Generation = m_Generation + 1;

  // Create a perturbation for separating the daugther cells
  Cell::VectorType perturbationVector;
  double perturbationLength = m_Radius * 0.75;
  for(unsigned int d=0; d<PointDimension; d++)
    {
    perturbationVector[d] = perturbationLength * 
                            static_cast<double>( rand()-RAND_MAX/2.0 ) / 
                            static_cast<double>( RAND_MAX );
    }

  CellularAggregate * aggregate = GetCellularAggregate();

  siblingA->m_ParentIdentifier = m_SelfIdentifier;
  siblingB->m_ParentIdentifier = m_SelfIdentifier;

  aggregate->Add( siblingA, siblingB, perturbationVector );

  this->MarkForRemoval();

}



/**
 *    Programmed Cell Death 
 *    This is the cellular equivalent of suicide.
 */ 
void
Prokariote
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
Prokariote
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
Prokariote
::CheckPointDivision(void) 
{
  bool super = SuperClass::CheckPointDivision();

  if( !super )
    {
    return super;
    }

  bool here = false;

  if( m_DivisionLatencyTime )
    {
    m_DivisionLatencyTime--;
    }
  else
    {
    here = true;
    }

  return ( super && here );

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
Prokariote
::CheckPointApoptosis(void) 
{
  bool super = SuperClass::CheckPointApoptosis();
  bool here  = false;
  return ( super || here );
}




/**
 *    Set Division Latency Time
 */ 
void
Prokariote
::SetDivisionMaximumLatencyTime( unsigned long latency )
{
  Prokariote::DivisionMaximumLatencyTime = latency;
}




/**
 *    Set Growth Latency Time
 */ 
void
Prokariote
::SetGrowthMaximumLatencyTime( unsigned long latency )
{
  Prokariote::GrowthMaximumLatencyTime = latency;
}



/**
 *    Get Division Latency Time
 */ 
unsigned long 
Prokariote
::GetDivisionMaximumLatencyTime( void )
{
  return Prokariote::DivisionMaximumLatencyTime;
}




/**
 *    Get Growth Latency Time
 */ 
unsigned long 
Prokariote
::GetGrowthMaximumLatencyTime( void )
{
  return Prokariote::GrowthMaximumLatencyTime; 
}




/**
 *    Set Division Latency Time
 */ 
void
Prokariote
::SetDivisionLatencyTime( unsigned long latency )
{
  m_DivisionLatencyTime = latency;
}




/**
 *    Set Growth Latency Time
 */ 
void
Prokariote
::SetGrowthLatencyTime( unsigned long latency )
{
  m_GrowthLatencyTime = latency;
}



/**
 *    Get Division Latency Time
 */ 
unsigned long 
Prokariote
::GetDivisionLatencyTime( void )
{
  return m_DivisionLatencyTime;
}




/**
 *    Get Growth Latency Time
 */ 
unsigned long 
Prokariote
::GetGrowthLatencyTime( void )
{
  return m_GrowthLatencyTime; 
}




}  // end namespace bio
