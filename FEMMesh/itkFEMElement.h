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


#include "itkFEMNodeBase.h"
#include "itkFEMCellBase.h"
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
class FEMElement : public TCell
{

public:

  // this type is used to represent components of the load
  // and the Stiffness matrix
  typedef float       RealType;

  typedef TFEMMesh                                FEMMeshType;
  typedef typename FEMMeshType::PixelType         DisplacementType;
  typedef typename FEMMeshType::CellTraits        CellTraits;

  
  typedef TCell                                   CellType;
  typedef CellType                                Superclass;

  typedef typename FEMMeshType::PointsContainer     PointsContainer;
  typedef typename FEMMeshType::PointDataContainer  PointDataContainer;
  typedef typename FEMMeshType::CellsContainer      CellsContainer;
  typedef typename FEMMeshType::PointIdentifier     PointIdentifier;
  


  /** Type for representing the Stiffnes and Mass matrices  */
  typedef  vnl_matrix< RealType >       MatrixType;

  /** Type for representing sets of external loads */
  typedef  vnl_vector< RealType >       LoadsVectorType;

  /**  Number of components of the displacemente field at one point */
  enum { NumberOfDisplacementComponentsPerPoint = NDisplacementComponentsPerPoint };

  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  unsigned int GetNumberOfDisplacementComponents( void ) 
    { return this->GetNumberOfPoints() * NumberOfDisplacementComponentsPerPoint; }


  /**
   *  The Cell return the displacement associated with the ith point
   *  of its list of points
   */
  const DisplacementType & 
     GetDisplacement(unsigned int i,const FEMMeshType *mesh ) 
      { const PointIdentifier Id = *( this->GetPointIds() + i );
        return mesh->GetPointData()->ElementAt( Id ); }


  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual MatrixType GetStiffnessMatrix( const FEMMeshType * ) const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual MatrixType GetMassMatrix( void ) const 
    { return MatrixType(); }



};




}} // end namespace itk::fem

#endif 



