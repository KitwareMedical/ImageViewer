


#include "SurfaceGenerator.h"
#include <FL/fl_file_chooser.H>


SurfaceGenerator::
SurfaceGenerator()
{
}


SurfaceGenerator::
~SurfaceGenerator()
{
}

void SurfaceGenerator::
Load(void) {

  const char *filename = fl_file_chooser("Curve file","*.crv","");
    
  if( !filename ) return;
    
  SurfaceGeneratorBase::Load(filename);

  //loadNodesInBrowser();
  //plotWindow->redraw();

}

void SurfaceGenerator::
glDrawEllipsoid() {
  
  // Fake data for having an initial state for the Mesh
  PointType p1;
  PointType p2;
  PointType p3;

  CriticalPoint cp1;
  CriticalPoint cp2;
  CriticalPoint cp3;

  p1[0] =  10.0f;
  p1[1] =   0.0f;
  p1[2] =   0.0f;

  p2[0] =   0.0f;
  p2[1] =  10.0f;
  p2[2] =   0.0f;

  p3[0] =   0.0f;
  p3[1] =   0.0f;
  p3[2] =  10.0f;

  // Normals
  CriticalPoint::NormalType normal1;
  CriticalPoint::NormalType normal2;
  CriticalPoint::NormalType normal3;

  normal1[0] = 1.0f;
  normal1[1] = 0.0f;
  normal1[2] = 0.0f;

  normal2[0] = 0.0f;
  normal2[1] = 1.0f;
  normal2[2] = 0.0f;

  normal3[0] = 0.0f;
  normal3[1] = 0.0f;
  normal3[2] = 1.0f;

  cp1.SetNormal( normal1 );
  cp2.SetNormal( normal2 );
  cp3.SetNormal( normal3 );

  // Maximum directions
  CriticalPoint::VectorType  maximumDirection1;
  CriticalPoint::VectorType  maximumDirection2;
  CriticalPoint::VectorType  maximumDirection3;

  maximumDirection1[0] = 0.0f;
  maximumDirection1[1] = 1.0f;
  maximumDirection1[2] = 0.0f;

  maximumDirection2[0] = 0.0f;
  maximumDirection2[1] = 0.0f;
  maximumDirection2[2] = 1.0f;

  maximumDirection3[0] = 0.0f;
  maximumDirection3[1] = 1.0f;
  maximumDirection3[2] = 0.0f;


  cp1.SetMaximumDirection( maximumDirection1 );
  cp2.SetMaximumDirection( maximumDirection2 );
  cp3.SetMaximumDirection( maximumDirection3 );

  // Minimum directions
  CriticalPoint::VectorType  minimumDirection1;
  CriticalPoint::VectorType  minimumDirection2;
  CriticalPoint::VectorType  minimumDirection3;

  minimumDirection1[0] = 0.0f;
  minimumDirection1[1] = 0.0f;
  minimumDirection1[2] = 1.0f;

  minimumDirection2[0] = 1.0f;
  minimumDirection2[1] = 0.0f;
  minimumDirection2[2] = 0.0f;

  minimumDirection3[0] = 1.0f;
  minimumDirection3[1] = 0.0f;
  minimumDirection3[2] = 0.0f;

  cp1.SetMinimumDirection( minimumDirection1 );
  cp2.SetMinimumDirection( minimumDirection2 );
  cp3.SetMinimumDirection( minimumDirection3 );

  // Maximum curvatures
  CriticalPoint::CurvatureType maximumCurvature1;
  CriticalPoint::CurvatureType maximumCurvature2;
  CriticalPoint::CurvatureType maximumCurvature3;

  maximumCurvature1 = 5.0f;
  maximumCurvature2 = 4.0f;
  maximumCurvature3 = 3.0f;

  cp1.SetMaximumCurvature( maximumCurvature1 );
  cp2.SetMaximumCurvature( maximumCurvature2 );
  cp3.SetMaximumCurvature( maximumCurvature3 );

  // Minimum curvatures
  CriticalPoint::CurvatureType minimumCurvature1;
  CriticalPoint::CurvatureType minimumCurvature2;
  CriticalPoint::CurvatureType minimumCurvature3;

  minimumCurvature1 = 4.0f;
  minimumCurvature2 = 3.0f;
  minimumCurvature3 = 2.0f;

  cp1.SetMinimumCurvature( minimumCurvature1 );
  cp2.SetMinimumCurvature( minimumCurvature2 );
  cp3.SetMinimumCurvature( minimumCurvature3 );

  IdentifierType pointId = 1;

  cp1.SetSelfIdentifier( pointId );
  m_Mesh->SetPoint( pointId, p1 );
  m_Mesh->SetPointData( pointId, cp1 );     
  pointId++;

  cp2.SetSelfIdentifier( pointId );
  m_Mesh->SetPoint( pointId, p2 );
  m_Mesh->SetPointData( pointId, cp2 );     
  pointId++;

  cp3.SetSelfIdentifier( pointId );
  m_Mesh->SetPoint( pointId, p3 );
  m_Mesh->SetPointData( pointId, cp3 );     
  pointId++;
/**/

  



}

  

