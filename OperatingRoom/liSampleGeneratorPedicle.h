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

#ifndef liSAMPLEGENERATORPEDICLE
#define liSAMPLEGENERATORPEDICLE

#include <liSampleGeneratorPedicleGUI.h>


class liSampleGeneratorPedicle : public liSampleGeneratorPedicleGUI {

public:
  liSampleGeneratorPedicle();
  virtual ~liSampleGeneratorPedicle();
  virtual void Hide( void );
  virtual void ShowParameters( void );
  virtual void GenerateSamples( SamplesType & samples );
};



#endif
