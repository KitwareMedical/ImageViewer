
#ifndef __BacterialColony_H
#define __BacterialColony_H


#include <iostream>
#include "CellularAggregate.h"
#include "Bacteria.h"
#include "BacterialColonyGUI.h"



namespace bio {

/**
 * \class BacterialColony
 * \brief This class represent an aggregation of bio::Bacteria.
 * The interactions between the bacteria are managed at this 
 * level. Global coordination is also performed for growing and
 * spreading
 */
class BacterialColony : public CellularAggregate
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef BacterialColony      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef CellularAggregate Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(BacterialColony, CellularAggregate );

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  


  typedef Cell::VectorType        VectorType;
  typedef Cell::PointType         PointType;


  /**
   * Show the GUI
   */
  void Show(void);

  /**
   * Hide the GUI
   */
  void Hide(void);


protected:

	BacterialColony();
  virtual ~BacterialColony();
  BacterialColony( const Self & ) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  
private:

  BacterialColonyGUI               m_GUI;


};

} // end namespace bio

#endif
