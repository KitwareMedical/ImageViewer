/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandUpdateRegistratorFieldOfView.h
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
#ifndef liCOMMANDUPDATEREGISTRATORFIELDOFVIEW
#define liCOMMANDUPDATEREGISTRATORFIELDOFVIEW

#include "liFluoroscopyCArmMobile.h"
#include "liVesselsRegistrator.h"
#include "itkCommand.h"
#include "liCommandEvents.h"

namespace li {

/** Implementation of the Command Pattern for updating 
 *  the perspective projection parameters of the 3D/2D
 *  registrator. */
class ITK_EXPORT CommandUpdateRegistratorFieldOfView : public itk::Command 
{
public:
  /** Standard class typedefs. */
  typedef CommandUpdateRegistratorFieldOfView   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Execute method will call redraw in the GL Window. */
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);

  /** Run-time type information (and related methods). */
  itkTypeMacro( CommandUpdateRegistratorFieldOfView, ::itk::Command );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
  
  /** Type defining the Registrator class. */
  typedef li::VesselsRegistrator    RegistratorType;

  /** Set Registrator object to update. */
  void SetRegistrator( RegistratorType * unit );

protected:
  /** Constructor */
  CommandUpdateRegistratorFieldOfView();

private:
  /** Registrator class to be updated. */
  RegistratorType::Pointer     m_Registrator;

};

} // end namespace li


#endif
