/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtk2itk.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// Disable warning for long symbol names in this file only
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "itkMesh.h"
#include "itkTriangleCell.h"
#include "itkQuadrilateralCell.h"
#include "vtkDataSetWriter.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDataSetReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkCellArray.h"

typedef itk::Mesh<float, 3,
  itk::DefaultStaticMeshTraits< float, 3, 3, float, float > > floatMesh;

void Display(vtkUnstructuredGrid* itkgrid, vtkUnstructuredGrid* vtkgrid)
{
// Create the renderer and window stuff
  vtkRenderer* ren1 = vtkRenderer::New();
  vtkRenderer* ren2 = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren1);
  renWin->AddRenderer(ren2);
  vtkRenderWindowInteractor* inter = vtkRenderWindowInteractor::New();
  inter->SetRenderWindow(renWin);

  vtkDataSetMapper* mapper = vtkDataSetMapper::New();
  mapper->SetInput(itkgrid);
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);
  vtkDataSetMapper* mapper2 = vtkDataSetMapper::New();
  mapper2->SetInput(vtkgrid);
  vtkActor* actor2 = vtkActor::New();
  actor2->SetMapper(mapper2);
  ren1->SetViewport(0.0, 0.0, 0.5, 1.0);
  ren2->SetViewport(0.5, 0.0, 1.0, 1.0);
  ren2->AddActor(actor2);
  // add the actor and start the render loop
  ren1->AddActor(actor);
  renWin->Render();
  inter->Start();
  
  mapper->Delete();
  actor->Delete();
  ren1->Delete();
  mapper2->Delete();
  actor2->Delete();
  ren2->Delete();
  renWin->Delete();
}

#ifndef VTK_HAS_ID_TYPE
// if you get a syntax error hear, then your VTK
// has vtkIdType already defined
typedef long vtkIdType;
#endif


floatMesh::Pointer MeshFromUnstructuredGrid(vtkUnstructuredGrid* grid)
{
  // Create a new mesh
  floatMesh::Pointer mesh(floatMesh::New());
  // Get the points from vtk
  vtkPoints* vtkpoints = grid->GetPoints();
  int numPoints = vtkpoints->GetNumberOfPoints();
  // Create a compatible point container for the mesh
  // the mesh is created with a null points container
  floatMesh::PointsContainer::Pointer points = 
    floatMesh::PointsContainer::New();
  // Resize the point container to be able to fit the vtk points
  points->Reserve(numPoints);
  // Set the point container on the mesh
  mesh->SetPoints(points);
  for(int i =0; i < numPoints; i++)
    {
    float* apoint = vtkpoints->GetPoint(i);
    mesh->SetPoint(i, floatMesh::PointType(apoint));
    }
  vtkCellArray* vtkcells = grid->GetCells();
  floatMesh::CellsContainerPointer cells = floatMesh::CellsContainer::New();
  mesh->SetCells(cells);
  // extract the cell id's from the vtkUnstructuredGrid
  int numcells = vtkcells->GetNumberOfCells();
  int* vtkCellTypes = new int[numcells];
  int cellId =0;
  for(; cellId < numcells; cellId++)
    {
    vtkCellTypes[cellId] = grid->GetCellType(cellId);
    }
  
  cells->Reserve(numcells);
  vtkIdType npts;
  vtkIdType* pts;
  cellId = 0;
  for(vtkcells->InitTraversal(); vtkcells->GetNextCell(npts, pts); cellId++)
    {
    floatMesh::CellAutoPointer c;
    switch(vtkCellTypes[cellId])
      {
      case VTK_TRIANGLE:
        {
        typedef itk::CellInterface<float, floatMesh::CellTraits> CellInterfaceType;
        typedef itk::TriangleCell<CellInterfaceType> TriangleCellType;
        TriangleCellType * t = new TriangleCellType;
        t->SetPointIds((unsigned long*)pts);
        c.TakeOwnership( t );
        break;
        }  
      case VTK_QUAD:
        {
        typedef itk::CellInterface<float, floatMesh::CellTraits> CellInterfaceType;
        typedef itk::QuadrilateralCell<CellInterfaceType> QuadrilateralCellType;
        QuadrilateralCellType * t = new QuadrilateralCellType;
        t->SetPointIds((unsigned long*)pts);
        c.TakeOwnership( t );
        break;
        }  
      case VTK_EMPTY_CELL:
      case VTK_VERTEX:
      case VTK_POLY_VERTEX:
      case VTK_LINE:
      case VTK_POLY_LINE:
      case VTK_TRIANGLE_STRIP:
      case VTK_POLYGON:
      case VTK_PIXEL:
      case VTK_TETRA:
      case VTK_VOXEL:
      case VTK_HEXAHEDRON:
      case VTK_WEDGE:
      case VTK_PYRAMID:
      case VTK_PARAMETRIC_CURVE:
      case VTK_PARAMETRIC_SURFACE:
      default:
        std::cerr << "Warning unhandled cell type " 
                  << vtkCellTypes[cellId] << std::endl;
        ;
      }
    mesh->SetCell(cellId, c);
    }

  mesh->Print(std::cout);
  return mesh;
}




class VistVTKCellsClass
{
  vtkCellArray* m_Cells;
  int* m_LastCell;
  int* m_TypeArray;
public:
  // typedef the itk cells we are interested in
  typedef itk::CellInterface<
                      floatMesh::PixelType, 
                      floatMesh::CellTraits >  CellInterfaceType;

  typedef itk::TriangleCell<CellInterfaceType>      floatTriangleCell;
  typedef itk::QuadrilateralCell<CellInterfaceType> floatQuadrilateralCell;

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
  // Visit a triangle and create the VTK_TRIANGLE cell 
  void Visit(unsigned long cellId, floatTriangleCell* t)
    {
      m_Cells->InsertNextCell(3,  (vtkIdType*)t->PointIdsBegin());
      m_TypeArray[*m_LastCell] = VTK_TRIANGLE;
      (*m_LastCell)++;
    }
  // Visit a triangle and create the VTK_QUAD cell 
  void Visit(unsigned long cellId, floatQuadrilateralCell* t)
    {
      m_Cells->InsertNextCell(4,  (vtkIdType*)t->PointIdsBegin());
      m_TypeArray[*m_LastCell] = VTK_QUAD;
      (*m_LastCell)++;
    }
};
  
typedef itk::CellInterfaceVisitorImplementation<
float, floatMesh::CellTraits,
  itk::TriangleCell< itk::CellInterface<floatMesh::PixelType, floatMesh::CellTraits > >, 
  VistVTKCellsClass> TriangleVisitor;


typedef itk::CellInterfaceVisitorImplementation<
float, floatMesh::CellTraits,
  itk::QuadrilateralCell< itk::CellInterface<floatMesh::PixelType, floatMesh::CellTraits > >, 
  VistVTKCellsClass> QuadrilateralVisitor;


vtkUnstructuredGrid* MeshToUnstructuredGrid(floatMesh* mesh)
{
  // Get the number of points in the mesh
  int numPoints = mesh->GetNumberOfPoints();
  if(numPoints == 0)
    {
    mesh->Print(std::cerr);
    std::cerr << "no points in Grid " << std::endl;
    exit(-1);
    }
  // Create a vtkUnstructuredGrid
  vtkUnstructuredGrid* vgrid = vtkUnstructuredGrid::New();

  // Create the vtkPoints object and set the number of points
  vtkPoints* vpoints = vtkPoints::New();
  vpoints->SetNumberOfPoints(numPoints);
  // iterate over all the points in the itk mesh filling in
  // the vtkPoints object as we go
  floatMesh::PointsContainer::Pointer points = mesh->GetPoints();
  for(floatMesh::PointsContainer::Iterator i = points->Begin();
      i != points->End(); ++i)
    {
    // Get the point index from the point container iterator
    int idx = i->Index();
    // Set the vtk point at the index with the the coord array from itk
    // itk returns a const pointer, but vtk is not const correct, so
    // we have to use a const cast to get rid of the const
    vpoints->SetPoint(idx, const_cast<float*>(i->Value().GetDataPointer()));
    }
  // Set the points on the vtk grid
  vgrid->SetPoints(vpoints);
  
  // Now create the cells using the MulitVisitor
  // 1. Create a MultiVisitor
  floatMesh::CellType::MultiVisitor::Pointer mv =
    floatMesh::CellType::MultiVisitor::New();
  // 2. Create a triangle and quadrilateral visitor
  TriangleVisitor::Pointer tv = TriangleVisitor::New();
  QuadrilateralVisitor::Pointer qv =  QuadrilateralVisitor::New();
  // 3. Set up the visitors
  int vtkCellCount = 0; // running counter for current cell being inserted into vtk
  int numCells = mesh->GetNumberOfCells();
  int *types = new int[numCells]; // type array for vtk 
  // create vtk cells and estimate the size
  vtkCellArray* cells = vtkCellArray::New();
  cells->EstimateSize(numCells, 4);
  // Set the TypeArray CellCount and CellArray for both visitors
  tv->SetTypeArray(types);
  tv->SetCellCounter(&vtkCellCount);
  tv->SetCellArray(cells);
  qv->SetTypeArray(types);
  qv->SetCellCounter(&vtkCellCount);
  qv->SetCellArray(cells);
  // add the visitors to the multivisitor
  mv->AddVisitor(tv);
  mv->AddVisitor(qv);
  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  mesh->Accept(mv);
  
  // Now set the cells on the vtk grid with the type array and cell array
  vgrid->SetCells(types, cells);
  
  // Clean up vtk objects (no vtkSmartPointer ... )
  cells->Delete();
  vpoints->Delete();
  // return the vtkUnstructuredGrid
  return vgrid;
}


int main(int ac, char** av)
{
  const char* fname = "c:/blow.vtk";
  if(ac > 1 )
    {
    fname = av[1];
    }
  
  
  vtkDataSetReader* reader = vtkDataSetReader::New();
  reader->SetFileName(fname);
  reader->SetScalarsName("thickness9");
  reader->SetVectorsName("displacement9");
  std::cerr << "Begin reading vtk data " << std::endl;
  reader->Update();
  std::cerr << "End reading vtk data " << std::endl;
  vtkDataSet* data = reader->GetOutput();
  if( !data )
    {
    std::cerr << "Data is null" << std::endl;
    }
  // Convert from vtk to itk
  std::cerr << "Begin conversion to itkMesh " << std::endl;
  floatMesh::Pointer m = 
    MeshFromUnstructuredGrid(reader->GetUnstructuredGridOutput());
  std::cerr << "End conversion to itkMesh " << std::endl;
  // Convert back from itk to vtk
  std::cerr << "Begin conversion to vtkUnstructuredGrid " << std::endl;
  vtkUnstructuredGrid* grid = MeshToUnstructuredGrid(m);
  std::cerr << "End conversion to vtkUnstructuredGrid " << std::endl;
  
  std::cerr << "Begin vtk dispaly of both grids " << std::endl;
  Display(grid, reader->GetUnstructuredGridOutput());
  vtkDataSetWriter* writer = vtkDataSetWriter::New();
  writer->SetInput(grid);
  writer->SetFileName("./itkblow.vtk");
  writer->Update();
#ifdef VTK_USE_ANSI_STDLIB  
  grid->Print(std::cout);
#endif
  reader->Delete();
  grid->Delete();
  return 0;
}

