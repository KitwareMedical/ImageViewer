/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkCylinder3D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/


#include "fltkCylinder3D.h"
#include <math.h>


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Cylinder3D::Cylinder3D(void)  
{
  m_Radius           = 1.0;
  m_Height           = 1.0;
  m_NumberOfSectors  =   6;
}





//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
Cylinder3D::SetRadius( double radius ) 
{
  m_Radius = radius;
}



//--------------------------------------------------
//
//    Set Height
//
//--------------------------------------------------
void 
Cylinder3D::SetHeight( double height ) 
{
  m_Height = height;
}



//--------------------------------------------------
//
//    Set Number of Sectors
//
//--------------------------------------------------
void 
Cylinder3D::SetNumberOfSectors( unsigned int number ) 
{
  m_NumberOfSectors = number;
}



//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
Cylinder3D::GetRadius( void ) const
{
  return m_Radius;
}


//--------------------------------------------------
//
//    Get Height
//
//--------------------------------------------------
double 
Cylinder3D::GetHeight( void ) const
{
  return m_Height;
}


//--------------------------------------------------
//
//    Get Length
//
//--------------------------------------------------
unsigned int 
Cylinder3D::GetNumberOfSectors( void ) const
{
  return m_NumberOfSectors;
}






//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
void 
Cylinder3D::DrawGeometry(void) const 
{

  if( GetDrawingMode() == none ) 
  {
    return;
  }

  // Angle subtended by each sector
  const double factor = 8.0 * atan( 1.0f ) / m_NumberOfSectors;

  if( GetDrawingMode() == points ) 
  {
    // Draw the points
    glBegin( GL_POINTS ); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }



  if( GetDrawingMode() == lines ) 
  {
    // Draw the bottom circle
    glBegin(GL_LINE_STRIP); 
    {
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, 0.0 );
      }
    }  
    glEnd();

    // Draw the top circle
    glBegin(GL_LINE_STRIP); 
    {
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
      }
    }  
    glEnd();

    // Draw the side
    glBegin(GL_LINES); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }

  if( GetDrawingMode() == triangles ) 
  {
    // Draw the bottom circle
    glBegin(GL_TRIANGLE_FAN); 
    {
      glNormal3d( 0.0, 0.0, -1.0 );
      glVertex3d( 0.0, 0.0,  0.0 );
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, 0.0 );
      }
    }  
    glEnd();

    // Draw the top circle
    glBegin(GL_TRIANGLE_FAN); 
    {
      glNormal3d( 0.0, 0.0, 1.0 );
      glVertex3d( 0.0, 0.0, m_Height );
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
      }
    }  
    glEnd();

    // Draw the side
    glBegin(GL_QUAD_STRIP); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glNormal3d( x, y, 0.0 );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }

}



}  // end namespace li

