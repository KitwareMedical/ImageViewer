/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorCollection.cxx
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


#include <liSampleGeneratorCollection.h>




//----------------------------------------------
//
//    Constructor
//
//----------------------------------------------
liSampleGeneratorCollection::liSampleGeneratorCollection() 
{
}


//----------------------------------------------
//
//    Destructor
//
//----------------------------------------------
liSampleGeneratorCollection::~liSampleGeneratorCollection() 
{
}




//----------------------------------------------
//
//    Show
//
//----------------------------------------------
void 
liSampleGeneratorCollection::Show( void ) 
{
  controlWindow->show();
}




//----------------------------------------------
//
//    Hide
//
//----------------------------------------------
void 
liSampleGeneratorCollection::Hide( void ) 
{
  CollectionType::const_iterator it = m_Collection.begin();
  while( it != m_Collection.end() )
  {
    (*it)->Hide();
    ++it;
  }
  controlWindow->hide();
}





//----------------------------------------------
//
//    Show Sample Generator
//
//----------------------------------------------
void 
liSampleGeneratorCollection::ShowSampleGenerator( void ) 
{
  unsigned int selected = generatorsBrowser->value();
  if( selected ) 
  {
    m_Collection[selected-1]->Show(); 
  }
}







//----------------------------------------------
//
//    Show Sample Generator
//
//----------------------------------------------
void 
liSampleGeneratorCollection::Add( liSampleGenerator * generator ) 
{
  m_Collection.push_back( generator );
  generatorsBrowser->add( generator->GetName() );
}




