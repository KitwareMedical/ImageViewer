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
        public FEMElement< LineCell < typename TFEMMesh::PixelType,
                                      typename TFEMMesh::CellTraits >,
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

  typedef TFEMMesh                                FEMMeshType;
  typedef typename FEMMeshType::CellTraits        CellTraits;
  typedef typename FEMMeshType::PointType         PointType;
  typedef typename FEMMeshType::PixelType         PixelType;
  typedef typename FEMMeshType::CoordRepType      CoordinateRepresentationType;

  // note that we cannot use "CellType" here because there 
  // is already an enum called "CellType" defined in CellInterface<>
  typedef LineCell< PixelType, CellTraits >       BaseCellType;
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
  itkElementVisitMacro( LINE_ELEMENT );


protected:
  /** Default constructor of an element */
  FEMElementLine() {}

  /** Default destructor of an element */
  virtual ~FEMElementLine() {}

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementLine_h

