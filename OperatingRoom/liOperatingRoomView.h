//--------------------------------------------
//
//     Project: Angio Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#ifndef liOPERATINGROOMVIEWCLASS
#define liOPERATINGROOMVIEWCLASS

#include <itkWin32Header.h>
#include <itkLightObject.h>
#include <fltkGlWindowInteractive.h>



namespace li 
{

/**
 * This class emulates a video camera observing
 * the operating room. It allows the user to 
 * interactively change positions, zoom and 
 * angles of view.
 */
     
class OperatingRoomView : public fltk::GlWindowInteractive
{

public:

	OperatingRoomView(int x,int y,int w,int h, const char * label=0);
	
  void draw(void);

  typedef itk::LightObject lightObject;

  lightObject::Pointer  GetNotifier(void);

private:
  lightObject::Pointer  m_Notifier;

};


} // end namespace li


#endif   



