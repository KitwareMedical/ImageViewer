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
#include "vtkLine.h"
#include "vtkAxes.h"
#include "vtkTubeFilter.h"

#include "itkNumericTraits.h"

#include "itkFEM2DDeformationNode.h"
#include "itkFEMLineCell.h"



FEMMeshApplicationBase
::FEMMeshApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_FEMMesh = FEMMeshType::New();

  m_Renderer->SetBackground( 1.0, 1.0, 1.0 ); 
  m_Renderer->GetActiveCamera()->Zoom( 1.0 ); 
  m_Renderer->GetActiveCamera()->SetPosition(0.0, 0.0, 10.0 ); 

}




FEMMeshApplicationBase
::~FEMMeshApplicationBase()
{
  m_RenderWindow->Delete();
  m_Renderer->Delete();

  PointsToBeDeletedContainer::iterator pointToBeDeleted = 
                                m_PointsToBeDeleted.begin();
  while( pointToBeDeleted != m_PointsToBeDeleted.end() )
    {
    delete (*pointToBeDeleted);
    ++pointToBeDeleted;
    }


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


  triangle->Delete();
  mapper->Delete();
  actor->Delete();
 

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
::DisplayFEMMesh(void)
{
 

  // Get the number of points in the mesh
  const unsigned int numberOfPoints = m_FEMMesh->GetNumberOfPoints();
  if( numberOfPoints == 0 )
    {
    itkGenericExceptionMacro(<<"Attempt to display a Mesh with no points !");
    }
    

  // Create a vtkUnstructuredGrid
  vtkUnstructuredGrid * vgrid = vtkUnstructuredGrid::New();
  

  // Create the vtkPoints object and set the number of points
  vtkPoints * vpoints = vtkPoints::New();
  vpoints->SetNumberOfPoints(numberOfPoints);

  

  // Transfer Points information from the FEMMesh to the vtkMesh
  vtkIdType pointId = itk::NumericTraits< vtkIdType >::Zero;
  for(unsigned int p=0; p<numberOfPoints; p++)
    {
    BasePointType & point = *( m_FEMMesh->GetPoint( p ) );
    vpoints->SetPoint(  pointId, point[0], point[1], point[2] );
    pointId++;
    }

  vgrid->SetPoints( vpoints );
  
  
  const unsigned int numberOfCells    = m_FEMMesh->GetNumberOfCells();  
  {
    vtkCellArray  * vcells   = vtkCellArray::New();
    int * cellTypes = new int[ numberOfCells ];

    for(unsigned int c=0; c<numberOfCells; c++)
      {
      vtkIdType pointIndex[2];
      pointIndex[0] = 0;
      pointIndex[1] = 1;
      cellTypes[c] = VTK_LINE;
      vcells->InsertNextCell ( cellTypes[c], pointIndex );
      }
    vgrid->SetCells ( cellTypes, vcells );
    std::cout << "Number of Cells in the Grid ";
    std::cout << vgrid->GetNumberOfCells() << std::endl;
    std::cout << "Number of Points in the Grid ";
    std::cout << vgrid->GetNumberOfPoints() << std::endl;
  }



  const unsigned int numberOfNodes    = m_FEMMesh->GetNumberOfNodes();  
  const unsigned int numberOfElements = m_FEMMesh->GetNumberOfElements();  

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
::CreateFEMMesh(void)
{

  // Create the FEMMesh that will hold both the Geometric 
  // and the Physical representation of the problem 
  m_FEMMesh = FEMMeshType::New();


  
  // The user (we here :-) ) is responsible for allocating
  // and releasing the memory for Points, Nodes, Cells and
  // Elements. The FEMMesh can not do that because that will
  // difficult the use of polymorphism.

  BasePointType * point1 = new BasePointType;

  (*point1)[0] =   1.0;
  (*point1)[1] =   0.2;
  (*point1)[2] =   0.0;

  BasePointType * point2 = new BasePointType;

  (*point2)[0] =  -1.0;
  (*point2)[1] =   0.2;
  (*point2)[2] =   0.0;

  m_FEMMesh->AddPoint( point1 );
  m_FEMMesh->AddPoint( point2 );

  m_PointsToBeDeleted.insert( point1 );
  m_PointsToBeDeleted.insert( point2 );


  // Note that the numberOfNodes is not necessarily equal to
  // the numberOfPoints because the geometry and the degrees
  // of freedom do not have to be represented with shape 
  // functions of the same order.
  typedef itk::fem::FEM2DDeformationNode Deformation2DNodeType;
  Deformation2DNodeType * node1 = new Deformation2DNodeType;
  Deformation2DNodeType * node2 = new Deformation2DNodeType;
  m_FEMMesh->AddNode( node1 );
  m_FEMMesh->AddNode( node2 );

  m_NodesToBeDeleted.insert( node1 );
  m_NodesToBeDeleted.insert( node2 );


  typedef itk::fem::FEMLineCell< PointsDimension > LineCellType;
  LineCellType * cell1 = new LineCellType;
  cell1->SetPoint( 0, point1 );
  cell1->SetPoint( 1, point2 );

  m_FEMMesh->AddCell( cell1 );

  m_CellsToBeDeleted.insert( cell1 );
  


  std::cout << "Number of points    = " << m_FEMMesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of cells     = " << m_FEMMesh->GetNumberOfCells() << std::endl;
  std::cout << "Number of nodes     = " << m_FEMMesh->GetNumberOfNodes() << std::endl;
  std::cout << "Number of elements  = " << m_FEMMesh->GetNumberOfElements() << std::endl;

  this->DisplayFEMMesh();

}




