
#include "CellularAggregate.h"
#include "FL/gl.h"
#include "itkCommand.h"


namespace bio {




CellularAggregate
::CellularAggregate()
{

  Cell::ColorType color;
  color.SetRed(0.9);
  color.SetGreen(0.0);
  color.SetBlue(0.0);
  Cell::SetDefaultColor( color );

}




CellularAggregate
::~CellularAggregate()
{
  this->KillAll();
}



unsigned int
CellularAggregate
::GetNumberOfCells(void) const
{
  return m_Cells->size();
}



void
CellularAggregate
::Draw(void) const
{
  
  glDisable( GL_LIGHTING );
  Cell::CellsListType::iterator cell = m_Cells->begin();
  Cell::CellsListType::iterator end  = m_Cells->end();
	while( cell != end )
    {
		(*cell)->Draw();
    cell++;
    }
}





void
CellularAggregate
::SetGrowthRadiusLimit( double value ) 
{
  Cell::SetGrowthRadiusLimit( value );
}




void
CellularAggregate
::SetGrowthRadiusIncrement( double value ) 
{
  Cell::SetGrowthRadiusIncrement( value );
}




void
CellularAggregate
::SetCells( Cell::CellsListType * cells ) 
{
  if( m_Cells == cells )
    {
    return;
    }

  this->KillAll();

  m_Cells = cells;

}






const Cell::CellsListType *
CellularAggregate
::GetCells( void ) const
{
  return m_Cells;
}




Cell::CellsListType *
CellularAggregate
::GetCells( void ) 
{
  return m_Cells;
}





void
CellularAggregate
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << "Cellular aggregate " << m_Cells << std::endl;

}






void
CellularAggregate
::AdvanceTimeStep(void)
{

  this->ComputeForces();
  
  Cell::CellsListType::iterator cell  =  m_Cells->begin();
  Cell::CellsListType::iterator end   = m_Cells->end();

  while( cell != end )
    {
    (*cell)->AdvanceTimeStep();
    ++cell;
    }
  
  this->InvokeEvent( itk::Command::ModifiedEvent );

}






void
CellularAggregate
::KillAll(void)
{
  if( !m_Cells )
    {
    return;
    }
  
    
  Cell::CellsListType::iterator cell = m_Cells->begin();
  Cell::CellsListType::iterator end  = m_Cells->end();

  while( cell != end )
    {
    delete (*cell);
    ++cell;
    }
}






void
CellularAggregate
::ClearForces(void)
{

  Cell::CellsListType::const_iterator cell = m_Cells->begin();
  Cell::CellsListType::const_iterator end  = m_Cells->end();
  
	while( cell != end )
    {
    (*cell)->ClearForce();
    ++cell;
    }
}







void
CellularAggregate
::ComputeForces(void)
{

  
  // Clear all the force accumulators
  this->ClearForces();


  Cell::CellsListType::const_iterator cell1 = m_Cells->begin();
  Cell::CellsListType::const_iterator end   = m_Cells->end();
  
  // compute forces 
	while( cell1 != end )
    {
    const Cell::PointType pA = (*cell1)->GetPosition();
    const double rA          = (*cell1)->GetRadius();

    Cell::CellsListType::const_iterator cell2 = cell1;
    cell2++;
    
    while( cell2 != end )
      {
      const Cell::PointType pB = (*cell2)->GetPosition();
      const double rB          = (*cell2)->GetRadius();
      Cell::VectorType relativePosition = pA - pB;

      const double distance = relativePosition.GetNorm();
      const double factor   = 1.0/distance;
      
      if( distance < rA + rB )
        {
        Cell::VectorType force = relativePosition * factor;
        (*cell1)->AddForce(  force );
        (*cell2)->AddForce( -force );
        }
      cell2++;
      }
    cell1++;
    }

  
  Cell::CellsListType::const_iterator bacteria = m_Cells->begin();
	while( bacteria != end )
	{
    (*bacteria)->UpdatePosition();
    bacteria++;
	}
}



}; // end namespace bio

