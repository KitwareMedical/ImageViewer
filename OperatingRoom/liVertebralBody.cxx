/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralBody.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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


#include "liVertebralBody.h"


namespace li {


//----------------------------------------------------
//
//    Constructor
//
//----------------------------------------------------
VertebralBody::VertebralBody() 
{

  m_SectorsNumber               = 6;
  m_LevelsNumber                = 5;
  m_UpperEndYRadius             = 15.25;
  m_UpperEndXRadius             = 16.0;
  m_UpperEndYConcavityDepth     = 0.4;
  m_UpperEndYConcavityWidth     = 0.4;
  m_UpperEndInclination         = -0.5;
  m_LowerEndYRadius             = 15.8;
  m_LowerEndXRadius             = 17.7;
  m_LowerEndYConcavityDepth     = 0.4;
  m_LowerEndYConcavityWidth     = 0.4;
  m_LowerEndInclination         = 2.2;
  m_MiddleYRadius               = 0.8;
  m_MiddleXRadius               = 0.8;
  m_Height                      = 20.2;
  
  GeneratePoints();
  
  fltk::Shape3D::IfstreamType ifx;
  ifx.open("hi");
  ifx.close();
}





//----------------------------------------------------
//
//    Destructor
//
//----------------------------------------------------
VertebralBody::~VertebralBody() 
{
}




//----------------------------------------------------
//
//    Set Sectors Number
//
//----------------------------------------------------
void VertebralBody::SetSectorsNumber(unsigned int num) 
{
  m_SectorsNumber = num;
}



//----------------------------------------------------
//
//    Set Levels Number
//
//----------------------------------------------------
void VertebralBody::SetLevelsNumber(unsigned int num) 
{
  m_LevelsNumber = num;
}



//----------------------------------------------------
//
//    Set Upper End X Radius
//
//----------------------------------------------------
void VertebralBody::SetUpperEndXRadius(double val) 
{
  m_UpperEndXRadius = val;
}


//----------------------------------------------------
//
//    Set Upper End Y Radius
//
//----------------------------------------------------
void VertebralBody::SetUpperEndYRadius(double val) 
{
  m_UpperEndYRadius = val;
}


//----------------------------------------------------
//
//    Set Upper End Y Concavity Depth
//
//----------------------------------------------------
void VertebralBody::SetUpperEndYConcavityDepth(double val) 
{
  m_UpperEndYConcavityDepth = val;
}


//----------------------------------------------------
//
//    Set Upper End Y Concavity Width
//
//----------------------------------------------------
void VertebralBody::SetUpperEndYConcavityWidth(double val) 
{
  m_UpperEndYConcavityWidth = val;
}


//----------------------------------------------------
//
//    Set Upper End Inclination
//
//----------------------------------------------------
void VertebralBody::SetUpperEndInclination(double val) 
{
  m_UpperEndInclination = val;
}


//----------------------------------------------------
//
//    Set Lower End Y Radius
//
//----------------------------------------------------
void VertebralBody::SetLowerEndYRadius(double val) 
{
  m_LowerEndYRadius = val;
}


//----------------------------------------------------
//
//    Set Lower End X Radius
//
//----------------------------------------------------
void VertebralBody::SetLowerEndXRadius(double val) 
{
  m_LowerEndXRadius = val;
}


//----------------------------------------------------
//
//    Set Lower End Y Concavity Depth
//
//----------------------------------------------------
void VertebralBody::SetLowerEndYConcavityDepth(double val) 
{
  m_LowerEndYConcavityDepth = val;
}


//----------------------------------------------------
//
//    Set Lower End Y Concavity Width
//
//----------------------------------------------------
void VertebralBody::SetLowerEndYConcavityWidth(double val) 
{
  m_LowerEndYConcavityWidth = val;
}


//----------------------------------------------------
//
//    Set Lower End Inclination
//
//----------------------------------------------------
void VertebralBody::SetLowerEndInclination(double val) 
{
  m_LowerEndInclination = val;
}



//----------------------------------------------------
//
//    Set Middle Y Radius
//
//----------------------------------------------------
void VertebralBody::SetMiddleYRadius(double val) 
{
  m_MiddleYRadius = val;
}


//----------------------------------------------------
//
//    Set Middle X Radius
//
//----------------------------------------------------
void VertebralBody::SetMiddleXRadius(double val) 
{
  m_MiddleXRadius = val;
}



//----------------------------------------------------
//
//    Set Height
//
//----------------------------------------------------
void VertebralBody::SetHeight(double val) 
{
  m_Height = val;
}



//----------------------------------------------------
//
//    Get Sectors Number
//
//----------------------------------------------------
unsigned int VertebralBody::GetSectorsNumber(void) const
{
  return m_SectorsNumber;
}



//----------------------------------------------------
//
//    Get Levels Number
//
//----------------------------------------------------
unsigned int VertebralBody::GetLevelsNumber(void) const
{
  return m_LevelsNumber;
}



//----------------------------------------------------
//
//    Get Upper End Y Radius
//
//----------------------------------------------------
double VertebralBody::GetUpperEndYRadius(void) const
{
  return m_UpperEndYRadius;
}


//----------------------------------------------------
//
//    Get Upper End X Radius
//
//----------------------------------------------------
double VertebralBody::GetUpperEndXRadius(void) const
{
  return m_UpperEndXRadius;
}


//----------------------------------------------------
//
//    Get Upper End Y Concavity Depth
//
//----------------------------------------------------
double VertebralBody::GetUpperEndYConcavityDepth(void) const
{
  return m_UpperEndYConcavityDepth;
}


//----------------------------------------------------
//
//    Set Upper End Y Concavity Width
//
//----------------------------------------------------
double VertebralBody::GetUpperEndYConcavityWidth(void) const
{
  return m_UpperEndYConcavityWidth;
}



//----------------------------------------------------
//
//    Get Upper End Inclination
//
//----------------------------------------------------
double VertebralBody::GetUpperEndInclination(void) const
{
  return m_UpperEndInclination;
}



//----------------------------------------------------
//
//    Get Lower End Y Radius
//
//----------------------------------------------------
double VertebralBody::GetLowerEndYRadius(void) const
{
  return m_LowerEndYRadius;
}


//----------------------------------------------------
//
//    Get Lower End X Radius
//
//----------------------------------------------------
double VertebralBody::GetLowerEndXRadius(void) const
{
  return m_LowerEndXRadius;
}



//----------------------------------------------------
//
//    Get Lower End Y Concavity Depth
//
//----------------------------------------------------
double VertebralBody::GetLowerEndYConcavityDepth(void) const
{
  return m_LowerEndYConcavityDepth;
}


//----------------------------------------------------
//
//    Get Lower End Y Concavity Width
//
//----------------------------------------------------
double VertebralBody::GetLowerEndYConcavityWidth(void) const
{
  return m_LowerEndYConcavityWidth;
}


//----------------------------------------------------
//
//    Get Lower End Inclination
//
//----------------------------------------------------
double VertebralBody::GetLowerEndInclination(void) const
{
  return m_LowerEndInclination;
}

//----------------------------------------------------
//
//    Get Middle Y Radius
//
//----------------------------------------------------
double VertebralBody::GetMiddleYRadius(void) const
{
  return m_MiddleYRadius;
}


//----------------------------------------------------
//
//    Get Middle X Radius
//
//----------------------------------------------------
double VertebralBody::GetMiddleXRadius(void) const
{
  return m_MiddleXRadius;
}


//----------------------------------------------------
//
//    Get Height
//
//----------------------------------------------------
double VertebralBody::GetHeight(void) const
{
  return m_Height;
}



//----------------------------------------------------
//
//    Generate Points for the Mesh
//
//----------------------------------------------------
void VertebralBody::GeneratePoints(void) 
{

  const double MyPi = atan(1.0)*4.0;
  
  m_Points.clear();
  m_Points.reserve( m_SectorsNumber * m_LevelsNumber );

  double alpha;
  double currentYRadius;
  double currentXRadius;
  double currentYConcavityDepth;
  double currentYConcavityWidth;

  for(unsigned int lv=0; lv< m_LevelsNumber; lv++) 
  {
    
    alpha = (double)lv/(double)(m_LevelsNumber-1);
    
    currentXRadius = (1.0-alpha) * m_LowerEndXRadius + alpha * m_UpperEndXRadius;
    currentYRadius = (1.0-alpha) * m_LowerEndYRadius + alpha * m_UpperEndYRadius;
    currentYConcavityDepth = (1.0-alpha) * m_LowerEndYConcavityDepth + alpha * m_UpperEndYConcavityDepth;
    currentYConcavityWidth = (1.0-alpha) * m_LowerEndYConcavityWidth + alpha * m_UpperEndYConcavityWidth;
    
    const double shape = (1.0+cos(alpha*atan(1.0)*8.0))/2.0;
    double xFactor = shape * (1 - m_MiddleXRadius) + m_MiddleXRadius;
    double yFactor = shape * (1 - m_MiddleYRadius) + m_MiddleYRadius;

    currentXRadius *= xFactor;
    currentYRadius *= yFactor;

    double currentInclination = (1.0-alpha)*(-m_LowerEndInclination) + alpha * m_UpperEndInclination;
    const double width2 = currentYConcavityWidth * currentYConcavityWidth;

    for(unsigned int sc=0; sc < m_SectorsNumber; sc++) 
    {
      double th = (double)sc / (double)m_SectorsNumber * 2.0 * MyPi;
      double thg = th;
      if( thg > MyPi ) 
      {
        thg = 2.0*MyPi - thg;
      }
      double x = currentXRadius * sin(th);
      double y = currentYRadius * cos(th) - currentYConcavityDepth * exp( -thg*thg/(2.0*width2) );
      double z = y*tan( currentInclination * atan(1.0)/45.0) + alpha * m_Height;
      PointType p;
      p = x, y, z;
      m_Points.push_back( p );
    }
  }

}






//----------------------------------------------------
//
//    Draw Geometry in OpenGL
//
//----------------------------------------------------
void VertebralBody::DrawGeometry(void) const 
{

  if(  m_Points.empty() ) 
  {
    return;
  }

  if( GetDrawingMode() == triangles ) 
  {
    for(unsigned int level=0; level < m_LevelsNumber-1; level++) 
    {
      glBegin(GL_QUAD_STRIP);
      PointType pa = m_Points[ ( level ) * m_SectorsNumber + m_SectorsNumber-1 ];
      PointType pb = m_Points[ (level+1) * m_SectorsNumber + m_SectorsNumber-1 ];
      for(unsigned int sector=0; sector < m_SectorsNumber; sector++) 
      {
        glNormal3d( pa[0], pa[1], pa[2] );
        glVertex3d( pa[0], pa[1], pa[2] );
        glNormal3d( pb[0], pb[1], pb[2] );
        glVertex3d( pb[0], pb[1], pb[2] );
        pa = m_Points[ ( level )* m_SectorsNumber + sector ];
        pb = m_Points[ (level+1)* m_SectorsNumber + sector ];
      }
      glNormal3d( pa[0], pa[1], pa[2] );
      glVertex3d( pa[0], pa[1], pa[2] );
      glNormal3d( pb[0], pb[1], pb[2] );
      glVertex3d( pb[0], pb[1], pb[2] );
      glEnd();
    }

    glBegin( GL_TRIANGLE_FAN );
    glNormal3d( 0.0, 0.0, -1.0 );
    glVertex3d( 0.0, 0.0, 0.0 );
    PointType pa = m_Points[ m_SectorsNumber-1 ];
    glVertex3d( pa[0], pa[1], pa[2] );
    for(unsigned int sector=0; sector< m_SectorsNumber; sector++) 
    {
      pa = m_Points[sector];
      glVertex3d( pa[0], pa[1], pa[2] );
    }
    glEnd();

    glBegin( GL_TRIANGLE_FAN );
    glNormal3d( 0.0, 0.0, 1.0 );
    glVertex3d( 0.0, 0.0, m_Height );
    pa = m_Points[ (m_LevelsNumber-1) * m_SectorsNumber + m_SectorsNumber-1 ];
    glVertex3d( pa[0], pa[1], pa[2] );
    {
      for(unsigned int sector=0; sector < m_SectorsNumber; sector++) 
      {
        pa = m_Points[ (m_LevelsNumber-1) * m_SectorsNumber + sector ];
        glVertex3d( pa[0], pa[1], pa[2] );
      }
    }
    glEnd();
  }


  if( GetDrawingMode() == lines ) 
  {
    glBegin(GL_LINES);

    PointType pa;
    PointType pb;
    for(unsigned int level=0; level < m_LevelsNumber; level++) 
    {
      pa = m_Points[ ( level ) * m_SectorsNumber + m_SectorsNumber - 1   ];
      for(unsigned int sector=0; sector < m_SectorsNumber; sector++) 
      {
        pb = m_Points[ ( level )* m_SectorsNumber + sector ];
        glVertex3d( pa[0], pa[1], pa[2] );
        glVertex3d( pb[0], pb[1], pb[2] );
        pa = pb;
      }
    }

    for(unsigned int level=0; level < m_LevelsNumber-1; level++) 
    {
      for(unsigned int sector=0; sector < m_SectorsNumber; sector++) 
      {
        pa = m_Points[ ( level )* m_SectorsNumber + sector ];
        pb = m_Points[ (level+1)* m_SectorsNumber + sector ];
        glVertex3d( pa[0], pa[1], pa[2] );
        glVertex3d( pb[0], pb[1], pb[2] );
      }
    }

    glEnd();
  }


  if( GetDrawingMode() == points ) 
  {
    glBegin( GL_POINTS );
    std::vector< PointType >::const_iterator  it = m_Points.begin();
    while( it != m_Points.end() )
    {
      const PointType p = *it;
      glVertex3d( p[0], p[1], p[2] );
      ++it;
    }
    glEnd();
  }

}




   


//----------------------------------------------------
//
//    Write to stream
//
//----------------------------------------------------
int VertebralBody::Write( fltk::Shape3D::OfstreamType &os, int level ) const 
{

  int stat = 1;

  char *indent = new char[level+1];
  for(int i=0; i<level; i++) indent[i]='\t';
  indent[level]='\0';

  os << indent << "VertebralBody { "  << std::endl;

  os << indent << '\t' << "Height  " << m_Height << std::endl;
  os << indent << '\t' << "UpperEndYRadius  " << m_UpperEndYRadius << std::endl;
  os << indent << '\t' << "UpperEndXRadius  " << m_UpperEndXRadius << std::endl;
  os << indent << '\t' << "UpperEndYConcavityDepth  " <<
    m_UpperEndYConcavityDepth << std::endl;
  os << indent << '\t' << "UpperEndYConcavityWidth  " <<
    m_UpperEndYConcavityWidth << std::endl;
  os << indent << '\t' << "UpperEndInclination  " << m_UpperEndInclination <<
    std::endl;
  os << indent << '\t' << "LowerEndYRadius  " << m_LowerEndYRadius << std::endl;
  os << indent << '\t' << "LowerEndXRadius  " << m_LowerEndXRadius << std::endl;
  os << indent << '\t' << "LowerEndYConcavityDepth  " <<
    m_LowerEndYConcavityDepth << std::endl;
  os << indent << '\t' << "LowerEndYConcavityWidth  " <<
    m_LowerEndYConcavityWidth << std::endl;
  os << indent << '\t' << "LowerEndInclination  " << m_LowerEndInclination <<
    std::endl;
  os << indent << '\t' << "MiddleYRadius  " << m_MiddleYRadius << std::endl;
  os << indent << '\t' << "MiddleXRadius  " << m_MiddleXRadius << std::endl;
  os << indent << '\t' << "SectorsNumber  " << m_SectorsNumber << std::endl;
  os << indent << '\t' << "LevelsNumber  " << m_LevelsNumber << std::endl;
  os << indent << '\t' << "DrawingMode  " << GetDrawingMode() << std::endl;
  os << indent << '\t' << "Color  " << GetColor() << std::endl;
  os << indent << '\t' << "}" << std::endl;

  delete [] indent;

  if( os.fail() ) 
  {
    std::cerr << "Error encountered writing Vertebral Body" << std::endl;
    return 0;
  }


  return stat;

}





//----------------------------------------------------
//
//    Read from stream
//
//----------------------------------------------------
int VertebralBody::Read( fltk::Shape3D::IfstreamType &is) 
{

  char field[200];

  int endOfGroup=0;

  is >> field;
  if( strcmp(field,"{")!=0 ) 
  { 
    std::cerr << "Problem reading Vertebral Body, missing '{'" << std::endl;
    return 0;
  }

  while( !endOfGroup ) 
  {

    is >> field;

    if( is.eof() ) 
    {
      std::cerr << "Unexpected end of file reading Vertebral Body" << std::endl;
      return 0;
    }
      
    if( is.fail() ) 
    {
      std::cerr  << "Error found reading Vertebral Body" << std::endl;
      return 0;
    }



    if( strcmp(field,"Color")==0 ) { ColorType color; 
                         is >> color; SetColor( color ); continue; }

    if( strcmp(field,"DrawingMode")==0 ) {     long mode; is >> mode; 
                 SetDrawingMode( static_cast<drawingModes>(mode) ); continue; }

    if( strcmp(field,"SectorsNumber")==0 ) { is >> m_SectorsNumber; continue; }
    if( strcmp(field,"LevelsNumber")==0 ) { is >> m_LevelsNumber; continue; }
    if( strcmp(field,"UpperEndYRadius")==0 ) { is >> m_UpperEndYRadius; continue; }
    if( strcmp(field,"UpperEndXRadius")==0 ) { is >> m_UpperEndXRadius; continue; }
    if( strcmp(field,"UpperEndYConcavityDepth")==0 ) { is >> m_UpperEndYConcavityDepth; continue; }
    if( strcmp(field,"UpperEndYConcavityWidth")==0 ) { is >> m_UpperEndYConcavityWidth; continue; }
    if( strcmp(field,"UpperEndInclination")==0 ) { is >> m_UpperEndInclination; continue; }
    if( strcmp(field,"LowerEndYRadius")==0 ) { is >> m_LowerEndYRadius; continue; }
    if( strcmp(field,"LowerEndXRadius")==0 ) { is >> m_LowerEndXRadius; continue; }
    if( strcmp(field,"LowerEndYConcavityDepth")==0 ) { is >> m_LowerEndYConcavityDepth; continue; }
    if( strcmp(field,"LowerEndYConcavityWidth")==0 ) { is >> m_LowerEndYConcavityWidth; continue; }
    if( strcmp(field,"LowerEndInclination")==0 ) { is >> m_LowerEndInclination; continue; }
    if( strcmp(field,"MiddleYRadius")==0 ) { is >> m_MiddleYRadius; continue; }
    if( strcmp(field,"MiddleXRadius")==0 ) { is >> m_MiddleXRadius; continue; }
    if( strcmp(field,"Height")==0 ) { is >> m_Height; continue; }

    if( strcmp(field,"}")==0 ) { endOfGroup=1; }
  }

    GeneratePoints();

  return 1;
}



}  // end namespace li
