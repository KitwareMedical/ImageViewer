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

#include "vtkItkCellMultiVisitor.h"

#include "itkNumericTraits.h"



FEMMeshApplicationBase
::FEMMeshApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_FEMMesh = FEMMeshType::New();

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
  m_Renderer->SetBackground(1,1,1); // Background color white

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


  const unsigned int numberOfCells = 1;
  static vtkIdType   cellPoints[numberOfCells][3] = {{0,1,2}};

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
  m_Renderer->SetBackground(1,1,1); // Background color white


  triangle->Delete();
  mapper->Delete();
  actor->Delete();
 

}




void
FEMMeshApplicationBase
::DisplayFEMMesh(void)
{
 
  typedef vtkItkCellMultiVisitor< 
                                  FEMMeshType::CellTraits 
                                                            > VisitorType;

  typedef itk::fem::VertexCell< NodeType::CellTraits > VertexCellType;


  // Define the Cell Visitor Types
  typedef itk::fem::CellInterfaceVisitorImplementation<
                                  FEMMeshType::CellTraits,
                                  VertexCellType,
                                  VisitorType  > VertexVisitorType;


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

  // 2. Create a VertexCell visitor
  VertexVisitorType::Pointer vertexVisitor = VertexVisitorType::New();

  // 3. Set up the visitors
  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = m_FEMMesh->GetNumberOfCells();

  int *types = new int[numCells]; // type array for vtk 

  // create vtk cells and estimate the size
  vtkCellArray* cells = vtkCellArray::New();
  cells->EstimateSize(numCells, 4);

  // Set the TypeArray CellCount and CellArray for both visitors
  vertexVisitor->SetTypeArray( types );
  vertexVisitor->SetCellCounter( &vtkCellCount );
  vertexVisitor->SetCellArray( cells );

  // add the visitors to the multivisitor
  multiVisitor->AddVisitor( vertexVisitor );

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
  m_Renderer->SetBackground(1,1,1); // Background color white

  vgrid->Delete();
  mapper->Delete();
  actor->Delete();

}






void
FEMMeshApplicationBase
::CreateFEMMesh(void)
{

  // Create the FEMMesh that will hold both the Geometric 
  // and the Physical representation of the problem 
  m_FEMMesh = FEMMeshType::New();



  // In this section we create Points and the Nodes associated with them

  // Start numbering points
  PointIdentifierType pointId = 
              ::itk::NumericTraits<PointIdentifierType>::Zero;

  // Start numbering Nodes (Note that Nodes are Zero Dimensional Cells !)
   CellIdentifierType cellId = 
              ::itk::NumericTraits<CellIdentifierType>::Zero;

   
  PointType           point;
  NodeType::Pointer   node;


  // BTW we are making just a pyramid here

  const float baseLength  = 10.0f;
  const float a           = baseLength / 2.0f;

  // First point of the base
  point[0] = -a; 
  point[1] = -a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  node = NodeType::New();
  node->SetPointId( pointId );
  m_FEMMesh->SetCell( cellId, node.GetPointer() );

  pointId++;
  cellId++;

  // Second point of the base
  point[0] =  a; 
  point[1] = -a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  node = NodeType::New();
  node->SetPointId( pointId );
  m_FEMMesh->SetCell( cellId, node.GetPointer() );

  pointId++;
  cellId++;

  // Third point of the base
  point[0] =  a; 
  point[1] =  a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  node = NodeType::New();
  node->SetPointId( pointId );
  m_FEMMesh->SetCell( cellId, node.GetPointer() );

  pointId++;
  cellId++;

  // Fourth point of the base
  point[0] = -a; 
  point[1] =  a; 
  point[2] = 0.0; 

  m_FEMMesh->SetPoint( pointId, point );

  node = NodeType::New();
  node->SetPointId( pointId );
  m_FEMMesh->SetCell( cellId, node.GetPointer() );

  pointId++;
  cellId++;


  // point at the top of the pyramid 
  point[0] = 0.0;
  point[1] = 0.0; 
  point[2] = 10;

  m_FEMMesh->SetPoint( pointId, point );

  node = NodeType::New();
  node->SetPointId( pointId );
  m_FEMMesh->SetCell( cellId, node.GetPointer() );

  pointId++;
  cellId++;


  std::cout << "Number of points = " << m_FEMMesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of cells  = " << m_FEMMesh->GetNumberOfCells() << std::endl;

  this->DisplayFEMMesh();

}




