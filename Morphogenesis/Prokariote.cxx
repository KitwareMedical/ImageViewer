/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Prokariote.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "Prokariote.h"
#include "CellularAggregate.h"
#include "vnl/vnl_sample.h"




namespace bio {


unsigned long Prokariote::DivisionMaximumLatencyTime  = 50; 


/**
 *    Constructor
 */ 
Prokariote
::Prokariote()
{
  // add a random time before starting to grow
  m_DivisionLatencyTime = 
      vnl_sample_uniform( 0, this->GetDivisionMaximumLatencyTime() );
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
  
  prokariote->m_Genome = new Genome;
  
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
::Mitosis(void) 
{
  SuperClass::Mitosis();

  Prokariote * siblingA = dynamic_cast<Prokariote*>( this->CreateNew() );
  Prokariote * siblingB = dynamic_cast<Prokariote*>( this->CreateNew() );

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
    std::cout << "Prokariote:: unlikely event happend" << std::endl;
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
Prokariote
::Apoptosis(void) 
{

  // Because this is a terminal method,
  // the superclass behavior is the last
  // to be called
  SuperClass::Apoptosis();
}



/**
 *    Check point before DNA replication (S phase)
 *    This check point will control the entrance in the replication stage.
 *    It returns true when conditions required for replication are satisfied.
 */ 
bool
Prokariote
::CheckPointDNAReplication(void) 
{
  bool super = SuperClass::CheckPointDNAReplication();

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
 *    Set Division Latency Time
 */ 
void
Prokariote
::SetDivisionMaximumLatencyTime( unsigned long latency )
{
  Prokariote::DivisionMaximumLatencyTime = latency;
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




}  // end namespace bio
