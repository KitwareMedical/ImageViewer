/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMHeatConduction2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMHeatConduction_h
#define __itkFEMHeatConduction_h


#include "itkFEMMesh.h"
#include "itkFEMElementHeatConductionVisitor.h"


namespace itk {
namespace fem {


/** 
 * \class FEMHeatConduction2D
 * \brief Definition of a Solver appropiated for 
 * solving the problem of Heat conduction in 2D
 *
 */

class FEMHeatConduction2D : public Object 
{
 
public:
  enum { PointsDimension              = 2 };
  enum { MaximumTopologicalDimension  = 2 };
  enum { DisplacementDimension        = 1 };

  // Type used for representing the coordinates of points
  // in geometric space
  typedef float     CoordinateRepresentationType;

  // Type used by the Cells for the weights required for
  // interpolation of points and values.
  typedef double    InterpolationWeightsType;

  // PointDataType is the type that will be used for the
  // Degrees of Freedom (or DisplacementField) of the 
  // FEM problem.
  typedef double                DisplacementRepresentationType;
  typedef double                DisplacementType; // one Temperature


  // Type used for computation and for the global stiffness matrix
  typedef double                RealType; 


  typedef DisplacementType      PointDataType;

  // For FEM it is expected that eventual values stored
  // at the Cells will be of the same type that those
  // values stored at the Points.
  typedef PointDataType         CellDataType;


  // Traits for all the elements of the Mesh
  typedef itk::DefaultStaticMeshTraits<
                                PointDataType,
                                PointsDimension,
                                MaximumTopologicalDimension,
                                CoordinateRepresentationType,
                                InterpolationWeightsType,
                                CellDataType
                                                  >  MeshTraits;

  // Type for the Cell Traits
  typedef MeshTraits::CellTraits                CellTraits;

  // Type for the Geometrical level
  typedef itk::Mesh<  PointDataType, 
                      PointsDimension, 
                      MeshTraits >              MeshType;

  /** FEM Mesh defined according to the geometrical level */
  typedef itk::fem::FEMMesh< MeshType >         FEMMeshType;

  /** Hold on to the type information specified by the template parameters. */
  typedef MeshType::CellMultiVisitorType          CellMultiVisitorType;

  
  typedef FEMElementHeatConductionVisitor< FEMMeshType > VisitorBaseType;


  typedef VisitorBaseType::StiffnessMatrixType    StiffnessMatrixType;


   const StiffnessMatrixType & GetStiffnessMatrix(void) const
            { return m_StiffnessMatrix;  }

public:

   /** Standard typedefs. */
  typedef FEMHeatConduction2D               Self;
  typedef Object                            Superclass;
  typedef SmartPointer<Self>                Pointer;
  typedef SmartPointer<const Self>          ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Standard part of every itk Object.
    \todo probably a fully resolved type should be used here... */
  itkTypeMacro( FEMHeatConduction2D, Object ); 

  /** Convenient enums obtained from TMeshTraits template parameter. */
  enum {PointDimension = MeshTraits::PointDimension};
  enum {MaxTopologicalDimension = MeshTraits::MaxTopologicalDimension};
  

  // this method visits every cell, applies the CellEquation() method
  // and collect values in the global Stiffness Matrix
  void AssembleMasterEquation(void);

  // Set/Get the Mesh used to solve this FEM problem
  itkSetObjectMacro( Mesh, FEMMeshType );
  FEMMeshType::Pointer GetMesh(void) 
                          { return m_Mesh.GetPointer(); }

protected:

  /** Constructor for use by New() method. */
  FEMHeatConduction2D();
  virtual ~FEMHeatConduction2D();
  void PrintSelf(std::ostream& os, Indent indent) const;
     
  // allocate the apropiated size for the matrix
  void InitializeStiffnessMatrix(void);


private:

  FEMHeatConduction2D(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
          
  // FEM Mesh
  FEMMeshType::Pointer            m_Mesh;

  // Global Stiffness Matrix
  StiffnessMatrixType             m_StiffnessMatrix;
  
};



}} // end namespace itk::fem


#endif /* #ifndef __itkFEMHeatConduction2D_h */



