/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkRegularStepGradientDescent.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#ifndef fltkREGULARSTEPGRADIENTDESCENT
#define fltkREGULARSTEPGRADIENTDESCENT

#include <fltkRegularStepGradientDescentGUI.h>


namespace fltk {


/**
 *  This class implements a GUI to an itkRegularStepGradientDescent 
 *  optimization method.
 *
 *  It derives from fltkRegularStepGradientDescentGUI class which is
 *  just the graphic front end.
 *
 *  This class communicates with fltkRegularStepGradientDescentGUI 
 *  by using Command classes
 *
 */
template<class TOptimizer>
class RegularStepGradientDescent : public RegularStepGradientDescentGUI
{

public:

  /**
   *  Type of the optimizer for which this class is a GUI
   */
  typedef     TOptimizer      OptimizerType;


  /**
   *  Constructor
   */
  RegularStepGradientDescent()
  {
    m_Optimizer = OptimizerType::New();
  }
  

  /**
   *  Destructor
   */
  virtual ~RegularStepGradientDescent() {};


  /**
   *  Connect an particular optimizer
   */
  void SetOptimizer( OptimizerType * optimizer )
  { m_Optimizer = optimizer; }

  
  /**
   *  Return the optimizer
   */
  typename TOptimizer::Pointer GetOptimizer( void )
  { return m_Optimizer.GetPointer(); }
      

  /**
   *  Delegate the Start Optimization method to the optimizer
   */ 
  void StartOptimization( void ) 
  { m_Optimizer->StartOptimization(); }


  /**
   *  Delegate the Stop Optimization method to the optimizer
   */ 
  void StopOptimization( void ) 
  { m_Optimizer->StopOptimization(); }


  /**
   *  Delegate the Resume Optimization method to the optimizer
   */ 
  void ResumeOptimization( void ) 
  { m_Optimizer->ResumeOptimization(); }


  /**
   *  Delegate the SetMinimumStepLength method to the optimizer
   */ 
  void SetMinimumStepLength( double val ) 
  { m_Optimizer->SetMinimumStepLength( val ); }


  /**
   *  Delegate the SetMaximumStepLength method to the optimizer
   */ 
  void SetMaximumStepLength( double val ) 
  { m_Optimizer->SetMaximumStepLength( val ); }


  /**
   *  Delegate the SetGradientMagnitudeTolerance method to the optimizer
   */ 
  void SetGradientMagnitudeTolerance( double val ) 
  { m_Optimizer->SetGradientMagnitudeTolerance( val ); }


  /**
   *  Delegate the SetMaximumNumberOfIterations method to the optimizer
   */ 
  void SetMaximumNumberOfIterations( unsigned long val ) 
  { m_Optimizer->SetMaximumNumberOfIterations( val ); }



private:

  typename OptimizerType::Pointer   m_Optimizer;
    
};

} // end namespace fltk


#endif



