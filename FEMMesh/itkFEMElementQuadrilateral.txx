/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementQuadrilateral.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementQuadrilateral_txx
#define __itkFEMElementQuadrilateral_txx

#include "itkFEMElementQuadrilateral.h"


namespace itk {
namespace fem {



/**
 * Constructor
 */
template < typename TFEMMesh >
FEMElementQuadrilateral< TFEMMesh >
::FEMElementQuadrilateral()
{
}
  


/**
 * Destructor
 */
template < typename TFEMMesh >
FEMElementQuadrilateral< TFEMMesh >
::~FEMElementQuadrilateral()
{
}
  



/**
 * Function that returns a point in the global coordinate
 * system corresponding to a given point in the master element.
 */
template < typename TFEMMesh >
void 
FEMElementQuadrilateral< TFEMMesh >
::ComputePositionAt( const ParametricPointType & parametricPoint,
                     const PointsContainer     & meshPoints,
                           PointType           & globalPoint  ) const
{

  PointIdConstIterator pts = this->GetCell().PointIdsBegin();
  const PointType & p0 = meshPoints.ElementAt( *pts++ );
  const PointType & p1 = meshPoints.ElementAt( *pts++ );
  const PointType & p2 = meshPoints.ElementAt( *pts++ );
  const PointType & p3 = meshPoints.ElementAt( *pts++ );

  ShapeFunctionsArrayType shapeFunctions;
  this->ComputeShapeFunctionsAt( parametricPoint, shapeFunctions );

  for(unsigned int i=0; i<PointDimension; i++ )
    {
    globalPoint[i] = shapeFunctions[0] * p0[i] +
                     shapeFunctions[1] * p1[i] +
                     shapeFunctions[2] * p2[i] +
                     shapeFunctions[3] * p3[i];
    }

}




/** 
 * Function that computes the Jacobian matrix of the
 * transformation from the master element.
 */
template < typename TFEMMesh >
void 
FEMElementQuadrilateral< TFEMMesh >
::ComputeJacobianMatrixAt( const ParametricPointType & parametricPoint,
                           const PointsContainer     & meshPoints,
                                 JacobianMatrixType  & jacobian ) const
{
  JacobianMatrixType & J = jacobian;

  /**
   * Get the derivatives of the shape functions at given
   * point x
   */
  ShapeFunctionsDerivativesType shapeFunctionsDerivatives;
  this->ComputeShapeFunctionDerivativesAt( parametricPoint, 
                                           shapeFunctionsDerivatives );

  PointIdConstIterator pts = this->GetCell().PointIdsBegin();
  const PointType & p0 = meshPoints.ElementAt( *pts++ );
  const PointType & p1 = meshPoints.ElementAt( *pts++ );
  const PointType & p2 = meshPoints.ElementAt( *pts++ );
  const PointType & p3 = meshPoints.ElementAt( *pts++ );
   
  /**
   * Compute the elements of the Jacobian matrix
   * for each coordinate of a node w.r.t. global coordinate system
   */
  J[0][0] = (shapeD[0][0] * m_node1->X)
          + (shapeD[1][0] * m_node2->X)
          + (shapeD[2][0] * m_node3->X)
          + (shapeD[3][0] * m_node4->X);

  J[0][1] = (shapeD[0][1] * m_node1->X)
          + (shapeD[1][1] * m_node2->X)
          + (shapeD[2][1] * m_node3->X)
          + (shapeD[3][1] * m_node4->X);

  J[1][0] = (shapeD[0][0] * m_node1->Y)
          + (shapeD[1][0] * m_node2->Y)
          + (shapeD[2][0] * m_node3->Y)
          + (shapeD[3][0] * m_node4->Y);
    
  J[1][1] = (shapeD[0][1] * m_node1->Y)
          + (shapeD[1][1] * m_node2->Y)
          + (shapeD[2][1] * m_node3->Y)
          + (shapeD[3][1] * m_node4->Y);

}





/** 
 * Function that computes the shape functions defining
 * the geometry of this finite element at a given point.
 */
template < typename TFEMMesh >
void 
FEMElementQuadrilateral< TFEMMesh >
::ComputeShapeFunctionsAt( const ParametricPointType & parametricPoint,
                                 ShapeFunctionsArrayType & shapeFunctions ) const
{
 
  /** given local point x=(r,s), where -1 <= r,s <= 1 and */
  const CoordinateRepresentationType & r = parametricPoint[0];
  const CoordinateRepresentationType & s = parametricPoint[1];

  /** shape function 1: (node 1) */
  shapeFunctions[0] = ( 1 - r ) * ( 1 - s ) * .25;

  /** shape function 2: (node 2) */
  shapeFunctions[1] = ( 1 + r ) * ( 1 - s ) * .25;

  /** shape function 3: (node 3) */
  shapeFunctions[2] = ( 1 + r ) * ( 1 + s ) * .25;

  /** shape function 1: (node 4) */
  shapeFunctions[3] = ( 1 - r ) * ( 1 + s ) * .25;

}




/**
 * Function that computes the derivatives of the shape
 * functions of this element at a given point.
 */
template < typename TFEMMesh >
void 
FEMElementQuadrilateral< TFEMMesh >
::ComputeShapeFunctionDerivativesAt( 
                                const ParametricPointType & parametricPoint,
                                ShapeFunctionsDerivativesType & shapeDerivative ) const
{
  /** functions at directions r and s.  */
  const CoordinateRepresentationType & r = parametricPoint[0];
  const CoordinateRepresentationType & s = parametricPoint[1];

  /** Derivative w.r.t r for shape function 1 (node 1) */
  shapeDerivative[0][0] = -( 1 - s ) * .25;

  /** Derivative w.r.t s for shape function 1 (node 1) */
  shapeDerivative[0][1] = -( 1 - r ) * .25;

  /** Derivative w.r.t r for shape function 2 (node 2) */
  shapeDerivative[1][0] = +( 1 - s ) * .25;

  /** Derivative w.r.t s for shape function 2 (node 2) */
  shapeDerivative[1][1] = -( 1 + r ) * .25;

  /** Derivative w.r.t r for shape function 3 (node 3) */
  shapeDerivative[2][0] = +( 1 + s ) * .25;

  /** Derivative w.r.t s for shape function 3 (node 3) */
  shapeDerivative[2][1] = +( 1 + r ) * .25;

  /** Derivative w.r.t r for shape function 4 (node 4) */
  shapeDerivative[3][0] = -( 1 + s ) * .25;

  /** Derivative w.r.t s for shape function 4 (node 4) */
  shapeDerivative[3][1] = +( 1 - r ) * .25;
}




}} // end namespace itk::fem

#endif 

