/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BacterialColony.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __BacterialColony_H
#define __BacterialColony_H


#include <iostream>
#include "CellularAggregate.h"
#include "Bacteria.h"
#include "BacterialColonyGUI.h"
#include "itkCommand.h"



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
   *  Command to receive redraw events 
   */ 
  typedef itk::SimpleConstMemberCommand< BacterialColony > RedrawCommandType;
  
  RedrawCommandType::Pointer GetRedrawCommand(void)
                                  { return m_RedrawCommand.GetPointer(); }

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

  RedrawCommandType::Pointer       m_RedrawCommand;

};

} // end namespace bio

#endif
