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

public:
 
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



