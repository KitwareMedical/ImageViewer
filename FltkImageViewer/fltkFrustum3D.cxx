/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkFrustum3D.cxx
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


#include "fltkFrustum3D.h"


namespace fltk {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Frustum3D::Frustum3D(void)  
{
  m_ApertureAngleX   = 30.0;
  m_ApertureAngleY   = 30.0;
  m_BottomPlane      = 10.0;
  m_TopPlane         =  2.0;
}






//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
void 
Frustum3D::DrawGeometry(void)  const 
{

  const double deg2rad = atan(1.0f)/45.0;

  const GLdouble h1 = m_TopPlane;
  const GLdouble h2 = m_BottomPlane;
  const GLdouble l1 = m_TopPlane    * sin( m_ApertureAngleX * deg2rad );
  const GLdouble w1 = m_TopPlane    * sin( m_ApertureAngleY * deg2rad );
  const GLdouble l2 = m_BottomPlane * sin( m_ApertureAngleX * deg2rad );
  const GLdouble w2 = m_BottomPlane * sin( m_ApertureAngleY * deg2rad );

		if( GetDrawingMode() == triangles ) 
      {
  		glBegin(GL_QUADS);

			glNormal3d(0.0, h2,-w2);
			glVertex3d(-l1, w1, h1);
			glVertex3d(-l2, w2, h2);
			glVertex3d( l2, w2, h2);
			glVertex3d( l1, w1, h1);

			glNormal3d(0.0,-h2,-w2);
			glVertex3d(-l1,-w1, h1);
			glVertex3d( l1,-w1, h1);
			glVertex3d( l2,-w2, h2);
			glVertex3d(-l2,-w2, h2);

			glNormal3d(-h2,0.0,-l2);
			glVertex3d(-l1, w1, h1);
			glVertex3d(-l1,-w1, h1);
			glVertex3d(-l2,-w2, h2);
			glVertex3d(-l2, w2, h2);

			glNormal3d( h2,0.0,-l2);
			glVertex3d( l1, w1, h1);
			glVertex3d( l2, w2, h2);
			glVertex3d( l2,-w2, h2);
			glVertex3d( l1,-w1, h1);

			glNormal3d(0.0,0.0,-1.0);
			glVertex3d(-l1, w1, h1);
			glVertex3d( l1, w1, h1);
			glVertex3d( l1,-w1, h1);
			glVertex3d(-l1,-w1, h1);

			glNormal3d(0.0,0.0,1.0);
			glVertex3d(-l2, w2, h2);
			glVertex3d(-l2,-w2, h2);
			glVertex3d( l2,-w2, h2);
			glVertex3d( l2, w2, h2);

      glEnd();
      }

		if( GetDrawingMode() == lines ) 
      {
			glDisable(GL_LIGHTING);
			glBegin(GL_LINE_LOOP);
				glVertex3d(-l2, w2, h2);
				glVertex3d(-l2,-w2, h2);
				glVertex3d( l2,-w2, h2);
				glVertex3d( l2, w2, h2);
			glEnd();
			glBegin(GL_LINE_LOOP);
				glVertex3d(-l1, w1, h1);
				glVertex3d(-l1,-w1, h1);
				glVertex3d( l1,-w1, h1);
				glVertex3d( l1, w1, h1);
			glEnd();
			glBegin(GL_LINES);
				glVertex3d( l1,-w1, h1);
				glVertex3d( l2,-w2, h2);
				glVertex3d( l1, w1, h1);
				glVertex3d( l2, w2, h2);
				glVertex3d(-l1,-w1, h1);
				glVertex3d(-l2,-w2, h2);
				glVertex3d(-l1, w1, h1);
				glVertex3d(-l2, w2, h2);
			glEnd();
			glEnable(GL_LIGHTING);
		}
}




} // end namespace fltk


