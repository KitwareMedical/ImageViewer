/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBar2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementBar2D_h
#define __itkFEMElementBar2D_h

#include "itkFEMElement.h"
#include "itkFEMMaterialStandard.h"
#include "itkMacro.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementBar2D
 * \brief 1D Bar (spring) finite element in 2D space.
 *
 * This element is defined by two points and a MaterialStandard object.
 */
template < typename TFEMMesh, unsigned int NDisplacementComponentsPerPoint >
class FEMElementBar2D : 
        public FEMElement< LineCell < typename TFEMMesh::PixelType,
                                      typename TFEMMesh::CellTraits >,
                           TFEMMesh,
                           NDisplacementComponentsPerPoint >
{

public:

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
  typedef typename LineCell< PixelType, CellTraits >  BaseCellType;
  typedef typename BaseCellType::PointIdIterator      PointIdIterator;
  typedef typename BaseCellType::PointIdConstIterator PointIdConstIterator;

  typedef FEMElement< BaseCellType, 
                      FEMMeshType,
                      NDisplacementComponentsPerPoint >   Superclass;

  typedef typename Superclass::MatrixType                 MatrixType;
  typedef typename Superclass::LoadsVectorType            LoadsVectorType;

  typedef typename Superclass::PointsContainer            PointsContainer;
  typedef typename Superclass::PointDataContainer         PointDataContainer;
  typedef typename Superclass::CellsContainer             CellsContainer;
  typedef typename Superclass::DisplacementType           DisplacementType;


  
  /**
   * Element stiffness matrix
   */
  MatrixType GetStiffnessMatrix( const FEMMeshType * mesh ) const
  {

    MatrixType stiffnessMatrix( NumberOfDisplacementComponents, NumberOfDisplacementComponents);
    
    PointIdConstIterator Id = this->GetPointIds(); 
    typename PointsContainer::ConstPointer points = mesh->GetPoints();

    PointType point1;
    PointType point2;

    points->GetElementIfIndexExists( *Id++, &point1 );
    points->GetElementIfIndexExists( *Id++, &point2 );

    const CoordinateRepresentationType x1 = point1[0];
    const CoordinateRepresentationType y1 = point1[1];
    const CoordinateRepresentationType x2 = point2[0];
    const CoordinateRepresentationType y2 = point2[1];

    return stiffnessMatrix;
  }

  /**
   * Function that handles all external loads applied to the element
   */
  LoadsVectorType GetExternalLoads(LoadElement * l) const;


protected:
  /** Default constructor of an element */
  FEMElementBar2D() {}

  /** Default destructor of an element */
  virtual ~FEMElementBar2D() {}


public:

  /**
   * Pointer to geometric and material properties of the element
   */
  MaterialStandard::ConstPointer m_Material;

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementBar2D_h

