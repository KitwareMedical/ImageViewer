/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementLine.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementLine_h
#define __itkFEMElementLine_h

#include "itkFEMElement.h"
#include "itkLineCell.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementLine
 * \brief Line finite element.
 *
 * This element is defined by two points.
 */
template < typename TFEMMesh >
class FEMElementLine : 
        public FEMElement< LineCell < 
                                CellInterface<
                                      typename TFEMMesh::PixelType,
                                      typename TFEMMesh::CellTraits > >,
                           TFEMMesh >
{

public:

  typedef FEMElementLine< TFEMMesh >  Self;

  typedef         Self * Pointer;
  typedef const   Self * ConstPointer;
  static  Pointer New(void) { return new Self; }

  /**
   * Number of Components of the Displacement Field
   * (also known as number of degrees of freedom)
   */
  enum { NumberOfDisplacementComponents = 2 };
  enum { ParametricSpaceDimension       = 1 };
  enum { NumberOfGaussIntegrationPoints = 1 };

  // This macro takes care of declaring all the types
  // required by the API of the superclass. This is 
  // is needed here because typedef's are not inherited
  itkFEMElementTypesMacro( LineCell );

  /** Declare the methods for visitors */
  itkElementVisitMacro( LINE_ELEMENT );


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
                                      ShapeFunctionsArrayType & shapeFunctions ) const;

  /**
   * Function that computes the derivatives of the shape
   * functions of this element at a given point.
   */
  void ComputeShapeFunctionDerivativesAt( 
                    const ParametricPointType & parametricPoint,
                          ShapeFunctionsDerivativesType & shapeFunctions ) const;



protected:
  /** Default constructor of an element */
  FEMElementLine();

  /** Default destructor of an element */
  virtual ~FEMElementLine();

};


}} // end namespace itk::fem

  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMElementLine.txx"
#endif

#endif // #ifndef __itkFEMElementLine_h

