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








