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


#ifndef liCOMMANDUPDATEREGISTRATORFIELDOFVIEW
#define liCOMMANDUPDATEREGISTRATORFIELDOFVIEW

#include <liFluoroscopyCArmMobile.h>
#include <liVesselsRegistrator.h>
#include <itkCommand.h>
#include <liCommandEvents.h>



namespace li {

/**
 *  Implementation of the Command Pattern for updating 
 *  the perspective projection parameters of the 3D/2D
 *  registrator
 */
class ITK_EXPORT CommandUpdateRegistratorFieldOfView : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandUpdateRegistratorFieldOfView   Self;


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


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandUpdateRegistratorFieldOfView, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  
  /**
   * Type defining the Registrator class
   */
  typedef li::VesselsRegistrator    RegistratorType;

  /**
   * Set Registrator object to update
   */
  void SetRegistrator( RegistratorType * unit );



protected:
  /**
   * Constructor
   */
  CommandUpdateRegistratorFieldOfView();

private:

  /**
   * Registrator class to be updated
   */
  RegistratorType::Pointer     m_Registrator;
 

  
};


} // end namespace li


#endif
