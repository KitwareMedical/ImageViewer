/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBar2D_test.h
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

#include "itkFEMLineCell.h"
#include "itkFEMElement_test.h"
#include "itkFEMMaterialStandard.h"

namespace itk {
namespace fem {




/**
 * \class Bar2D
 * \brief 1D Bar (spring) finite element in 2D space.
 *
 * This element is defined by two NodeXY object and a MaterialStandard object.
 */
class Bar2D : public FEMLineCell<2>, public FEMElement
{

public:

  /**
   * 4 DOF. Constant for faster access within the class.
   */
  enum {NDOF=4};
  
  /**
   * Required virtual functions
   */

  /**
   * Access to NDOF from base class
   */
  virtual int NumberOfDegreesOfFreedom( void ) const { return NDOF; };

  /**
   * Element stiffness matrix
   */
  vnl_matrix<FloatType> Ke()
  {
    /* trying to obtain the X coordinate of the first point... */
    GetPoint(0)[0];
    
    return *new vnl_matrix<FloatType>(2,2); // dummy thing to avoid compile errors.
  }

  /**
   * Function that handles all external loads applied to the element
   */
  vnl_vector<FloatType> Fe(void* l) const;


  /**
   * Default constructor
   */
  Bar2D();

public:

  /**
   * Return corresponding DOF for given local dof id.
   */
  virtual DOFType GetDOF( unsigned int id )
  {
    return dofs[id];
  }

  /**
   * Set the DOF.
   */
  virtual void SetDOF( unsigned int id, DOFType dof)
  {
    dofs[id]=dof;
  }

  /**
   * Clear the dofs array, and hereby release all
   * DOFs previously used by this element.
   */
  virtual void ClearDOFs(void)
  {
    for(int i=0;i<NDOF;i++) { dofs[i]=-1; }
  }

  /**
   * Array that stores DOFType objects.
   */
  DOFType dofs[NDOF];




  /** 
   * Returns the i-th DOF at given point pt. If dof doesn't exist
   * invalid DOF object is returned.
   */
  virtual DOFType GetDOFatPoint( unsigned int pt, unsigned int i )
  {
    if(i>=2 || pt>=GetNumberOfPoints()) return -1;
    return GetDOF(pt2dof[pt][i]);
  }

  /** 
   * Array that stores mapping between points in cell and corresponding DOFs.
   * This array is used by function GetDOFatPoint.
   */
  static const unsigned int pt2dof[2][2];


  /**
   * Pointer to geometric and material properties of the element
   */
  MaterialStandard::ConstPointer m_mat;

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementBar2D_h
