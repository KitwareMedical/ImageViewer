/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMElement_h
#define __itkFEMElement_h

#include "itkFEMElementBase.h"
#include "vnl/vnl_matrix.h"


namespace itk {
namespace fem {


/** 
 * \brief Base class for Elements in a FEM system.
 * 
 */
template <    typename TCell, 
              typename TFEMMesh, 
              unsigned int NDisplacementComponentsPerPoint  >
class FEMElement : public TCell, 
                   public FEMElementBase< TFEMMesh >
{

public:

  // this type is used to represent components of the load
  // and the Stiffness matrix
  typedef FEMElementBase<TFEMMesh>                  ElementBaseType;
  typedef ElementBaseType::RealType                 RealType;
  typedef ElementBaseType::MatrixType               MatrixType;
  typedef ElementBaseType::LoadsVectorType          LoadsVectorType;
  typedef ElementBaseType::DisplacementType         DisplacementType;

  typedef TFEMMesh                                  FEMMeshType;
  typedef typename FEMMeshType::CellTraits          CellTraits;

  
  // note that we cannot use "CellType" here because there 
  // is already an enum called "CellType" defined in CellInterface<>
  typedef TCell                                     BaseCellType;
  typedef BaseCellType                              Superclass;

  typedef typename FEMMeshType::PointsContainer     PointsContainer;
  typedef typename FEMMeshType::PointDataContainer  PointDataContainer;
  typedef typename FEMMeshType::CellsContainer      CellsContainer;
  typedef typename FEMMeshType::PointIdentifier     PointIdentifier;
  

  /**  Number of components of the displacemente field at one point */
  enum { NumberOfDisplacementComponentsPerPoint = NDisplacementComponentsPerPoint };

  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  unsigned int GetNumberOfDisplacementComponents( void ) const
    { return this->GetNumberOfPoints() * NumberOfDisplacementComponentsPerPoint; }


  /**
   *  The Cell return the displacement associated with the ith point
   *  of its list of points
   */
  const DisplacementType & 
     GetDisplacement(unsigned int i,FEMMeshType *mesh ) const
      { const PointIdentifier Id = *( this->GetPointIds() + i );
        return mesh->GetPointData()->ElementAt( Id ); }


};




}} // end namespace itk::fem

#endif 



