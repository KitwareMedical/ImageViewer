/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MaximumLikelihoodRatioDecisionRule.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "MaximumLikelihoodRatioDecisionRule.h"

MaximumLikelihoodRatioDecisionRule::MaximumLikelihoodRatioDecisionRule()
{
  m_TotalSampleSize = 0 ;
}


void MaximumLikelihoodRatioDecisionRule::AddClassSampleSize(unsigned int size)
{
  m_ClassSizes.push_back(size) ;
  m_APrioriRatioMatrix.resize(m_ClassSizes.size(), m_ClassSizes.size()) ;
  m_TempScores.resize(m_ClassSizes.size()) ;
  m_TotalSampleSize += size ;
  unsigned int row, column ;
  double APrioriRatio ;
  for (row = 0 ; row < m_ClassSizes.size() ; row++)
    {
      for (column = 0 ; column < m_ClassSizes.size() ; column++)
        {
          APrioriRatio = (double)m_ClassSizes[column] / (double)m_ClassSizes[row] ;
          m_APrioriRatioMatrix.put(row, column, APrioriRatio) ;
        }
    }
}










