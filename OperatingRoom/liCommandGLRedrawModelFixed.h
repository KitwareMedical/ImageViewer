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


#ifndef liCOMMANDGLREDRAWMODELFIXEDCLASS
#define liCOMMANDGLREDRAWMODELFIXEDCLASS

#include <itkCommand.h>
#include <liCommandEvents.h>
#include <liShape3D.h>


namespace li {

/**
 *  Implementation of the Command Pattern for invoking OpenGL redraw
 *  This command ask 3D models to draw themselves using OpenGL commands
 */
class ITK_EXPORT CommandGLRedrawModelFixed : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandGLRedrawModelFixed   Self;


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
  itkTypeMacro( CommandGLRedrawModelFixed, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  

  /**
   * Set GL Model to redraw
   */
  void SetModelShape( Shape3D * window );



protected:
  /**
   * Constructor
   */
  CommandGLRedrawModelFixed();


  /**
   * Open GL Model to redraw
   */
  Shape3D::Pointer  m_ModelShape;
 

};


} // end namespace li


#endif
