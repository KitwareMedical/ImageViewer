/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBar2D.h
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

#include "itkFEMElement.h"
#include "itkFEMDisplacementNode.h"
#include "itkFEMMaterialStandard.h"
#include "itkFEMLoadElementBase.h"
#include "itkMacro.h"


namespace itk {
namespace fem {




/**
 * \class FEMElementBar2D
 * \brief 1D Bar (spring) finite element in 2D space.
 *
 * This element is defined by two NodeXY object and a MaterialStandard object.
 */
class FEMElementBar2D : public FEMElement
{

public:

  /** Type of Node used by this element  */
  typedef FEMDisplacementNode< 2 >          NodeType;   


  typedef FEMElement                        Superclass;
  typedef Superclass::MatrixType            MatrixType;
  typedef Superclass::LoadsVectorType       LoadsVectorType;


  /**
   * Number of Components of the Displacement Field
   * (also known as number of degrees of freedom)
   */
  enum { NumberOfDisplacementComponents = 4 };
  
  /**
   * Required virtual functions
   */

  /** Return the number of components of the Displacement Field 
      (also known as the number of Degrees of Freedom */
  virtual unsigned int GetNumberOfDisplacementComponents(void) const 
    { return NumberOfDisplacementComponents; }


  virtual unsigned int GetNumberOfNodes( void ) const 
    { return 2; }

  /**
   * Element stiffness matrix
   */
  MatrixType GetStiffnessMatrix(void) const
  {

    MatrixType stiffnessMatrix( NumberOfDisplacementComponents, NumberOfDisplacementComponents);


    /* X coordinate of the first Node... */
    /* 
    // Maybe what we really need here is the *Jacobian* of the transformation... 
    // something like:   this->GetCell()->GetJacobian();
    // not the actual coordinates of the points... (?)
    // Otherwise the element will have to be templated over the 
    // dimension of the points...which doesn't make much sense...   :-/
    //
    const PointType * point1 = this->GetCell()->GetPoint( 0 );
    const PointType * point2 = this->GetCell()->GetPoint( 0 );
    const CoordinateRepresentationType x1 = (*point1)[0];
    const CoordinateRepresentationType y1 = (*point1)[1];
    const CoordinateRepresentationType x2 = (*point2)[0];
    const CoordinateRepresentationType y2 = (*point2)[1];
    */

    return stiffnessMatrix;
  }

  /**
   * Function that handles all external loads applied to the element
   */
  LoadsVectorType GetExternalLoads(LoadElement * l) const;

  /**
   * Return a particular Component of the Displacement Field
   */
  const DisplacementType & GetDisplacementComponent(unsigned int i) const 
    {
    switch ( i ) {
    case 0:
      return m_Node1->GetDisplacement( 0 );
      break;
    case 1:
      return m_Node1->GetDisplacement( 1 );
      break;
    case 2:
      return m_Node2->GetDisplacement( 0 );
      break;
    case 3:
      return m_Node2->GetDisplacement( 1 );
      break;
    }
    itkGenericExceptionMacro(<<"Attempt to access displacement field variable with index not in [0:3]");
    return NumericTraits< DisplacementType >::Zero;
  }


  /**
   * Default constructor of an element must initialize the Nodes (add vertex objects)
   * But if a node for a specific point already exist, we should not create
   * another one.
   */
  FEMElementBar2D()
  {
    m_Node1 = 0;
    m_Node2 = 0;
  }

  void SetNode(unsigned int Id, FEMNode * node )
    {
      switch( Id )
      {
      case 0:   
        m_Node1 = dynamic_cast<NodeType *>( node );  
        break;
      case 1:
        m_Node2 = dynamic_cast<NodeType *>( node );
        break;
      default:
        itkGenericExceptionMacro(<<"FEM Element Bar only accepts Nodes with Id 0 or 1");
      }
    }

public:

  /**
   * Pointers to node objects that defines the element
   */
  NodeType * m_Node1;
  NodeType * m_Node2;

  /**
   * Pointer to geometric and material properties of the element
   */
  MaterialStandard::ConstPointer m_Material;

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementBar2D_h
