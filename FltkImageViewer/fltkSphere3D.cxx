/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSphere3D.cxx
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



#include "fltkSphere3D.h"


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Sphere3D::Sphere3D(void)  
{
  m_Radius           = 1.0;
  m_NumberOfSlices   =   6;
  m_NumberOfStacks   =   6;
  m_GluQuadric = gluNewQuadric();
}



//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
Sphere3D::~Sphere3D(void)  
{
  gluDeleteQuadric( m_GluQuadric );
}





//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
Sphere3D::SetRadius( double radius ) 
{
  m_Radius = radius;
}



//--------------------------------------------------
//
//    Set Number of Slices
//
//--------------------------------------------------
void 
Sphere3D::SetNumberOfSlices( unsigned int number )
{
  m_NumberOfSlices = number;
}



//--------------------------------------------------
//
//    Set Number of Stacks
//
//--------------------------------------------------
void 
Sphere3D::SetNumberOfStacks( unsigned int number ) 
{
  m_NumberOfStacks = number;
}



//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
Sphere3D::GetRadius( void ) const
{
  return m_Radius;
}




//--------------------------------------------------
//
//    Get Number of Sectors
//
//--------------------------------------------------
unsigned int 
Sphere3D::GetNumberOfSlices( void ) const
{
  return m_NumberOfSlices;
}





//--------------------------------------------------
//
//    Get Number of Stacks
//
//--------------------------------------------------
unsigned int 
Sphere3D::GetNumberOfStacks( void ) const
{
  return m_NumberOfStacks;
}





//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
void 
Sphere3D::DrawGeometry(void) const 
{

  switch( this->GetDrawingMode() )
    {
    case points:
      gluQuadricDrawStyle( m_GluQuadric, GLU_POINT );
      break;
    case lines:
      gluQuadricDrawStyle( m_GluQuadric, GLU_LINE );
      break;
    case triangles:
      gluQuadricDrawStyle( m_GluQuadric, GLU_FILL );
      break;
    }
  gluQuadricNormals( m_GluQuadric, GL_SMOOTH );
  gluSphere( m_GluQuadric, 
             m_Radius, 
             m_NumberOfSlices, 
             m_NumberOfStacks   );

}



}  // end namespace li

