


#include "Cell.h"
#include "FL/gl.h"
#include <new>


namespace bio {


  
Cell::ColorType    Cell::DefaultColor;

double             Cell::DefaultRadius         =       10; // microns

double             Cell::GrowthRadiusIncrement =    0.001; // microns
double             Cell::GrowthRadiusLimit     =       20; // microns

double             Cell::NutrientSelfRepairLevel  =    50; 
double             Cell::EnergySelfRepairLevel    =    50; 

double             Cell::DefaultEnergyIntake      =     1; 
double             Cell::DefaultNutrientsIntake   =     1; 


unsigned long      Cell::Counter = 0; // number of cells created


/**
 *    Constructor Lonely Cell
 */ 
Cell
::Cell()
{

	m_Radius      = DefaultRadius;
  m_Color       = DefaultColor;
  
  m_ParentIdentifier = 0;    // Parent cell has to write here

  // The first Cell is numbered as 1
  Counter++;
  m_SelfIdentifier = Counter;  

}




/**
 *    Destructor   
 */ 
Cell
::~Cell()
{
}



/**
 *    Cell Division
 */ 
void
Cell
::Divide(void) 
{
}




/**
 *    Create a New cell
 *    this method behave like a factory, it is 
 *    intended to be overloaded in any class 
 *    deriving from Cell.
 */ 
Cell *
Cell 
::CreateNew(void) 
{
  Cell * cell = new Cell;
  cell->m_ParentIdentifier = m_SelfIdentifier;
  return cell;
}




/**
 *    OpenGL Drawing
 */ 
void
Cell
::Draw( const PointType & position ) const
{

  const unsigned int NumberOfSides = 12;
  const double PI = 4.0 * atan(1.0);
  const double sectorAngle = 2.0 * PI / static_cast<double>( NumberOfSides );

  glBegin( GL_LINE_LOOP );
    glColor3f( m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue() );
    for(unsigned int side=0; side < NumberOfSides; side++)
      {
      const double angle = static_cast<double>( side ) * sectorAngle;
      double x = m_Radius * cos( angle ) + position[0];
      double y = m_Radius * sin( angle ) + position[1];
      glVertex2d( x, y );
      }
  glEnd(); 

}



/**
 *   Cell Growth
 *   Growth is conditioned to the availability of 
 *   nutrients and energy beyond the critical limit 
 *   of self-repair
 *   
 *   Growth is limited by a constraint in the size
 *   of cell's radius
 */ 
void
Cell
::Grow(void) 
{
  if ( m_NutrientsReserveLevel > NutrientSelfRepairLevel &&
       m_EnergyReserveLevel    > EnergySelfRepairLevel       )
    {
    m_Radius += GrowthRadiusIncrement;
    if( m_Radius > GrowthRadiusLimit )
      {
      m_Radius = GrowthRadiusLimit;
      }
    }
}



/**
 *    Clear the cumulator for applied forces
 */ 
void
Cell
::ClearForce(void) 
{
  m_Force.Fill( 0.0 );
}


/**
 *    Return the cumulated force
 */ 
const Cell::VectorType &
Cell
::GetForce(void) const
{
  return m_Force;
}



/**
 *    Return the ID  of this cell
 */ 
Cell::IdentifierType
Cell
::GetSelfIdentifier(void) const
{
  return m_SelfIdentifier;
}



/**
 *    Return the ID  of the parent cell
 */ 
Cell::IdentifierType
Cell
::GetParentIdentifier(void) const
{
  return m_ParentIdentifier;
}




/**
 *    Return a const pointer to the Cellular Aggregate
 */ 
const CellularAggregate *
Cell
::GetCellularAggregate(void) const
{
  return m_Aggregate;
}



/**
 *    Return a pointer to the Cellular Aggregate
 */ 
CellularAggregate *
Cell
::GetCellularAggregate(void) 
{
  return m_Aggregate;
}





/**
 *   Set Cellular Aggregate
 */ 
void
Cell
::SetCellularAggregate( CellularAggregate * cells ) 
{
  m_Aggregate = cells;
}



/**
 *    Return the radius 
 */ 
double
Cell
::GetRadius(void) const
{
  return m_Radius;
}


/**
 *    Return the Color 
 */ 
Cell::ColorType
Cell
::GetColor(void) const
{
  return m_Color;
}




/**
 *    Add a force to the cumulator
 */ 
void
Cell
::AddForce( const VectorType & force )
{
  m_Force += force;
}




/**
 *    Set the value of the limiting cell radius
 *    this is a static value used for the whole
 *    cellular aggregate
 */ 
void
Cell
::SetGrowthRadiusLimit( double value ) 
{
  GrowthRadiusLimit = value;
}


/**
 *    Set the value of the increment in cellular
 *    radius at each time step
 *    this is a static value used for the whole
 *    cellular aggregate
 */ 
void
Cell
::SetGrowthRadiusIncrement( double value ) 
{
  GrowthRadiusIncrement = value;
}




/**
 *    Execute a time step in the life of the cell
 *    Nutrients are acquired
 *    Energy is acquired
 *    If conditions allow it, the cell will grow
 *    The position will be updated according to
 *    applied forces
 */ 
void
Cell
::AdvanceTimeStep(void) 
{
  
  this->NutrientsIntake();
  this->EnergyIntake();
  this->Grow();

}



/**
 *    Ingestion of nutrients
 */
void
Cell
::NutrientsIntake(void) 
{
  m_NutrientsReserveLevel += DefaultNutrientsIntake;
}



/**
 *    Acquisition of energy
 */
void
Cell
::EnergyIntake(void) 
{
  m_EnergyReserveLevel += DefaultEnergyIntake;
}


/**
 *    Set default Color
 */
void
Cell
::SetDefaultColor( const ColorType & color )
{
  DefaultColor = color;
}


};  // end namespace bio
