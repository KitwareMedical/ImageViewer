/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Bacteria.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "Bacteria.h"
#include "CellularAggregate.h"




namespace bio {



/**
 *    Constructor
 */ 
Bacteria
::Bacteria()
{
}







/**
 *    Destructor
 */ 
Bacteria
::~Bacteria()
{
}





/**
 *    Cell Division
 */ 
Cell *
Bacteria
::CreateNew()
{
  Bacteria * bacteria = new Bacteria;
  bacteria->m_ParentIdentifier = m_SelfIdentifier;
  return bacteria;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
Bacteria
::CreateEgg()
{
  SetGrowthMaximumLatencyTime( 100 );
  SetDivisionMaximumLatencyTime( 100 );
  Cell::GrowthRadiusIncrement = 0.01;
  Cell::GrowthRadiusLimit     = 2.00;
  
  Cell::SphereShape = fltk::Sphere3D::New();
  Cell::SphereShape->SetNumberOfSlices( 12 );
  Cell::SphereShape->SetNumberOfStacks( 6 );

  Bacteria * bacteria = new Bacteria;
  bacteria->m_ParentIdentifier = 0;
  bacteria->m_SelfIdentifier = 1;
  bacteria->m_Generation = 0;
  bacteria->m_Genome = new Genome;

  bacteria->ComputeGeneNetwork();
  bacteria->SecreteProducts();

  return bacteria;
}


/**
 *   Compute the Gene Network
 *   This method update the level of expression of 
 *   all the genes in the cell's genome.
 *   see: http://www.ingeneue.org  for details
 */ 
void
Bacteria
::ComputeGeneNetwork(void) 
{
  m_Genome->SetExpressionLevel( Cell::RedGene,   1.0 );
  m_Genome->SetExpressionLevel( Cell::GreenGene, 1.0 );
  m_Genome->SetExpressionLevel( Cell::BlueGene,  1.0 );
}





}  // end namespace bio
