/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MaximumLikelihoodRatioDecisionRule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MaximumLikelihoodRatioDecisionRule_h
#define __MaximumLikelihoodRatioDecisionRule_h

#include <vector>
#include "vnl/vnl_matrix.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

/** \class MaximumLikelihoodRatioDecisionRule
 *  \brief A Decision rule that choose the class that has maximum value
 */
 
class MaximumLikelihoodRatioDecisionRule : 
  public itk::Object
{
 public:
  /** Standard class typedefs */ 
  typedef MaximumLikelihoodRatioDecisionRule Self ;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  
  /** Run-time type information (and related methods) */
  itkTypeMacro(MaximumLikelihoodRatioDecisionRule, Object);
  
  /** Standard New() method support */
  itkNewMacro(Self) ;
  
  unsigned int Evaluate(std::vector< double > discriminantScores) ;
  void AddClassSampleSize(unsigned int size) ;

 protected:
  MaximumLikelihoodRatioDecisionRule() ;
  virtual ~MaximumLikelihoodRatioDecisionRule() {}
  
 private:
  std::vector< unsigned int > m_ClassSizes ;
  std::vector< double > m_TempScores ;
  vnl_matrix< double > m_APrioriRatioMatrix ;
  unsigned long m_TotalSampleSize ;
} ; // end of class

#endif







