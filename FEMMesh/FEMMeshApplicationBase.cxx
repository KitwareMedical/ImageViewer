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

#include "itkNumericTraits.h"



FEMMeshApplicationBase
::FEMMeshApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_FEMMesh = FEMMeshType::New();

  m_Points    = 0;
  m_Nodes     = 0;
  m_Cells     = 0;
  m_Elements  = 0;

}




FEMMeshApplicationBase
::~FEMMeshApplicationBase()
{
  m_RenderWindow->Delete();
  m_Renderer->Delete();

  if( m_Points )
    {
    delete [] m_Points;
    } 

  if( m_Nodes )
    {
    delete [] m_Nodes;
    } 

  if( m_Cells )
    {
    delete [] m_Cells;
    } 
  if( m_Elements )
    {
    delete [] m_Elements;
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

  
  // HERE CONVERT THE FEMMESH to a VTK Mesh
  const unsigned int numberOfCells    = m_FEMMesh->GetNumberOfCells();  
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

  // The user (we here :-) ) is responsible for allocating
  // and releasing the memory for Points, Nodes, Cells and
  // Elements. The FEMMesh can not do that because that will
  // difficult the use of polymorphism.

  const unsigned int numberOfPoints = 10;

  m_Points = new ActualPointType[ numberOfPoints ];

  m_Points[0][0] = 10;
  m_Points[0][1] = 15;
  m_Points[0][2] = 18;

  m_Points[0][0] = 20;
  m_Points[0][1] = 15;
  m_Points[0][2] = 18;

  m_FEMMesh->AddPoint( &(m_Points[0]) );
  m_FEMMesh->AddPoint( &(m_Points[1]) );


  // Note that the numberOfNodes is not necessarily equal to
  // the numberOfPoints because the geometry and the degrees
  // of freedom do not have to be represented with shape 
  // functions of the same order.
  const unsigned int numberOfNodes = 10;
  m_Nodes = new ActualNodeType[ numberOfNodes ];
  m_FEMMesh->AddNode( &(m_Nodes[0]) );
  m_FEMMesh->AddNode( &(m_Nodes[1]) );


  const unsigned int numberOfCells = 2;
  m_Cells = new ActualCellType[ numberOfCells ];
  m_Cells[0].SetPoint( 0, &(m_Points[0]) );
  m_Cells[0].SetPoint( 1, &(m_Points[1]) );
  m_FEMMesh->AddCell( &(m_Cells[0]) );

  
  std::cout << "Number of points    = " << m_FEMMesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of cells     = " << m_FEMMesh->GetNumberOfCells() << std::endl;
  std::cout << "Number of nodes     = " << m_FEMMesh->GetNumberOfNodes() << std::endl;
  std::cout << "Number of elements  = " << m_FEMMesh->GetNumberOfElements() << std::endl;

  this->DisplayFEMMesh();

}




