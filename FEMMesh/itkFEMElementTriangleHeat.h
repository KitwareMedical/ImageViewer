/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementTriangleHeat.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMElementTriangleHeat_h
#define __itkFEMElementTriangleHeat_h

#include "itkFEMElementTriangle.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementTriangle
 * \brief Triangular finite element in 2D space.
 *
 * This element is defined by two points and a MaterialStandard object.
 */
template < typename TFEMMesh >
class FEMElementTriangleHeat : public FEMElementTriangle< TFEMMesh >
{

public:

  typedef FEMElementTriangleHeat< TFEMMesh > Self;

  typedef         Self * Pointer;
  typedef const   Self * ConstPointer;
  static  Pointer New(void) { return new Self; }
  /**
   * Number of Components of the Displacement Field
   * (also known as number of degrees of freedom)
   */
  enum { NumberOfDisplacementComponents = 3 };

  typedef TFEMMesh                                FEMMeshType;
  typedef typename FEMMeshType::CellTraits        CellTraits;
  typedef typename FEMMeshType::PointType         PointType;
  typedef typename FEMMeshType::PixelType         PixelType;
  typedef typename FEMMeshType::CoordRepType      CoordinateRepresentationType;

  // note that we cannot use "CellType" here because there 
  // is already an enum called "CellType" defined in CellInterface<>
  typedef CellInterface< PixelType, CellTraits >      CellInterfaceType;
  typedef TriangleCell< CellInterfaceType >           BaseCellType;
  typedef typename BaseCellType::PointIdIterator      PointIdIterator;
  typedef typename BaseCellType::PointIdConstIterator PointIdConstIterator;

  typedef FEMElementTriangle< FEMMeshType  >              Superclass;

  typedef typename Superclass::MatrixType                 MatrixType;
  typedef typename Superclass::LoadsVectorType            LoadsVectorType;

  typedef typename Superclass::PointsContainer            PointsContainer;
  typedef typename Superclass::PointDataContainer         PointDataContainer;
  typedef typename Superclass::CellsContainer             CellsContainer;
  typedef typename Superclass::DisplacementType           DisplacementType;


  /** Define the method to accept a visitor for this particular element type */
  itkElementVisitMacro(TRIANGLE_ELEMENT);


  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  virtual unsigned int GetNumberOfDisplacementComponents( void ) const 
             { return 3; } // three points, one scalar per point 

  
    /**
   * Element stiffness matrix
   */
  MatrixType GetStiffnessMatrix( const FEMMeshType * mesh ) const
  {

    if( !mesh ) 
      {
      itkGenericExceptionMacro(<<"Null mesh pointer passed to GetStiffnessMatrix()");
      }


    MatrixType stiffnessMatrix( NumberOfDisplacementComponents, NumberOfDisplacementComponents);
    
    PointIdConstIterator Id = this->GetCell().GetPointIds(); 
    typename PointsContainer::ConstPointer points = mesh->GetPoints();

    PointType point0;
    PointType point1;
    PointType point2;

    points->GetElementIfIndexExists( *Id++, &point0 );
    points->GetElementIfIndexExists( *Id++, &point1 );
    points->GetElementIfIndexExists( *Id++, &point2 );

    const CoordinateRepresentationType x0 = point0[0];
    const CoordinateRepresentationType y0 = point0[1];
    const CoordinateRepresentationType x1 = point1[0];
    const CoordinateRepresentationType y1 = point1[1];
    const CoordinateRepresentationType x2 = point2[0];
    const CoordinateRepresentationType y2 = point2[1];

    const RealType a0 = x1 * y2 - x2 * y1;
    const RealType a1 = x2 * y0 - x0 * y2;
    const RealType a2 = x0 * y1 - x1 * y0;

    const RealType b0 = y1 - y2;
    const RealType b1 = y2 - y0;
    const RealType b2 = y0 - y1;

    const RealType c0 = x2 - x1;
    const RealType c1 = x0 - x2;
    const RealType c2 = x1 - x0;

    const RealType area = a0 + a1 + a2;
    
    const RealType factor = m_TermalConductivity / ( 4.0 * area );

    stiffnessMatrix( 0,0 ) = factor * ( b0 * b0 + c0 * c0  );
    stiffnessMatrix( 0,1 ) = factor * ( b0 * b1 + c0 * c1  );
    stiffnessMatrix( 0,2 ) = factor * ( b0 * b2 + c0 * c2  );

    stiffnessMatrix( 1,0 ) = factor * ( b1 * b0 + c1 * c0  );
    stiffnessMatrix( 1,1 ) = factor * ( b1 * b1 + c1 * c1  );
    stiffnessMatrix( 1,2 ) = factor * ( b1 * b2 + c1 * c2  );

    stiffnessMatrix( 2,0 ) = factor * ( b2 * b0 + c2 * c0  );
    stiffnessMatrix( 2,1 ) = factor * ( b2 * b1 + c2 * c1  );
    stiffnessMatrix( 2,2 ) = factor * ( b2 * b2 + c2 * c2  );

    return stiffnessMatrix;

  }

  /**
   * Function that handles all external loads applied to the element
   */
  LoadsVectorType GetExternalLoads(LoadElement * l) const;


protected:
  /** Default constructor of an element */
  FEMElementTriangleHeat() {}

  /** Default destructor of an element */
  virtual ~FEMElementTriangleHeat() {}


private:

  /**
   * Pointer to geometric and material properties of the element
   */
  MaterialStandard::ConstPointer m_Material;


};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementTriangle_h

