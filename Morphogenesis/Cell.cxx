/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Cell.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "Cell.h"
#include "CellularAggregate.h"
#include "vnl/vnl_math.h"
#include "vnl/vnl_sample.h"
#include <new>
#include "FL/gl.h"



namespace bio {


  
Cell::ColorType    Cell::DefaultColor;

double             Cell::DefaultRadius         =       1.0; // microns


double             Cell::GrowthRadiusIncrement =       0.01; // microns
double             Cell::GrowthRadiusLimit     =       2.0; // microns
unsigned long      Cell::MaximumGenerationLimit =      30L; // 30th generation 

unsigned long      Cell::GrowthMaximumLatencyTime    = 50; 

double             Cell::NutrientSelfRepairLevel  =      0; 
double             Cell::EnergySelfRepairLevel    =      0; 

double             Cell::DefaultEnergyIntake      =      1; 
double             Cell::DefaultNutrientsIntake   =      1; 

fltk::Sphere3D::Pointer    Cell::SphereShape;

unsigned long      Cell::Counter = 0; // number of cells created

int                Cell::DisplayList = 0;  // OpenGL display list

Cell::GeneIdType   Cell::RedGene   = "Red";
Cell::GeneIdType   Cell::GreenGene = "Green";
Cell::GeneIdType   Cell::BlueGene  = "Blue";
Cell::GeneIdType   Cell::Cdk2E     = "Cdk2E";
Cell::GeneIdType   Cell::Caspase   = "Caspase";


/**
 *    Constructor Lonely Cell
 */ 
Cell
::Cell()
{

  m_Genome      = 0;
  m_GenomeCopy  = 0;
  
  m_Radius      = DefaultRadius;
  m_Color       = DefaultColor;
  
  m_ParentIdentifier = 0;    // Parent cell has to write here

  // The first Cell is numbered as 1
  Counter++;
  m_SelfIdentifier = Counter;  

  m_Generation     = 0;
  m_CycleState     = Gap1;  // cells are created in Gap1 state

  // Start with minimum reserves
  m_NutrientsReserveLevel = NutrientSelfRepairLevel + DefaultNutrientsIntake;
  m_EnergyReserveLevel    = EnergySelfRepairLevel   + DefaultEnergyIntake;

  // delay before starting to grow after Mitosis
  m_GrowthLatencyTime   = 
       vnl_sample_uniform( 0, this->GetGrowthMaximumLatencyTime() );

  // too young to die...
  m_MarkedForRemoval = false;

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
::Mitosis(void) 
{
}



/**
 *    DNA Replication 
 */ 
void
Cell
::DNAReplication(void) 
{
  m_GenomeCopy = new Genome;
  m_GenomeCopy->Copy( *m_Genome );
}




/**
 *    Programmed Cell Death 
 *    This is the cellular equivalent of suicide.
 */ 
void
Cell
::Apoptosis(void) 
{

  delete m_Genome;
  delete m_GenomeCopy;

  m_Genome     = 0;
  m_GenomeCopy = 0;

  CellularAggregate * aggregate = GetCellularAggregate();
  // "this" cell will be destroyed here
  aggregate->Remove( this ); 

}



/**
 *    Check point after division
 *    This check point will control
 *    the entrance in the growth stage.
 *    It returns true when conditions
 *    required for growth are satisfied.
 */ 
bool
Cell
::CheckPointGrowth(void) 
{
  return true;
}


/**
 *    Check point before initiating DNA replication.
 *    This check point controls the entrance in the 
 *    duplication of the genome by DNA synthesis, also
 *    known as the S phase of the Cell cycle.
 *    This method returns true when conditions required 
 *    for DNA replication are satisfied.
 */ 
bool
Cell
::CheckPointDNAReplication(void) 
{
  // radius & teleomerasa counting should be removed from here
  // and be related to Cdk expression by using proteins like P53
  // The radius should be estimated by a cytoskeleton-related protein.
  const bool fatality = (m_Generation < MaximumGenerationLimit );
  const bool radius   = (m_Radius >= GrowthRadiusLimit);

  bool isOkToReplicate = true;
  const double cdk2E = m_Genome->GetExpressionLevel( Cdk2E );
  if( cdk2E < 0.8 )
    {
    isOkToReplicate = false;
    }

  return ( radius && fatality && isOkToReplicate );
}



/**
 *    Check point before dividing the cell in two daughter cells
 *    at this point DNA replication has already been performed
 *    as well as DNA proofreading and error corrections. This 
 *    check point in principle shoult test if the resulting 
 *    genomes satisfy the quality standards of a living cell. 
 */
bool
Cell
::CheckPointMitosis(void) 
{
  const bool DNAProofRead = ( m_GenomeCopy && m_Genome );
  if( !DNAProofRead ) 
  {
    std::cerr << "PANIC: DNA failed ! " << std::endl;
  }
  return DNAProofRead;
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
Cell
::CheckPointApoptosis(void) 
{
  bool executeApoptosis = false;
  if(  m_Genome->GetExpressionLevel( Caspase ) > 0.8 )
    {
    executeApoptosis = true;
    }
  else
    {
    executeApoptosis = false;
    }
  return executeApoptosis;
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
 *    Create a New Egg Cell
 *    this method behave like a factory, it is 
 *    intended to be overloaded in any class 
 *    deriving from Cell.
 */ 
Cell *
Cell 
::CreateEgg(void) 
{

  // Create the representation for 3D
  SphereShape = fltk::Sphere3D::New();
  SphereShape->SetNumberOfSlices( 12 );
  SphereShape->SetNumberOfStacks( 6 );

  Cell * cell = new Cell;
  cell->m_ParentIdentifier = 0;
  cell->m_SelfIdentifier = 1;
  cell->m_Generation = 0;

  cell->m_Genome = new Genome;

  return cell;
  
}




/**
 *  Mark this cell for removal
 *  The cellular aggregate with remove
 *  this cell from its list at the earliest occasion
 */ 
void
Cell 
::MarkForRemoval(void) 
{
  m_MarkedForRemoval = true;
}



/**
 *  Mark this cell for removal
 *  The cellular aggregate with remove
 *  this cell from its list at the earliest occasion
 */ 
bool
Cell 
::MarkedForRemoval(void) const
{
  return m_MarkedForRemoval;
}




/**
 *    OpenGL Drawing
 */ 
void
Cell
::Draw( const PointType & position ) const
{

  glPushMatrix();

  switch( Dimension )
  {
  case 2:
    glTranslated( position[0], position[1], 0.0 );
    glColor3f( m_Color.GetRed(), 
               m_Color.GetGreen(), 
               m_Color.GetBlue() );
    break;
  case 3:  
    glTranslated( position[0], position[1], position[2] );
    GLfloat color[] = { 
      static_cast<GLfloat>( m_Color.GetRed() ), 
      static_cast<GLfloat>( m_Color.GetGreen() ),
      static_cast<GLfloat>( m_Color.GetBlue() ), 
      static_cast<GLfloat>( 1.0 )
      };
    glMaterialfv(GL_FRONT,GL_DIFFUSE,color);
    break;
  }

  const double scale = m_Radius;
  glScaled( scale, scale, scale );

  if( DisplayList )
    {
    glCallList( DisplayList ); // shape common to all cells
    } 
  else 
    {
    // Create the display list
    DisplayList = glGenLists( 1 ); 
    glNewList( DisplayList, GL_COMPILE_AND_EXECUTE );
    switch( Dimension )
      {
      case 2: 
        {
        DrawCircle();
        break;
        }
      case 3: 
        {
        DrawSphere();
        //DrawIcosaedron();
        break;
        }
      }
    glEndList();
    }

  glPopMatrix();

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
  if( m_GrowthLatencyTime )
  {
    m_GrowthLatencyTime--;
    return;
  }

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
 *    Set Growth Latency Time
 */ 
void
Cell
::SetGrowthMaximumLatencyTime( unsigned long latency )
{
  Cell::GrowthMaximumLatencyTime = latency;
}




/**
 *    Get Growth Latency Time
 */ 
unsigned long 
Cell
::GetGrowthMaximumLatencyTime( void ) 
{
  return Cell::GrowthMaximumLatencyTime;
}





/**
 *    Clear the cumulator for applied forces
 */ 
void
Cell
::ClearForce(void) 
{
  m_Force.Fill( 0.0f );
  m_Pressure  = 0.0f;
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
  double factor = 1.0 / pow( m_Radius, Cell::Dimension );
  m_Force    += force;
  m_Pressure += force.GetNorm() * factor;
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
 *    Set the value of the limit of cell generation.
 *    After this generation cells will stop dividing
 *    A mechanism similar to the inhibition of Telomerase
 *    that impose a limit to the maximum number of times
 *    that the genome can be replicated.
 */ 
void
Cell
::SetMaximumGenerationLimit( unsigned long generationLimit )
{
  MaximumGenerationLimit = generationLimit;
}



/**
 *    Get the value of the limiting cell radius
 *    this is a static value used for the whole
 *    cellular aggregate
 */ 
double
Cell
::GetGrowthRadiusLimit( void ) 
{
  return GrowthRadiusLimit;
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
 *    Execute a time step in the life of the cell.
 *    This is one step in the cell cycle.
 *
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

  // get input from the environment
  this->ReceptorsReading(); 

  // update the level of expression of all the
  // genes in the gene network
  this->ComputeGeneNetwork();

  // this methods produce the effects of gene
  // activation and protein synthesis. It is
  // mostly used for secreting proteins already
  // synthetized in the ComputeGeneNetwork method.
  this->SecreteProducts();

  // If this happens, it is an
  // emergency situation: Do it first.
  if( this->CheckPointApoptosis() )
    {
    m_CycleState = Apop;
    }


  switch( m_CycleState )
  {
  case M: // Mitosis
    m_CycleState = Gap1;
    break;
  case Gap1: // Gap 1 : growing
    {
    if( this->CheckPointDNAReplication() )
      {
      m_CycleState = S;
      }

    break;
    }
  case S:
    m_CycleState = Gap2;
    break;
  case Gap2:
    if( this->CheckPointMitosis() )
      {
      m_CycleState = M;
      }
    break;
  case Gap0:
    // The cell is in cell cycle arrest
    m_CycleState = Gap0;
    break;
  case Apop:
    m_CycleState = Apop;
    break;
  }



  // Atomaton : Execute action
  switch( m_CycleState )
  {
  case M:  // Mitosis
    // This is a terminal action. The implementation of the cell 
    // is destroyed after division. Our abstraction assumes that 
    // the cell disapears and two new cell are created.
    this->Mitosis();
    break;
  case Gap1:
    // Eat and grow
    this->NutrientsIntake();
    this->EnergyIntake();
    this->Grow();
    break;
  case Gap0:
    this->NutrientsIntake();
    this->EnergyIntake();
    break;
  case S:
    this->DNAReplication();
    break; 
  case Gap2:
    break;
  case Apop:
    this->Apoptosis();
    break;
  }
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
 *    Reading substrate using receptors
 */
void
Cell
::ReceptorsReading(void) 
{
}



/**
 *   Compute the Gene Network
 *   This method update the level of expression of 
 *   all the genes in the cell's genome.
 *   see: http://www.ingeneue.org  for details
 */ 
void
Cell
::ComputeGeneNetwork(void) 
{
}


/**
 *   Secrete synthetized products resulting from 
 *   the gene network update
 */ 
void
Cell
::SecreteProducts(void) 
{
  m_Color.SetRed(    m_Genome->GetExpressionLevel( RedGene   ) );
  m_Color.SetGreen(  m_Genome->GetExpressionLevel( GreenGene ) );
  m_Color.SetBlue(   m_Genome->GetExpressionLevel( BlueGene  ) );
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



/**
 *    Reset the counter 
 */
void
Cell
::ResetCounter( void )
{
  Counter = 0;
}


/**
 *   Draw a circle representing a 2D cell
 */ 
void
Cell
::DrawCircle(void) const
{
  const unsigned int NumberOfSides = 12;
  const double PI = 4.0 * atan(1.0);
  const double sectorAngle = 2.0 * PI / static_cast<double>( NumberOfSides );

  glBegin( GL_LINE_LOOP );
    for(unsigned int side=0; side < NumberOfSides; side++)
      {
      const double angle = static_cast<double>( side ) * sectorAngle;
      const double x = cos( angle ); 
      const double y = sin( angle );
      glVertex2d( x, y );
      }
  glEnd(); 
}
 



/**
 *   Draw a Sphere representing a 3D cell
 */ 
void
Cell
::DrawSphere(void) const
{
  SphereShape->DrawGeometry();
}


/**
 *   Draw an Icosaedron representing a 3D cell
 */ 
void
Cell
::DrawIcosaedron(void) const
{

  PointType vertex[12];

  const double r =                       1.0; 
  const double a =                   r / 2.0;
  const double b =    r * sqrt( 3.0f ) / 2.0;
  const double c =  8.0 * atan( 1.0f ) / 5.0;
  
  vertex[ 0 ][ 0 ] = 0;
  vertex[ 0 ][ 1 ] = 0;
  vertex[ 0 ][ 2 ] = r;
  
  for( unsigned int i=0; i<5; i++)
    {
    vertex[ i+1 ][ 0 ] =   b * cos( i * c );
    vertex[ i+1 ][ 1 ] =   b * sin( i * c );
    vertex[ i+1 ][ 2 ] =   a;

    vertex[ i+6 ][ 0 ] =  -b * cos( i * c );
    vertex[ i+6 ][ 1 ] =   b * sin( i * c );
    vertex[ i+6 ][ 2 ] =  -a;
    }

  vertex[ 11 ][ 0 ] =  0;
  vertex[ 11 ][ 1 ] =  0;
  vertex[ 11 ][ 2 ] = -r;
  
  // Draw triangles
  glBegin( GL_TRIANGLE_FAN );
    glNormal3d( vertex[  0 ][0], vertex[  0 ][1], vertex[  0 ][2] );
    glVertex3d( vertex[  0 ][0], vertex[  0 ][1], vertex[  0 ][2] );
    glNormal3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glVertex3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glNormal3d( vertex[  2 ][0], vertex[  2 ][1], vertex[  2 ][2] );
    glVertex3d( vertex[  2 ][0], vertex[  2 ][1], vertex[  2 ][2] );
    glNormal3d( vertex[  3 ][0], vertex[  3 ][1], vertex[  3 ][2] );
    glVertex3d( vertex[  3 ][0], vertex[  3 ][1], vertex[  3 ][2] );
    glNormal3d( vertex[  4 ][0], vertex[  4 ][1], vertex[  4 ][2] );
    glVertex3d( vertex[  4 ][0], vertex[  4 ][1], vertex[  4 ][2] );
    glNormal3d( vertex[  5 ][0], vertex[  5 ][1], vertex[  5 ][2] );
    glVertex3d( vertex[  5 ][0], vertex[  5 ][1], vertex[  5 ][2] );
    glNormal3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glVertex3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
  glEnd();

  glBegin( GL_TRIANGLE_FAN );
    glNormal3d( vertex[ 11 ][0], vertex[ 11 ][1], vertex[ 11 ][2] );
    glVertex3d( vertex[ 11 ][0], vertex[ 11 ][1], vertex[ 11 ][2] );
    glNormal3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
    glVertex3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
    glNormal3d( vertex[  9 ][0], vertex[  9 ][1], vertex[  9 ][2] );
    glVertex3d( vertex[  9 ][0], vertex[  9 ][1], vertex[  9 ][2] );
    glNormal3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
    glVertex3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
    glNormal3d( vertex[  7 ][0], vertex[  7 ][1], vertex[  7 ][2] );
    glVertex3d( vertex[  7 ][0], vertex[  7 ][1], vertex[  7 ][2] );
    glNormal3d( vertex[  6 ][0], vertex[  6 ][1], vertex[  6 ][2] );
    glVertex3d( vertex[  6 ][0], vertex[  6 ][1], vertex[  6 ][2] );
    glNormal3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
    glVertex3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
  glEnd();
  
  glBegin( GL_TRIANGLE_STRIP );
    glNormal3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glVertex3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glNormal3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
    glVertex3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
    glNormal3d( vertex[  2 ][0], vertex[  2 ][1], vertex[  2 ][2] );
    glVertex3d( vertex[  2 ][0], vertex[  2 ][1], vertex[  2 ][2] );
    glNormal3d( vertex[  7 ][0], vertex[  7 ][1], vertex[  7 ][2] );
    glVertex3d( vertex[  7 ][0], vertex[  7 ][1], vertex[  7 ][2] );
    glNormal3d( vertex[  3 ][0], vertex[  3 ][1], vertex[  3 ][2] );
    glVertex3d( vertex[  3 ][0], vertex[  3 ][1], vertex[  3 ][2] );
    glNormal3d( vertex[  6 ][0], vertex[  6 ][1], vertex[  6 ][2] );
    glVertex3d( vertex[  6 ][0], vertex[  6 ][1], vertex[  6 ][2] );
    glNormal3d( vertex[  4 ][0], vertex[  4 ][1], vertex[  4 ][2] );
    glVertex3d( vertex[  4 ][0], vertex[  4 ][1], vertex[  4 ][2] );
    glNormal3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
    glVertex3d( vertex[ 10 ][0], vertex[ 10 ][1], vertex[ 10 ][2] );
    glNormal3d( vertex[  5 ][0], vertex[  5 ][1], vertex[  5 ][2] );
    glVertex3d( vertex[  5 ][0], vertex[  5 ][1], vertex[  5 ][2] );
    glNormal3d( vertex[  9 ][0], vertex[  9 ][1], vertex[  9 ][2] );
    glVertex3d( vertex[  9 ][0], vertex[  9 ][1], vertex[  9 ][2] );
    glNormal3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glVertex3d( vertex[  1 ][0], vertex[  1 ][1], vertex[  1 ][2] );
    glNormal3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
    glVertex3d( vertex[  8 ][0], vertex[  8 ][1], vertex[  8 ][2] );
  glEnd();

}



 
}  // end namespace bio
