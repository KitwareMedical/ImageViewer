/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMMeshBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "FEMMeshBase.h"

#include "vtkPolyDataMapper.h"
#include "vtkSphereSource.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"



FEMMeshBase
::FEMMeshBase()
{
  m_RenderWindow = vtkRenderWindow::New();
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

}




FEMMeshBase
::~FEMMeshBase()
{
  m_RenderWindow->Delete();
  m_Renderer->Delete();
}



void
FEMMeshBase
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
FEMMeshBase
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



