/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEM2DDeformationNode.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEM2DDeformationNode_h
#define __itkFEM2DDeformationNode_h

#include "itkFEMNode.h"

namespace itk {
namespace fem {



/**
 * \class FEM2DDeformationNode
 * \brief Base class for Nodes in a mesh which is used for finite element modeling.
 *
 * Every point in finite element mesh should also define the degrees of freedom
 * that exist at that point. A point with the degrees of freedom defined will
 * be refered to as a FEM Node.
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
 * interpolating the degrees of freedom.
 *
 */
class FEM2DDeformationNode : public FEMNode
{

public:

  
  typedef FEMNode     Superclass;

  // Type used for representing the deegrees of freedom.
  // This is defined in order to provide a common API for the 
  // FEM solver which will only receive arrays of this type 
  typedef  Superclass::DisplacementType   DisplacementType;
  typedef  Superclass::FloatType          FloatType;

  virtual unsigned int GetNumberOfDegreesOfFreedom(void) const 
    { return 2; }

  FEM2DDeformationNode()  { /** default constructor */
      m_Displacements = DisplacementContainerType(2);
      }            
  virtual ~FEM2DDeformationNode() {}    /** virtual destructor */

private:

  // Derived classes will provide storage for the actual degrees of freedom.

};





}} // end namespace itk::fem

#endif 



