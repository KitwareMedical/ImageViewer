//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#ifndef liCOMMANDUPDATESPINEMODELGUICLASS
#define liCOMMANDUPDATESPINEMODELGUICLASS

#include "liSpineModelGUI.h"
#include "liCommandEvents.h"



namespace li {

/**
 *  Implementation of the Command Patter for invoking OpenGL redraw
 */
class ITK_EXPORT CommandUpdateSpineModelGUI : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandUpdateSpineModelGUI   Self;


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
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandUpdateSpineModelGUI, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  

  /**
   * Set Fluoroscopy Unit to update
   */
  void SetSpineModelGUI( SpineModelGUI * unit );



protected:
  /**
   * Constructor
   */
  CommandUpdateSpineModelGUI();

private:

  /**
   * SpineModelGUI
   */
  SpineModelGUI          *  m_SpineModelGUI;
 

  
};


} // end namespace li


#endif
