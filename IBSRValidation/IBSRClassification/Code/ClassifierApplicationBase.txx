/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierApplicationBase.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierApplicationBase_txx
#define _ClassifierApplicationBase_txx

#include "ClassifierApplicationBase.h"

namespace itk
{

template < typename TParser,
           typename TClassifier,
           typename TOutputGenerator >
ClassifierApplicationBase< TParser, TClassifier, TOutputGenerator >
::ClassifierApplicationBase()
{
  m_Parser          = ParserType::New();
  m_Classifier      = ClassifierType::New();
  m_OutputGenerator = OutputGeneratorType::New();
}


template < typename TParser,
           typename TClassifier,
           typename TOutputGenerator >
void
ClassifierApplicationBase< TParser, TClassifier, TOutputGenerator >
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
   this->InitializeClassifier();
   m_Classifier->Execute();
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

