/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MSClassifierApplicationBase.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MSClassifierApplicationBase_txx
#define _MSClassifierApplicationBase_txx

#include "MSClassifierApplicationBase.h"

namespace itk
{

template < typename TParser,
           typename TMSClassifier,
           typename TOutputGenerator >
MSClassifierApplicationBase< TParser, TMSClassifier, TOutputGenerator >
::MSClassifierApplicationBase()
{
  m_Parser          = ParserType::New();
  m_MSClassifier      = MSClassifierType::New();
  m_OutputGenerator = OutputGeneratorType::New();
}


template < typename TParser,
           typename TMSClassifier,
           typename TOutputGenerator >
void
MSClassifierApplicationBase< TParser, TMSClassifier, TOutputGenerator >
::Execute()
{

  /**************************
   * Parse input
   **************************/
  std::cout << "Parsing input ... " << std::endl;
  
  try
    {
    this->InitializeParser();
    m_Parser->Execute();
    }
  catch(...)
   {
   std::cout << "Error occurred during input parsing." << std::endl;
   throw;
   }

  /**************************
   * Classify the images the images before registration
   **************************/

  std::cout << "Classify the images ... " << std::endl;

  try
    {
   this->InitializeMSClassifier();
   m_MSClassifier->Execute();
    }
  catch(...)
    {
    std::cout << "Error occured during preprocessing." << std::endl;
    throw;
    }

  /**************************
   * Generating output
   **************************/

  std::cout << "Generating output ... " << std::endl;
  
  try
    {
    this->InitializeOutputGenerator();
    m_OutputGenerator->Execute();
    }
  catch(...)
   {
   std::cout << "Error occurred during output generation." << std::endl;
   throw;
   }

}


} // namespace itk

#endif

