//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------



#include <liC_Arm3D.h>
#include <vcl_cmath.h>

namespace li {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
C_Arm3D::C_Arm3D() 
{
  m_Width           = 0.1f;
  m_Radius          = 1.0f;
  m_RadialWidth     = 0.1f;
  SetNumberOfSectors(10);  // allocate points
}



//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
C_Arm3D::~C_Arm3D() 
{

}



//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
C_Arm3D::SetRadius( double radius ) 
{
  m_Radius = radius;
  SetupGeometry();
}


//--------------------------------------------------
//
//    Set Width
//
//--------------------------------------------------
void 
C_Arm3D::SetWidth( double width ) 
{
  m_Width = width;
  SetupGeometry();
}



//--------------------------------------------------
//
//    Set RadialWidth
//
//--------------------------------------------------
void 
C_Arm3D::SetRadialWidth( double radialWidth ) 
{
  m_RadialWidth = radialWidth;
  SetupGeometry();
}



//--------------------------------------------------
//
//    Set Number of Sectors
//
//--------------------------------------------------
void 
C_Arm3D::SetNumberOfSectors( unsigned int numberOfSectors ) 
{
  m_NumberOfSectors = numberOfSectors;
  const unsigned int numberOfPoints = 4 * ( numberOfSectors + 1 );

  if( m_Points.size() != numberOfPoints )  
  {
    m_Points.resize( numberOfPoints );
  }
  SetupGeometry();
}




//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
C_Arm3D::GetRadius( void ) const
{
  return m_Radius;
}




//--------------------------------------------------
//
//    Get Width
//
//--------------------------------------------------
double 
C_Arm3D::GetWidth( void ) const
{
  return m_Width;
}




//--------------------------------------------------
//
//    Get RadialWidth
//
//--------------------------------------------------
double 
C_Arm3D::GetRadialWidth( void ) const
{
  return m_RadialWidth;
}




//--------------------------------------------------
//
//    Get NumberOfSectors
//
//--------------------------------------------------
unsigned int
C_Arm3D::GetNumberOfSectors( void ) const
{
  return m_NumberOfSectors;
}






//--------------------------------------------------
//
//    Draw the geometry using OpenGL commands
//
//--------------------------------------------------
void C_Arm3D::DrawGeometry(void) const 
{

  unsigned int a = 2;  // indices for the triangle strip
  unsigned int b = 1;

  const unsigned int n = m_NumberOfSectors;

  PointType pa = m_Points[a];
  PointType pb = m_Points[b];

  // External band
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3d( pa[0], pa[1], pa[2] );
  glVertex3d( pa[0], pa[1], pa[2] );
  glNormal3d( pb[0], pb[1], pb[2] );
  glVertex3d( pb[0], pb[1], pb[2] );
  {
    for(unsigned int i=0; i<n; i++) 
    {
      a+=4;
      b+=4;
      pa = m_Points[a];
      pb = m_Points[b];
      glNormal3d( pa[0], pa[1], pa[2] );
      glVertex3d( pa[0], pa[1], pa[2] );
      glNormal3d( pb[0], pb[1], pb[2] );
      glVertex3d( pb[0], pb[1], pb[2] );
    }
  }

  glEnd();

  // Internal band
  glBegin(GL_TRIANGLE_STRIP);
  a = 0;
  b = 3;
  pa = m_Points[a];
  pb = m_Points[b];
  glNormal3d( -pa[0], -pa[1], -pa[2] );
  glVertex3d(  pa[0],  pa[1],  pa[2] );
  glNormal3d( -pb[0], -pb[1], -pb[2] );
  glVertex3d(  pb[0],  pb[1],  pb[2] );
  {
    for(unsigned int i=0; i<n; i++) 
    {
      a+=4;
      b+=4;
      pa = m_Points[a];
      pb = m_Points[b];
      glNormal3d( -pa[0], -pa[1], -pa[2] );
      glVertex3d(  pa[0],  pa[1],  pa[2] );
      glNormal3d( -pb[0], -pb[1], -pb[2] );
      glVertex3d(  pb[0],  pb[1],  pb[2] );
    }
  }

  glEnd();

  // Caudal band
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3d(-1.0,0.0,0.0);
  a = 1;
  b = 0;
  pa = m_Points[a];
  pb = m_Points[b];
  glVertex3d(  pa[0],  pa[1],  pa[2] );
  glVertex3d(  pb[0],  pb[1],  pb[2] );
  {
    for(unsigned int i=0; i<n; i++) 
    {
      a+=4;
      b+=4;
      pa = m_Points[a];
      pb = m_Points[b];
      glVertex3d(  pa[0],  pa[1],  pa[2] );
      glVertex3d(  pb[0],  pb[1],  pb[2] );
    }
  }

  glEnd();

  // Cranial band
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3d(1.0,0.0,0.0);
  a = 3;
  b = 2;
  pa = m_Points[a];
  pb = m_Points[b];
  glVertex3d(  pa[0],  pa[1],  pa[2] );
  glVertex3d(  pb[0],  pb[1],  pb[2] );
  {
    for(unsigned int i=0; i<n; i++) 
    {
      a+=4;
      b+=4;
      pa = m_Points[a];
      pb = m_Points[b];
      glVertex3d(  pa[0],  pa[1],  pa[2] );
      glVertex3d(  pb[0],  pb[1],  pb[2] );
    }
  }

  glEnd();


}



   
//--------------------------------------------------
//
//    Rebuild the Representation
//
//--------------------------------------------------
void 
C_Arm3D::SetupGeometry(void) 
{

   const double w2 = (double)m_Width/2.0f;
   const unsigned long n = m_NumberOfSectors;
   const double factor = 4.0*atan(1.0f)/(double)n;

   for(unsigned int i=0; i<n+1; i++) 
   {
     int j = i<<2;
     const double   th = (double)i*factor;
     const double y1 = m_Radius * cos( th );
     const double z1 = m_Radius * sin( th );
     const double y2 = ( m_Radius + m_RadialWidth )*cos( th );
     const double z2 = ( m_Radius + m_RadialWidth )*sin( th );
     m_Points[j++] = -w2, y1, z1;
     m_Points[j++] = -w2, y2, z2;
     m_Points[j++] =  w2, y2, z2;
     m_Points[j++] =  w2, y1, z1;
   }

  ScheduleToUpdateDisplayList();

}



   


}  // end namespace li
