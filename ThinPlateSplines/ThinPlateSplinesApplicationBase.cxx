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


#include "itkNumericTraits.h"




ThinPlateSplinesApplicationBase
::ThinPlateSplinesApplicationBase()
{
  m_RenderWindow = vtkRenderWindow::New();

  m_Renderer = 0;
  this->CreateRenderer();
  
  m_VTKSourceLandMarks   = vtkPoints::New();
  m_VTKTargetLandMarks   = vtkPoints::New();
  m_VTKPointsToTransform = vtkPoints::New();
  m_VTKLinesToTransform  = vtkCellArray::New();

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

  m_SourceLandMarks.clear();
  m_TargetLandMarks.clear();

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
        m_SourceLandMarks.push_back( p );
        rx  =  x;
        rx  += deltax;
        p[0] = rx;
        m_TargetLandMarks.push_back( p );
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

  const unsigned int numberOfLandMarks = m_SourceLandMarks.size();

  if( numberOfLandMarks != m_TargetLandMarks.size() )
    {
    itkGenericExceptionMacro( << "Number of Source Landmarks != Target Landmarks " );
    }

  m_VTKSourceLandMarks->Allocate( numberOfLandMarks );
  m_VTKTargetLandMarks->Allocate( numberOfLandMarks );

  vtkIdType pointId = itk::NumericTraits< vtkIdType >::Zero; 

  PointArrayType::iterator tlm = m_TargetLandMarks.begin();
  PointArrayType::iterator slm = m_SourceLandMarks.begin();
  PointArrayType::iterator end = m_SourceLandMarks.end();
  while( slm != end )
    {
    m_VTKSourceLandMarks->InsertNextPoint( (*slm)[0],(*slm)[1],(*slm)[2] );
    m_VTKTargetLandMarks->InsertNextPoint( (*tlm)[0],(*tlm)[1],(*tlm)[2] );
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
   
  this->RemoveActors();

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

  m_PointsToTransform.clear();

  m_VTKPointsToTransform->Delete();
  m_VTKPointsToTransform = vtkPoints::New();

  m_VTKLinesToTransform->Delete();
  m_VTKLinesToTransform = vtkCellArray::New();

  const unsigned int fac = 2;

  const unsigned int nx =  6 * fac;
  const unsigned int ny =  6 * fac;
  const unsigned int nz =  1;

  CoordinateRepresentationType deltax = 1.0f;

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
      ry -= ny/(2*fac);
      p[1] = ry;
      vtkIdType endpoints[2];
      for(unsigned int x=0; x<nx; x++)
        {
        CoordinateRepresentationType rx = x;
        rx /= fac;
        rx -= nx/fac + deltax;
        p[0] = rx;
        m_PointsToTransform.push_back( p );
        m_VTKPointsToTransform->InsertNextPoint( p[0], p[1], p[2] );
        pointCounter++;
        if( x > 0 )
          {
          endpoints[0] = pointCounter-1;
          endpoints[1] = pointCounter;
std::cout << endpoints[0] << " -> " << endpoints[1] << std::endl;
          m_VTKLinesToTransform->InsertNextCell( VTK_LINE, endpoints );
          }
        }
      }
    }

}


 
void
ThinPlateSplinesApplicationBase
::DisplayPoints(void)
{

  vtkPolyData * sourcePolyData = vtkPolyData::New();
  sourcePolyData->SetPoints( m_VTKPointsToTransform );
  sourcePolyData->SetLines( m_VTKLinesToTransform );

  // map to graphics library
  vtkPolyDataMapper * sourceMapper   = vtkPolyDataMapper::New();
  sourceMapper->SetInput( sourcePolyData );

  vtkPolyData * targetPolyData = vtkPolyData::New();
//  targetPolyData->SetPoints( m_VTKPointsTransformed );

  // map to graphics library
  vtkPolyDataMapper * targetMapper   = vtkPolyDataMapper::New();
  targetMapper->SetInput( targetPolyData );

  // actor coordinates geometry, properties, transformation
  vtkActor * sourceActor = vtkActor::New();
  sourceActor->SetMapper( sourceMapper );
  sourceActor->GetProperty()->SetRepresentationToWireframe();
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
  sourcePolyData->Delete();

  targetMapper->Delete();
  targetPolyData->Delete();
  

}




