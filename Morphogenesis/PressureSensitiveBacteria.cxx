


#include "PressureSensitiveBacteria.h"
#include "CellularAggregate.h"




namespace bio {


// Class static variables
double    PressureSensitiveBacteria::PressureThresold =  5.0f;
double    PressureSensitiveBacteria::PressureRamp     =  5.0f;



/**
 *    Constructor
 */ 
PressureSensitiveBacteria
::PressureSensitiveBacteria()
{
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

  Cell::SphereShape = fltk::Sphere3D::New();
  Cell::SphereShape->SetNumberOfSlices( 12 );
  Cell::SphereShape->SetNumberOfStacks( 6 );

  PressureSensitiveBacteria * bacteria = new PressureSensitiveBacteria;
  bacteria->m_ParentIdentifier = 0;
  bacteria->m_SelfIdentifier = 1;
  bacteria->m_Generation = 0;
  
  bacteria->m_Genome = new Genome;
  
  return bacteria;
  
}





/**
 *   Compute the Gene Network
 *   This method update the level of expression of 
 *   all the genes in the cell's genome.
 *   see: http://www.ingeneue.org  for details
 */ 
void
PressureSensitiveBacteria
::ComputeGeneNetwork(void) 
{
  // Color the bacteria acording to pressure.
  // This is done by generating pigments under 
  // the influence of presure.
  const double red = Genome::Sigmoide( PressureThresold, 
                                       PressureRamp, 
                                       m_Pressure );

  if( m_Pressure > 1e-5 ) {
  std::cout << m_Pressure  << "  ->  " << red << std::endl;
  }
  m_Genome->SetExpressionLevel( RedGene,      red  );
  m_Genome->SetExpressionLevel( BlueGene,    0.0   );
  m_Genome->SetExpressionLevel( GreenGene, 1.0-red );

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
PressureSensitiveBacteria
::CheckPointDNAReplication(void) 
{
  const bool parent = Prokariote::CheckPointDNAReplication();
  if( !parent )
    {
    return parent;
    }
  bool pressureOk = true;
  if( m_Pressure > PressureThresold )
    {
    pressureOk = false;
    // this should rather be done by relating a
    // pressure sensitive protein with one of the 
    // cycline kinases involved in cell cycle control.
    std::cout << "Cell " << m_SelfIdentifier << " stopped replication ";
    std::cout << " pressure = " << m_Pressure << std::endl;
    }

  return pressureOk;

}





}  // end namespace bio
