/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMTruss.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEM.h"
#include <iostream>

using namespace itk::fem;
using namespace std;



/**
 * Easy access to the FEMObjectFactory. We create a new class
 * whose name is shorter and it's not templated...
 */
class FEMOF : public FEMObjectFactory<FEMLightObject>
{};


/**
 * This example constructs a same problem as described in file truss.fem
 * by creating the appropriate classes.
 */
int main( int argc, char *argv[] ) {

  /**
   * First we create the FEM solver object. This object stores pointers
   * to all objects that define the FEM problem. One solver object
   * effectively defines one FEM problem.
   */
  Solver S;




  /**
   * Below we'll define a FEM problem described in the chapter 25.3-4,
   * from the book, which can also be downloaded from 
   * http://titan.colorado.edu/courses.d/IFEM.d/IFEM.Ch25.d/IFEM.Ch25.pdf
   */





  /**
   * We start by creating four Node objects. One of them is of
   * class NodeXY. It has two displacements in two degrees of freedom.
   * 3 of them are of class NodeXYrotZ, which also includes the rotation
   * around Z axis.
   */
  
  /** We'll need these pointers to create and initialize the objects. */
  NodeXYrotZ::Pointer n1;
  NodeXY::Pointer n2;

  /**
   * We create the objects through the object factory to make everything
   * compatible when both smart and dumb pointers are used. Since we
   * want to cast the pointer to the NodeXYrotZ object, not the SmartPointer
   * class, we use both reference and de-reference operator (&*). This
   * has absolutely no effect on a compiled code when smart pointers are
   * not used. The resulting (dumb) pointer NodeXYrotZ* is then automatically
   * converted to SmartPointer if necessary and stored in n1.
   */
  n1=static_cast<NodeXYrotZ*>( &*FEMOF::Create(NodeXYrotZ::OFID) );

  /**
   * Additionally we could create the objects in the standard way...
   */
  //n1=new NodeXYrotZ;
  /**
   * ... or using SmartPointers...
   */
  //n1=NodeXYrotZ::New();

  /**
   * Initialize the data members inside the node objects. Basically here
   * we only have to specify the X and Y coordinate of the node in global
   * coordinate system.
   */
  n1->X=-4.0;
  n1->Y=3.0;

  /**
   * Convert the node pointer into a special pointer (FEMP) and add it to
   * the nodes array inside the solver class. The special pointer now
   * owns the object and we don't have to keep track of it anymore.
   * Here we again have to use both reference and de-reference operator (&*),
   * because we can't cast SmartPointer<NodeXYrotZ> to SmartPointer<Node>.
   * If smart pointers are not used, the operators have no effect on the
   * compiled code.
   */
  S.node.push_back( FEMP<Node>(&*n1) );
     
  /**
   * Special pointers (class FEMP) create a copy of the objects,
   * when a pointer object is copied. We can use that feature to quickly
   * create many similar objects without using the FEMObjectFactory,
   * New() function, or new operator.
   *
   * Operator[] on FEMPArray returns the special pointer (FEMP), while
   * the function operator () returns the actual pointer.
   */
  S.node.push_back( S.node[0] );
  S.node.push_back( S.node[0] );
  
  /**
   * Now we have to update coordinates inside the newly created nodes.
   * Since we're getting back the pointers to base class, we need to
   * cast it to the proper class. dynamic_cast is used in this case.
   * Note that the Y coordinate of a node remains 3.0, so we don't have
   * to change it.
   */
  dynamic_cast<NodeXYrotZ*>( &*S.node(1) )->X=0.0;
  dynamic_cast<NodeXYrotZ*>( &*S.node(2) )->X=4.0;

  /**
   * Note that we could also create new objects by always using the
   * FEMObjectFactory and not copying the objects inside arrays.
   * This is what we'll do for the final node.
   */
  n2=static_cast<NodeXY*>( &*FEMOF::Create(NodeXY::OFID) );
  n2->X=0.0;
  n2->Y=0.0;
  S.node.push_back( FEMP<Node>(&*n2) );

  /**
   * Automatically assign the global numbers (IDs) to
   * all the objects in the array. (first object gets number 0,
   * second 1, and so on). We could have also specified the GN
   * member in all the created objects above, but this is easier.
   */
  S.node.Renumber();





  
  /**
   * Then we have to create the materials that will define
   * the elements.
   */
  MaterialStandard::Pointer m;
  m=static_cast<MaterialStandard*>( &*FEMOF::Create(MaterialStandard::OFID) );
  m->GN=0;       /** Global number of the material */
  m->E=30000.0;  /** Young modulus */
  m->A=0.02;     /** Crossection area */
  m->I=0.004;    /** Momemt of inertia */
  S.mat.push_back( FEMP<Material>(&*m) );

  m=static_cast<MaterialStandard*>( &*FEMOF::Create(MaterialStandard::OFID) );
  m->GN=1;       /** Global number of the material */
  m->E=200000.0;  /** Young modulus */
  m->A=0.001;     /** Crossection area */
  /**
   * Momemt of inertia. This material will be used in
   * the Bar element, which doesn't need this constant.
   */
  m->I=0.0;
  S.mat.push_back( FEMP<Material>(&*m) );

  m=static_cast<MaterialStandard*>( &*FEMOF::Create(MaterialStandard::OFID) );
  m->GN=2;       /** Global number of the material */
  m->E=200000.0;  /** Young modulus */
  m->A=0.003;     /** Crossection area */
  /**
   * Momemt of inertia. This material will be used in
   * the Bar element, which doesn't need this constant.
   */
  m->I=0.0;
  S.mat.push_back( FEMP<Material>(&*m) );





  /**
   * Next we create the finite elements that use the above
   * created nodes. We'll have 3 Bar elements ( a simple
   * spring in 2D space ) and 2 Beam elements that also
   * accounts for bending.
   */
  Beam2D::Pointer e1;
  Bar2D::Pointer e2;

  e1=static_cast<Beam2D*>( &*FEMOF::Create(Beam2D::OFID) );

  /**
   * Initialize the pointers to correct node objects. We use the
   * Find function of the FEMPArray to search for object (in this
   * case node) with given GN. Since the Beam2D element requires
   * nodes of class NodeXYrotZ, we have to make sure that we
   * have the right node object by using dynamic_cast.
   */
  e1->GN=0;
  e1->m_node[0]=dynamic_cast<NodeXYrotZ*>( &*S.node.Find(0) );
  e1->m_node[1]=dynamic_cast<NodeXYrotZ*>( &*S.node.Find(1) );

  /** same for material */
  e1->m_mat=dynamic_cast<MaterialStandard*>( &*S.mat.Find(0) );
  S.el.push_back( FEMP<Element>(&*e1) );

  /** Create the other elements */
  e1=static_cast<Beam2D*>( &*FEMOF::Create(Beam2D::OFID) );
  e1->GN=1;
  e1->m_node[0]=dynamic_cast<NodeXYrotZ*>( &*S.node.Find(1) );
  e1->m_node[1]=dynamic_cast<NodeXYrotZ*>( &*S.node.Find(2) );
  e1->m_mat=dynamic_cast<MaterialStandard*>( &*S.mat.Find(0) );
  S.el.push_back( FEMP<Element>(&*e1) );

  /**
   * Note that Bar2D requires nodes of class NodeXY. But since the
   * class NodeXYrotZ is derived from NodeXY, we can cast it
   * to NodeXY without loosing any information and seemlessly
   * connect these two elements together. Error checking is
   * guarantied by compiler either at compile time or by
   * dynamic_cast operator.
   */
  e2=static_cast<Bar2D*>( &*FEMOF::Create(Bar2D::OFID) );
  e2->GN=2;
  e2->m_node[0]=dynamic_cast<NodeXY*>( &*S.node.Find(0) );
  e2->m_node[1]=dynamic_cast<NodeXY*>( &*S.node.Find(3) );
  e2->m_mat=dynamic_cast<MaterialStandard*>( &*S.mat.Find(1) );
  S.el.push_back( FEMP<Element>(&*e2) );

  e2=static_cast<Bar2D*>( &*FEMOF::Create(Bar2D::OFID) );
  e2->GN=3;
  e2->m_node[0]=dynamic_cast<NodeXY*>( &*S.node.Find(1) );
  e2->m_node[1]=dynamic_cast<NodeXY*>( &*S.node.Find(3) );
  e2->m_mat=dynamic_cast<MaterialStandard*>( &*S.mat.Find(2) );
  S.el.push_back( FEMP<Element>(&*e2) );

  e2=static_cast<Bar2D*>( &*FEMOF::Create(Bar2D::OFID) );
  e2->GN=4;
  e2->m_node[0]=dynamic_cast<NodeXY*>( &*S.node.Find(2) );
  e2->m_node[1]=dynamic_cast<NodeXY*>( &*S.node.Find(3) );
  e2->m_mat=dynamic_cast<MaterialStandard*>( &*S.mat.Find(1) );
  S.el.push_back( FEMP<Element>(&*e2) );






  /**
   * Apply the boundary conditions and external forces (loads).
   */

  /**
   * The first node is completely fixed i.e. both displacements
   * are fixed to 0.
   * 
   * This is done by using the LoadBC class.
   */
  LoadBC::Pointer l1;

  l1=static_cast<LoadBC*>( &*FEMOF::Create(LoadBC::OFID) );

  /**
   * Here we're saying that the first degree of freedom at first node
   * is fixed to value m_value=0.0. See comments in class LoadBC declaration
   * for more information. Note that the m_value is a vector. This is useful
   * when having isotropic elements. This is not the case here, so we only
   * have a scalar.
   */
  l1->m_element = &*S.el.Find(0);
  l1->m_dof = 0;
  l1->m_value = vnl_vector<double>(1,0.0);
  S.load.push_back( FEMP<Load>(&*l1) );

  /**
   * In a same way we also fix the second DOF in a first node and the
   * second DOF in a third node (it's only fixed in Y direction).
   */
  l1=static_cast<LoadBC*>( &*FEMOF::Create(LoadBC::OFID) );
  l1->m_element = &*S.el.Find(0);
  l1->m_dof = 1;
  l1->m_value = vnl_vector<double>(1,0.0);
  S.load.push_back( FEMP<Load>(&*l1) );

  l1=static_cast<LoadBC*>( &*FEMOF::Create(LoadBC::OFID) );
  l1->m_element = &*S.el.Find(1);
  l1->m_dof = 4;
  l1->m_value = vnl_vector<double>(1,0.0);
  S.load.push_back( FEMP<Load>(&*l1) );


  /**
   * Now we apply the external force on the fourth node. The force is specified
   * by a vector [20,-20] in global coordinate system. This is the second point
   * of the third element in a system.
   */
  LoadNode::Pointer l2;

  l2=static_cast<LoadNode*>( &*FEMOF::Create(LoadNode::OFID) );
  l2->m_element=S.el.Find(2);
  l2->m_pt=1;
  l2->F=vnl_vector<double>(2);
  l2->F[0]=20;
  l2->F[1]=-20;
  S.load.push_back( FEMP<Load>(&*l2) );





  /**
   * The whole problem is now stored inside the Solver class.
   * Note that in the code above we don't use any of the
   * constructors that make creation of objects easier and with
   * less code. See declaration of classes for more info.
   */





  /**
   * We can now solve for displacements.
   */

  /**
   * Assign a unique id (global freedom number - GFN)
   * to every degree of freedom (DOF) in a system.
   */
  S.GenerateGFN();

  /**
   * Assemble the master stiffness matrix. In order to do this
   * the GFN's should already be assigned to every DOF.
   */
  S.AssembleK();

  /**
   * Invert the master stiffness matrix.
   */
  S.DecomposeK();

  /**
   * Assemble the master force vector (from the applied loads)
   */
  S.AssembleF();

  /**
   * Solve the system of equations for displacements (u=K^-1*F)
   */
  S.Solve();

  /**
   * Copy the displacemenets which are now stored inside
   * the solver class back to nodes, where they belong.
   */
  S.UpdateDisplacements();

  /**
   * Output displacements of all nodes in a system;
   */
  std::cout<<"\nNodal displacements:\n";
  for( ::itk::fem::Solver::NodeArray::iterator n = S.node.begin(); n!=S.node.end(); n++)
  {
    std::cout<<"Node#: "<<(*n)->GN<<": ";
    /** For each DOF in the node... */
    for( unsigned int d=0, dof; (dof=(*n)->GetDegreeOfFreedom(d))!=::itk::fem::Element::InvalidDegreeOfFreedomID; d++ )
    {
      std::cout<<S.GetSolution(dof);
      std::cout<<",  ";
    }
    std::cout<<"\b\b\b \b\n";
  }

  cout<<"\n";

  return 0;

}
