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

#ifndef liSAMPLEGENERATOR
#define liSAMPLEGENERATOR

#include <liSampleGeneratorGUI.h>


class liSampleGenerator : public liSampleGeneratorGUI {

public:
  liSampleGenerator();
  virtual ~liSampleGenerator();
  virtual void Show( void );
  virtual void Hide( void );
  virtual void SetName( const char * name );
  virtual const char * GetName( void ) const;
  virtual void SetPosition( const PointType & point );

};



#endif
