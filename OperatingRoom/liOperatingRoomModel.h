/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liOperatingRoomModel.h
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

#ifndef liOPERATINGROOMMODELCLASS
#define liOPERATINGROOMMODELCLASS


#include "liLight.h"
#include "liPatient.h"
#include "liSurgeryTable.h"
#include "liFluoroscopyCArmMobile.h"




namespace li {


/**
 *  OperatingRoomModel is the full 3D model of the Operating
 *  Room. It integrates some basic elements that can be added
 *  or removed in a modular way. Among the elements there are:
 *  a Table, a Fluorosocopy unit, Ligths, a Patient model.
 */

class ITK_EXPORT OperatingRoomModel : public fltk::Shape3D
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef OperatingRoomModel   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltk::Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( OperatingRoomModel, fltk::Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Draw the model using OpenGL commands
   */
        void DrawGeometry(void) const;


  /**
   * Returns the flurosocopy unit
   */
  li::FluoroscopyCArmMobile::Pointer  GetFluoroscopyUnit(void) const;


  /**
   * Returns the Patient
   */
  li::Patient::Pointer                GetPatient(void) const;


  /**
   * Returns the Table
   */
  li::SurgeryTable::Pointer           GetSurgeryTable(void) const;



  /**
   * Destructor
   */
        ~OperatingRoomModel();


protected:
  /**
   * Constructor
   */
        OperatingRoomModel();

private:

  /**
   * Model for the C_Arm supporting the Source and the Intensifier
   */
  li::FluoroscopyCArmMobile::Pointer     m_FluoroscopyUnit; 


  /**
   * Model for the operating Table
   */
  li::SurgeryTable::Pointer              m_Table;
 
  
  /**
   * Model of patient's anatomy
   */  
  li::Patient::Pointer                   m_Patient;


  /**
   * Model of the floor
   */
  fltk::Box3D::Pointer                   m_Floor;
    
  /**
   * Light 0
   */
  li::Light::Pointer                     m_Light0;
    
  /**
   * Light 1
   */
  li::Light::Pointer                     m_Light1;
    
   
};


} // end namespace li


#endif   
