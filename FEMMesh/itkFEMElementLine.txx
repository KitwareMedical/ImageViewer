/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementLine.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementLine_txx
#define __itkFEMElementLine_txx

#include "itkFEMElementLine.h"


namespace itk {
namespace fem {



/**
 * Constructor
 */
template < typename TFEMMesh >
FEMElementLine< TFEMMesh >
::FEMElementLine()
{
}
  


/**
 * Destructor
 */
template < typename TFEMMesh >
FEMElementLine< TFEMMesh >
::~FEMElementLine()
{
}
  



/**
 * Function that returns a point in the global coordinate
 * system corresponding to a given point in the master element.
 */
template < typename TFEMMesh >
void 
FEMElementLine< TFEMMesh >
::ComputePositionAt( const ParametricPointType & parametricPoint,
                           PointType           & globalPoint,
                     const PointsContainer     & points       ) const
{

}




/** 
 * Function that computes the Jacobian matrix of the
 * transformation from the master element.
 */
template < typename TFEMMesh >
void 
FEMElementLine< TFEMMesh >
::ComputeJacobianMatrixAt( const ParametricPointType & parametricPoint,
                                 JacobianMatrixType  & jacobian ) const
{

}





/** 
 * Function that computes the shape functions defining
 * the geometry of this finite element at a given point.
 */
template < typename TFEMMesh >
void 
FEMElementLine< TFEMMesh >
::ComputeShapeFunctionsAt( const ParametricPointType & parametricPoint,
                                 ShapeFunctionsArrayType & shapeFunctions ) const
{

}




/**
 * Function that computes the derivatives of the shape
 * functions of this element at a given point.
 */
template < typename TFEMMesh >
void 
FEMElementLine< TFEMMesh >
::ComputeShapeFunctionDerivativesAt( 
                                const ParametricPointType & parametricPoint,
                                ShapeFunctionsDerivativesType & shapeDerivative ) const
{

}




}} // end namespace itk::fem

#endif 

