/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementQuadrilateral.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementQuadrilateral_h
#define __itkFEMElementQuadrilateral_h

#include "itkFEMElement.h"
#include "itkQuadrilateralCell.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementQuadrilateral
 * \brief Quadrilateral finite element.
 *
 * This element is defined by four points 
 */
template < typename TFEMMesh >
class FEMElementQuadrilateral : 
        public FEMElement< QuadrilateralCell <  typename TFEMMesh::PixelType,
                                                typename TFEMMesh::CellTraits >,
                           TFEMMesh >
{

public:

  typedef FEMElementQuadrilateral< TFEMMesh >  Self;

  typedef         Self * Pointer;
  typedef const   Self * ConstPointer;
  static  Pointer New(void) { return new Self; }

  /**
   * Number of Components of the Displacement Field
   * (also known as number of degrees of freedom)
   */
  enum { NumberOfDisplacementComponents = 4 };
  enum { ParametricSpaceDimension       = 2 };


  // This macro takes care of declaring all the types
  // required by the API of the superclass. This is 
  // is needed here because typedef's are not inherited
  itkFEMElementTypesMacro( QuadrilateralCell );

  /** Declare the methods for visitors */
  itkElementVisitMacro( QUADRILATERAL_ELEMENT );




  /**
   * Function that returns a point in the global coordinate
   * system corresponding to a given point in the master element.
   */
  void ComputePositionAt( const ParametricPointType & parametricPoint,
                          const PointsContainer     & points,
                                PointType           & globalPoint  ) const;

  /** 
   * Function that computes the Jacobian matrix of the
   * transformation from the master element.
   */
  void ComputeJacobianMatrixAt( const ParametricPointType & parametricPoint,
                                const PointsContainer     & points,
                                      JacobianMatrixType  & jacobian ) const;

  /** 
   * Function that computes the shape functions defining
   * the geometry of this finite element at a given point.
   */
  void ComputeShapeFunctionsAt( const ParametricPointType & parametricPoint,
                                      ShapeFunctionsArrayType & ) const;

  /**
   * Function that computes the derivatives of the shape
   * functions of this element at a given point.
   */
  void ComputeShapeFunctionDerivativesAt( 
                                const ParametricPointType & parametricPoint,
                                ShapeFunctionsDerivativesType & ) const;



protected:
  /** Default constructor of an element */
  FEMElementQuadrilateral();

  /** Default destructor of an element */
  virtual ~FEMElementQuadrilateral();

};



}} // end namespace itk::fem

  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMElementQuadrilateral.txx"
#endif

#endif // #ifndef __itkFEMElementQuadrilateral_h

