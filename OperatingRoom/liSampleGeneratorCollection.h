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

#ifndef liSAMPLEGENERATORCOLLECTION
#define liSAMPLEGENERATORCOLLECTION

#include <liSampleGeneratorCollectionGUI.h>


class liSampleGeneratorCollection : public liSampleGeneratorCollectionGUI {

public:
  liSampleGeneratorCollection();
  virtual ~liSampleGeneratorCollection();
  virtual void Show( void );
  virtual void Hide( void );
  virtual void Add( liSampleGenerator * );
  virtual void ShowSampleGenerator( void );

};



#endif

