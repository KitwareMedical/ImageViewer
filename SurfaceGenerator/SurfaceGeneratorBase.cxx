#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif



#include "SurfaceGeneratorBase.h"
#include "FL/fl_file_chooser.H"

#include <iostream>  
//#include <fstream>
#include <cstdio>

#define PI 3.1415926535897932384626433832795


SurfaceGeneratorBase
::SurfaceGeneratorBase()
{

  m_Mesh = MeshType::New();

  m_Mesh->SetPoints( PointsContainer::New() );
  m_Mesh->SetPointData( PointDataContainer::New() );

  m_DrawCommand = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::Draw );

  m_Display.GetGlWindow()->SetBackground( 0.7, 0.8, 0.9 );
  m_Display.GetGlWindow()->SetZoom( 1.0 );
  m_Display.SetLabel("Surface Viewer");


  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             this->GetDrawCommand().GetPointer() );

  m_DisplayList = 0; 

}


SurfaceGeneratorBase
::~SurfaceGeneratorBase()
{
  this->HideDisplay();
}



/**
 *    Show the Display
 */ 
void 
SurfaceGeneratorBase
::ShowDisplay(void)
{
  m_Display.Show();
}





/**
 *    Hide the Display
 */ 
void 
SurfaceGeneratorBase
::HideDisplay(void)
{
  m_Display.Hide();
}






/**
 *    Load
 */ 
void 
SurfaceGeneratorBase
::Load(const char * filename)
{
/*  
  int c;
  PointType p;
  CriticalPoint cp;
  CriticalPoint::NormalType normal;
  CriticalPoint::VectorType  maximumDirection;
  CriticalPoint::VectorType  minimumDirection;
  CriticalPoint::CurvatureType minimumCurvature;
  CriticalPoint::CurvatureType maximumCurvature;

  IdentifierType pointId = 1;
  std::ifstream finp;
  finp.open(filename);
  if(finp.fail()) return;

  while( ! finp.eof() ) {
    finp >> p;

    finp >> normal[0];
    finp >> normal[1];
    finp >> normal[2];
    cp.SetNormal( normal );

    finp >> maximumDirection[0];
    finp >> maximumDirection[1];
    finp >> maximumDirection[2];
    cp.SetMaximumDirection( maximumDirection );

    finp >> minimumDirection[0];
    finp >> minimumDirection[1];
    finp >> minimumDirection[2];
    cp.SetMinimumDirection( minimumDirection );

    finp >> maximumCurvature;
    cp.SetMaximumCurvature( maximumCurvature );

    finp >> minimumCurvature;
    cp.SetMinimumCurvature( minimumCurvature );

    cp.SetSelfIdentifier( pointId );
    m_Mesh->SetPoint( pointId, p );
    m_Mesh->SetPointData( pointId, cp );     
    pointId++;
    //finp >> node.cusp;
    if( !finp.fail() ) 
    //nodes.push_back(node);
    c = 0;
  }

    finp.close();
*/
}





/**
 *    Generate an Ellipsoid
 */ 
void 
SurfaceGeneratorBase
::GenerateEllipsoid( double a, double b, double c )
{
  
  m_Mesh->SetPoints( PointsContainer::New() );
  m_Mesh->SetPointData( PointDataContainer::New() );

  if( m_DisplayList )
    {
    glDeleteLists( m_DisplayList, 1 );
    m_DisplayList = 0;
    }

  
  CriticalPoint cp;
  IdentifierType pointId = 1;

  // Ellipsoid's coefficients
  const unsigned int uStep = 100;
  const unsigned int vStep = 100;
  
  const double randomLevel = 0.01 / RAND_MAX;;

  for( unsigned int u=0; u<uStep; u++ )
    {
    for( unsigned int v=0; v<vStep; v++ )
      {

      const double cu = u * PI / ( 1.0 * uStep ) + randomLevel * rand();
      const double cv = v * PI / ( 0.5 * vStep ) + randomLevel * rand();

      const double x = a * sin( cu ) * sin( cv );
      const double y = b * sin( cu ) * cos( cv );
      const double z = c * cos( cu );

      PointType p;
      p[0] = x;
      p[1] = y;
      p[2] = z;

      const double a2 = a * a;
      const double b2 = b * b;
      const double c2 = c * c;

      const double ia = 2.0f / a2;
      const double ib = 2.0f / b2;
      const double ic = 2.0f / c2;

      const double ia2 = ia * ia;
      const double ib2 = ib * ib;
      const double ic2 = ic * ic;

      // Normals
      CriticalPoint::NormalType normal;

      normal[0] = x * ia;
      normal[1] = y * ib;
      normal[2] = z * ic;
      
      const double norm = normal.GetNorm();
      
      normal /= norm;

      cp.SetNormal( normal );

      const double x2 = x * x;
      const double y2 = y * y;
      const double z2 = z * z;

      const double alpha = ia2 * ( ib + ic ) * x2 +
                           ib2 * ( ia + ic ) * y2 +
                           ic2 * ( ia + ib ) * z2;
        
      const double bc = ib - ic;
      const double ac = ia - ic;
      const double ab = ia - ib;

      const double cb = ic - ib;
      const double ca = ic - ia;
      const double ba = ib - ia;

      const double beta  =  ia2*ia2 * bc*bc  * x2*x2 +
                            ib2*ib2 * ac*ac  * y2*y2 +
                            ic2*ic2 * ab*ab  * z2*z2 +
                            2 * ac * bc * ia2 * ib2 * x2 * y2 +
                            2 * ab * cb * ia2 * ic2 * x2 * z2 +
                            2 * ba * ca * ib2 * ic2 * y2 * z2 ;
      
      const double L = sqrt( ia2 * x2 + ib2 * y2 + ic2 * z2 );
      
      const double L3 = L * L * L;
      
      const double betaroot = sqrt( beta );

      const CriticalPoint::CurvatureType   k1 = (alpha + betaroot) / ( 2.0 * L3 );
      const CriticalPoint::CurvatureType   k2 = (alpha - betaroot) / ( 2.0 * L3 );

      // Curvatures
      cp.SetMaximumCurvature( k1 );
      cp.SetMinimumCurvature( k2 );

      /*
      std::cout << "====================" << std::endl;
      std::cout << "Point  = " << x << "  " << y << "  " << z << std::endl;
      std::cout << "Normal = " << normal << std::endl;
      std::cout << "Alpha  = " << alpha  << std::endl;
      std::cout << "Beta   = " << beta   << std::endl;
      std::cout << "Norm   = " << norm   << std::endl;
      std::cout << "L      = " << L      << std::endl;
      std::cout << "K1     = " << k1     << std::endl;
      std::cout << "K2     = " << k2     << std::endl;
      */
      

      CriticalPoint::VectorType  maximumDirection;
      CriticalPoint::VectorType  minimumDirection;

      minimumDirection[0]  = ia * ib * (  ic * x * z );
      minimumDirection[1]  = ia * ib * (   ic * y * z );
      minimumDirection[2]  = ia * ib * ( - ia * x2 - ib * y2 );
       
      minimumDirection[0] += L * k1 * ( ia * ic * x * z );
      minimumDirection[1] += L * k1 * ( ib * ic * y * z );
      minimumDirection[2] += L * k1 * ( - ia2 * x2 - ib2 * y2 );


      maximumDirection[0] = ia * ib * ( -y * ( ia*ib*x2 + ib2*y2 + ic2*z2 ));
      maximumDirection[1] = ia * ib * (  x * ( ia2*x2 + ia*ib*y2 + ic2*z2 ));
      maximumDirection[2] = ia * ib * ( ic * ( ab ) * x * y * z );
         
      maximumDirection[0] += L3 * k1 * ( -ib * y );
      maximumDirection[1] += L3 * k1 * (  ia * x );
      maximumDirection[2] += 0.0;

      
      maximumDirection.Normalize();
      minimumDirection.Normalize();

      if( fabs ( maximumDirection * minimumDirection ) > 1e-4 || 
          fabs ( normal           * maximumDirection ) > 1e-4 || 
          fabs ( normal           * minimumDirection ) > 1e-4 )
        {
        std::cout << "Not completly orthogonal ";
        std::cout << normal << "  " << maximumDirection;
        std::cout << "  " << minimumDirection << std::endl;  
        }

      cp.SetMaximumDirection( maximumDirection );
      cp.SetMinimumDirection( minimumDirection );

      cp.SetSelfIdentifier( pointId );
      m_Mesh->SetPoint( pointId, p );
      m_Mesh->SetPointData( pointId, cp );     
      pointId++;

    }
  }

  m_Display.Redraw();

}





/**
 *    Generate a Patch
 */ 
void 
SurfaceGeneratorBase
::GeneratePatch(void) 
{

  m_Mesh->SetPoints( PointsContainer::New() );
  m_Mesh->SetPointData( PointDataContainer::New() );

  // Fake data for having an initial state for the Mesh
  PointType p1;
  PointType p2;
  PointType p3;

  CriticalPoint cp1;
  CriticalPoint cp2;
  CriticalPoint cp3;

  p1[0] = 10.0f;
  p1[1] =  0.0f;
  p1[2] =  0.0f;

  p2[0] =  0.0f;
  p2[1] = 10.0f;
  p2[2] =  0.0f;

  p3[0] =  0.0f;
  p3[1] =  0.0f;
  p3[2] = 10.0f;

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

  m_Display.Redraw();

}




/**
 *    Save
 */ 
void 
SurfaceGeneratorBase
::Save(void) const
{

}




/**
 *    Return the Draw command
 */ 
SurfaceGeneratorBase::DrawCommandPointer 
SurfaceGeneratorBase
::GetDrawCommand(void)
{
  return m_DrawCommand;
}

  

void
SurfaceGeneratorBase
::Draw(void) const
{  

  bool creatingDisplayList = false;
  
  if( m_DisplayList )
    {
    glCallList( m_DisplayList );
    return;
    }
  else
    {
    m_DisplayList = glGenLists(1);
    glNewList( m_DisplayList, GL_COMPILE_AND_EXECUTE );
    creatingDisplayList = true;
    }

  PointType position;

  PointDataConstIterator criticalIt = m_Mesh->GetPointData()->Begin();
  PointDataConstIterator end        = m_Mesh->GetPointData()->End();

  while( criticalIt != end )
    {
    CriticalPoint criticalPoint = criticalIt.Value();
    const IdentifierType id = criticalPoint.GetSelfIdentifier();
    m_Mesh->GetPoint( id, &position );
    criticalPoint.Draw( position );
    criticalIt++;
    }

  if( creatingDisplayList )
    {
    glEndList();
    }
}



