//--------------------------------------------
//
//		Project Registrator
//
//		Author:  Luis Ibanez
//	
//		Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------

#ifndef liSAMPLEGENERATORCOLLECTIONBASE
#define liSAMPLEGENERATORCOLLECTIONBASE

#include <liSampleGenerator.h>
#include <vector>
#include <liSample.h>


class liSampleGeneratorCollectionBase {
public:  

  typedef  std::vector< liSampleGenerator * > CollectionType;

  typedef  std::vector<liSample>          SamplesType;

  typedef  std::vector<SamplesType>       SetsOfSamplesType;

public:
  liSampleGeneratorCollectionBase();
  virtual ~liSampleGeneratorCollectionBase();
  virtual void GenerateSamples( SetsOfSamplesType & samples );

protected:

  CollectionType  m_Collection; 

};



#endif



