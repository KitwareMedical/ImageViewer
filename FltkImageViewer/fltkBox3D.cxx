/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkBox3D.cxx
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


#include "liBox3D.h"


namespace fltk {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Box3D::Box3D(void)  
{
  m_Width   = 1.0;
  m_Height  = 1.0;
  m_Length  = 1.0;
}





//--------------------------------------------------
//
//    Set Width
//
//--------------------------------------------------
void 
Box3D::SetWidth( double width )
{
  m_Width  = width;
}



//--------------------------------------------------
//
//    Set Height
//
//--------------------------------------------------
void 
Box3D::SetHeight( double height )
{
  m_Height  = height;
}




//--------------------------------------------------
//
//    Set Length
//
//--------------------------------------------------
void 
Box3D::SetLength( double length )
{
  m_Length  = length;
}





//--------------------------------------------------
//
//    Get Width
//
//--------------------------------------------------
double 
Box3D::GetWidth( void ) const
{
  return m_Width;
}


//--------------------------------------------------
//
//    Get Height
//
//--------------------------------------------------
double 
Box3D::GetHeight( void ) const
{
  return m_Height;
}


//--------------------------------------------------
//
//    Get Length
//
//--------------------------------------------------
double 
Box3D::GetLength( void ) const
{
  return m_Length;
}






//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
void 
Box3D::DrawGeometry(void)  const 
{

    const GLdouble l2 = (GLdouble)m_Length/(GLdouble)2.0;
    const GLdouble w2 = (GLdouble)m_Width/(GLdouble)2.0;
    const GLdouble h  = (GLdouble)m_Height;

    glPushMatrix();

    if( GetDrawingMode() == triangles ) 
    {
      glBegin(GL_QUADS);

      glNormal3d(0.0,1.0, 0.0);
      glVertex3d(-l2, w2, 0.0);
      glVertex3d(-l2, w2,   h);
      glVertex3d( l2, w2,   h);
      glVertex3d( l2, w2, 0.0);

      glNormal3d(0.0,-1.0,0.0);
      glVertex3d(-l2,-w2, 0.0);
      glVertex3d( l2,-w2, 0.0);
      glVertex3d( l2,-w2,   h);
      glVertex3d(-l2,-w2,   h);

      glNormal3d(-1.0,0.0,0.0);
      glVertex3d(-l2, w2, 0.0);
      glVertex3d(-l2,-w2, 0.0);
      glVertex3d(-l2,-w2,   h);
      glVertex3d(-l2, w2,   h);

      glNormal3d( 1.0,0.0,0.0);
      glVertex3d( l2, w2, 0.0);
      glVertex3d( l2, w2,   h);
      glVertex3d( l2,-w2,   h);
      glVertex3d( l2,-w2, 0.0);

      glNormal3d(0.0,0.0,-1.0);
      glVertex3d(-l2, w2, 0.0);
      glVertex3d( l2, w2, 0.0);
      glVertex3d( l2,-w2, 0.0);
      glVertex3d(-l2,-w2, 0.0);

      glNormal3d(0.0,0.0,1.0);
      glVertex3d(-l2, w2,   h);
      glVertex3d(-l2,-w2,   h);
      glVertex3d( l2,-w2,   h);
      glVertex3d( l2, w2,   h);

      glEnd();

    }

    if( GetDrawingMode()== lines ) 
    {
      glBegin(GL_LINE_LOOP);
        glVertex3d(-l2, w2,   h);
        glVertex3d(-l2,-w2,   h);
        glVertex3d( l2,-w2,   h);
        glVertex3d( l2, w2,   h);
      glEnd();
      glBegin(GL_LINE_LOOP);
        glVertex3d(-l2, w2, 0.0);
        glVertex3d(-l2,-w2, 0.0);
        glVertex3d( l2,-w2, 0.0);
        glVertex3d( l2, w2, 0.0);
      glEnd();
      glBegin(GL_LINES);
        glVertex3d( l2,-w2, 0.0);
        glVertex3d( l2,-w2,   h);
        glVertex3d( l2, w2, 0.0);
        glVertex3d( l2, w2,   h);
        glVertex3d(-l2,-w2, 0.0);
        glVertex3d(-l2,-w2,   h);
        glVertex3d(-l2, w2, 0.0);
        glVertex3d(-l2, w2,   h);
      glEnd();
    }

    glPopMatrix();

}




} // end namespace fltk


