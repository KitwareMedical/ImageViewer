/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BilayerOrganism.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "BilayerOrganism.h"
#include "CellularAggregate.h"




namespace bio {



/**
 *    Constructor
 */ 
BilayerOrganism
::BilayerOrganism()
{
}





/**
 *    Destructor
 */ 
BilayerOrganism
::~BilayerOrganism()
{
}


/**
 *    Cell Division
 */ 
Cell *
BilayerOrganism
::CreateNew()
{
  BilayerOrganism * eukariote = new BilayerOrganism;
  eukariote->m_ParentIdentifier = m_SelfIdentifier;
  return eukariote;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
BilayerOrganism
::CreateEgg()
{
  BilayerOrganism * eukariote = new BilayerOrganism;
  eukariote->m_ParentIdentifier = 0;
  eukariote->m_SelfIdentifier = 1;
  eukariote->m_Generation = 0;
  
  eukariote->m_Genome = new Genome;
  
  return eukariote;
  
}


/**
 *   Compute the Gene Network
 *   This method update the level of expression of 
 *   all the genes in the cell's genome.
 *   see: http://www.ingeneue.org  for details
 */ 
void
BilayerOrganism
::ComputeGeneNetwork(void) 
{
  // force the cell cycle : this should be replaced
  // by computations from Teleomerasa counting and
  // cell radius
  m_Genome->SetExpressionLevel( Cdk2E, 0.9 ); 
}




}  // end namespace bio
