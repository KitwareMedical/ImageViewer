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

  
  // HERE CONVERT THE MESH to a VTK Mesh


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

  PointType * point1 = new PointType;
  (*point1)[0] = 10;
  (*point1)[1] = 15;
  (*point1)[2] = 13;
  m_FEMMesh->AddPoint( point1 );


  typedef itk::fem::FEM2DDeformationNode   NodeType;

  NodeType  * node1  = new NodeType;
  m_FEMMesh->AddNode( node1 );


  typedef itk::fem::FEMLineCell<PointsDimension>   LineCellType;

  LineCellType  * cell1 = new LineCellType;
  cell1->SetPoint( 0, point1 );
  m_FEMMesh->AddCell( cell1 );


  
  std::cout << "Number of points    = " << m_FEMMesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of cells     = " << m_FEMMesh->GetNumberOfCells() << std::endl;
  std::cout << "Number of nodes     = " << m_FEMMesh->GetNumberOfNodes() << std::endl;
  std::cout << "Number of elements  = " << m_FEMMesh->GetNumberOfElements() << std::endl;

  this->DisplayFEMMesh();

}




