//--------------------------------------------
//
//     Project: Gaussian Filter
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#ifndef liLIGHTBUTTONCLASS
#define liLIGHTBUTTONCLASS

#include <liCommandUpdateLightButton.h>
#include <FL/Fl_Light_Button.H>


namespace li {

/**
 *  GUI object that derives from a FLTK Light Button
 *  and contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class LightButton : public Fl_Light_Button     
{
public:
  /**
   * Constructor
   */
  LightButton(int x, int y, int w, int h, char * label):
    Fl_Light_Button( x, y, w, h, label ) {
      m_Command = li::CommandUpdateLightButton::New();
      m_Command->SetButton( this );
    }


  
  /**
   * Get Command
   */
  CommandUpdateLightButton::Pointer GetCommand( void ) const
  {
    return m_Command;
  }

private:

  CommandUpdateLightButton::Pointer m_Command;
  
};


} // end namespace li


#endif
