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
 *  Elements are associated one-to-one with Cells a pointer to 
 *  an Abstract Cell class is maintained in the Element so the
 *  geometric information can be made available to the Element.
 * 
 *  Elements also keep a fixed array to pointer to Nodes.  
 *  Every particular type of Element have a specific number of 
 *  nodes that do not change after construction.
 * 
 *  Elements are responsible for providing interpolation between
 *  the values (degrees of freedom) in the Nodes. This is done
 *  by using "Shape Functions" which are interpolating basis 
 *  functions defined in parametric coordinates.
 *
 */
class FEMElement 
{

public:

  /** Use a standard symplified type for the Node base class */
  typedef FEMNodeBase             NodeType;

  /** Use a standard symplified type for the Cell base class */
  typedef FEMCellBase             CellType;

  /**
   * Real type used in Node and derived classes
   * This type is used for representing the 
   * components of the Stiffnes Matrix.
   */
  typedef  float      RealType;

  /** Type for representing the Stiffnes and Mass matrices  */
  typedef  vnl_matrix< RealType >       MatrixType;

  /** Type for representing sets of external loads */
  typedef  vnl_vector< RealType >       LoadsVectorType;

  /**
   * Class type used in Node and derived classes to specify
   * displacements is redefined here for easier access.
   */
   typedef NodeType::DisplacementType  DisplacementType;

  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  virtual unsigned int GetNumberOfDisplacementComponents( void ) const = 0;

  /**
   * Return the number of Nodes used by this Element
   */
  virtual unsigned int GetNumberOfNodes( void ) const = 0;


  /**
   *
   * The memory address of the Displacement component is used as an Identifier in the global
   * FEM mesh. This Identifier allows to align the contribution of equations comming from every
   * Element in the system.
   *
   * Number of different pointers in a whole system determines global number of displacement
   * components. If two pointers in two different elements point to the same location, this 
   *  means that those two elements share that displacement component and are connected together.
   *
   * Normally this function is overriden by defining a simple switch statement that returns
   * const references to members in nodes object that define the element. If an error occurs
   * (when i is out of range for example), an ExceptionObject is thrown.
   */
  virtual const DisplacementType & GetDisplacementComponent(unsigned int i) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual MatrixType GetStiffnessMatrix( void ) const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual MatrixType GetMassMatrix( void ) const 
    { return MatrixType(); }

  /** Set the pointer to the Cell associated with this Element.
   *  Cells represent the geometry of the object while Element represent
   *  the distribition of the Displacement Field over the object      */
  void SetCell( const CellType * cell )
           { m_Cell = cell; }

  /** Get the pointer to the Cell associated with this Element.
   *  Cells represent the geometry of the object while Element represent
   *  the distribition of the Displacement Field over the object          */
  const CellType * GetCell( void ) const
           { return m_Cell; }


private:

   const CellType  *   m_Cell;


};




}} // end namespace itk::fem

#endif 



