/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkFrustum3D.h
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


#ifndef __fltkFrustum3D_h
#define __fltkFrustum3D_h

#include "fltkShape3D.h"



namespace fltk {




/**
 *  \class fltk::Frustum3D derives from Shape3D and represents a parallepiped in 3D.
 */
class ITK_EXPORT Frustum3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Frustum3D   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Frustum3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    
  

  /**
   * Get/Set the Top plane position
   */
  itkGetMacro( TopPlane, double );
  itkSetMacro( TopPlane, double );


  /**
   * Get/Set the Bottom plane position
   */
  itkGetMacro( BottomPlane, double );
  itkSetMacro( BottomPlane, double );


  /**
   * Get/Set Aperture Angle along X
   */
  itkGetMacro( ApertureAngleX, double );
  itkSetMacro( ApertureAngleX, double );



  /**
   * Get/Set Aperture Angle along Y
   */
  itkGetMacro( ApertureAngleY, double );
  itkSetMacro( ApertureAngleY, double );



  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



protected:

  /**
   * Constructor
   */
  Frustum3D();


  /**
   * Copy Constructor
   */
  Frustum3D( const Frustum3D & box );


  /**
   * Assignment Operator
   */
  const Frustum3D & operator=( const Frustum3D & box );


private:

  /**
   * Aperture angle of the pyramid along the X direction
   */
  double m_ApertureAngleX;

  /**
   * Aperture angle of the pyramid along the Y direction
   */
  double m_ApertureAngleY;


  /**
   * Distance from the Apex to the top plane
   */
  double m_TopPlane;

 
  /**
   * Distance from the Apex to the bottom plane
   */
  double m_BottomPlane;

 
 
};


} // end namespace fltk


#endif
