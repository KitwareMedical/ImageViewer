/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElementBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkFEMElementBase_h
#define __itkFEMElementBase_h


#include "vnl/vnl_matrix.h"
#include "itkFEMElementVisitor.h"


// Define a macro for ElementsBase sub-classes to use
// to define the Accept and GetTopologyId virtuals used
// by the MultiVisitor class
#define itkElementVisitMacro(TopologyId) \
  static int GetTopologyId() {return TopologyId;}\
  virtual void Accept(unsigned long elementid, typename FEMElementBase< TFEMMesh >::MultiVisitor* mv)\
    {\
      FEMElementVisitor<TFEMMesh>::Pointer v = mv->GetVisitor(TopologyId);\
      if(v)\
        {\
        v->VisitFromElement(elementid,this);\
        }\
    }


namespace itk {
namespace fem {


/** 
 * \brief Base class for Elements in a FEM system.
 * 
 */
template < typename TFEMMesh >
class FEMElementBase 
{

public:

  /** Standard class typedefs. */
  typedef FEMElementBase            Self;
  typedef LightObject               Superclass;
  typedef        Self  *   Pointer;
  typedef  const Self  *   ConstPointer;


  /** \warning This is an Abstract class it is not expected to
      be instantiated. It should not have a New() macro  */


  /** Run-time type information (and related methods). */
  itkTypeMacro(Image, ImageBase);
 
  // this type is used to represent components of the load
  // and the Stiffness matrix
  typedef float       RealType;

  /** Type for representing the Stiffnes and Mass matrices  */
  typedef  vnl_matrix< RealType >       MatrixType;

  /** Type for representing sets of external loads */
  typedef  vnl_vector< RealType >       LoadsVectorType;

  /** Type of the displacement field at each point */
  typedef TFEMMesh                                  FEMMeshType;
  typedef typename FEMMeshType::PixelType           DisplacementType;

  /**  Cell Visitor interfaces */
  enum CellType {VERTEX_ELEMENT=0, LINE_ELEMENT, TRIANGLE_ELEMENT, 
        QUADRILATERAL_ELEMENT, POLYGON_ELEMENT, TETRAHEDRON_ELEMENT, 
        HEXAHEDRON_ELEMENT, LAST_ITK_ELEMENT, MAX_ITK_ELEMENTS=255};

  /**
   * Return the number of components of the Displacement field
   * also known as degrees of freedom (DOF) for a derived element class
   */
  virtual unsigned int GetNumberOfDisplacementComponents( void ) const = 0;


  /**
   *  The Cell return the displacement associated with the ith point
   *  of its list of points
   */
  virtual const DisplacementType & 
     GetDisplacement(unsigned int i, FEMMeshType *mesh ) const = 0;

  /**
   * Compute and return element stiffnes matrix in global coordinate system
   */
  virtual MatrixType GetStiffnessMatrix( const FEMMeshType * ) const = 0;

  /**
   * Compute and return element mass matrix in global coordinate system.
   * This is needed if dynamic problems (parabolic or hyperbolix d.e.)
   * need to be solved.
   */
  virtual MatrixType GetMassMatrix( void ) const 
    { return MatrixType(); }

  /** \brief A visitor that can visit different cell types in a mesh.
   * CellInterfaceVisitor instances can be registered for each
   * type of cell that needs to be visited.
   *
   * \ingroup MeshAccess */
  class MultiVisitor : public LightObject
  { 
  public:
    /**  Visitor type, because VisualC++ 6.0 does not like
     *  Visitor being a nested type of CellInterfaceVisitor   */
    typedef FEMElementVisitor< TFEMMesh > VisitorType;

    /** Standard class typedefs.   */
    typedef MultiVisitor       Self;
    typedef SmartPointer<Self>  Pointer;
      
    /** Method for creation through the object factory.   */
    //itkNewMacro(Self);
    static  Pointer New(void) { return new Self; }
  
    /** Run-time type information (and related methods).   */
    itkTypeMacro(MultiVisitor,LightObject);
  
    /** Typedefs for the visitor class.   */
    typedef typename VisitorType::Pointer VisitorPointer;

  public:
    VisitorPointer GetVisitor(int id)
      {
        if(id <= LAST_ITK_ELEMENT)
          {
          return m_Visitors[id];
          }
        else
          {
          std::map<int, VisitorType::Pointer>:: iterator pos = m_UserDefined.find(id);
          if(pos != m_UserDefined.end())
            {
            return (*pos).second;
            }
          }
        return 0;
      }
    void AddVisitor(VisitorType* v)
      {
        int id = v->GetElementTopologyId();
        if(id <= LAST_ITK_ELEMENT)
          {
          m_Visitors[id] = v;
          }
        else
          {
          m_UserDefined.insert(std::map<int, VisitorPointer>::value_type(id,v));
          }
      }
    ~MultiVisitor() {}
  protected:
    VisitorPointer m_Visitors[LAST_ITK_ELEMENT]; // fixed array set to the size from the enum
    std::map<int,VisitorPointer> m_UserDefined;  // user defined element types go here
  };


  /** This must be implemented by all sub-classes of FEMElementBase */
  virtual void Accept(unsigned long cellId, MultiVisitor * )= 0; 
  

protected:

  /** Constructor */
  FEMElementBase() {}

  /** Virtual Destructor */
  virtual ~FEMElementBase() {};



};




}} // end namespace itk::fem

#endif 



