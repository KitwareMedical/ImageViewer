/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMCell.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMCell_h
#define __itkFEMCell_h

#include "itkFEMCellBase.h"
#include "itkPoint.h"

namespace itk {
namespace fem {


/** 
 * \brief Base class for Cells in a FEM system.
 *  
 * This class is abstract as FEMCellBase is. It adds the
 * dimension of the space in which the Cell is inmersed.
 * This dimension is used for the points defining the Cell.
 *
 */
template < unsigned int NPointDimension >
class FEMCell : public FEMCellBase {

public:

  /** Superclass type definition  */
  typedef FEMCellBase     Superclass;

  /** Dimension of the geometric space */
  enum { PointDimension = NPointDimension };

  /** Type used for representing the coordinates of Points in space */
  typedef Superclass::CoordinateRepresentationType      
                                             CoordinateRepresentationType;

  /** Type used to represent positions in space  */
  typedef Point< CoordinateRepresentationType, PointDimension > PointType;  


  /**
   * Default constructor.
   */
  FEMCell()  {}

  /**
   * Virtual destructor
   */
  virtual ~FEMCell() {}


  /** Set and Get a Point defining the Cell. 
   *  Cells are not necessarily polygons. Higher order cells
   *  are also supported by this class. For them some of the 
   *  points will be interior points. */
  virtual PointType * GetPoint( unsigned int id ) = 0;
  virtual void SetPoint( unsigned int id, PointType * ) = 0;

private:

  /* 
   *  Derived classes should provide storage for a Fixed size array of 
   *  pointers to Points.
   */

};


}} // end namespace itk::fem

#endif 



