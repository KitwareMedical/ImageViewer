/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ApplicationBase.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ApplicationBase_txx
#define _ApplicationBase_txx

#include "ApplicationBase.h"

namespace itk
{

template < typename TParser, typename TPreprocessor,
  typename TRegistrator, typename TLabeler, typename TGenerator >
ApplicationBase< TParser,TPreprocessor,
  TRegistrator, TLabeler, TGenerator>
::ApplicationBase()
{
  m_Parser       = ParserType::New();
  m_Preprocessor = PreprocessorType::New();
  m_Registrator  = RegistratorType::New();
  m_Labeler      = LabelerType::New();
  m_Generator    = GeneratorType::New();
}


template < typename TParser, typename TPreprocessor,
  typename TRegistrator, typename TLabeler, typename TGenerator >
void
ApplicationBase< TParser,TPreprocessor,
  TRegistrator, TLabeler, TGenerator>
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
   * Preprocess the images before registration
   **************************/

  std::cout << "Preprocess the images ... " << std::endl;

  try
    {
    this->InitializePreprocessor();
    m_Preprocessor->Execute();
    }
  catch(...)
    {
    std::cout << "Error occured during preprocessing." << std::endl;
    throw;
    }


  /**************************
   * Registered the processed images
   **************************/
  std::cout << "Register the images ... " << std::endl;

  try
    {
    this->InitializeRegistrator();
    m_Registrator->Execute();
    }
  catch(...)
    {
    std::cout << "Error occured during registration" << std::endl;
    throw;
    }


  /**************************
   * Label image using atlas
   **************************/
  std::cout << "Label image using atlas ... " << std::endl;

  try
    {
    this->InitializeLabeler();
    m_Labeler->Execute();
    }
  catch(...)
    {
    std::cout << "Error occured during registration" << std::endl;
    throw;
    }


  /**************************
   * Generating output
   **************************/
  std::cout << "Generating output ... " << std::endl;
  
  try
    {
    this->InitializeGenerator();
    m_Generator->Execute();
    }
  catch(...)
   {
   std::cout << "Error occurred during output generation." << std::endl;
   throw;
   }

}


} // namespace itk

#endif

