/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GradientEatingBacteria.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "GradientEatingBacteria.h"
#include "CellularAggregate.h"




namespace bio {


// Class static variables
double    GradientEatingBacteria::ChemoAttractantLowThreshold  =  5.0f;
double    GradientEatingBacteria::ChemoAttractantHighThreshold = 100.0f;

Cell::ColorType GradientEatingBacteria::WellNourishedColor;
Cell::ColorType GradientEatingBacteria::HopefullColor;
Cell::ColorType GradientEatingBacteria::StarvingColor;



/**
 *    Constructor
 */ 
GradientEatingBacteria
::GradientEatingBacteria()
{
  m_ScheduleApoptosis    = false;
  m_ChemoAttractantLevel = 200.0f;
}







/**
 *    Destructor
 */ 
GradientEatingBacteria
::~GradientEatingBacteria()
{
}





/**
 *    Cell Division
 */ 
Cell *
GradientEatingBacteria
::CreateNew()
{
  GradientEatingBacteria * bacteria = new GradientEatingBacteria;
  bacteria->m_ParentIdentifier = m_SelfIdentifier;
  return bacteria;
}


/**
 *    Create Egg (A cell with generation counter in zero)
 */ 
Cell *
GradientEatingBacteria
::CreateEgg()
{
  SetGrowthMaximumLatencyTime( 100 );
  SetDivisionMaximumLatencyTime( 100 );

  SetMaximumGenerationLimit( 40 );

  Cell::GrowthRadiusIncrement = 0.01;
  Cell::GrowthRadiusLimit     = 2.00;

  WellNourishedColor.Set(    0.0, 0.0, 1.0 );
  HopefullColor.Set(         0.0, 1.0, 0.0 );
  StarvingColor.Set(         1.0, 0.0, 0.0 );

  SetDefaultColor( HopefullColor );

  GradientEatingBacteria * bacteria = new GradientEatingBacteria;
  bacteria->m_ParentIdentifier = 0;
  bacteria->m_SelfIdentifier = 1;
  bacteria->m_Generation = 0;
  
  bacteria->m_Genome = new Genome;

  return bacteria;
  
}



/**
 *    Check point before DNA replication (S phase)
 *    This check point will control the entrance in the replication stage.
 *    It returns true when conditions required for replication are satisfied.
 */ 
bool
GradientEatingBacteria
::CheckPointDNAReplication(void) 
{
  bool super = SuperClass::CheckPointDNAReplication();

  if( !super )
    {
    return super;
    }

  bool here = false;

  if( !m_ScheduleApoptosis )
    {
    if( m_ChemoAttractantLevel > ChemoAttractantLowThreshold )
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
GradientEatingBacteria
::AddForce( const VectorType & force )
{
  SuperClass::AddForce( force );
}



/**
 *    Clear the Pressure cumulator at 
 *    the same time the Force cumulator
 *    is cleared.
 */ 
void
GradientEatingBacteria
::ClearForce( void )
{
  SuperClass::ClearForce();
}





void
GradientEatingBacteria
::Mitosis( void )
{
  SuperClass::Mitosis();
}
 



void
GradientEatingBacteria
::ReceptorsReading( void )
{
  SuperClass::ReceptorsReading();
  
  CellularAggregate::SubstrateType::PixelType substrate0 =
              m_Aggregate->GetSubstrateValue( m_SelfIdentifier, 0 );
  
  m_ChemoAttractantLevel = substrate0;

  if( m_ChemoAttractantLevel > ChemoAttractantHighThreshold )
    {
    m_Color = WellNourishedColor;
    }
  else if( m_ChemoAttractantLevel > ChemoAttractantLowThreshold )
    {
    m_Color = HopefullColor;
    }
  else
    {
    m_Color = StarvingColor;
    }
}
 



}  // end namespace bio
