/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateProgress.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef liCOMMANDUPDATEPROGRESSCLASS
#define liCOMMANDUPDATEPROGRESSCLASS

#include <itkCommand.h>
#include <liImageRegistrationConsoleBase.h>


namespace li {

/**
 *  Implementation of the Command Patter for invoking OpenGL redraw
 */
class CommandUpdateProgress : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandUpdateProgress   Self;


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
    RedrawEvent = itk::Command::UserEvent+1
  };


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandUpdateProgress, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  static Pointer New(void);

  

  /**
   * Set Console to update
   */
  void SetConsole( liImageRegistrationConsoleBase * console );



protected:
  /**
   * Constructor
   */
  CommandUpdateProgress();

private:

  /**
   * Emulator of the fluoroscopy unit imaging system
   */
  liImageRegistrationConsoleBase     *  m_Console;
 

  
};


} // end namespace li


#endif
