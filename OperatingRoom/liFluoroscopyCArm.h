/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFluoroscopyCArm.h
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

#ifndef liFLUOROSCOPYCARMCLASS
#define liFLUOROSCOPYCARMCLASS

#include "fltkBox3D.h"
#include "fltkCone3D.h"
#include "fltkCylinder3D.h"
#include "liC_Arm3D.h"


namespace li {


/**
 *  Flurosocopy C Arm modelize the fluoroscopy machine.
 *  Having a X-Ray source and an Image Intensifier both
 *  mounted in a geometric C_Arm3D.
 */

class ITK_EXPORT FluoroscopyCArm : public fltk::Shape3D
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef FluoroscopyCArm   Self;

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
  itkTypeMacro( FluoroscopyCArm, fltk::Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Enum with the possible configurations of the detector
   */
  typedef enum {
    DetectorUp,
    DetectorDown
  } DetectorConfiguration;

  /**
   * Draw the model using OpenGL commands
   */
        void DrawGeometry(void) const;


  /**
   * Destructor
   */
        ~FluoroscopyCArm();


  /**
   * Set Cranio/Caudal angle
   */
  void SetCranioCaudalAngle( double angle );


  /**
   * Set Cranio/Caudal angle
   */
  void SetRaoLaoAngle( double angle );


  /**
   * Set Visibility of Field of View Cone representation
   */
  void SetFovVisible( bool );


  /**
   * Set Source to Film Distance
   */
  void SetSourceToFilmDistance( double distance );


  /**
   * Set Detector Radius
   */
  void SetDetectorRadius( double radius );


  /**
   * Set Detector Position (Up/Down)
   */
  void SetDetectorPosition( DetectorConfiguration pos )
                          { m_DetectorConfiguration = pos; }

  /**
   * Get Source to Film Distance
   */
  double GetSourceToFilmDistance( void ) const;


  /**
   * Get Cranio/Caudal angle
   */
  double GetCranioCaudalAngle( void ) const;


  /**
   * Get Cranio/Caudal angle
   */
  double GetRaoLaoAngle( void ) const;


  /**
   * Get the position of the X-rays source
   */
  const PointType & GetSourcePosition( void ) const;


  /**
   * Get the position of the X-rays detector.
   * This is the central point of the image
   * intensifier.
   */
  const PointType & GetDetectorPosition( void ) const;


  /**
   * Get the direction of the vertical from 
   * the detector's point of view
   */
  const VectorType & GetDetectorVerticalDirection( void ) const;

  /**
   * Get the radius of the X-rays detector.
   * This is used to compute the field of view.
   */
  double GetDetectorRadius( void ) const;






protected:
  /**
   * Constructor
   */
        FluoroscopyCArm();

private:

  /**
   * Draw the Source
   */
        void DrawSource(void) const;


  /**
   * Draw the Detector
   */
        void DrawDetector(void) const;



  /**
   * Model for the C_Arm supporting the Source and the Intensifier
   */
  li::C_Arm3D::Pointer     m_CArm;


  /**
   * Model for the Image Intensifier 
   */
  fltk::Cylinder3D::Pointer  m_Intensifier;


  /**
   * Model for the Image Detector Support
   */
  fltk::Cylinder3D::Pointer  m_DetectorSupport;
  

  /**
   * Model for the Image Detector
   */
  fltk::Cylinder3D::Pointer  m_Detector;
  
  
  /**
   * Model for the Support of the Intensifier 
   */
  fltk::Box3D::Pointer       m_IntensifierSupport;
  
  
  /**
   * Model for the X-rays source
   */
  fltk::Box3D::Pointer       m_Source;
  

  /**
   * Model for the support of the X-rays source
   */
  fltk::Box3D::Pointer       m_SourceSupport;
  

  /**
   * Model for representing the Field of View 
   */
  fltk::Cone3D::Pointer      m_FovCone;

  
  /**
   * Model for the turning base of the C-Arm
   */
  fltk::Box3D::Pointer       m_TurningBase;
 
  
  /**
   * Axial offset of the image intensifier.
   * This value changes de source-film distance
   * and as a consequence alters the field of view
   */
        double        m_IntensifierOffset;

  
  /**
   * Vector to be used as Up direction in the camera model
   */
        mutable VectorType m_DetectorVerticalDirection;

  
  /**
   * Angle to move the system in the Left/Right direction
   */
        double m_RaoLaoAngle;

  
  /**
   * Angle to move the system in the Cranio/Caudal direction
   */
        double m_CranioCaudalAngle;


  /**
   * Define whether the Field of View cone is visible or not
   */
        bool m_FovIsVisible;

  /**
   * Position of the Detector in the CArm
   */
   DetectorConfiguration    m_DetectorConfiguration;



};


} // end namespace li


#endif   
