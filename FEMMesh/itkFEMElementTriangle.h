/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementTriangle.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementTriangle_h
#define __itkFEMElementTriangle_h

#include "itkFEMElement.h"
#include "itkTriangleCell.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementTriangle
 * \brief Triangular finite element.
 *
 * This element is defined by three points
 */
template < typename TFEMMesh >
class FEMElementTriangle : 
        public FEMElement< TriangleCell < CellInterface <
                                            typename TFEMMesh::PixelType,
                                            typename TFEMMesh::CellTraits > >,
                           TFEMMesh >
{

public:

  typedef FEMElementTriangle< TFEMMesh >  Self;

  typedef         Self * Pointer;
  typedef const   Self * ConstPointer;
  static  Pointer New(void) { return new Self; }

  /**
   * Number of Components of the Displacement Field
   * (also known as number of degrees of freedom)
   */
  enum { NumberOfDisplacementComponents = 3 };
  enum { ParametricSpaceDimension       = 2 };
  enum { NumberOfGaussIntegrationPoints = 1 };

  // This macro takes care of declaring all the types
  // required by the API of the superclass. This is 
  // is needed here because typedef's are not inherited
  itkFEMElementTypesMacro( TriangleCell );

  /** Declare the methods for visitors */
  itkElementVisitMacro(TRIANGLE_ELEMENT);


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
                                      ShapeFunctionsArrayType & jacobian ) const;

  /**
   * Function that computes the derivatives of the shape
   * functions of this element at a given point.
   */
  void ComputeShapeFunctionDerivativesAt( 
                const ParametricPointType & parametricPoint,
                ShapeFunctionsDerivativesType & shapeFunctionsDerivatives ) const;




protected:
  /** Default constructor of an element */
  FEMElementTriangle();

  /** Default destructor of an element */
  virtual ~FEMElementTriangle();

};



}} // end namespace itk::fem

  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMElementTriangle.txx"
#endif



#endif // #ifndef __itkFEMElementTriangle_h

