#include "BacterialColony.h"


namespace bio {




BacterialColony
::BacterialColony()
{
  // Create the first individual
  Bacteria * bacteria = new Bacteria;
  m_Cells = bacteria->GetAggregate();
}




BacterialColony
::~BacterialColony()
{
  this->KillAll();
}



unsigned int
BacterialColony
::GetNumberOfCells(void) const
{
  return m_Cells->size();
}



void
BacterialColony
::Draw(void) const
{
  
  Cell::CellsListType::iterator bacteria = m_Cells->begin();
  Cell::CellsListType::iterator end      = m_Cells->end();
	while( bacteria != end )
    {
		(*bacteria)->Draw();
    bacteria++;
    }
}





void
BacterialColony
::SetGrowthRadiusLimit( double value ) 
{
  Cell::SetGrowthRadiusLimit( value );
}




void
BacterialColony
::SetGrowthRadiusIncrement( double value ) 
{
  Cell::SetGrowthRadiusIncrement( value );
}




Cell::CellsListType *
BacterialColony
::GetCellsAggregate( void ) 
{
  return m_Cells;
}


void
BacterialColony
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << "Cellular aggregate " << m_Cells << std::endl;

}



void
BacterialColony
::KillAll(void)
{
  Cell::CellsListType::iterator bacteria = m_Cells->begin();
  Cell::CellsListType::iterator end      = m_Cells->end();

  while( bacteria != end )
    {
    delete (*bacteria);
    ++bacteria;
    }
}



void
BacterialColony
::ComputeForces(void)
{

  Cell::CellsListType::const_iterator bacteria1 = m_Cells->begin();
  Cell::CellsListType::const_iterator end       = m_Cells->end();
  
	while( bacteria1 != end )
    {
    const Cell::PointType pA = (*bacteria1)->GetPosition();
    const double rA          = (*bacteria1)->GetRadius();
    Cell::CellsListType::const_iterator bacteria2 = bacteria1;
    bacteria1++;
    
    while( bacteria2 != end )
      {
      const Cell::PointType pB = (*bacteria2)->GetPosition();
      const double rB          = (*bacteria2)->GetRadius();
      Cell::VectorType relativePosition = pA - pB;

      const double distance = relativePosition.GetNorm();
      const double factor   = 1.0/distance;
      
      if( distance < rA + rB )
        {
        Cell::VectorType force = relativePosition * factor;
        (*bacteria2)->AddForce(  force );
        (*bacteria1)->AddForce( -force );
        }
      bacteria2++;
      }
    bacteria1++;
    }

  
  Cell::CellsListType::const_iterator bacteria = m_Cells->begin();
	while( bacteria != end )
	{
    (*bacteria)->UpdatePosition();
    bacteria++;
	}
}

}; // end namespace bio

