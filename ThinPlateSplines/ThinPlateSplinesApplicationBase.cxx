/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThinPlateSplinesApplicationBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ThinPlateSplinesApplicationBase.h"

#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkPoints.h"
#include "vtkAxes.h"
#include "vtkTubeFilter.h"
#include "vtkGlyph3D.h"
#include "vtkSphereSource.h"
#include "vtkUnstructuredGrid.h"

#include "itkNumericTraits.h"




ThinPlateSplinesApplicationBase
::ThinPlateSplinesApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();

  m_Renderer = 0;
  this->CreateRenderer();

  m_SourceLandMarks = PointSetType::New();
  m_TargetLandMarks = PointSetType::New();
  
  m_VTKSourceLandMarks   = vtkPoints::New();
  m_VTKTargetLandMarks   = vtkPoints::New();

  m_VTKPointsToTransform = vtkPoints::New();
  m_VTKLinesToTransform  = vtkCellArray::New();

  m_VTKPointsTransformed = vtkPoints::New();
  m_VTKLinesTransformed  = vtkCellArray::New();

  m_ThinPlateSplineTransform = ThinPlateSplineTransformType::New();

}




ThinPlateSplinesApplicationBase
::~ThinPlateSplinesApplicationBase()
{
  m_RenderWindow->Delete();
  m_Renderer->Delete();

  m_VTKTargetLandMarks->Delete();
  m_VTKSourceLandMarks->Delete();

}



void
ThinPlateSplinesApplicationBase
::CreateRenderer()
{
  if( m_Renderer )
    {
    m_Renderer->Delete();
    }
  
  m_Renderer     = vtkRenderer::New();

  // a renderer and render window
  m_RenderWindow->AddRenderer( m_Renderer );

  m_Renderer->SetBackground( 1.0, 1.0, 1.0 ); 
  m_Renderer->GetActiveCamera()->Zoom( 1.0 ); 
  m_Renderer->GetActiveCamera()->SetPosition(0.0, 0.0, 20.0 ); 
}




void
ThinPlateSplinesApplicationBase
::RemoveActors()
{
  
  ActorsArrayType::iterator actor = m_ActorsToDelete.begin();
  ActorsArrayType::iterator end   = m_ActorsToDelete.end();
  while( actor != end )
    {
    m_Renderer->RemoveActor( *actor );
    (*actor)->Delete();
    actor++;
    }
  m_ActorsToDelete.clear();
}





void
ThinPlateSplinesApplicationBase
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
  m_ActorsToDelete.insert( axesActor );

  axesMapper->Delete();
  axesTubes->Delete();
  axes->Delete();

}




void
ThinPlateSplinesApplicationBase
::CreateSpline(void)
{
  this->CreateLandMarks();
  this->CreateSourcePoints();
}




void
ThinPlateSplinesApplicationBase
::ShowTimeProbes(void)
{
  m_TimeCollector.Show();
}


 
void
ThinPlateSplinesApplicationBase
::CreateLandMarks(void)
{

  PointSetType::PointsContainer::Pointer sourceLandMarks = 
                                   m_SourceLandMarks->GetPoints();

  PointSetType::PointsContainer::Pointer targetLandMarks = 
                                   m_TargetLandMarks->GetPoints();

  typedef PointSetType::PointIdentifier  PointIdType;

  PointIdType landmarkId = itk::NumericTraits< PointIdType >::Zero;

  const unsigned int nx =  6;
  const unsigned int ny =  6;
  const unsigned int nz =  1;

  CoordinateRepresentationType deltax = 1;

  PointType p;
  for(unsigned int z=0; z<nz; z++)
    {
    CoordinateRepresentationType rz = z;
    p[2] = rz;  
    for(unsigned int y=0; y<ny; y++)
      {
      CoordinateRepresentationType ry = y;
      ry -= ny/2;
      p[1] = ry;
      for(unsigned int x=0; x<nx; x++)
        {
        CoordinateRepresentationType rx = x;
        rx -= nx + deltax;
        p[0] = rx;
        sourceLandMarks->InsertElement( landmarkId, p );
        rx  =  x;
        rx  += deltax;
        p[0] = rx;
        targetLandMarks->InsertElement( landmarkId, p );
        landmarkId++;
        }
      }
    }

  this->DisplayAxes();
  this->DisplayLandMarks();

}


 
void
ThinPlateSplinesApplicationBase
::TransferLandMarksToVTK(void)
{

  m_VTKSourceLandMarks->Delete();
  m_VTKTargetLandMarks->Delete();

  m_VTKSourceLandMarks = vtkPoints::New();
  m_VTKTargetLandMarks = vtkPoints::New();

  const unsigned int numberOfLandMarks = m_SourceLandMarks->GetNumberOfPoints();

  if( numberOfLandMarks != m_TargetLandMarks->GetNumberOfPoints() )
    {
    itkGenericExceptionMacro( << "Number of Source Landmarks != Target Landmarks " );
    }

  m_VTKSourceLandMarks->Allocate( numberOfLandMarks + 2  );
  m_VTKTargetLandMarks->Allocate( numberOfLandMarks + 2  );

  vtkIdType pointId = itk::NumericTraits< vtkIdType >::Zero; 

  typedef PointSetType::PointsContainer::Iterator  PointIteratorType;
  PointIteratorType slm = m_SourceLandMarks->GetPoints()->Begin();
  PointIteratorType end = m_SourceLandMarks->GetPoints()->End();
  PointIteratorType tlm = m_TargetLandMarks->GetPoints()->Begin();
  while( slm != end )
    {
    PointType & srclm = slm.Value();
    PointType & trglm = tlm.Value();
    m_VTKSourceLandMarks->InsertNextPoint( srclm[0], srclm[1], srclm[2] );
    m_VTKTargetLandMarks->InsertNextPoint( trglm[0], trglm[1], trglm[2] );
    ++slm;
    ++tlm;
    ++pointId;
    }

}




void
ThinPlateSplinesApplicationBase
::DisplayLandMarks(void)
{

  this->TransferLandMarksToVTK();
   
  vtkSphereSource * typicalSphere = vtkSphereSource::New();
  typicalSphere->SetRadius(0.05);

  vtkPolyData * sourcePolyData = vtkPolyData::New();
  vtkGlyph3D  * sourceSpheres  = vtkGlyph3D::New();

  sourceSpheres->SetSource( typicalSphere->GetOutput() );
  sourcePolyData->SetPoints( m_VTKSourceLandMarks );
  sourceSpheres->SetInput( sourcePolyData );

  vtkPolyData * targetPolyData = vtkPolyData::New();
  vtkGlyph3D  * targetSpheres  = vtkGlyph3D::New();

  targetSpheres->SetSource( typicalSphere->GetOutput() );
  targetPolyData->SetPoints( m_VTKTargetLandMarks );
  targetSpheres->SetInput( targetPolyData );

  // map to graphics library
  vtkPolyDataMapper * sourceMapper   = vtkPolyDataMapper::New();
  sourceMapper->SetInput( sourceSpheres->GetOutput() );

  // map to graphics library
  vtkPolyDataMapper * targetMapper   = vtkPolyDataMapper::New();
  targetMapper->SetInput( targetSpheres->GetOutput() );

  // actor coordinates geometry, properties, transformation
  vtkActor * sourceActor = vtkActor::New();
  sourceActor->SetMapper( sourceMapper );
  sourceActor->GetProperty()->SetColor(0,0,1); 

  // actor coordinates geometry, properties, transformation
  vtkActor * targetActor = vtkActor::New();
  targetActor->SetMapper( targetMapper );
  targetActor->GetProperty()->SetColor(1,0,0); 

  // add the actor to the scene
  m_Renderer->AddActor( sourceActor );
  m_Renderer->AddActor( targetActor );

  m_ActorsToDelete.insert( sourceActor );
  m_ActorsToDelete.insert( targetActor );

  sourceMapper->Delete();
  sourceSpheres->Delete();
  sourcePolyData->Delete();

  targetMapper->Delete();
  targetSpheres->Delete();
  targetPolyData->Delete();

  typicalSphere->Delete();
  
}

   
 
void
ThinPlateSplinesApplicationBase
::MapPoints(void)
{
 
  m_ThinPlateSplineTransform->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_ThinPlateSplineTransform->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_ThinPlateSplineTransform->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformed.clear();

  m_TimeCollector.Start("ITK TPS");

  while( point != end )
    {
    m_PointsTransformed.push_back( 
                  m_ThinPlateSplineTransform->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK TPS");

  // Convert transformed point to a VTK structure for visualization
  m_VTKPointsTransformed->Delete();
  m_VTKPointsTransformed = vtkPoints::New();

  m_VTKLinesTransformed->Delete();
  m_VTKLinesTransformed = vtkCellArray::New();

  m_VTKPointsTransformed->SetNumberOfPoints( m_PointsTransformed.size() );

  vtkIdType pointCounter = itk::NumericTraits< vtkIdType >::Zero;
  point = m_PointsTransformed.begin();
  end   = m_PointsTransformed.end();
  while( point != end )
    {
    const PointType p = *point;
    m_VTKPointsTransformed->SetPoint( pointCounter, p[0], p[1], p[2] );
    m_VTKLinesTransformed->InsertNextCell( VTK_VERTEX, &pointCounter );
    ++point;
    ++pointCounter;
    }


}


 
void
ThinPlateSplinesApplicationBase
::CreateSourcePoints(void)
{
  
  m_PointsToTransform.clear();

  m_VTKPointsToTransform->Delete();
  m_VTKPointsToTransform = vtkPoints::New();

  m_VTKLinesToTransform->Delete();
  m_VTKLinesToTransform = vtkCellArray::New();

  const unsigned int fac = 10;

  const unsigned int nx =  6 * fac;
  const unsigned int ny =  6 * fac;
  const unsigned int nz =  1;

  CoordinateRepresentationType deltax = 1.5f;

  m_VTKPointsToTransform->SetNumberOfPoints( nx * ny * nz );

  vtkIdType pointCounter = itk::NumericTraits< vtkIdType >::Zero;

  PointType p;
  for(unsigned int z=0; z<nz; z++)
    {
    CoordinateRepresentationType rz = z;
    p[2] = rz;  
    for(unsigned int y=0; y<ny; y++)
      {
      CoordinateRepresentationType ry = y;
      ry /= fac;
      ry -= ny/(2*fac)+0.5;
      p[1] = ry;
      for(unsigned int x=0; x<nx; x++)
        {
        CoordinateRepresentationType rx = x;
        rx /= fac;
        rx -= nx/fac + deltax;
        p[0] = rx;
        m_PointsToTransform.push_back( p );
        m_VTKPointsToTransform->SetPoint( pointCounter, p[0], p[1], p[2] );
        m_VTKLinesToTransform->InsertNextCell( VTK_VERTEX, &pointCounter );
        pointCounter++;
        }
      }
    }

}



 
void
ThinPlateSplinesApplicationBase
::DisplayPoints(void)
{

  vtkUnstructuredGrid * sourceGrid = vtkUnstructuredGrid::New();
  vtkUnstructuredGrid * targetGrid = vtkUnstructuredGrid::New();

  sourceGrid->SetPoints( m_VTKPointsToTransform );
  targetGrid->SetPoints( m_VTKPointsTransformed );

  const unsigned int numberOfCells = m_VTKLinesToTransform->GetNumberOfCells(); 
  int * cellTypes = new int[ numberOfCells ];
  for(unsigned int i=0; i<numberOfCells; i++)
    {
    cellTypes[i] = VTK_VERTEX;
    }

  sourceGrid->SetCells( cellTypes, m_VTKLinesToTransform );
  targetGrid->SetCells( cellTypes, m_VTKLinesTransformed );

  // map to graphics library
  vtkDataSetMapper * sourceMapper   = vtkDataSetMapper::New();
  sourceMapper->SetInput( sourceGrid );

  vtkDataSetMapper * targetMapper   = vtkDataSetMapper::New();
  targetMapper->SetInput( targetGrid );

  // actor coordinates geometry, properties, transformation
  vtkActor * sourceActor = vtkActor::New();
  sourceActor->SetMapper( sourceMapper );
  sourceActor->GetProperty()->SetRepresentationToPoints();
  sourceActor->GetProperty()->SetColor(0,0,1); 

  // actor coordinates geometry, properties, transformation
  vtkActor * targetActor = vtkActor::New();
  targetActor->SetMapper( targetMapper );
  targetActor->GetProperty()->SetRepresentationToPoints();
  targetActor->GetProperty()->SetColor(1,0,0); 

  // add the actor to the scene
  m_Renderer->AddActor( sourceActor );
  m_Renderer->AddActor( targetActor );

  m_ActorsToDelete.insert( sourceActor );
  m_ActorsToDelete.insert( targetActor );

  sourceMapper->Delete();
  sourceGrid->Delete();

  targetMapper->Delete();
  targetGrid->Delete();
  
  delete [] cellTypes;

}




