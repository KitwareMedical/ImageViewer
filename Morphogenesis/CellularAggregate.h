
#ifndef __Cellular_Aggregate_H
#define __Cellular_Aggregate_H


#include <iostream>
#include "itkDefaultDynamicMeshTraits.h"
#include "itkMesh.h"
#include "Cell.h"
#include "itkPolygonCell.h"



namespace bio {


  
/**
 * \class CellularAggregate
 * \brief This class represent an aggregation of bio::Cell objects
 * This class is the base for different types of cellular groups
 * including bacterial colonies and pluricellular organisms 
 */
class CellularAggregate : public itk::LightObject
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef CellularAggregate      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::LightObject  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(CellularAggregate, itk::LightObject);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  


  /** 
   * Type to be used for data associated 
   * with each point in the mesh 
   */
  typedef    Cell *    PointPixelType;
  typedef    double    CellPixelType;


  /** 
   *      Mesh Traits
   */
  typedef itk::DefaultDynamicMeshTraits<  
              PointPixelType,           // PixelType
              Cell::PointDimension,     // Points Dimension
              Cell::PointDimension,     // Max.Topologycal Dimension
              double,                   // Type for coordinates
              double,                   // Type for interpolation 
              CellPixelType             // Type for values in the cells  
              >  MeshTraits;
  
  
  /** 
   *      Mesh Traits
   */
  typedef itk::Mesh<  MeshTraits::PixelType,
                      MeshTraits::PointDimension,
                      MeshTraits  >               MeshType;

  
  /** 
   *      Mesh Associated types
   */
  typedef MeshType::Pointer                       MeshPointer;
  typedef MeshType::ConstPointer                  MeshConstPointer;

  typedef MeshType::PointType                     PointType;
  typedef Cell::VectorType                        VectorType;


  typedef MeshType::PointsContainer               PointsContainer;
  typedef MeshType::PointDataContainer            PointDataContainer;
  typedef MeshType::CellsContainer                VoronoiRegionsContainer;

  typedef PointsContainer::Iterator               PointsIterator;
  typedef PointDataContainer::Iterator            CellsIterator;
  typedef VoronoiRegionsContainer::Iterator       VoronoiIterator;

  typedef PointsContainer::ConstIterator          PointsConstIterator;
  typedef PointDataContainer::ConstIterator       CellsConstIterator;
  typedef VoronoiRegionsContainer::ConstIterator  VoronoiConstIterator;

  typedef MeshType::PointIdentifier               IdentifierType;
  


  /** 
   *   Voronoi region around a bio::Cell
   */
  typedef itk::PolygonCell<  MeshType::CellPixelType, 
                             MeshType::CellTraits > VoronoiRegionType;

  typedef VoronoiRegionType::Pointer VoronoiRegionPointer;


public:

	unsigned int GetNumberOfCells(void) const;
 
  void Draw(void) const;
  void SetGrowthRadiusLimit( double value );
  void SetGrowthRadiusIncrement( double value );
  
  MeshPointer         GetCells(void);
  MeshConstPointer    GetCells(void) const;

  virtual void AdvanceTimeStep(void);

  virtual void Add( Cell * cell, const VectorType & perturbation );
  virtual void Add( Cell * cell, Cell *cell, const VectorType & perturbation );
  virtual void Add( Cell * cell );
  virtual void Remove( Cell * cell );
  virtual VoronoiRegionType::Pointer GetVoronoi( IdentifierType cellId ) const;

  void DumpContent( std::ostream & os ) const;


protected:

	CellularAggregate();
  virtual ~CellularAggregate();
  CellularAggregate( const Self & ) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void ComputeForces(void);
  virtual void UpdatePositions(void);
  virtual void ComputeClosestPoints(void);
  virtual void ClearForces(void);
  virtual void KillAll(void);
  
private:

  MeshPointer    m_Mesh;

  unsigned long  m_Iteration;
  unsigned long  m_ClosestPointComputationInterval;

};

}; // end namespace bio

#endif
