


#include "PressureSensitiveBacteria.h"
#include "CellularAggregate.h"




namespace bio {


// Class static variables
double    PressureSensitiveBacteria::PressureThresold = 10.0f;

Cell::ColorType PressureSensitiveBacteria::LowPressureColor;
Cell::ColorType PressureSensitiveBacteria::MediumPressureColor;
Cell::ColorType PressureSensitiveBacteria::HighPressureColor;
Cell::ColorType PressureSensitiveBacteria::VeryHighPressureColor;




/**
 *    Constructor
 */ 
PressureSensitiveBacteria
::PressureSensitiveBacteria()
{
  m_Pressure          = 0.0f;
  m_PreviousPressure  = 0.0f;
  m_CycleArrest       = false;
  m_ColorByPressure   = true;
}







/**
 *    Destructor
 */ 
PressureSensitiveBacteria
::~PressureSensitiveBacteria()
{
}





/**
 *    Cell Division
 */ 
Cell *
PressureSensitiveBacteria
::CreateNew()
{
  PressureSensitiveBacteria * bacteria = new PressureSensitiveBacteria;
  bacteria->m_ParentIdentifier = m_SelfIdentifier;
  return bacteria;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
PressureSensitiveBacteria
::CreateEgg()
{
  SetGrowthMaximumLatencyTime( 100 );
  SetDivisionMaximumLatencyTime( 100 );
  Cell::GrowthRadiusIncrement = 0.01;
  Cell::GrowthRadiusLimit     = 2.00;

  LowPressureColor.Set(      0.0, 0.0, 1.0 );
  MediumPressureColor.Set(   0.0, 1.0, 1.0 );
  HighPressureColor.Set(     1.0, 0.0, 0.0 );
  VeryHighPressureColor.Set( 0.0, 1.0, 1.0 );

  SetDefaultColor( LowPressureColor );

  PressureSensitiveBacteria * bacteria = new PressureSensitiveBacteria;
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
PressureSensitiveBacteria
::CheckPointDivision(void) 
{
  bool super = SuperClass::CheckPointDivision();

  if( !super )
    {
    return super;
    }

  bool here = false;

  if( !m_CycleArrest )
    {
    if( m_Pressure < PressureThresold )
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
PressureSensitiveBacteria
::AddForce( const VectorType & force )
{
  double factor = 1.0 / pow( m_Radius, Cell::Dimension );

  Cell::AddForce( force );
  m_Pressure += force.GetNorm() * factor;

  if( m_Pressure > PressureThresold )
    {
    if( m_ColorByPressure )
      {
      m_Color = HighPressureColor;
      }
    m_CycleArrest = true;
    }
  else 
    {
    if( !m_CycleArrest )
      {
      if( m_ColorByPressure )
        {
        m_Color = LowPressureColor;
        }
      }
    }

}



/**
 *    Clear the Pressure cumulator at 
 *    the same time the Force cumulator
 *    is cleared.
 */ 
void
PressureSensitiveBacteria
::ClearForce( void )
{
  Cell::ClearForce();
  m_PreviousPressure = m_Pressure;
  m_Pressure = 0.0f;
}




void
PressureSensitiveBacteria
::SetColorByPressure( bool colorByPressure )
{
  m_ColorByPressure = colorByPressure;
}



void
PressureSensitiveBacteria
::Divide( void )
{

  SuperClass::Divide();
}
 



}  // end namespace bio
