/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkItkElementMultiVisitor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkItkElementMultiVisitor_h
#define __vtkItkElementMultiVisitor_h

#include "vtkVertex.h"
#include "vtkLine.h"
#include "vtkTriangle.h"


#include "itkFEMElementTriangle.h"




/**

  Implementation of the Multi Visitor Pattern

   */
template < typename TMesh >
class vtkItkElementMultiVisitor
{

  vtkCellArray * m_Cells;
  int* m_LastCell;
  int* m_TypeArray;

public:

  // typedef the itkElements we are interested in
  typedef itk::fem::FEMElementTriangle< TMesh >       TriangleElementType;


  // Set the vtkElementArray that will be constructed
  void SetCellArray(vtkCellArray * a) 
    {
      m_Cells = a;
    }


  // Set the cell counter pointer
  void SetCellCounter(int* i)
    {
      m_LastCell = i;
    }


  // Set the type array for storing the vtk cell types
  void SetTypeArray(int* i)
    {
      m_TypeArray = i;
    }


  //
  // Starting here we add a set of Visit() methods.
  // One per type of cell the visitor expect to encounter.
  //


  // Visit a line and create the VTK_TRIANGLE cell 
  void Visit(unsigned long cellId, TriangleElementType * t)
    {
    vtkIdType numberOfPoints = 3;
    vtkIdType pointIds[numberOfPoints];
    typedef TriangleElementType::BaseCellType CellType;
    CellType & cell = t->GetCell();
    CellType::PointIdConstIterator pts = cell.PointIdsBegin();
    for(vtkIdType i=0; i<numberOfPoints; i++, pts++)
      {
      pointIds[i] = *pts;
      }
    m_Cells->InsertNextCell( numberOfPoints, pointIds ); 
    m_TypeArray[*m_LastCell] = VTK_TRIANGLE;
    (*m_LastCell)++;
    }


};
  


#endif


