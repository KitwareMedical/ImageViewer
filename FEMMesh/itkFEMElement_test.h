/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement_test.h
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
 *
 * In addition to cell, each element is also capable of storing
 * information in it.
 * This additional information is represented as degrees of
 * freedom (DOF). One could, for example, add temparature values
 * to the surface mesh and later display this mesh with different
 * colors representing different temparatures.
 *
 * To expand the above example, one could store not just one, but
 * several temparature values in each cell inside a mesh. Typically
 * values are stored at locations corresponding to a point that
 * defines a mesh, but this is not necessary. The problem now
 * arrises, how to link the DOFs between different Cells in a mesh.
 * Clearly, if the temparature is stored at a point, and this point
 * is shared by many cellls, all of them must also share that
 * temparature values. DOFs of cells must somehow be linked together
 * much like the points in mesh.
 *
 * To keep things general, we define a virtual function GetDOFatPoint
 * that maps between points to ALL corresponding DOFs. Since DOF can
 * also be associated with the edge in a cell, the array for mapping
 * from edges to DOFs also exits.
 * Note that these two fucntion could return negative values to
 * indicate that the DOF doesn't exist at that point/edge (for
 * example: if a temparature DOF is stored only in the middle of the
 * cell, or there is no temparature assiciated with an edge...)
 * 
 * Note that derived classes must provide storage for this maping
 * tables. They can be stored in arrays that are STATIC to the cell,
 * so that memory requirements are negligible.
 *
 * Points that define cells, have to be specified from outside
 * (this is normally done by the Mesh class) by using SetPoint
 * member function.
 *
 * Once all points are specified, the function LinkDOFs (Luis,
 * think of a better name) can be called to connect the DOFs in
 * a mesh if they exist.
 */
class FEMElement 
{
public:

  ///////////////////////////
  // Frequently used types
  //

  typedef FEMNode   NodeType;

  /**
   * Float type used in Node and derived classes
   */
  typedef NodeType::FloatType     FloatType;

  /**
   * Class type used to specify degrees of freedom.
   */
  typedef int DOFType;



  /////////////////////////////////////////////////////////////
  // Methods related to finite element modeling that
  // are required to assemble the system by the Solver class
  //

  /**
   * Return the number of degrees of freedom (DOF) for a derived element class
   */
  virtual int NumberOfDegreesOfFreedom( void ) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual vnl_matrix<FloatType> Ke() = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  //virtual vnl_matrix<FloatType> Me() const;




  ////////////////////////////////////
  // Methods related to DOF linking
  //

  /**
   * Methods to get and set DOFs. Note that DOFType can be
   * anyting, but is usually something simple like int or a pointer
   * to an object. Derived classes must also provide storage for
   * objects of class DOFType.
   */
  virtual DOFType GetDOF( unsigned int id ) = 0;
  virtual void SetDOF( unsigned int id, DOFType ) = 0;
  virtual void ClearDOFs(void) = 0; /**< Releases all DOFs used by element */

  /**
   * Returns a DOF object that is associated with given point.
   * There may be more DOFs associated with each point. You can
   * specify which one, by providing the dof parameter.
   * Function returnes invalid DOF object if parameters are out of range.
   *
   * \param pt Local index of a point within a cell (0 - number_of_points-1).
   * \param dof Number of DOF present at point pt (0 - number_of_dofs_at_point-1).
   *
   */
  virtual DOFType GetDOFatPoint( unsigned int pt, unsigned int dof ) = 0;

  /**
   * Links the DOFs in a current element with the elements
   * in the neighborhood. The basic implementation of this
   * function is provided here with the base class. If the
   * derived class requires some special DOF linking, this
   * function can be redeclared.
   */
  virtual void LinkDOFs(void);

};




}} // end namespace itk::fem

#endif 



