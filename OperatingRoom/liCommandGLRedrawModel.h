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


#ifndef liCOMMANDGLREDRAWMODELCLASS
#define liCOMMANDGLREDRAWMODELCLASS

#include "itkCommand.h"
#include "liCommandEvents.h"
#include "fltkGlWindow.h"
#include "fltkGlDrawer.h"


namespace li {

/**
 *  Implementation of the Command Pattern for invoking OpenGL redraw
 *  This command ask 3D models to draw themselves using OpenGL commands
 */
class ITK_EXPORT CommandGLRedrawModel : public itk::Command 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef CommandGLRedrawModel   Self;


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
  void Execute(itk::Object *caller, unsigned long event);
  void Execute(const itk::Object *caller, unsigned long event);


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( CommandGLRedrawModel, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );

  

  /**
   * Set GL Model to redraw
   */
  void SetModel( fltk::GlDrawer * window );


protected:
  /**
   * Constructor
   */
  CommandGLRedrawModel();


  /**
   * Open GL Model to redraw
   */
  fltk::GlDrawer  *  m_Model;
 

};


} // end namespace li


#endif
