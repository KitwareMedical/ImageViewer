


#include "GradientEatingBacteria.h"
#include "CellularAggregate.h"




namespace bio {


// Class static variables
double    GradientEatingBacteria::ChemoAttractantLowThreshold  =  10.0f;
double    GradientEatingBacteria::ChemoAttractantHighThreshold = 100.0f;

Cell::ColorType GradientEatingBacteria::WellNourishedColor;
Cell::ColorType GradientEatingBacteria::HopefullColor;
Cell::ColorType GradientEatingBacteria::StarvingColor;



/**
 *    Constructor
 */ 
GradientEatingBacteria
::GradientEatingBacteria()
{
  m_ScheduleApoptosis    = false;
  m_ChemoAttractantLevel = 200.0f;
  this->SetColorByPressure( false );
}







/**
 *    Destructor
 */ 
GradientEatingBacteria
::~GradientEatingBacteria()
{
}





/**
 *    Cell Division
 */ 
Cell *
GradientEatingBacteria
::CreateNew()
{
  GradientEatingBacteria * bacteria = new GradientEatingBacteria;
  bacteria->m_ParentIdentifier = m_SelfIdentifier;
  return bacteria;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
GradientEatingBacteria
::CreateEgg()
{
  SetGrowthMaximumLatencyTime( 100 );
  SetDivisionMaximumLatencyTime( 100 );

  SetMaximumGenerationLimit( 40 );

  Cell::GrowthRadiusIncrement = 0.01;
  Cell::GrowthRadiusLimit     = 2.00;

  WellNourishedColor.Set(    0.0, 0.0, 1.0 );
  HopefullColor.Set(         0.0, 1.0, 0.0 );
  StarvingColor.Set(         1.0, 0.0, 0.0 );

  SetDefaultColor( HopefullColor );

  GradientEatingBacteria * bacteria = new GradientEatingBacteria;
  bacteria->m_ParentIdentifier = 0;
  bacteria->m_SelfIdentifier = 1;
  bacteria->m_Generation = 0;
  return bacteria;
}




/**
 *    Check point before division
 *    This check point will control
 *    the entrance in the division stage.
 *    It returns true when conditions
 *    required for division are satisfied.
 */ 
bool
GradientEatingBacteria
::CheckPointDivision(void) 
{
  bool super = SuperClass::CheckPointDivision();

  if( !super )
    {
    return super;
    }

  bool here = false;

  if( !m_ScheduleApoptosis )
    {
    if( m_ChemoAttractantLevel > ChemoAttractantLowThreshold )
      {
      here = true;
      }
    }

  return ( super && here );

}



/**
 *    Add a force to the cumulator for evaluating pressure
 */ 
void
GradientEatingBacteria
::AddForce( const VectorType & force )
{
  SuperClass::AddForce( force );
}



/**
 *    Clear the Pressure cumulator at 
 *    the same time the Force cumulator
 *    is cleared.
 */ 
void
GradientEatingBacteria
::ClearForce( void )
{
  SuperClass::ClearForce();
}





void
GradientEatingBacteria
::Divide( void )
{
  SuperClass::Divide();
}
 



void
GradientEatingBacteria
::ReceptorsReading( void )
{
  SuperClass::ReceptorsReading();
  
  CellularAggregate::SubstrateType::PixelType substrate0 =
              m_Aggregate->GetSubstrateValue( m_SelfIdentifier, 0 );
  
  m_ChemoAttractantLevel = substrate0;

  if( m_ChemoAttractantLevel > ChemoAttractantHighThreshold )
    {
    m_Color = WellNourishedColor;
    }
  else if( m_ChemoAttractantLevel > ChemoAttractantLowThreshold )
    {
    m_Color = HopefullColor;
    }
  else
    {
    m_Color = StarvingColor;
    }
}
 



}  // end namespace bio
