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

#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
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

  m_VTKPointsTransformedByITK = vtkPoints::New();
  m_VTKLinesTransformedByITK  = vtkCellArray::New();

  m_VTKPointsTransformedByVTK = vtkPoints::New();
  m_VTKLinesTransformedByVTK  = vtkCellArray::New();

  m_ThinPlateSplineTransformITK = ThinPlateSplineTransformType::New();

  m_ThinPlateR2LogRSplineTransformITK = ThinPlateR2LogRSplineTransformType::New();

  m_ElasticBodySplineTransformITK = ElasticBodySplineTransformType::New();

  m_ElasticBodyReciprocalSplineTransformITK = ElasticBodyReciprocalSplineTransformType::New();

  m_VolumeSplineTransformITK = VolumeSplineTransformType::New();

  m_ThinPlateSplineTransformVTK = vtkThinPlateSplineTransform::New();

}




ThinPlateSplinesApplicationBase
::~ThinPlateSplinesApplicationBase()
{

  m_TimeCollector.Hide();
  
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

  vtkIdType pointId = 0;

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
  sourceActor->GetProperty()->SetPointSize(2); 

  // actor coordinates geometry, properties, transformation
  vtkActor * targetActor = vtkActor::New();
  targetActor->SetMapper( targetMapper );
  targetActor->GetProperty()->SetColor(1,0,0); 
  targetActor->GetProperty()->SetPointSize(2); 

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
  this->MapPointsITK();
  this->MapPointsVTK();
}
 
  
 
void
ThinPlateSplinesApplicationBase
::MapPointsVTK(void)
{
 
  m_ThinPlateSplineTransformVTK->SetSourceLandmarks( m_VTKSourceLandMarks );
  m_ThinPlateSplineTransformVTK->SetTargetLandmarks( m_VTKTargetLandMarks );

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_VTKPointsTransformedByVTK->Delete();
  m_VTKPointsTransformedByVTK = vtkPoints::New();

  m_VTKLinesTransformedByVTK->Delete();
  m_VTKLinesTransformedByVTK = vtkCellArray::New();

  m_VTKPointsTransformedByVTK->SetNumberOfPoints( m_PointsToTransform.size() );

  m_TimeCollector.Start("VTK Thin Plate Spline");

  vtkIdType pointCounter = 0;
  while( point != end )
    {
    PointType & p = *point;
    m_VTKPointsTransformedByVTK->SetPoint( pointCounter, 
         m_ThinPlateSplineTransformVTK->TransformPoint( p[0], p[1], p[2] )  );
    ++point;
    ++pointCounter;
    }

  m_TimeCollector.Stop("VTK Thin Plate Spline");

  pointCounter = 0;
  for( unsigned int i=0; i<m_PointsToTransform.size(); i++ )
    {
    m_VTKLinesTransformedByVTK->InsertNextCell( VTK_VERTEX, &pointCounter );
    ++pointCounter;
    }

}

 


void
ThinPlateSplinesApplicationBase
::MapPointsITK(void)
{
  // overloaded in the derived class
}
  



void
ThinPlateSplinesApplicationBase
::MapPointsThinPlateSplineITK(void)
{
  m_ThinPlateSplineTransformITK->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_ThinPlateSplineTransformITK->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_ThinPlateSplineTransformITK->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformedByITK.clear();

  m_TimeCollector.Start("ITK Thin Plate Spline");

  while( point != end )
    {
    m_PointsTransformedByITK.push_back( 
                  m_ThinPlateSplineTransformITK->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK Thin Plate Spline");

  this->ConvertITKMappedPointsToVTK();

}

void
ThinPlateSplinesApplicationBase
::MapPointsThinPlateR2LogRSplineITK(void)
{
  m_ThinPlateR2LogRSplineTransformITK->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_ThinPlateR2LogRSplineTransformITK->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_ThinPlateR2LogRSplineTransformITK->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformedByITK.clear();

  m_TimeCollector.Start("ITK Thin Plate R2LogR Spline");

  while( point != end )
    {
    m_PointsTransformedByITK.push_back( 
         m_ThinPlateR2LogRSplineTransformITK->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK Thin Plate R2LogR Spline");

  this->ConvertITKMappedPointsToVTK();

}



void
ThinPlateSplinesApplicationBase
::MapPointsElasticBodyReciprocalSplineITK(void)
{
 
  m_ElasticBodyReciprocalSplineTransformITK->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_ElasticBodyReciprocalSplineTransformITK->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_ElasticBodyReciprocalSplineTransformITK->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformedByITK.clear();

  m_TimeCollector.Start("ITK Elastic Body Reciprocal Spline");

  while( point != end )
    {
    m_PointsTransformedByITK.push_back( 
      m_ElasticBodyReciprocalSplineTransformITK->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK Elastic Body Reciprocal Spline");

  this->ConvertITKMappedPointsToVTK();

}


void
ThinPlateSplinesApplicationBase
::MapPointsElasticBodySplineITK(void)
{
 
  m_ElasticBodySplineTransformITK->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_ElasticBodySplineTransformITK->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_ElasticBodySplineTransformITK->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformedByITK.clear();

  m_TimeCollector.Start("ITK Elastic Body Spline");

  while( point != end )
    {
    m_PointsTransformedByITK.push_back( 
                  m_ElasticBodySplineTransformITK->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK Elastic Body Spline");

  this->ConvertITKMappedPointsToVTK();

}



void
ThinPlateSplinesApplicationBase
::MapPointsVolumeSplineITK(void)
{
 
  m_VolumeSplineTransformITK->SetSourceLandmarks( m_SourceLandMarks.GetPointer() );
  m_VolumeSplineTransformITK->SetTargetLandmarks( m_TargetLandMarks.GetPointer() );
  m_VolumeSplineTransformITK->ComputeWMatrix();

  PointArrayType::iterator point = m_PointsToTransform.begin();
  PointArrayType::iterator end   = m_PointsToTransform.end();

  m_PointsTransformedByITK.clear();

  m_TimeCollector.Start("ITK Volume Spline");

  while( point != end )
    {
    m_PointsTransformedByITK.push_back( 
                  m_VolumeSplineTransformITK->TransformPoint( *point )
                  );
    ++point;
    }

  m_TimeCollector.Stop("ITK Volume Spline");

  this->ConvertITKMappedPointsToVTK();

}


void
ThinPlateSplinesApplicationBase
::ConvertITKMappedPointsToVTK(void)
{

  // Convert ITK transformed points to a VTK structure for visualization
  m_VTKPointsTransformedByITK->Delete();
  m_VTKPointsTransformedByITK = vtkPoints::New();

  m_VTKLinesTransformedByITK->Delete();
  m_VTKLinesTransformedByITK = vtkCellArray::New();

  m_VTKPointsTransformedByITK->SetNumberOfPoints( m_PointsTransformedByITK.size() );

  PointArrayType::iterator point = m_PointsTransformedByITK.begin();
  PointArrayType::iterator end   = m_PointsTransformedByITK.end();

  vtkIdType pointCounter = 0;

  while( point != end )
    {
    const PointType p = *point;
    m_VTKPointsTransformedByITK->SetPoint( pointCounter, p[0], p[1], p[2] );
    m_VTKLinesTransformedByITK->InsertNextCell( VTK_VERTEX, &pointCounter );
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

  vtkIdType pointCounter = 0;

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

  vtkUnstructuredGrid * sourceGrid      = vtkUnstructuredGrid::New();
  vtkUnstructuredGrid * targetGridByVTK = vtkUnstructuredGrid::New();
  vtkUnstructuredGrid * targetGridByITK = vtkUnstructuredGrid::New();

  sourceGrid->SetPoints( m_VTKPointsToTransform );
  targetGridByITK->SetPoints( m_VTKPointsTransformedByITK );
  targetGridByVTK->SetPoints( m_VTKPointsTransformedByVTK );

  const unsigned int numberOfCells = m_VTKLinesToTransform->GetNumberOfCells(); 
  int * cellTypes = new int[ numberOfCells ];
  for(unsigned int i=0; i<numberOfCells; i++)
    {
    cellTypes[i] = VTK_VERTEX;
    }

  sourceGrid->SetCells( cellTypes, m_VTKLinesToTransform );
  targetGridByVTK->SetCells( cellTypes, m_VTKLinesTransformedByVTK );
  targetGridByITK->SetCells( cellTypes, m_VTKLinesTransformedByITK );

  // map to graphics library
  vtkDataSetMapper * sourceMapper   = vtkDataSetMapper::New();
  sourceMapper->SetInput( sourceGrid );

  vtkDataSetMapper * targetMapperByITK   = vtkDataSetMapper::New();
  targetMapperByITK->SetInput( targetGridByITK );

  vtkDataSetMapper * targetMapperByVTK   = vtkDataSetMapper::New();
  targetMapperByVTK->SetInput( targetGridByVTK );

  // actor coordinates geometry, properties, transformation
  vtkActor * sourceActor = vtkActor::New();
  sourceActor->SetMapper( sourceMapper );
  sourceActor->GetProperty()->SetRepresentationToPoints();
  sourceActor->GetProperty()->SetColor(0,0,1); 

  // actor coordinates geometry, properties, transformation
  vtkActor * targetActorByVTK = vtkActor::New();
  targetActorByVTK->SetMapper( targetMapperByVTK );
  targetActorByVTK->GetProperty()->SetRepresentationToPoints();
  targetActorByVTK->GetProperty()->SetColor(0,1,0); 

  // actor coordinates geometry, properties, transformation
  vtkActor * targetActorByITK = vtkActor::New();
  targetActorByITK->SetMapper( targetMapperByITK );
  targetActorByITK->GetProperty()->SetRepresentationToPoints();
  targetActorByITK->GetProperty()->SetColor(1,0,1); 

  // add the actor to the scene
  m_Renderer->AddActor( sourceActor );
  m_Renderer->AddActor( targetActorByVTK );
  m_Renderer->AddActor( targetActorByITK );

  m_ActorsToDelete.insert( sourceActor );
  m_ActorsToDelete.insert( targetActorByITK );
  m_ActorsToDelete.insert( targetActorByVTK );

  sourceMapper->Delete();
  sourceGrid->Delete();

  targetMapperByITK->Delete();
  targetMapperByVTK->Delete();

  targetGridByITK->Delete();
  targetGridByVTK->Delete();
  
  delete [] cellTypes;

}




