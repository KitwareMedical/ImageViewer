/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateSpineModelGUI.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liCOMMANDUPDATESPINEMODELGUICLASS
#define liCOMMANDUPDATESPINEMODELGUICLASS

#include "liSpineModelGUI.h"
#include "liCommandEvents.h"

namespace li {

/** Implementation of the Command Patter for invoking OpenGL redraw. */
class ITK_EXPORT CommandUpdateSpineModelGUI : public itk::Command 
{
public:
  /** Standard class typedefs. */
  typedef CommandUpdateSpineModelGUI   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Execute method will call redraw in the GL Window. */
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);

  /*** Run-time type information (and related methods). */
  itkTypeMacro( CommandUpdateSpineModelGUI, ::itk::Command );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Set Fluoroscopy Unit to update. */
  void SetSpineModelGUI( SpineModelGUI * unit );

protected:
  /** Constructor. */
  CommandUpdateSpineModelGUI();

private:
  /** SpineModelGUI. */
  SpineModelGUI          *  m_SpineModelGUI;
  
};

} // end namespace li


#endif
