/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorCollectionBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liSAMPLEGENERATORCOLLECTIONBASE
#define liSAMPLEGENERATORCOLLECTIONBASE

#include "liSampleGenerator.h"
#include "liSample.h"
#include <vector>

class liSampleGeneratorCollectionBase 
{
public:  
  typedef  std::vector< liSampleGenerator * > CollectionType;
  typedef  std::vector<liSample>          SamplesType;
  typedef  std::vector<SamplesType>       SetsOfSamplesType;

  liSampleGeneratorCollectionBase();
  virtual ~liSampleGeneratorCollectionBase();
  virtual void GenerateSamples( SetsOfSamplesType & samples );

protected:
  CollectionType  m_Collection; 

};

#endif



