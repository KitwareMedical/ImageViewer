/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMMesh.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMMesh_h
#define __itkFEMMesh_h


#include "itkMesh.h"
#include "itkFEMElementBase.h"


namespace itk {
namespace fem {




/** 
 * \class FEMMesh
 * \brief Translate itk mesh to appropriate FEM input
 *
 * FEM solver object which links to the itk's mesh. It takes a mesh as an input and
 * converts it into a FEM representation. It is templated over the input mesh type i.e.
 * we assume that the template parameter class has itk mesh functionality.
 */
template<class TMesh>
class FEMMesh : public TMesh 

{

public:
   /** Standard typedefs. */
  typedef FEMMesh                   Self;
  typedef TMesh                     Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Standard part of every itk Object.
    \todo probably a fully resolved type should be used here... */
  itkTypeMacro(Mesh, TMesh); 

  /** Hold on to the type information specified by the template parameters. */
  typedef typename Superclass::MeshTraits                    MeshTraits;
  typedef typename Superclass::CellTraits                    CellTraits;
  typedef typename Superclass::CellsContainer                CellsContainer;
  typedef typename Superclass::CellIdentifier                CellIdentifier;
  typedef typename Superclass::CellIdentifier                ElementIdentifier;

  /** Convenient enums obtained from TMeshTraits template parameter. */
  enum {PointDimension = MeshTraits::PointDimension};
  enum {MaxTopologicalDimension = MeshTraits::MaxTopologicalDimension};
  
    
  /** The container type for use in storing Elements.  It must conform to
   * the IndexedContainer interface. */
  typedef FEMElementBase< Self >                                  ElementBaseType;
  typedef ElementBaseType::Pointer                                ElementBasePointer;
  typedef VectorContainer< CellIdentifier , ElementBasePointer  > ElementsContainer;
  typedef ElementsContainer::Iterator                             ElementsContainerIterator;
  typedef ElementBaseType::MultiVisitor                           ElementMultiVisitorType;
 
  itkGetObjectMacro( ElementsContainer, ElementsContainer );
  itkSetObjectMacro( ElementsContainer, ElementsContainer );


  /** This method iterates over all the element in the FEMMesh and has
   *  each element Accept the MultiVisitor. See MultiVisitor for more 
   *  information.  (Note, this follows the Visitor Design Pattern.) */
  virtual void Accept(ElementMultiVisitorType* mv);


protected:

  /** Constructor for use by New() method. */
  FEMMesh();
  virtual ~FEMMesh();
  void PrintSelf(std::ostream& os, Indent indent) const;
     
private:
  FEMMesh(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
          
  ElementsContainer::Pointer      m_ElementsContainer;

};


 
template <typename TMesh>
void
FEMMesh<TMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}



}} // end namespace itk::fem

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFEMMesh.txx"
#endif

#endif /* #ifndef __itkFEMMesh_h */



