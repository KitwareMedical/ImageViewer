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
 * This element is defined by two points and a MaterialStandard object.
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

  typedef TFEMMesh                                FEMMeshType;
  typedef typename FEMMeshType::CellTraits        CellTraits;
  typedef typename FEMMeshType::PointType         PointType;
  typedef typename FEMMeshType::PixelType         PixelType;
  typedef typename FEMMeshType::CoordRepType      CoordinateRepresentationType;

  // note that we cannot use "CellType" here because there 
  // is already an enum called "CellType" defined in CellInterface<>
  typedef QuadrilateralCell< PixelType, CellTraits >       BaseCellType;
  typedef typename BaseCellType::PointIdIterator      PointIdIterator;
  typedef typename BaseCellType::PointIdConstIterator PointIdConstIterator;

  typedef FEMElement< BaseCellType, FEMMeshType >         Superclass;

  typedef typename Superclass::MatrixType                 MatrixType;
  typedef typename Superclass::LoadsVectorType            LoadsVectorType;

  typedef typename Superclass::PointsContainer            PointsContainer;
  typedef typename Superclass::PointDataContainer         PointDataContainer;
  typedef typename Superclass::CellsContainer             CellsContainer;

 
  /**
   * Function that handles all external loads applied to the element
   */
  LoadsVectorType GetExternalLoads(LoadElement * l) const;

  /** Declare the methods for visitors */
  itkElementVisitMacro(QUADRILATERAL_ELEMENT);

protected:
  /** Default constructor of an element */
  FEMElementQuadrilateral() {}

  /** Default destructor of an element */
  virtual ~FEMElementQuadrilateral() {}

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementQuadrilateral_h

