
#ifndef __Cellular_Aggregate_H
#define __Cellular_Aggregate_H


#include <iostream>
#include "itkIndent.h"
#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "Bacteria.h"



namespace bio {

/**
 * \class CellularAggregate
 * \brief This class represent an aggregation of bio::Cell objects
 * This class is the base for different types of cellular groups
 * includeing bacterial colonies and pluricellular organisms 
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


  typedef Cell::VectorType        VectorType;
  typedef Cell::PointType         PointType;


public:

	unsigned int GetNumberOfCells(void) const;
 
  void Draw(void) const;
  void SetGrowthRadiusLimit( double value );
  void SetGrowthRadiusIncrement( double value );
  Cell::CellsListType * GetCells(void);
  const Cell::CellsListType * GetCells(void) const;
  void SetCells( Cell::CellsListType * );


public:
  
  virtual void AdvanceTimeStep(void);


protected:

	CellularAggregate();
  virtual ~CellularAggregate();
  CellularAggregate( const Self & ) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void ComputeForces(void);
  virtual void ClearForces(void);
  virtual void KillAll(void);
  
private:

  Cell::CellsListType    *  m_Cells;

};

}; // end namespace bio

#endif
