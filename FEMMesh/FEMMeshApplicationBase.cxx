/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshApplicationBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "FEMMeshApplicationBase.h"

#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkSphereSource.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include "vtkAxes.h"
#include "vtkTubeFilter.h"

#include "vtkItkCellMultiVisitor.h"
#include "vtkItkElementMultiVisitor.h"

#include "itkNumericTraits.h"
#include "itkCellInterfaceVisitor.h"

#include "itkFEMElementBar2D.h"


// Headers for the Heat Conduction problem
#include "itkFEMElementTriangleHeat.h"




// Headers for the Area Computing problem
#include "itkFEMElementTriangleAreaVisitor.h"
#include "itkFEMElementQuadrilateralAreaVisitor.h"






// Constructor
FEMMeshApplicationBase
::FEMMeshApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_Renderer->SetBackground( 1.0, 1.0, 1.0 ); 
  m_Renderer->GetActiveCamera()->Zoom( 1.0 ); 
  m_Renderer->GetActiveCamera()->SetPosition(0.0, 0.0, 30.0 ); 

  m_HeatSolver = HeatSolverType::New();

}




FEMMeshApplicationBase
::~FEMMeshApplicationBase()
{
  m_RenderWindow->Delete();
  m_Renderer->Delete();
}






void
FEMMeshApplicationBase
::DisplayAxes(void) const
{
    
  vtkAxes *axes = vtkAxes::New();
    axes->SetOrigin( 0.0, 0.0, 0.0 );
    axes->SetScaleFactor( 1.0 );

  vtkTubeFilter *axesTubes = vtkTubeFilter::New();
    axesTubes->SetInput(axes->GetOutput());
    axesTubes->SetRadius(axes->GetScaleFactor()/100.0);
    axesTubes->SetNumberOfSides(6);

  vtkPolyDataMapper *axesMapper = vtkPolyDataMapper::New();
    axesMapper->SetInput(axesTubes->GetOutput());

  vtkActor *axesActor = vtkActor::New();
    axesActor->SetMapper(axesMapper);

  m_Renderer->AddActor( axesActor );

  axesActor->Delete();
  axesMapper->Delete();
  axesTubes->Delete();
  axes->Delete();

}







void
FEMMeshApplicationBase
::CreateFEMMesh(void)
{

  // Create the FEMMesh that will hold both the Geometric 
  // and the Physical representation of the problem 
  HeatMeshType::Pointer  heatMesh = HeatMeshType::New();
  m_HeatSolver->SetMesh( heatMesh );

  typedef itk::fem::FEMElementTriangle< HeatMeshType >        TriangleElementType;
  typedef itk::fem::FEMElementQuadrilateral< HeatMeshType >   QuadrilateralElementType;
  
  const unsigned int nx = 10;
  const unsigned int ny = 15;

  const double dx = 1.5;
  const double dy = 1.0;

  // Create the points
  typedef HeatMeshType::PointType         HeatPointType;
  typedef HeatMeshType::PointsContainer   HeatPointsContainer;
  HeatPointsContainer::Pointer points = HeatPointsContainer::New();
  points->Reserve( ( nx + 1 ) * ( ny + 1 ) );

  typedef HeatMeshType::PointIdentifier   PointIdentifierType;
  PointIdentifierType pointId = itk::NumericTraits< PointIdentifierType >::Zero;

  typedef HeatMeshType::PointType::ValueType CoordinateRepresentationType;

  CoordinateRepresentationType centerx = nx * dx / 2.0f;
  CoordinateRepresentationType centery = ny * dy / 2.0f;

  // Create the Points
  for(unsigned int y=0; y<=ny; y++) 
    {
    HeatPointType point;
    point[1] = y * dy - centery;
    for(unsigned int x=0; x<=nx; x++) 
      {
      point[0] = x * dx - centerx;
      points->SetElement( pointId++, point );
      }
    }

  heatMesh->SetPoints( points.GetPointer() );

  // Create the ElementsContainer
  typedef HeatMeshType::ElementsContainer    HeatElementsContainer;
  HeatElementsContainer::Pointer elements  = HeatElementsContainer::New();

  typedef HeatMeshType::ElementIdentifier  ElementIdentifierType;
  ElementIdentifierType elementId = 
                  itk::NumericTraits< ElementIdentifierType >::Zero;

  const unsigned int nyt = 10;          // number of rows with triangles
  const unsigned int nyq = ny - nyt;    // number of rows with quadrilaterals
  const unsigned int numberOfTriangularElements    = 2 * nx * nyt;
  const unsigned int numberOfQuadrilateralElements =     nx * nyq;

  elements->Reserve(  numberOfTriangularElements    + 
                      numberOfQuadrilateralElements   );

  // Create the Triangular Elements
  for(unsigned int y=0; y<nyt; y++) 
    {
    for(unsigned int x=0; x<nx; x++) 
      {
      const unsigned int pointsPerRow = nx+1;
      const PointIdentifierType point0 =   x   + (   y   * pointsPerRow );
      const PointIdentifierType point1 = (x+1) + (   y   * pointsPerRow );
      const PointIdentifierType point2 =   x   + ( (y+1) * pointsPerRow );
      const PointIdentifierType point3 = (x+1) + ( (y+1) * pointsPerRow );

      TriangleElementType::Pointer elementA = TriangleElementType::New();
      TriangleElementType::BaseCellType & cellA = elementA->GetCell();
      cellA.SetPointId( 0, point0 );
      cellA.SetPointId( 1, point1 );
      cellA.SetPointId( 2, point3 );
      elements->SetElement( elementId++, elementA );

      TriangleElementType::Pointer elementB = TriangleElementType::New();
      TriangleElementType::BaseCellType & cellB = elementB->GetCell();
      cellB.SetPointId( 0, point0 );
      cellB.SetPointId( 1, point3 );
      cellB.SetPointId( 2, point2 );
      elements->SetElement( elementId++, elementB );

      }
    }

  // Create the Quadrilateral Elements
  for(unsigned int y=nyt; y<nyq+nyt; y++) 
    {
    for(unsigned int x=0; x<nx; x++) 
      {
      const unsigned int pointsPerRow = nx+1;
      const PointIdentifierType point0 =   x   + (   y   * pointsPerRow );
      const PointIdentifierType point1 = (x+1) + (   y   * pointsPerRow );
      const PointIdentifierType point2 =   x   + ( (y+1) * pointsPerRow );
      const PointIdentifierType point3 = (x+1) + ( (y+1) * pointsPerRow );

      QuadrilateralElementType::Pointer elementA = QuadrilateralElementType::New();
      QuadrilateralElementType::BaseCellType & cellA = elementA->GetCell();
      cellA.SetPointId( 0, point0 );
      cellA.SetPointId( 1, point1 );
      cellA.SetPointId( 3, point2 );  // defined in zigzag
      cellA.SetPointId( 2, point3 );  // defined in zigzag
      elements->SetElement( elementId++, elementA );

      }
    }


  heatMesh->SetElementsContainer( elements.GetPointer() );

  const unsigned int numberOfPoints = heatMesh->GetNumberOfPoints();
  HeatMeshType::PointDataContainerPointer values = 
                              HeatMeshType::PointDataContainer::New();

  values->Reserve( numberOfPoints );
  HeatMeshType::PointDataContainer::Iterator pointData = values->Begin();
  for(unsigned int i=0; i<numberOfPoints; i++) 
    {
    pointData.Value() =  itk::NumericTraits< HeatMeshType::PixelType >::Zero;
    ++pointData;
    }

  heatMesh->SetPointData( values );

  this->DisplayFEMMesh();

}





void
FEMMeshApplicationBase
::DisplayFEMMesh(void)
{

  typedef itk::fem::FEMElementTriangle< HeatMeshType >       TriangleElementType;
  typedef itk::fem::FEMElementQuadrilateral< HeatMeshType >  QuadrilateralElementType;
 
  typedef vtkItkElementMultiVisitor< HeatMeshType >     HeatVisitorType;
  

  // Define the Cell Visitor Types
  typedef itk::fem::FEMElementVisitorImplementation<
                                          HeatMeshType,
                                          TriangleElementType,
                                          HeatVisitorType  
                                                      >  HeatTriangleVisitorType;

  typedef itk::fem::FEMElementVisitorImplementation<
                                          HeatMeshType,
                                          QuadrilateralElementType,
                                          HeatVisitorType  
                                                      >  HeatQuadrilateralVisitorType;


  // Get the number of points in the mesh
  HeatMeshType::Pointer heatMesh = m_HeatSolver->GetMesh();

  const unsigned int numPoints = heatMesh->GetNumberOfPoints();
  if( numPoints == 0 )
    {
    itkGenericExceptionMacro(<<"Attempt to display a Mesh with no points !");
    }
    
  // Create a vtkUnstructuredGrid
  vtkUnstructuredGrid * vgrid = vtkUnstructuredGrid::New();

  // Create the vtkPoints object and set the number of points
  vtkPoints * vpoints = vtkPoints::New();
  vpoints->SetNumberOfPoints(numPoints);

  // iterate over all the points in the itk mesh filling in
  // the vtkPoints object as we go
  HeatMeshType::PointsContainer::Pointer points = heatMesh->GetPoints();

  for(HeatMeshType::PointsContainer::Iterator i = points->Begin();
      i != points->End(); ++i)
    {
    // Get the point index from the point container iterator
    int idx = i->Index();
    // Set the vtk point at the index with the the coord array from itk
    // itk returns a const pointer, but vtk is not const correct, so
    // we have to use a const cast to get rid of the const
    const HeatMeshType::PointType & p = i->Value();
    vpoints->SetPoint(idx, p[0], p[1], 0.0f );
    }

  // Set the points on the vtk grid
  vgrid->SetPoints(vpoints);
  

  // Now create the cells using the MulitVisitor
  // Create a MultiVisitor
  typedef HeatMeshType::ElementMultiVisitorType       HeatCellMultiVisitorType;
  HeatCellMultiVisitorType::Pointer multiVisitor    = HeatCellMultiVisitorType::New();

  // Create a Element visitors
  HeatTriangleVisitorType::Pointer triangleVisitor = 
                                              HeatTriangleVisitorType::New();

  HeatQuadrilateralVisitorType::Pointer quadrilateralVisitor =
                                              HeatQuadrilateralVisitorType::New();



  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = heatMesh->GetNumberOfElements();

  int *types = new int[numCells]; // type array for vtk 

  // create vtk cells and allocate memory for the cells.
  vtkCellArray* cells = vtkCellArray::New();
  cells->Allocate( numCells );  

  // Set the TypeArray CellCount and CellArray for both visitors
  triangleVisitor->SetTypeArray( types );
  triangleVisitor->SetCellCounter( &vtkCellCount );
  triangleVisitor->SetCellArray( cells );

  quadrilateralVisitor->SetTypeArray( types );
  quadrilateralVisitor->SetCellCounter( &vtkCellCount );
  quadrilateralVisitor->SetCellArray( cells );


  // add the visitors to the multivisitor
  multiVisitor->AddVisitor( triangleVisitor );
  multiVisitor->AddVisitor( quadrilateralVisitor );

  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  heatMesh->Accept( multiVisitor );
  
  // Now set the cells on the vtk grid with the type array and cell array
  vgrid->SetCells( types, cells );
  
  // Clean up vtk objects (no vtkSmartPointer ... )
  cells->Delete();
  vpoints->Delete();


  // connect the vtkUnstructuredGrid to a visualization Pipeline

  // map to graphics library
  vtkDataSetMapper * mapper = vtkDataSetMapper::New();
  mapper->SetInput( vgrid  );

  // actor coordinates geometry, properties, transformation
  vtkActor * actor = vtkActor::New();
  actor->SetMapper( mapper );
  actor->GetProperty()->SetColor(0,0,1); // color blue
  actor->GetProperty()->SetRepresentationToWireframe();

  // add the actor to the scene
  m_Renderer->AddActor( actor );


  mapper->Delete();
  actor->Delete();
  vgrid->Delete();

  delete [] types;

}








//
// Instantiate Visitors that compute Element's Area
// and send them to visit all the Element in the Mesh.
//
void
FEMMeshApplicationBase
::ComputeArea(void) const
{

  // Types for Triangles
  typedef itk::fem::FEMElementTriangle< 
                            HeatMeshType >  TriangleElementType;
 
  typedef itk::fem::FEMElementTriangleAreaVisitor< 
                            HeatMeshType >  TriangleAreaVisitorType;

  
  typedef itk::fem::FEMElementVisitorImplementation<
                                        HeatMeshType,
                                        TriangleElementType,
                                        TriangleAreaVisitorType  
                                            >  TriangleAreaVisitorImplementationType;


  TriangleAreaVisitorImplementationType::Pointer triangleVisitor = 
                                  TriangleAreaVisitorImplementationType::New();



  // Types for Quadrilaterals
  typedef itk::fem::FEMElementQuadrilateral< 
                            HeatMeshType >  QuadrilateralElementType;
 
  typedef itk::fem::FEMElementQuadrilateralAreaVisitor< 
                            HeatMeshType >  QuadrilateralAreaVisitorType;

  
  typedef itk::fem::FEMElementVisitorImplementation<
                                        HeatMeshType,
                                        QuadrilateralElementType,
                                        QuadrilateralAreaVisitorType  
                                            >  QuadrilateralAreaVisitorImplementationType;


  QuadrilateralAreaVisitorImplementationType::Pointer quadrilateralVisitor = 
                                  QuadrilateralAreaVisitorImplementationType::New();



  HeatMeshType::Pointer heatMesh = m_HeatSolver->GetMesh();


  triangleVisitor->SetMesh( heatMesh );
  quadrilateralVisitor->SetMesh( heatMesh );


  // Create a MultiVisitor
  typedef HeatMeshType::ElementMultiVisitorType       MultiVisitorType;

  MultiVisitorType::Pointer multiVisitor    = MultiVisitorType::New();


  multiVisitor->AddVisitor( triangleVisitor );
  multiVisitor->AddVisitor( quadrilateralVisitor );



  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  heatMesh->Accept( multiVisitor );
  

  // Print the subtotals by Element Type
  std::cout << "Area of Triangles      = " << triangleVisitor->GetArea() << std::endl;
  std::cout << "Area of Quadrilaterals = " << quadrilateralVisitor->GetArea() << std::endl;

}




//
// Instantiate Visitors that compute Element's HeatConduction
// and send them to visit all the Element in the Mesh.
// This is all delegated to the HeatConduction2D object.
//
void
FEMMeshApplicationBase
::ComputeHeatConduction(void) const
{
  m_HeatSolver->AssembleMasterEquation();
}

