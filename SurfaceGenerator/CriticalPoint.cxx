
#include "CriticalPoint.h"
#include "FL/gl.h"

CriticalPoint::
CriticalPoint()
{
}



CriticalPoint::
~CriticalPoint()
{
}



void
CriticalPoint::
Draw( const PointType & position ) const
{

  glPushMatrix();

  glTranslated( position[0], position[1], position[2] );
  glBegin(GL_LINES);
  
  // Normal
  glColor3f( 0, 0, 0 );
  glVertex3d( 0.0f, 0.0f, 0.0f );
  glVertex3d( m_Normal[0], m_Normal[1], m_Normal[2] );

  // Draw principal directions on the surface
  /*
  const double scale = 1.0;
  glVertex3d( m_MaximumDirection[0] * scale, 
              m_MaximumDirection[1] * scale, 
              m_MaximumDirection[2] * scale );
  
  glVertex3d( -m_MaximumDirection[0] * scale, 
              -m_MaximumDirection[1] * scale, 
              -m_MaximumDirection[2] * scale );
  
  glColor3f( 0, 0, 0 );
  //glVertex3d( 0.0f, 0.0f, 0.0f );
  glVertex3d( m_MinimumDirection[0] * scale, 
              m_MinimumDirection[1] * scale, 
              m_MinimumDirection[2] * scale );
  
  glVertex3d( -m_MinimumDirection[0] * scale, 
              -m_MinimumDirection[1] * scale, 
              -m_MinimumDirection[2] * scale );
  
  // Draw normals in the Umblic points
  const double tolerance = 1e-3;
  if( fabs ( m_MinimumCurvature - m_MaximumCurvature ) < tolerance )
    {
    glColor3f( 0, 0, 1 );
    glVertex3d( 0.0f, 0.0f, 0.0f );
    glVertex3d( m_Normal[0], m_Normal[1], m_Normal[2] );
    }
  */


  
  // Evolute Surface
  glColor3f( 1, 0, 0 );
  glVertex3d( - m_Normal[0] / m_MaximumCurvature,
              - m_Normal[1] / m_MaximumCurvature,
              - m_Normal[2] / m_MaximumCurvature);

  glVertex3d( - m_Normal[0] / m_MaximumCurvature +  m_MaximumDirection[0],
              - m_Normal[1] / m_MaximumCurvature +  m_MaximumDirection[1],
              - m_Normal[2] / m_MaximumCurvature +  m_MaximumDirection[2] );

  glColor3f( 0, 0, 1 );
  glVertex3d( - m_Normal[0] / m_MinimumCurvature,
              - m_Normal[1] / m_MinimumCurvature,
              - m_Normal[2] / m_MinimumCurvature);

  glVertex3d( - m_Normal[0] / m_MinimumCurvature +  m_MinimumDirection[0],
              - m_Normal[1] / m_MinimumCurvature +  m_MinimumDirection[1],
              - m_Normal[2] / m_MinimumCurvature +  m_MinimumDirection[2] );
  

  glEnd();

  glPopMatrix();

}





