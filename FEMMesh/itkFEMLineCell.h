/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLineCell.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMLineCell_h
#define __itkFEMLineCell_h

#include "itkFEMCell.h"
#include "itkFixedArray.h"

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
template < unsigned int PointDimension > 
class FEMLineCell : public FEMCell<PointDimension> {

public:

  typedef FEMCell<PointDimension>     Superclass;
  typedef typename Superclass::PointType       PointType;


  /**
   * Default constructor.
   */
  FEMLineCell()  {}

  /**
   * Virtual destructor
   */
  virtual ~FEMLineCell() {}

  enum { NumberOfPoints = 2 };
 
  /** 
   * Return the number of points required to specify the Cell.
   */
  virtual unsigned int GetNumberOfPoints( void ) const 
    { return NumberOfPoints; }



  PointType * GetPoint( unsigned int id ) 
    { return m_Points[ id ];  }

  void SetPoint( unsigned int id, PointType * p )
    { m_Points[ id ] = p;  }


private:

  FixedArray< PointType *, NumberOfPoints >  m_Points; 

};


}} // end namespace itk::fem

#endif 



