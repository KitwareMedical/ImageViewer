/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CommandIterationUpdate.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkCommandIterationUpdate_h
#define __itkCommandIterationUpdate_h

#include <itkCommand.h>
#include "itkWeakPointer.h"


/**
 *  Implementation of the Command Pattern to be invoked every iteration
 */
template < class TOptimizer >
class ITK_EXPORT CommandIterationUpdate : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandIterationUpdate   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will print data at each iteration
   */
  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event);
  }

  void Execute(const itk::Object *caller, const itk::EventObject & event)
  {
    if( !m_Browser )
      {
      return;
      }
    if( typeid( event ) == typeid( itk::StartEvent ) )
      {
      m_Browser->clear();
      }    
    else if( typeid( event ) == typeid( itk::IterationEvent ) )
      {
      std::ostringstream text;
      text << m_Optimizer->GetCurrentIteration() << " = ";
      text << m_Optimizer->GetValue() << " : ";
      text << m_Optimizer->GetCurrentPosition() << std::endl;
      m_Browser->add( text.str().c_str() );
      }
    else if( typeid( event ) == typeid( itk::EndEvent ) )
      {
      std::ostringstream text;
      text << std::endl << std::endl;
      text << "After " << m_Optimizer->GetCurrentIteration();
      text << "  iterations " << std::endl;
      text << "Solution is    = " << m_Optimizer->GetCurrentPosition();
      text << std::endl;
      text << "With value     = " << m_Optimizer->GetValue();
      text << std::endl;
      text << "Stop condition = " << m_Optimizer->GetStopCondition();
      text << std::endl;
      m_Browser->add( text.str().c_str() );
      }
    m_Browser->bottomline( m_Browser->size() );
    m_Browser->redraw();
    Fl::check();
  }


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandIterationUpdate, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  
  /**
   * Type defining the optimizer
   */
  typedef    TOptimizer     OptimizerType;


  /**
   * Set Optimizer
   */
  void SetOptimizer( OptimizerType * optimizer )
    { 
    m_Optimizer = optimizer;
    m_Optimizer->AddObserver( itk::IterationEvent(), this );
    }


  /**
   * Set FLTK Browser for writing the output
   */
  void SetBrowser( Fl_Browser * browser )
    { 
    m_Browser = browser;
    m_Browser->clear();
    }



protected:

  /**
   * Constructor
   */
  CommandIterationUpdate() 
    {
    m_Browser = 0;
    }

private:

  /**
   *  WeakPointer to the Optimizer
   */
  itk::WeakPointer<OptimizerType>   m_Optimizer;
 
  Fl_Browser *                      m_Browser;

  
};




#endif

