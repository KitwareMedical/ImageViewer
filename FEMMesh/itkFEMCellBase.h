/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMCellBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMCellBase_h
#define __itkFEMCellBase_h

#include "itkPoint.h"

namespace itk {
namespace fem {


/** 
 * \brief Base class for Cells in a FEM system.
 *  
 *  A Cell represents a region of the geometrical space.
 *  Cells have links to points that help them define its 
 *  spatial configuration. Cells suport not only polygonal
 *  shapes but also higher order representation like Hermite
 *  polynomials, Bezier patches and Pythagorean hodographs.
 *
 *  The Cell class is the Base class for a hierarchy of classes
 *  that share a common API
 *
 */
class FEMCellBase {

public:

   /** type used for representing coordinates in space 
      it is used as template argument for the Points and Vectors */ 
  typedef float   CoordinateRepresentationType;


  /**
   * Default constructor.
   */
  FEMCellBase()  {}

  /**
   * Virtual destructor
   */
  virtual ~FEMCellBase() {}

 
  /** 
   * Return the number of points required to specify the Cell.
   * It will be 3 for a triangle, 4 for a quad, 4 for a tetrahedron
   * 6 for a triangualar quadric patch, 10 for a triangular Bezier patch...
   * Some of this points will be internal to the Cell while others will
   * be external and then could be shared with neighbor cells.
   */
  virtual unsigned int GetNumberOfPoints( void ) const = 0;


private:

  /* 
   *  Derived classes should provide storage for a Fixed size array of 
   *  pointers to Points.
   */

  

};


}} // end namespace itk::fem

#endif 



