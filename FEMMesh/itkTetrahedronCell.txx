/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTetrahedronCell.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkTetrahedronCell_txx
#define _itkTetrahedronCell_txx
#include "itkTetrahedronCell.h"

namespace itk
{
namespace fem
{

/**
 * Standard CellInterface:
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellPointer
TetrahedronCell< TCellTraits >
::MakeCopy(void)
{
  CellPointer newCell(Self::New());
  newCell->SetPointIds(this->GetPointIds());
  return newCell;
}


/**
 * Standard CellInterface:
 * Get the topological dimension of this cell.
 */
template <typename TCellTraits>
int
TetrahedronCell< TCellTraits >
::GetDimension(void) const
{
  return Self::CellDimension;
}


/**
 * Standard CellInterface:
 * Get the number of points required to define the cell.
 */
template <typename TCellTraits>
int
TetrahedronCell< TCellTraits >
::GetNumberOfPoints(void) const
{
  return Self::NumberOfPoints;
}  


/**
 * Standard CellInterface:
 * Get the number of boundary features of the given dimension.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellFeatureCount
TetrahedronCell< TCellTraits >
::GetNumberOfBoundaryFeatures(int dimension) const
{
  switch (dimension)
    {
    case 0: return GetNumberOfVertices();
    case 1: return GetNumberOfEdges();
    case 2: return GetNumberOfFaces();
    default: return 0;
    }
}


/**
 * Standard CellInterface:
 * Get the boundary feature of the given dimension specified by the given
 * cell feature Id.
 * The Id can range from 0 to GetNumberOfBoundaryFeatures(dimension)-1.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellPointer
TetrahedronCell< TCellTraits >
::GetBoundaryFeature(int dimension, CellFeatureIdentifier featureId)
{
  switch (dimension)
    {
    case 0: return CellPointer(GetVertex(featureId));
    case 1: return CellPointer(GetEdge(featureId));
    case 2: return CellPointer(GetFace(featureId));
    default: return CellPointer(NULL);
    }
}


/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the given
 * iterator can be incremented and safely de-referenced enough times to 
 * get all the point ids needed by the cell.
 */
template <typename TCellTraits>
void
TetrahedronCell< TCellTraits >
::SetPointIds(PointIdConstIterator first)
{
  PointIdConstIterator ii(first);
  for(int i=0; i < Self::NumberOfPoints ; ++i)
    {
    m_PointIds[i] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the range
 * of iterators [first, last) contains the correct number of points needed to
 * define the cell.  The position *last is NOT referenced, so it can safely
 * be one beyond the end of an array or other container.
 */
template <typename TCellTraits>
void
TetrahedronCell< TCellTraits >
::SetPointIds(PointIdConstIterator first, PointIdConstIterator last)
{
  int localId=0;
  PointIdConstIterator ii(first);
  
  while(ii != last)
    {
    m_PointIds[localId++] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set an individual point identifier in the cell.
 */
template <typename TCellTraits>
void
TetrahedronCell< TCellTraits >
::SetPointId(int localId, PointIdentifier ptId)
{
  m_PointIds[localId] = ptId;
}


/**
 * Standard CellInterface:
 * Get a begin iterator to the list of point identifiers used by the cell.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::PointIdIterator
TetrahedronCell< TCellTraits >
::PointIdsBegin(void)
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get a const begin iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::PointIdConstIterator
TetrahedronCell< TCellTraits >
::PointIdsBegin(void) const
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get an end iterator to the list of point identifiers used by the cell.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::PointIdIterator
TetrahedronCell< TCellTraits >
::PointIdsEnd(void)
{
  return &m_PointIds[Self::NumberOfPoints];
}


/**
 * Standard CellInterface:
 * Get a const end iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::PointIdConstIterator
TetrahedronCell< TCellTraits >
::PointIdsEnd(void) const
{
  return &m_PointIds[Self::NumberOfPoints];
}


/**
 * Tetrahedron-specific:
 * Get the number of vertices defining the tetrahedron.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellFeatureCount
TetrahedronCell< TCellTraits >
::GetNumberOfVertices(void) const
{
  return Self::NumberOfVertices;
}


/**
 * Tetrahedron-specific:
 * Get the number of edges defined for the tetrahedron.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellFeatureCount
TetrahedronCell< TCellTraits >
::GetNumberOfEdges(void) const
{
  return Self::NumberOfEdges;
}


/**
 * Tetrahedron-specific:
 * Get the number of faces defined for the tetrahedron.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::CellFeatureCount
TetrahedronCell< TCellTraits >
::GetNumberOfFaces(void) const
{
  return Self::NumberOfFaces;
}


/**
 * Tetrahedron-specific:
 * Get the vertex specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfVertices()-1.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::VertexPointer
TetrahedronCell< TCellTraits >
::GetVertex(CellFeatureIdentifier vertexId)
{
  VertexPointer vert(Vertex::New());
  vert->SetPointId(0, m_PointIds[vertexId]);
  
  return vert;
}


/**
 * Tetrahedron-specific:
 * Get the edge specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfEdges()-1.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::EdgePointer
TetrahedronCell< TCellTraits >
::GetEdge(CellFeatureIdentifier edgeId)
{
  EdgePointer edge(Edge::New());

  for(int i=0; i < Edge::NumberOfPoints; ++i)
    {
    edge->SetPointId(i, m_PointIds[ m_Edges[edgeId][i] ]);
    }
  
  return edge;
}


/**
 * Tetrahedron-specific:
 * Get the face specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfFaces()-1.
 */
template <typename TCellTraits>
TetrahedronCell< TCellTraits >::FacePointer
TetrahedronCell< TCellTraits >
::GetFace(CellFeatureIdentifier faceId)
{
  FacePointer face(Face::New());
  
  for(int i=0; i < Face::NumberOfPoints; ++i)
    {
    face->SetPointId(i, m_PointIds[ m_Faces[faceId][i] ]);
    }
  
  return face;
}


/**
 * The tetrahedron's topology data: Edges
 */
template <typename TCellTraits>
const int
TetrahedronCell< TCellTraits >
::m_Edges[6][2] = { {0,1}, {1,2}, {2,0}, {0,3}, {1,3}, {2,3} };

/**
 * The tetrahedron's topology data: Faces
 */
template <typename TCellTraits>
const int
TetrahedronCell< TCellTraits >
::m_Faces[4][3] = { {0,1,3}, {1,2,3}, {2,0,3}, {0,2,1} };


} // end namespace fem
} // end namespace itk

#endif
