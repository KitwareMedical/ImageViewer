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


#ifndef liCOMMANDUPDATEIMAGEINTENSIFIERCLASS
#define liCOMMANDUPDATEIMAGEINTENSIFIERCLASS

#include "liFluoroscopyUnitView.h"
#include "liFluoroscopyCArmMobile.h"
#include "itkCommand.h"
#include "liCommandEvents.h"


namespace li {

/**
 *  Implementation of the Command Patter for invoking OpenGL redraw
 */
class ITK_EXPORT CommandUpdateImageIntensifier : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandUpdateImageIntensifier   Self;


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
  itkTypeMacro( CommandUpdateImageIntensifier, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  

  /**
   * Set Fluoroscopy Unit to update
   */
  void SetFluoroscopyUnitView( FluoroscopyUnitView * unit );



protected:
  /**
   * Constructor
   */
  CommandUpdateImageIntensifier();

private:

  /**
   * Emulator of the fluoroscopy unit imaging system
   */
  FluoroscopyUnitView          *  m_FluoroscopyUnitView;
 

  
};


} // end namespace li


#endif
