/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandGLRedrawModelSensitive.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liCOMMANDGLREDRAWMODELSENSITIVECLASS
#define liCOMMANDGLREDRAWMODELSENSITIVECLASS

#include "fltkShape3D.h"
#include "itkCommand.h"
#include "liCommandEvents.h"

namespace li {

/** Implementation of the Command Pattern for invoking OpenGL redraw
 *  This command ask 3D models to draw themselves using OpenGL commands. */
class ITK_EXPORT CommandGLRedrawModelSensitive : public itk::Command 
{
public:
  /** Standard class typedefs. */
  typedef CommandGLRedrawModelSensitive   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Execute method will call redraw in the GL Window. */
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);

  /*** Run-time type information (and related methods). */
  itkTypeMacro( CommandGLRedrawModelSensitive, ::itk::Command );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Set GL Model to redraw. */
  void SetModelShape( fltk::Shape3D * window );

protected:
  /** Constructor */
  CommandGLRedrawModelSensitive();

  /** Open GL Model to redraw */
  fltk::Shape3D::Pointer  m_ModelShape;

};

} // end namespace li


#endif
