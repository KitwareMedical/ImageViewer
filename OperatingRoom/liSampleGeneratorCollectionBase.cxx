/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorCollectionBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//    Project Registrator
//
//    Author:  Luis Ibanez
//  
//    Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------


#include <liSampleGeneratorCollectionBase.h>




//----------------------------------------------
//
//    Constructor
//
//----------------------------------------------
liSampleGeneratorCollectionBase::liSampleGeneratorCollectionBase() 
{
}


//----------------------------------------------
//
//    Destructor
//
//----------------------------------------------
liSampleGeneratorCollectionBase::~liSampleGeneratorCollectionBase() 
{
  CollectionType::const_iterator it = m_Collection.begin();
  while( it != m_Collection.end() )
  {
    delete *it;
    ++it;
  }
  
}



//----------------------------------------------
//
//    Generate Samples
//
//----------------------------------------------
void 
liSampleGeneratorCollectionBase
::GenerateSamples( SetsOfSamplesType & samples ) 
{
  samples.clear();
  samples.resize( m_Collection.size() );

  SetsOfSamplesType::iterator is = samples.begin();

  CollectionType::const_iterator it = m_Collection.begin();
  while( it != m_Collection.end() )
  {
    (*it)->GenerateSamples( *is );
    ++it;
    ++is;
  }
  
}








