
#include "CellularAggregate.h"
#include "FL/gl.h"
#include "CommandEvents.h"


namespace bio {




CellularAggregate
::CellularAggregate()
{

  Cell::ColorType color;
  color.SetRed(0.9);
  color.SetGreen(0.0);
  color.SetBlue(0.0);
  Cell::SetDefaultColor( color );

  m_Mesh = MeshType::New();

  m_Mesh->SetPoints( PointsContainer::New() );
  m_Mesh->SetPointData( PointDataContainer::New() );

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
  return m_Mesh->GetPointData()->Size();
}



void
CellularAggregate
::Draw(void) const
{
  
  glDisable( GL_LIGHTING );
  PointType position;

  CellsIterator cellIt = m_Mesh->GetPointData()->Begin();
  CellsIterator end    = m_Mesh->GetPointData()->End();

	while( cellIt != end )
    {
    Cell * cell = cellIt.Value();
    const Cell::IdentifierType id = cell->GetSelfIdentifier();
    m_Mesh->GetPoint( id, &position );
		cell->Draw( position );
    cellIt++;
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
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << "Cellular aggregate " << m_Mesh << std::endl;

}



void
CellularAggregate
::Remove( Cell * cell )
{
  if( !cell )
  {
    itk::ExceptionObject exception;
    exception.SetDescription("Trying to remove a null pointer to cell");
    exception.SetLocation("CellularAggregate::Remove(Cell*)");
    throw exception;
  }
  
  Cell::IdentifierType id = cell->GetSelfIdentifier();

  m_Mesh->GetPoints()->DeleteIndex( id );
  m_Mesh->GetPointData()->DeleteIndex( id );

  delete cell;

}





void
CellularAggregate
::Add( Cell * cell )
{
  VectorType perturbation;
  perturbation.Fill( 0.0 );
  Add( cell, perturbation );
}




void
CellularAggregate
::Add( Cell * cell, const VectorType & perturbation )
{

  CellIdentifierType  newcellId       = cell->GetSelfIdentifier();
  CellIdentifierType  newcellparentId = cell->GetParentIdentifier();
    
  PointType position;

  // If the cell does not have a parent 
  // from which receive a position
  if( !newcellparentId )
    {
    position.Fill( 0.0 );
    }
  else
    {
    bool exist = m_Mesh->GetPoint( newcellparentId, &position );
    if( !exist ) 
      {
        itk::ExceptionObject exception;
        exception.SetDescription( "Parent cell does not exist in the container" );
        exception.SetLocation( "CellularAggregate::Add( cell * )" );
        throw exception;
      }
    }

  position += perturbation;

  m_Mesh->SetPoint(       newcellId, position );
  m_Mesh->SetPointData(   newcellId, cell     );
 
  m_Mesh->GetPoint( newcellId, &position );
    
  cell->SetCellularAggregate( this );

}



void
CellularAggregate
::AdvanceTimeStep(void)
{

  this->ComputeForces();
  this->UpdatePositions();
  
  CellsIterator cell = m_Mesh->GetPointData()->Begin();
  CellsIterator end  = m_Mesh->GetPointData()->End();

  while( cell != end )
    {
    cell.Value()->AdvanceTimeStep();
    ++cell;
    }
  

  this->InvokeEvent( TimeStepEvent );


}






void
CellularAggregate
::KillAll(void)
{
    
  if( !m_Mesh  )
    {
    return;
    }
  
  CellsIterator cell = m_Mesh->GetPointData()->Begin();
  CellsIterator end  = m_Mesh->GetPointData()->End();

  while( cell != end )
    {
    delete (cell.Value());
    ++cell;
    }
}






void
CellularAggregate
::ClearForces(void)
{

  CellsIterator cell = m_Mesh->GetPointData()->Begin();
  CellsIterator end  = m_Mesh->GetPointData()->End();

	while( cell != end )
    {
    cell.Value()->ClearForce();
    ++cell;
    }
}





void
CellularAggregate
::UpdatePositions(void)
{
 
  CellsConstIterator cellIt = m_Mesh->GetPointData()->Begin();
  CellsConstIterator end    = m_Mesh->GetPointData()->End();

  PointType position;

	while( cellIt != end )
    {
    Cell * cell = cellIt.Value();
    Cell::IdentifierType cellId = cell->GetSelfIdentifier();
    m_Mesh->GetPoint( cellId, &position );
    position += cell->GetForce() / 2.0;
		m_Mesh->SetPoint( cellId, position );
    cellIt++;
    }

}
  


void
CellularAggregate
::ComputeForces(void)
{

  
  // Clear all the force accumulators
  this->ClearForces();

  CellsConstIterator   cell1It     = m_Mesh->GetPointData()->Begin();
  CellsConstIterator   end         = m_Mesh->GetPointData()->End();

  // compute forces 
	while( cell1It != end )
    {

    Cell     * cell1      =  cell1It.Value();
    PointType  position1;
    m_Mesh->GetPoint( cell1->GetSelfIdentifier(), &position1 );
    const double rA          = cell1->GetRadius();

    CellsConstIterator   cell2It   = cell1It;
    cell2It++;
    
    while( cell2It != end )
      {

      Cell     * cell2      =  cell2It.Value();
      PointType  position2;
      m_Mesh->GetPoint( cell2->GetSelfIdentifier(), &position2 );
      const double rB          = cell2->GetRadius();

      Cell::VectorType relativePosition = position1 - position2;

      const double distance = relativePosition.GetNorm();
      const double factor   = 1.0/distance;
      
      if( distance < rA + rB )
        {
        Cell::VectorType force = relativePosition * factor;
        cell1->AddForce(  force );
        cell2->AddForce( -force );
        }
      cell2It++;
      }
    cell1It++;
    }

 }


void
CellularAggregate
::DumpContent(void) const
{
  CellsConstIterator   cell1It     = m_Mesh->GetPointData()->Begin();
  CellsConstIterator   endCell     = m_Mesh->GetPointData()->End();
  
  while( cell1It != endCell )
    {
    std::cout << cell1It.Index() << " == ";
    std::cout << cell1It.Value()->GetSelfIdentifier() << std::endl;
    cell1It++;
    }

  PointsConstIterator  pointIt     = m_Mesh->GetPoints()->Begin();
  PointsConstIterator  endPoint    = m_Mesh->GetPoints()->End();

  while( pointIt != endPoint )
    {
    std::cout << pointIt.Index() << "  ";
    std::cout << pointIt.Value() << std::endl; 
    pointIt++;
    }


}



}; // end namespace bio

