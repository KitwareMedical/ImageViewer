
#ifndef __BacterialColony_H
#define __BacterialColony_H


#include <iostream>
#include "itkIndent.h"
#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "Bacteria.h"



namespace bio {

/**
 * \class BacterialColony
 * \brief This class represent an aggregation of bio::Bacteria.
 * The interactions between the bacteria are managed at this 
 * level. Global coordination is also performed for growing and
 * spreading
 */
class BacterialColony : public itk::LightObject
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef BacterialColony      Self;

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
  itkTypeMacro(BacterialColony, itk::LightObject);

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
  Cell::CellsListType * GetCellsAggregate(void);


protected:

	BacterialColony();
  virtual ~BacterialColony();
  BacterialColony( const Self & ) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void ComputeForces(void);
  void KillAll(void);
  
private:

  Cell::CellsListType    *  m_Cells;

};

}; // end namespace bio

#endif
