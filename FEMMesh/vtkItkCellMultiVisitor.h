/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkItkCellMultiVisitor.h
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

#ifndef __vtkItkCellMultiVisitor_h
#define __vtkItkCellMultiVisitor_h

#include "vtkVertex.h"
#include "vtkLine.h"


#include "itkVertexCell.h"
#include "itkLineCell.h"




/**

  Implementation of the Multi Visitor Pattern

   */
template < typename TCellPixelType, typename TCellTraits >
class vtkItkCellMultiVisitor
{

  vtkCellArray* m_Cells;
  int* m_LastCell;
  int* m_TypeArray;

public:


  typedef TCellTraits                         CellTraits;
  typedef TCellPixelType                      CellPixelType;


  // typedef the itkCells we are interested in
  typedef itk::LineCell<CellPixelType,CellTraits>     LineCellType;
  typedef itk::VertexCell<CellPixelType,CellTraits>   VertexCellType;


  // Set the vtkCellArray that will be constructed
  void SetCellArray(vtkCellArray* a) 
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



  // Visit a vertex and create the VTK_VERTEX cell 
  void Visit(unsigned long cellId, VertexCellType * t)
    {
      m_Cells->InsertNextCell(1,  (vtkIdType*)t->PointIdsBegin());
      m_TypeArray[*m_LastCell] = VTK_VERTEX;
      (*m_LastCell)++;
    }


  // Visit a line and create the VTK_LINE cell 
  void Visit(unsigned long cellId, LineCellType * t)
    {
      m_Cells->InsertNextCell(2,  (vtkIdType*)t->PointIdsBegin());
      m_TypeArray[*m_LastCell] = VTK_LINE;
      (*m_LastCell)++;
    }

};
  


#endif


