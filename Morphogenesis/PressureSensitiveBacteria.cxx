/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    PressureSensitiveBacteria.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "PressureSensitiveBacteria.h"
#include "CellularAggregate.h"




namespace bio {




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
  const double red = Genome::Sigmoide( 5.0, 1.0, m_Pressure );

  m_Genome->SetExpressionLevel( RedGene,       red );
  m_Genome->SetExpressionLevel( BlueGene,  1.0-red );
  m_Genome->SetExpressionLevel( GreenGene,     0.0 );

  const double cdk2E = Genome::Sigmoide( 2.0, -0.5, m_Pressure );
  m_Genome->SetExpressionLevel( Cdk2E, cdk2E );

  const double caspase = Genome::Sigmoide( 3.0, 90.0, m_Pressure );
  m_Genome->SetExpressionLevel( Caspase, caspase );

}





}  // end namespace bio
