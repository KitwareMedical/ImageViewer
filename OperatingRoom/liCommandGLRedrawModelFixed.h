/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandGLRedrawModelFixed.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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

#include "fltkShape3D.h"
#include "itkCommand.h"
#include "liCommandEvents.h"


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
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);


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
  void SetModelShape( fltk::Shape3D * window );



protected:
  /**
   * Constructor
   */
  CommandGLRedrawModelFixed();


  /**
   * Open GL Model to redraw
   */
  fltk::Shape3D::Pointer  m_ModelShape;
 

};


} // end namespace li


#endif
