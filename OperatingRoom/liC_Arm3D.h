/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liC_Arm3D.h
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
#ifndef liCARM3DCLASS
#define liCARM3DCLASS

#include "fltkShape3D.h"
#include <vector>

namespace li {

/** C_Arm3D implements the 3D model of the C-Shape structure
 *  that supports the X-Ray source and the Image Intensifier. */
class ITK_EXPORT C_Arm3D : public fltk::Shape3D 
{
public:
  /** Standard class typedefs. */
  typedef C_Arm3D   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /*** Run-time type information (and related methods). */
  itkTypeMacro( C_Arm3D, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /**  Destructor */
  ~C_Arm3D();

  /**  Return the Width of the C, the dimension normal to the C plane */
  double GetWidth(void) const;

  /**  Return the Radius of the C. */
  double GetRadius(void) const;

  /**  Return the Radial Width of the C. */
  double GetRadialWidth(void) const;

  /**  Return the Radial Width of the C. */
  unsigned int GetNumberOfSectors(void) const;

  /**  Set the Radius of the C. */
  void SetRadius( double radius );

  /**  Set the Width of the C, the dimension normal to the C plane. */
  void SetWidth( double width );

  /**  Set the Radial Width of the C. */
  void SetRadialWidth( double radialWidth );

  /**  Set the number of sector to digitize the C shape. */
  void SetNumberOfSectors( unsigned int numberOfSectors );

  /**  Draw the model with OpenGL commands. */
  void DrawGeometry(void) const;

protected:
  /**  Constructor */
  C_Arm3D();

  /** Generate points with the discretization of the C. * this function
   *  should be called after any change in geometric parameters.  */
  void SetupGeometry(void);

private:
  /** Dimension of the C-Arm in the direcction normal to the C plane */
  double m_Width;

  /** Radius of the C-Arm. */
  double m_Radius;

  /** Dimension of the C along the radial direction. Along with the m_Width
   *  this value define the cross-section of the C.  */
  double m_RadialWidth;

  /** Number of Sector on which the C will be discretized. */
  unsigned int m_NumberOfSectors;

  /** Points that hold the internal discretization of the C. */
  std::vector< PointType > m_Points;

};

}  // end namespace li
 

#endif




