/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMNode.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMNode_h
#define __itkFEMNode_h

#include "itkArray.h"


namespace itk {
namespace fem {



/**
 * \class FEMNode
 * \brief Base class for Nodes in a mesh which is used for finite element modeling.
 *
 * Every Node in a finite element mesh should also define the Displacement Field 
 * Variables that exist at that parametric point. A parametric point with the 
 * Displacement Field defined will be refered to as a FEM Node.
 *
 * Derived class should define a function that creates and initializes a
 * proper FEM Node object. 
 * 
 * By using different derived classes in a mesh as a Node types 
 * it is possible to create different FEM Nodes in a system.
 *
 * Note that Nodes are not necessarily associated one-to-one with the Points
 * at the geometrical level. This is due to the fact that the Cells representing
 * the geometry of the element can use a different order than the one used for
 * interpolating the degrees of freedom. For example triangular flat cells are
 * a linear approximation to the shape of an object but they can still be associated
 * to quadric Elements that compute a second order representation of the Displacement
 * Fields Variables.
 *
 */
class FEMNode 
{

public:


  // Type used for representing the displacement field variables
  // (also known as "Deegrees of Freedom").
  //
  // This is defined in order to provide a common API for the 
  // FEM solver which will only receive arrays of this type 
  //
  typedef  float  DisplacementType;

  virtual unsigned int GetNumberOfDisplacementComponents(void) const = 0;

  // Return one of the components of the Displacement field.
  // The component is identified by the integer "Id".
  virtual const DisplacementType & GetDisplacement( unsigned int Id ) const = 0;



  FEMNode()  {}            /** default constructor */
  virtual ~FEMNode() {}    /** virtual destructor */

  
private:

   // Every Derived class is responsible for instantiating ivars
   // for its Displacement Field Components.
   // That should allow to minimize the extra memory required for
   // storage. It should be expected to create thousands or millions
   // of nodes.

};





}} // end namespace itk::fem

#endif 



