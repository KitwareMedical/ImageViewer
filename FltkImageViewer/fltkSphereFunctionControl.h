/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSphereFunctionControl.h
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
#ifndef __fltkSphereFunctionControl_h
#define __fltkSphereFunctionControl_h

#include "fltkSphereFunctionControlGUI.h"
#include "itkSphereSpatialFunction.h"
#include "fltkSphere3D.h"
#include "itkCommand.h"


namespace fltk {

template < class TSphereSpatialFunction >
class SphereFunctionControl : 
                    public itk::Object, 
                    public fltkSphereFunctionControlGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef SphereFunctionControl         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(SphereFunctionControl, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Spatial Function Type
   */
  typedef TSphereSpatialFunction        SpatialFunctionType;
  

  /**
   * Set the Spatial Function Type
   */
  itkSetObjectMacro( SpatialFunction, SpatialFunctionType );


  /**
   * Shape type
   */
  typedef Sphere3D                      ShapeType;
  
  
  /**
   *  Simple Command for drawing in OpenGL 
   */
  typedef itk::SimpleConstMemberCommand<Self>  DrawCommandType;
  itkGetObjectMacro( DrawCommand, DrawCommandType );
    

  /**
   * Get the Shape type
   */
  itkGetObjectMacro( Shape, ShapeType );


  /**
   * Set Radius
   */
  void SetRadius( double radius );

  /**
   * Set Center
   */
  void SetCenter(  double x, double y, double z );
  void SetCenterX( double x );
  void SetCenterY( double y );
  void SetCenterZ( double z );

  /**
   * Set Parameters from GUI
   */
  void SetParametersFromGUI( void );

  /**
   * Draw the shape in OpenGL
   */
  void glDraw( void ) const;


protected:
  
  SphereFunctionControl();
  virtual ~SphereFunctionControl();


private:
  typename SpatialFunctionType::Pointer    m_SpatialFunction;
  ShapeType::Pointer                       m_Shape;
  DrawCommandType::Pointer                 m_DrawCommand;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSphereFunctionControl.txx>
#endif




#endif
