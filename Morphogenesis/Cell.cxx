


#include "Cell.h"
#include "GL/gl.h"


namespace bio {


  
Cell::PointType    Cell::DefaultPosition;
Cell::ColorType    Cell::DefaultColor;

double             Cell::DefaultRadius         =       10; // microns

double             Cell::GrowthRadiusIncrement =    0.001; // microns
double             Cell::GrowthRadiusLimit     =       20; // microns

double             Cell::NutrientSelfRepairLevel  =    50; 
double             Cell::EnergySelfRepairLevel    =    50; 

double             Cell::DefaultEnergyIntake      =     1; 
double             Cell::DefaultNutrientsIntake   =     1; 

std::list<Cell *>  Cell::m_Aggregate;       




/**
 *    Constructor
 */ 
Cell
::Cell()
{

	m_Radius      = DefaultRadius;
	m_Position    = DefaultPosition;
  m_Color       = DefaultColor;
  
  m_Aggregate.push_back( this );

}





/**
 *    Destructor   
 */ 
Cell
::~Cell()
{
}




/**
 *    OpenGL Drawing
 */ 
void
Cell
::Draw(void) const
{
  glBegin( GL_TRIANGLE_FAN );
    glColor3f( m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue() );
    glVertex3d(m_Position[0],m_Position[1],m_Position[2]);
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
 *    Add a force to the cumulator
 */ 
void
Cell
::AddForce( const VectorType & force )
{
  m_Force += force;
}



/**
 *    Return a pointer to the static list that
 *    hold reference to all the cells
 */ 
Cell::CellsListType *
Cell
::GetAggregate(void) 
{
  return & m_Aggregate;
}



/**
 *    Get the position of the cell in space
 */ 
const Cell::PointType &
Cell
::GetPosition(void) const
{
  return m_Position;
}



/**
 *    Update the position by applying the force
 *    a viscous medium is assumed, in which 
 *    kinetic energy is disipated, so speeds are
 *    not managed
 */ 
void
Cell
::UpdatePosition(void) 
{
  m_Position += m_Force / 2.0;
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
