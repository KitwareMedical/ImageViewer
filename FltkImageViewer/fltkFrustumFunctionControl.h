/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkFrustumFunctionControl.h
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
#ifndef __fltkFrustumFunctionControl_h
#define __fltkFrustumFunctionControl_h

#include "fltkFrustumFunctionControlGUI.h"
#include "itkFrustumSpatialFunction.h"
#include "fltkFrustum3D.h"
#include "itkCommand.h"


namespace fltk {

template < class TFrustumSpatialFunction >
class FrustumFunctionControl : 
                    public itk::Object, 
                    public fltkFrustumFunctionControlGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef FrustumFunctionControl         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(FrustumFunctionControl, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Spatial Function Type
   */
  typedef TFrustumSpatialFunction                 SpatialFunctionType;
  typedef typename SpatialFunctionType::Pointer   SpatialFunctionPointer;
  

  /**
   * Set the Spatial Function Type
   */
  itkSetObjectMacro( SpatialFunction, SpatialFunctionType );


  /**
   * Shape type
   */
  typedef Frustum3D                      ShapeType;
  typedef ShapeType::Pointer             ShapePointer;
  
  
  /**
   *  Simple Command for drawing in OpenGL 
   */
  typedef itk::SimpleConstMemberCommand<Self>  DrawCommandType;
  typedef typename DrawCommandType::Pointer    DrawCommandPointer;
  itkGetObjectMacro( DrawCommand, DrawCommandType );
    

  /**
   * Get the Shape type
   */
  itkGetObjectMacro( Shape, ShapeType );


  /**
   * Set Angle Z
   */
  void SetAngleZ( double angle );

  /**
   * Set Aperture Angle X
   */
  void SetApertureAngleX( double angle );

  /**
   * Set Aperture Angle Y
   */
  void SetApertureAngleY( double angle );

  /**
   * Set Top Plane
   */
  void SetTopPlane( double distance );

  /**
   * Set Bottom Plane
   */
  void SetBottomPlane( double distance );

  /**
   * Set Apex
   */
  void SetApex(  double x, double y, double z );
  void SetApexX( double x );
  void SetApexY( double y );
  void SetApexZ( double z );

  /**
   * Set Parameters from GUI
   */
  void SetParametersFromGUI( void );

  /**
   * Draw the shape in OpenGL
   */
  void glDraw( void ) const;


protected:
  
  FrustumFunctionControl();
  virtual ~FrustumFunctionControl();


private:
  SpatialFunctionPointer                m_SpatialFunction;
  ShapePointer                          m_Shape;
  DrawCommandPointer                    m_DrawCommand;
  double                                m_AngleZ;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkFrustumFunctionControl.txx>
#endif




#endif
