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

#include "itkNumericTraits.h"
#include "itkCellInterfaceVisitor.h"

#include "itkFEMElementBar2D.h"


#include "itkFEMElementTriangle.h"




FEMMeshApplicationBase
::FEMMeshApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_Renderer->SetBackground( 1, 1, 1 );

  m_FEMMesh = FEMMeshType::New();

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
::CreateSphere(void)
{

  // create sphere geometry
  vtkSphereSource * sphere = vtkSphereSource::New();
  sphere->SetRadius(1.0);
  sphere->SetThetaResolution(18);
  sphere->SetPhiResolution(18);

  // map to graphics library
  vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
  mapper->SetInput( sphere->GetOutput()  );

  // actor coordinates geometry, properties, transformation
  vtkActor * actor = vtkActor::New();
  actor->SetMapper( mapper );
  actor->GetProperty()->SetColor(0,0,1); 
  actor->GetProperty()->SetOpacity(0.2); 

  // add the actor to the scene
  m_Renderer->AddActor( actor );

  sphere->Delete();
  mapper->Delete();
  actor->Delete();
  

}





void
FEMMeshApplicationBase
::CreateTriangle(void)
{


  const float cos30 = sqrt( 3.0 ) / 2.0;
  const float sin30 = 0.5;

  // Z coordinate of the points
  const float z0 = 0.0;
  const float z1 = 0.0;
  const float z2 = 0.0;

  const unsigned int numberOfPoints = 3;

  static float x[numberOfPoints][3]=
  {
    {  cos30, -sin30,  z0 }, 
    {    0.0,    1.0,  z1 },
    { -cos30, -sin30,  z2 } 
  };


  const unsigned int numberOfLines = 3;
  const unsigned int pointsPerLine = 2;
  static vtkIdType   linePoints[numberOfLines][pointsPerLine] = 
  {
    {0,1},
    {1,2},
    {2,0}
  };
  
  // We'll create the building blocks of polydata including data attributes.
  vtkPolyData   * triangle = vtkPolyData::New();
  vtkPoints     * points   = vtkPoints::New();
  vtkCellArray  * lines    = vtkCellArray::New();

  // Load the point, cell, and data attributes.
  for (unsigned int i=0; i<numberOfPoints; i++) 
    {
    points->InsertPoint(i,x[i]);
    }


  for (unsigned int l=0; l<numberOfLines; l++) 
    {
    lines->InsertNextCell(pointsPerLine,linePoints[l]);
    }


  // We now assign the pieces to the vtkPolyData.
  triangle->SetPoints(points);
  points->Delete();
  triangle->SetLines(lines);


  // map to graphics library
  vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
  mapper->SetInput( triangle  );

  // actor coordinates geometry, properties, transformation
  vtkActor * actor = vtkActor::New();
  actor->SetMapper( mapper );
  actor->GetProperty()->SetColor(0,0,1); // color blue

  // add the actor to the scene
  m_Renderer->AddActor( actor );


  triangle->Delete();
  mapper->Delete();
  actor->Delete();
 

}




void
FEMMeshApplicationBase
::DisplayFEMMesh2(void)
{
 
  typedef vtkItkCellMultiVisitor< 
                              CellDataType, 
                              CellTraits 
                                            > 
                                              VisitorType;

  

  // Define the Cell Visitor Types
  typedef itk::CellInterfaceVisitorImplementation<
                                              PointDataType,
                                              CellTraits,
                                              LineCellType,
                                              VisitorType  
                                                      >  LineVisitorType;


  // Get the number of points in the mesh
  const unsigned int numPoints = m_FEMMesh->GetNumberOfPoints();
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
  FEMMeshType::PointsContainer::Pointer points = m_FEMMesh->GetPoints();

  for(FEMMeshType::PointsContainer::Iterator i = points->Begin();
      i != points->End(); ++i)
    {
    // Get the point index from the point container iterator
    int idx = i->Index();
    // Set the vtk point at the index with the the coord array from itk
    // itk returns a const pointer, but vtk is not const correct, so
    // we have to use a const cast to get rid of the const
    vpoints->SetPoint(idx, const_cast<float*>(i->Value().GetDataPointer()));
    }
  // Set the points on the vtk grid
  vgrid->SetPoints(vpoints);
  
  // Now create the cells using the MulitVisitor
  // 1. Create a MultiVisitor
  CellMultiVisitorType::Pointer multiVisitor = CellMultiVisitorType::New();

  // 2. Create a LineCell visitor
  LineVisitorType::Pointer lineVisitor = LineVisitorType::New();

  // 3. Set up the visitors
  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = m_FEMMesh->GetNumberOfCells();

  int *types = new int[numCells]; // type array for vtk 

  // create vtk cells and estimate the size
  vtkCellArray* cells = vtkCellArray::New();
  cells->EstimateSize(numCells, 4);

  // Set the TypeArray CellCount and CellArray for both visitors
  lineVisitor->SetTypeArray( types );
  lineVisitor->SetCellCounter( &vtkCellCount );
  lineVisitor->SetCellArray( cells );

  // add the visitors to the multivisitor
  multiVisitor->AddVisitor( lineVisitor );

  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  m_FEMMesh->Accept( multiVisitor );
  
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

  // add the actor to the scene
  m_Renderer->AddActor( actor );

  vgrid->Delete();
  mapper->Delete();
  actor->Delete();

}






void
FEMMeshApplicationBase
::CreateFEMMesh2(void)
{

  // Create the FEMMesh that will hold both the Geometric 
  // and the Physical representation of the problem 
  m_FEMMesh = FEMMeshType::New();



  // In this section we create Points and the Nodes associated with them

  // Start numbering points
  PointIdentifierType pointId = 
              itk::NumericTraits<PointIdentifierType>::Zero;


  PointType           point;


  // BTW we are making just a pyramid here

  const float baseLength  = 10.0f;
  const float a           = baseLength / 2.0f;

  // First point of the base
  point[0] = -a; 
  point[1] = -a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  pointId++;

  // Second point of the base
  point[0] =  a; 
  point[1] = -a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  pointId++;

  // Third point of the base
  point[0] =  a; 
  point[1] =  a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  pointId++;

  // Fourth point of the base
  point[0] = -a; 
  point[1] =  a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  pointId++;

  // point at the top of the pyramid 
  point[0] = 0.0;
  point[1] = 0.0; 
  point[2] = 10;

  m_FEMMesh->SetPoint( pointId, point );

  pointId++;



  // Create the array of values to associate with the points.
  // In this particular case we are using Vectors that represent
  // displacementes in 3D.  It could be anything because this is 
  // a template parameter for the mesh.
  typedef FEMMeshType::PointDataContainer DisplacementsContainerType;
  DisplacementsContainerType::Pointer displacementContainer = 
                                        DisplacementsContainerType::New();

  const unsigned int numberOfPoints = m_FEMMesh->GetNumberOfPoints();

  PointIdentifierType displacementId = itk::NumericTraits< PointIdentifierType >::Zero;
  displacementContainer->Reserve( numberOfPoints ); 
  for(unsigned int p=0; p<numberOfPoints; p++) 
    {
    DisplacementType displacement;
    displacement.Fill( itk::NumericTraits< DisplacementRepresentationType >::Zero );
    displacementContainer->SetElement( displacementId, displacement );
    ++displacementId;
    }

  m_FEMMesh->SetPointData( displacementContainer );

  std::cout << "Number of points = " << m_FEMMesh->GetNumberOfPoints() << std::endl;

  //-----------------------------------------------
  // Now we can add Elements to the FEMMesh ...
  //-----------------------------------------------

  // For example a Bar2D type is defined here
  typedef itk::fem::FEMElementBar2D< 
                              FEMMeshType, 
                              DisplacementDimension  >   ElementBarType;

  //  Create the Bar
  ElementBarType::Pointer bar1 = ElementBarType::New();

  // Associate the Bar with two of the current points
  bar1->SetPointId(0, 2 ); // associate first point with mesh point  #2
  bar1->SetPointId(1, 3 ); // associate second point with mesh point #3

  // Add the bar to the list of Elements. This is done by inserting the 
  // Bar as a Cells in the mesh. The ElementBar derives from LineCell<>
  // so its pointer is a valid derived class from CellInterface<>

  CellIdentifierType cellId = itk::NumericTraits< CellIdentifierType >::Zero;
  m_FEMMesh->SetCell( cellId++, bar1.GetPointer() ); // GetPointer() returns a normal pointer


  std::cout << "Number of cells  = " << m_FEMMesh->GetNumberOfCells() << std::endl;

  this->DisplayFEMMesh();

}




void
FEMMeshApplicationBase
::DisplayAxes(void)
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

  const unsigned int degreesOfFreedomPerPoint = 1;
  typedef itk::fem::FEMElementTriangle< 
                        HeatMeshType, 
                        degreesOfFreedomPerPoint >  HeatCellType;
  
  const unsigned int nx = 10;
  const unsigned int ny = 10;

  const double dx = 1.5;
  const double dy = 1.0;

  // Create the points
  typedef HeatMeshType::PointType         HeatPointType;
  typedef HeatMeshType::PointsContainer   HeatPointsContainer;
  HeatPointsContainer::Pointer points = HeatPointsContainer::New();
  points->Reserve( ( nx + 1 ) * ( ny + 1 ) );

  typedef HeatMeshType::PointIdentifier   HeatPointIdentifier;
  HeatPointIdentifier pointId = itk::NumericTraits< HeatPointIdentifier >::Zero;

  for(unsigned int y=0; y<=ny; y++) 
    {
    HeatPointType point;
    point[1] = y * dy;
    for(unsigned int x=0; x<=nx; x++) 
      {
      point[0] = x * dx;
      points->SetElement( pointId++, point );
      }
    }

  heatMesh->SetPoints( points.GetPointer() );

  // Create the points
  typedef HeatMeshType::CellsContainer  HeatCellsContainer;
  HeatCellsContainer::Pointer cells = HeatCellsContainer::New();
  cells->Reserve( 2 * nx * ny );

  typedef HeatMeshType::CellIdentifier  HeatCellIdentifier;
  HeatCellIdentifier cellId = itk::NumericTraits< HeatCellIdentifier >::Zero;

  for(unsigned int y=0; y<ny; y++) 
    {
    for(unsigned int x=0; x<nx; x++) 
      {
      HeatCellType::Pointer cell = HeatCellType::New();
      cells->SetElement( cellId++, cell.GetPointer() );
      }
    }


  heatMesh->SetCells( cells.GetPointer() );

  std::cout << "Number of Points = " << heatMesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of Cells  = " << heatMesh->GetNumberOfCells()  << std::endl;

  m_HeatSolver->AssembleMasterEquation();


}




void
FEMMeshApplicationBase
::DisplayFEMMesh(void)
{

  const unsigned int degreesOfFreedomPerPoint = 1;
  typedef itk::fem::FEMElementTriangle< 
                        HeatMeshType, 
                        degreesOfFreedomPerPoint >  HeatCellType;
 
  typedef vtkItkCellMultiVisitor< 
                              HeatSolverType::DisplacementType, 
                              HeatMeshType::CellTraits 
                                            > 
                                              HeatVisitorType;

  

  // Define the Cell Visitor Types
  typedef itk::CellInterfaceVisitorImplementation<
                                              HeatSolverType::DisplacementType,
                                              HeatCellType::CellTraits,
                                              HeatCellType,
                                              HeatVisitorType  
                                                      >  HeatTriangleVisitorType;

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
    vpoints->SetPoint(idx, const_cast<float*>(i->Value().GetDataPointer()));
    }
  // Set the points on the vtk grid
  vgrid->SetPoints(vpoints);
  
  // Now create the cells using the MulitVisitor
  // 1. Create a MultiVisitor
  typedef HeatMeshType::CellMultiVisitorType       HeatCellMultiVisitorType;
  HeatCellMultiVisitorType::Pointer multiVisitor = HeatCellMultiVisitorType::New();

  // 2. Create a LineCell visitor
  HeatTriangleVisitorType::Pointer triangleVisitor = HeatTriangleVisitorType::New();


  // 3. Set up the visitors
  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = heatMesh->GetNumberOfCells();

  int *types = new int[numCells]; // type array for vtk 

  // create vtk cells and estimate the size
  vtkCellArray* cells = vtkCellArray::New();
  cells->EstimateSize(numCells, 4);

  // Set the TypeArray CellCount and CellArray for both visitors
  triangleVisitor->SetTypeArray( types );
  triangleVisitor->SetCellCounter( &vtkCellCount );
  triangleVisitor->SetCellArray( cells );

  // add the visitors to the multivisitor
//  multiVisitor->AddVisitor( triangleVisitor );

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

  // add the actor to the scene
  m_Renderer->AddActor( actor );

  vgrid->Delete();
  mapper->Delete();
  actor->Delete();

}
