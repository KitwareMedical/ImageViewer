/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Eukariote.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "Eukariote.h"
#include "CellularAggregate.h"
#include "vnl/vnl_sample.h"





namespace bio {


unsigned long Eukariote::DivisionMaximumLatencyTime  = 50; 


/**
 *    Constructor
 */ 
Eukariote
::Eukariote()
{
  // add a random time before starting to grow
  m_DivisionLatencyTime = static_cast<unsigned long>(
      vnl_sample_uniform( 0, this->GetDivisionMaximumLatencyTime() ) );
}







/**
 *    Destructor
 */ 
Eukariote
::~Eukariote()
{
}




/**
 *    Growth
 */ 
void
Eukariote
::Grow(void) 
{
  // This introduces a delay before 
  // starting the growth process
  SuperClass::Grow();
}



/**
 *    Cell Division
 */ 
Cell *
Eukariote
::CreateNew()
{
  Eukariote * eukariote = new Eukariote;
  eukariote->m_ParentIdentifier = m_SelfIdentifier;
  return eukariote;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
Eukariote
::CreateEgg()
{
  Eukariote * eukariote = new Eukariote;
  eukariote->m_ParentIdentifier = 0;
  eukariote->m_SelfIdentifier = 1;
  eukariote->m_Generation = 0;
  
  eukariote->m_Genome = new Genome;
  
  return eukariote;
  
}



/**
 *    Cell Division
 *    This is a terminal operation. This cell will
 *    be destroyed in the process, and two new cell
 *    will be created.
 */ 
void
Eukariote
::Mitosis(void) 
{
  SuperClass::Mitosis();

  Eukariote * siblingA = dynamic_cast<Eukariote*>( this->CreateNew() );
  Eukariote * siblingB = dynamic_cast<Eukariote*>( this->CreateNew() );

  siblingA->m_Radius   = m_Radius / sqrt( 2.0f );
  siblingB->m_Radius   = m_Radius / sqrt( 2.0f );

  siblingA->m_Generation = m_Generation + 1;
  siblingB->m_Generation = m_Generation + 1;

  // Pass the genome to each daughter cell
  siblingA->m_Genome = m_Genome;
  siblingB->m_Genome = m_GenomeCopy;

  // Create a perturbation for separating the daugther cells
  Cell::VectorType perturbationVector;
  for(unsigned int d=0; d<PointDimension; d++)
    {
    perturbationVector[d] = 
           vnl_sample_uniform( -1.0f, 1.0f ); 
    }

  const double perturbationLength = m_Radius * 0.75;

  const double norm = perturbationVector.GetNorm();
  if( vnl_math_abs( norm ) > 1e-10 ) 
    {
    perturbationVector *= perturbationLength / norm;
    }
  else
    {
    // this event should rarely happen... very rarely
    std::cout << "Eukariote:: unlikely event happend" << std::endl;
    perturbationVector[0] = perturbationLength;
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
Eukariote
::Apoptosis(void) 
{

  // Because this is a terminal method,
  // the superclass behavior is the last
  // to be called
  SuperClass::Apoptosis();
}




/**
 *    Check point before initiating DNA replication (S phase)
 *    This check point will control the entrance in the replication stage.
 *    It returns true when conditions required for replication are satisfied
 */ 
bool
Eukariote
::CheckPointDNAReplication(void) 
{
  const bool super = SuperClass::CheckPointDNAReplication();

  if( !super )
    {
    return super;
    }

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
  return ( super && here );

}






/**
 *    Set Division Latency Time
 */ 
void
Eukariote
::SetDivisionMaximumLatencyTime( unsigned long latency )
{
  Eukariote::DivisionMaximumLatencyTime = latency;
}




/**
 *    Get Division Latency Time
 */ 
unsigned long 
Eukariote
::GetDivisionMaximumLatencyTime( void )
{
  return Eukariote::DivisionMaximumLatencyTime;
}





/**
 *   Compute the Gene Network
 *   This method update the level of expression of 
 *   all the genes in the cell's genome.
 *   see: http://www.ingeneue.org  for details
 */ 
void
Eukariote
::ComputeGeneNetwork(void) 
{
  // force the cell cycle : this should be replaced
  // by computations from Teleomerasa counting and
  // cell radius
  m_Genome->SetExpressionLevel( Cdk2E, 0.9 ); 
}




}  // end namespace bio
