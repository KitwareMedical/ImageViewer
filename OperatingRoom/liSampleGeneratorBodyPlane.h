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

#ifndef liSAMPLEGENERATORBODYPLANE
#define liSAMPLEGENERATORBODYPLANE

#include "liSampleGeneratorBodyPlaneGUI.h"


class liSampleGeneratorBodyPlane : public liSampleGeneratorBodyPlaneGUI {

public:
  liSampleGeneratorBodyPlane();
  virtual ~liSampleGeneratorBodyPlane();
  virtual void Hide( void );
  virtual void ShowParameters( void );
  virtual void GenerateSamples( SamplesType & samples );
};



#endif
