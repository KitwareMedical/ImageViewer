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

#include "itkSmartPointer.h"

namespace itk {
namespace fem {


/** 
 * \brief Base class for Elements in a FEM system.
 * 
 * Actual FEM problems are expected to be described by a FEMMesh in which the 
 * geometrical aspects of the problem are represented at the Mesh/Cell levels
 * while the physics of the problem are represented at the FEMMesh/Element level.
 *
 * Since a cell in itk mesh defines only geometry, we create classes derived
 * from cells that add the finite elements functionality to cells. In general
 * multiple inheritance is used to link the two worlds (geometrical and 
 * phisical system). We also need to store information about degrees of freedom
 * at each point in a mesh, this is done by using Zero dimensional cells/elements
 * based on the itk::CellVertex class.  
 *
 * In this structure the PointData and CellData capabilities of the itk::Mesh are
 * not used at all.
 *
 * A cell in a mesh corresponds to an Element in FEM. FEM solver can only operate
 * on the cells which are derived from this class. (use multiple inheritance)
 * Derived classes should define a function that creates and initializes a
 * proper FEM Element object (CreateElement). The function receives a vector which 
 * holds the pointers to nodes that define the element and pointer to material
 * object (everything that most elements need).
 * Another function that needs to be defined in derived classes is CreateMaterial,
 * which creates the proper material object that the element needs.
 * By using different derived classes in a mesh it is possible to create different
 * FEM Elements a system.
 */
class FEMElement {

  /**
   * Float type used in Node and derived classes
   */
  typedef Node::Float Float;

  /**
   * Class type used in Node and derived classes to specify
   * displacements is redefined here for easier access.
   */
  typedef Node::Displacement Displacement;

  /**
   * Return the number of degrees of freedom (DOF) for a derived element class
   */
  virtual int N() const = 0;

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
  virtual Displacement* uDOF(int i) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual vnl_matrix<Float> Ke() const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual vnl_matrix<Float> Me() const;

  /**
   * Easy access of LoadElement pointer type. When using SmartPointers,
   * this is a pointer to FEMLightObject to avoid cyclic references between
   * LoadElement and Element classes.
   * As a consequence whenever you need to use a pointer to LoadElement class
   * within the element's declaration or definition, ALWAYS use this typedef
   * instead.
   * When calling the Fe(...) function from outside, you should ALWAYS first
   * convert the argument to Element::LoadElementPointer. See code of function
   * Solver::AssembleF(...) for more info.
   */
#ifndef FEM_USE_SMART_POINTERS
  typedef LoadElement* LoadElementPointer;
#else
  typedef SmartPointer<FEMLightObject> LoadElementPointer;
#endif


  /** 
   * Compute and return the element force vector. Basically this is the contribution
   * of this element on the right side of the master matrix equation. Returned vector
   * should include only nodal forces that correspond to the given Load object.
   * Within the function definition you should use the dynamic_cast operator on the 
   * provided pointer to the Load object to obtain the pointer to the derived class
   * and act accordingly. If the implementation of the Fe function does not handle
   * given Load class (dynamic_cast returns 0), parent's Fe member should be called.
   * Several types of loads can be implemented using several if statements.
   *
   * Example:
   *
   *  class Quad2D : public Element {
   *    ...
   *    vnl_vector<Float> Fe(LoadElement* l) {
   *      if (LoadGravity l0=dynamic_cast<LoadGravity*>(l)) {
   *        ... implement the gravity load using l0 pointer
   *      } else
   *        return Element::Fe(l);
   *    }
   *    ...
   *  };
   */
  virtual vnl_vector<Float> Fe(LoadElementPointer l) const {
    /**
     * If this function is not defined in the derived class we return a vector
     * containing all zeros. it has the dimension of N(). This quietly accepts
     * all load classes, but they have no affect on the element.
     */
    return vnl_vector<Float>( N(), 0.0 );
  };









  /**
   * Material 
   */
  typedef  itk::Object    MaterialType;
  /** 
   * Storage type for pointers to nodes that define the element
   */
  typedef std::vector< FEMNode::Pointer > NodesVector;

  /** 
   * Function that creates the corresponding FEM element object for a particular itk cell
   * Every derived class should define this function
   */
  virtual Element::Pointer CreateElement( 
                              const NodesVector&, 
                              MaterialType::Pointer  ) = 0;

  /**
   * Function that creates the object material class that the FEM element needs
   * Every derived class should define this function
   */
  virtual MaterialType::Pointer CreateMaterial() = 0;


protected:
  /**
   * Default constructor.
   */
  FEMMeshCell()  {}

  /**
   * Virtual destructor
   */
  virtual ~FEMMeshElement() {}

public:

  /**
   * ID of the material object that this element uses. if the material with
   * this id does not exist when creating the element, it will be created.
   * the actual data that define the matrial (E, ...) will be filled in the
   * object later
   * 
   *  ? Do we need it ? if we will have the Pointer to the actual material ?
   */
  int m_MaterialID;

  MaterialType::Pointer     m_Material;

};


}} // end namespace itk::fem

#endif 



