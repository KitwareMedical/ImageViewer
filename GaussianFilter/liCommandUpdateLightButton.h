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


#ifndef liCOMMANDUPDATELIGHTBUTTONCLASS
#define liCOMMANDUPDATELIGHTBUTTONCLASS

#include <itkCommand.h>
#include <liFilterConsoleBase.h>
#include <FL/Fl_Light_Button.H>


namespace li {

/**
 *  Implementation of the Command Pattern 
 *  for refreshing a FLTK light button
 */
class CommandUpdateLightButton : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandUpdateLightButton   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the GL Window
   */
  void Execute(itk::LightObject *caller, unsigned long event);
  void Execute(const itk::LightObject *caller, unsigned long event);

  // all the currently defined events
  // developers can use -- Command::UserEvent + int to
  // specify their own events. 
  enum GLEventIds {
    RedrawEvent = itk::Command::UserEvent+1,
  };


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandUpdateLightButton, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  static Pointer New(void);

  

  /**
   * Set Button to update
   */
  void SetButton( Fl_Light_Button * button );



protected:
  /**
   * Constructor
   */
  CommandUpdateLightButton();

private:

  /**
   * Emulator of the fluoroscopy unit imaging system
   */
  Fl_Light_Button     *  m_Button;
 

  
};


} // end namespace li


#endif
