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


#include "itkFEMNode.h"
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

  typedef FEMNode   NodeType;

  /**
   * Float type used in Node and derived classes
   */
  typedef NodeType::FloatType     FloatType;

  /**
   * Class type used in Node and derived classes to specify
   * displacements is redefined here for easier access.
   */
   typedef NodeType::DisplacementType  DisplacementType;

  /**
   * Return the number of degrees of freedom (DOF) for a derived element class
   */
  virtual int NumberOfDegreesOfFreedom( void ) const = 0;

  /**
   * Return the number of Nodes used by this Element
   */
  virtual int NumberOfNodes( void ) const = 0;


  /**
   * Pure virtual function that returns a pointer to an allocated memory that stores displacement
   * of i-th degree of freedom (DOF) of this element.
   *
   * Number of different pointers in a whole system determines global number of DOF. If two
   * pointers in two different elements point to the same location, this means that those two
   * elements share that DOF and are connected together.
   *
   * Normally this function is overriden by defining a simple switch statement that returns
   * pointers to members in nodes object that define the element. If an error occurs
   * (when i is out of range for example), derived function should call
   * implementation in base class (this one).
   */
  virtual DisplacementType * uDOF(int i) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual vnl_matrix<FloatType> Ke() const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual vnl_matrix<FloatType> Me() const;



};




}} // end namespace itk::fem

#endif 



