#include "Tissue.h"


namespace bio {


Tissue
::Tissue()
{
}

Tissue
::~Tissue()
{
}


void
Tissue
::SetCells( CellsListType * cells ) 
{
  m_Cells = cells;
}



unsigned int
Tissue
::GetNumberOfCells(void) const
{
  return m_Cells->size();
}


void
Tissue
::Draw(void) const
{
	CellsListType::const_iterator cell = m_Cells->begin();
	while( cell != m_Cells->end() )
	{
		(*cell)->Draw();
    cell++;
	}
}


void
Tissue
::SetGrowthRadiusLimit( double value ) 
{
  Cell::SetGrowthRadiusLimit( value );
}


void
Tissue
::SetGrowthRadiusIncrement( double value ) 
{
  Cell::SetGrowthRadiusIncrement( value );
}



void
Tissue
::Grow(void)
{
  CellsListType::const_iterator cell = m_Cells->begin();
	while( cell != m_Cells->end() )
	{
		(*cell)->Grow();
    cell++;
	}
}


void
Tissue
::Restart(void)
{

  CellsListType::iterator cell = m_Cells->begin();
  cell++;
  m_Cells->erase( cell, m_Cells->end() );
}


Tissue::VectorType
Tissue
::WellPotentialGradient(const VectorType &  relativePosition) const
{
  double distance      = relativePosition.GetNorm();
  VectorType direction = relativePosition / distance;
  double factor        = 1/(distance*distance);
  factor *= 1e-5;

  return direction * factor;
}


void
Tissue
::Spread(void)
{

  CellsListType::const_iterator cellA = m_Cells->begin();
  CellsListType::const_iterator end   = m_Cells->end();
  
	while( cellA != end )
	{
    const Cell::PointType pA = (*cellA)->GetPosition();
    CellsListType::const_iterator cellB = cellA;
    cellB++;
    while( cellB != end )
    {
      const Cell::PointType pB = (*cellB)->GetPosition();
      Cell::VectorType relativePosition = pA - pB;
      const double distance = relativePosition.GetNorm();
      
      const Cell::VectorType force = WellPotentialGradient( relativePosition );

      (*cellB)->AddForce(  force );
      (*cellA)->AddForce( -force );

      cellB++;
    }
    cellA++;
	}

  
  CellsListType::const_iterator cell   = m_Cells->begin();
	while( cell != end )
	{
    (*cell)->UpdatePosition();
    cell++;
	}
}

}; // end namespace bio

