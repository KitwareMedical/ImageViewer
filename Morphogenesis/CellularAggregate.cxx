
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
  m_Mesh->SetCells( VoronoiRegionsContainer::New() );

  m_Iteration = 0;
  m_ClosestPointComputationInterval = 500;

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
  
  if( Cell::Dimension == 2 ) 
    {
    glDisable( GL_LIGHTING );
    }

  if( Cell::Dimension == 3 ) 
    {
    glEnable( GL_LIGHTING );
    }

  PointType position;

  CellsIterator cellIt = m_Mesh->GetPointData()->Begin();
  CellsIterator end    = m_Mesh->GetPointData()->End();

	while( cellIt != end )
    {
    Cell * cell = cellIt.Value();
    const IdentifierType id = cell->GetSelfIdentifier();
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
  
  IdentifierType id = cell->GetSelfIdentifier();

  MeshType::CellPointer   region;
  bool regionExist = m_Mesh->GetCell( id, &region );
  if( regionExist )
    {
    VoronoiRegionPointer realRegion( 
              dynamic_cast < VoronoiRegionType * >( region.GetPointer() ) );

    VoronoiRegionType::PointIdIterator neighbor = realRegion->PointIdsBegin();
    VoronoiRegionType::PointIdIterator end      = realRegion->PointIdsEnd();
    while( neighbor != end )
      {
      neighbor++;
      }
    
    // update voronoi connections
    }
 
  m_Mesh->GetCells()->DeleteIndex( id );
  m_Mesh->GetPoints()->DeleteIndex( id );
  m_Mesh->GetPointData()->DeleteIndex( id );

 
  delete cell;

}




CellularAggregate::VoronoiRegionPointer 
CellularAggregate
::GetVoronoi( IdentifierType cellId ) const
{

  MeshType::CellPointer cellPointer;

  bool voronoiExists = m_Mesh->GetCell( cellId, &cellPointer );
  if( !voronoiExists ) 
    {
    itk::ExceptionObject exception;
    exception.SetDescription( "voronoi region does not exist in the container" );
    exception.SetLocation( "GetVoronoi( IdentifierType");
    throw exception;
    }
  return dynamic_cast < VoronoiRegionType * >( cellPointer.GetPointer() );

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
::Add( Cell * cellA, Cell * cellB, const VectorType & perturbation )
{

  this->Add( cellA,  perturbation );
  this->Add( cellB, -perturbation );

  const IdentifierType cellAId = cellA->GetSelfIdentifier();
  const IdentifierType cellBId = cellB->GetSelfIdentifier();

  VoronoiRegionPointer voronoiPointerA = GetVoronoi( cellAId );
  VoronoiRegionPointer voronoiPointerB = GetVoronoi( cellBId );

  voronoiPointerA->AddPointId( cellB->GetSelfIdentifier() );
  voronoiPointerB->AddPointId( cellA->GetSelfIdentifier() );

}
 




void
CellularAggregate
::Add( Cell * cell, const VectorType & perturbation )
{

  IdentifierType  newcellId       = cell->GetSelfIdentifier();
  IdentifierType  newcellparentId = cell->GetParentIdentifier();
    
  PointType position;
  VoronoiRegionPointer selfVoronoi;

  // If the cell does not have a parent 
  // from which receive a position
  if( !newcellparentId )
    {
    position.Fill( 0.0 );
    selfVoronoi = VoronoiRegionType::New();
    }
  else
    {

    bool parentPositionExist = m_Mesh->GetPoint( newcellparentId, &position );
    if( !parentPositionExist ) 
      {
        itk::ExceptionObject exception;
        exception.SetDescription( "Parent cell does not exist in the container" );
        exception.SetLocation( "CellularAggregate::Add( cell * )" );
        throw exception;
      }

    VoronoiRegionPointer parentVoronoi = GetVoronoi( newcellparentId );

    selfVoronoi = dynamic_cast< VoronoiRegionType *>( 
                                    parentVoronoi->MakeCopy().GetPointer() );

    }

  position += perturbation;

  m_Mesh->SetCell(      newcellId, selfVoronoi.GetPointer() );
  m_Mesh->SetPoint(     newcellId, position   );
  m_Mesh->SetPointData( newcellId, cell       );
 
  cell->SetCellularAggregate( this );

  // Add this new cell as neighbor to cells in its neighborhood

  VoronoiRegionType::PointIdIterator neighbor = selfVoronoi->PointIdsBegin();
  VoronoiRegionType::PointIdIterator end      = selfVoronoi->PointIdsEnd();

  while( neighbor != end )
    {
    const IdentifierType neighborId = (*neighbor);
    MeshType::CellPointer cellPointer;
    bool neighborVoronoiExist = m_Mesh->GetCell( neighborId, &cellPointer );
    if( neighborVoronoiExist ) 
      {
      VoronoiRegionPointer neighborVoronoi( 
         dynamic_cast < VoronoiRegionType * >( cellPointer.GetPointer() ) );
      neighborVoronoi->AddPointId( newcellId );
      }
    neighbor++;
    }

}



void
CellularAggregate
::AdvanceTimeStep(void)
{
  
  if( m_Iteration % m_ClosestPointComputationInterval == 0 ) 
  {
    this->ComputeClosestPoints();
  }

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

  m_Iteration++;

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
::Show(void)
{
}



void
CellularAggregate
::Hide(void)
{
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
    IdentifierType cellId = cell->GetSelfIdentifier();
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

    const IdentifierType cell1Id = cell1It.Index();

    Cell     * cell1      =  cell1It.Value();

    PointType  position1;
    m_Mesh->GetPoint( cell1Id, &position1 );

    const double rA          = cell1->GetRadius();
 
    VoronoiRegionPointer voronoiRegion = GetVoronoi( cell1Id );
              
    VoronoiRegionType::PointIdIterator neighbor = voronoiRegion->PointIdsBegin();
    VoronoiRegionType::PointIdIterator end      = voronoiRegion->PointIdsEnd();

    while( neighbor != end )
      {

      const IdentifierType cell2Id = (*neighbor);  

      Cell     * cell2;
      PointType  position2;
      
      if( !m_Mesh->GetPoint(      cell2Id, &position2 ) )
        {
        neighbor++;  // if the neigbor has been removed, skip it
        continue;
        }
      m_Mesh->GetPointData(  cell2Id, &cell2     );
      
      const double rB      = cell2->GetRadius();

      Cell::VectorType relativePosition = position1 - position2;

      const double distance = relativePosition.GetNorm();
      const double factor   = 1.0/distance;
      
      if( distance < rA + rB )
        {
        Cell::VectorType force = relativePosition * factor;
        cell1->AddForce(  force );
        cell2->AddForce( -force );
        }
      
      neighbor++;
      }
    
    cell1It++;
    }

 }




void
CellularAggregate
::ComputeClosestPoints(void)
{

  PointsConstIterator   beginPoints   = m_Mesh->GetPoints()->Begin();
  PointsConstIterator   endPoints     = m_Mesh->GetPoints()->End();

  PointsConstIterator   point1It = beginPoints;

	while( point1It != endPoints )
    {

    PointType  position1 = point1It.Value();

    PointsConstIterator   point2It   = beginPoints;

    Cell  * cell1;

    IdentifierType cell1Id = point1It.Index();
    m_Mesh->GetPointData( cell1Id, &cell1 );
    const double radius  = cell1->GetRadius();
    const double limitDistance = radius * 4.0;
 
    VoronoiRegionPointer voronoiRegion = GetVoronoi( cell1Id ); 

    voronoiRegion->ClearPoints();

    while( point2It != endPoints )
      {

      if( point2It == point1It )
        {
        point2It++;
        continue;
        }

      PointType  position2 = point2It.Value();

      Cell::VectorType relativePosition = position1 - position2;

      const double distance = relativePosition.GetNorm();
      if( distance < limitDistance )
        {
        voronoiRegion->AddPointId( point2It.Index() );
        }
      point2It++;
      }

    point1It++;
    }

 }





void
CellularAggregate
::DumpContent( std::ostream & os ) const
{
  CellsConstIterator   beginCell   = m_Mesh->GetPointData()->Begin();
  CellsConstIterator   endCell     = m_Mesh->GetPointData()->End();
  
  CellsConstIterator cell1It = beginCell;

  os << "Cell Identifiers " << std::endl;
  while( cell1It != endCell )
    {
    os << cell1It.Index() << " == ";
    os << cell1It.Value()->GetSelfIdentifier() << std::endl;
    cell1It++;
    }

  os << std::endl << "Points " << std::endl;
  PointsConstIterator  pointIt     = m_Mesh->GetPoints()->Begin();
  PointsConstIterator  endPoint    = m_Mesh->GetPoints()->End();

  while( pointIt != endPoint )
    {
    os << pointIt.Index() << "  ";
    os << pointIt.Value() << std::endl; 
    pointIt++;
    }


  os << std::endl << "Neighborhoods " << std::endl;
  cell1It = beginCell;
  while( cell1It != endCell )
    {
    VoronoiRegionPointer voronoiRegion = GetVoronoi( cell1It.Index() );
    VoronoiRegionType::PointIdIterator neighbor = voronoiRegion->PointIdsBegin();
    VoronoiRegionType::PointIdIterator end      = voronoiRegion->PointIdsEnd();

    while( neighbor != end )
      {
      os << (*neighbor) << "   ";
      neighbor++;
      }
    os << std::endl;

    cell1It++;
    }
      
}



}; // end namespace bio

