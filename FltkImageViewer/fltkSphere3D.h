/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSphere3D.h
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


#ifndef __fltkSphere3D_h
#define __fltkSphere3D_h

#include "fltkShape3D.h"
#include <GL/glu.h>



namespace fltk {




/**
 *  \class fltk::Sphere3D derives from Shape3D and represents a sphere in 3D.
 */
class ITK_EXPORT Sphere3D : public Shape3D 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef Sphere3D   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Define the PointType using the Superclass
   */
  typedef Superclass::PointType     PointType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Sphere3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Set the Radius
   */
	void SetRadius( double radius );


  /**
   * Set the Center
   */
	void SetCenter( const PointType & center );


  /**
   * Set the Number Of Slices to digitize the base
   */
	void SetNumberOfSlices( unsigned int number );


  /**
   * Set the Number Of Stacks to digitize the base
   */
	void SetNumberOfStacks( unsigned int number );


  /**
   * Get the Radius
   */
	double GetRadius(void) const;


  /**
   * Get the Center
   */
	const PointType & GetCenter(void) const;


  /**
   * Get the Number of Slices
   */
	unsigned int GetNumberOfSlices(void) const;


  /**
   * Get the Number of Stacks
   */
	unsigned int GetNumberOfStacks(void) const;


  /**
   * Draw the Cone using OpenGL commands
   */
	void DrawGeometry(void) const;



protected:
  /**
   * Constructor
   */
	Sphere3D();

  /**
   * Destructor
   */
	~Sphere3D();

private:
  /**
   *  Radius of the base
   */
	double          m_Radius;

  /**
   *   Height of the cone
   */
	double          m_Height;

  /**
   *   Number of Slices in which the circle is digitized
   */
	unsigned int    m_NumberOfSlices;

  /**
   *   Number of Stacks in which the circle is digitized
   */
	unsigned int    m_NumberOfStacks;

  /**
   *   Sphere using GLU support for quadrics.
   */
  GLUquadricObj   * m_GluQuadric;  

	};


}   // end namespace li

#endif
