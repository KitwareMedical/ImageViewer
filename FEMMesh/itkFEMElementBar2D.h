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

#include "itkFEMElementBase.h"
#include "itkFEMNodeXY.h"
#include "itkFEMMaterialStandard.h"

namespace itk {
namespace fem {




/**
 * \class Bar2D
 * \brief 1D Bar (spring) finite element in 2D space.
 *
 * This element is defined by two NodeXY object and a MaterialStandard object.
 */
class Bar2D : public LikeCell<CellTraits>, public FEMElement
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
  int N() const { return NDOF; };

  /**
   * Element stiffness matrix
   */
  vnl_matrix<Float> Ke() const
  {
    /* trying to obtain the X coordinate of the first point... */
    PointsIdBegin()->Position[0];  // something like this (or even simpler) would be nice
  }

  /**
   * Function that handles all external loads applied to the element
   */
  vnl_vector<Float> Fe(LoadElementPointer l) const;

  /**
   * Pointers to DOF displacements, which are stored in node classes.
   *
   * Here I need to return a specific pointer to Displacement objects
   * that are stored as an additional information in Points. How can I do that?
   * GetVertex just creates a new vertex.
   */
  Displacement* uDOF(int i) const {
    switch ( i ) {
    case 0:
      return &GetVertex[0]->uX;
      break;
    case 1:
      return &GetVertex[0]->uY;
      break;
    case 2:
      return &GetVertex[1]->uX;
      break;
    case 3:
      return &GetVertex[1]->uY;
      break;
    }
    return Element::uDOF(i);
  }


  /**
   * Default constructor of an element must initialize the Nodes (add vertex objects)
   * But if a node for a specific point already exist, we should not create
   * another one.
   */
  Bar2D()
  {
    for(int i=0;i<GetNumberOfVertices();i++)
    {
      // If a node for point with id i doesn't already exist, we must create a new one.
      if(GetVertex[i]==0)
      {
        // Create new node and make sure that the link
        // between corresponding point and node is correct.
      }
      else
      {
        // if a node does exist, we must check whether the node object is of correct type
        if (dynamic_cast<Bar2D::NodeType*>(&*GetVertex[i])==0)
        {
          // wrong node type
        }

      }

      // to avoid dynamic casting in the future, we should probably keep an array of pointers
      // locally in a class. This increases the memory requirements.

    }
  }


public:

  /**
   * Pointers to node objects that defines the element
   */
  NodeXY::ConstPointer m_node1;
  NodeXY::ConstPointer m_node2;

  /**
   * Pointer to geometric and material properties of the element
   */
  MaterialStandard::ConstPointer m_mat;

};





}} // end namespace itk::fem

#endif // #ifndef __itkFEMElementBar2D_h
