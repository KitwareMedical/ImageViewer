/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFluoroscopyCArmMobile.h
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
#ifndef liFLUOROSCOPYCARMMOBILECLASS
#define liFLUOROSCOPYCARMMOBILECLASS

#include "liFluoroscopyCArm.h"

namespace li {

/** Flurosocopy C Arm Mobile modelize a fluoroscopy machine
 *  mounted in wheels. This is one of the common equipments
 *  used on the operating room. */
class ITK_EXPORT FluoroscopyCArmMobile : public fltk::Shape3D
{
public:
  /** Standard class typedefs. */
  typedef FluoroscopyCArmMobile   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( FluoroscopyCArmMobile, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
  
  /** Type for points in 2D. */
  typedef itk::Point< double, 2 >    Point2DType;

  /** Draw the model using OpenGL commands. */
  void DrawGeometry(void) const;

  /** Set Visibility of Field of View Cone representation. */
  void SetFovVisible( bool );

  /** Set the position of the machine on the floor. */
  void SetPosition( const  Point2DType & point );

  /** Set the axial position of the machine on the floor. */
  void SetCranioCaudalPosition( double val );

  /** Set the axial position of the machine on the floor. */
  void SetLateralPosition( double val );

  /** Set the orientation of the machine on the floor. */
  void SetAzimuthAngle( double angle );

  /** Set the tilt of the axis. */
  void SetAxisTilt( double angle );

  /** Set Cranio/Caudal angle. */
  void SetCranioCaudalAngle( double angle );

  /** Set Cranio/Caudal angle. */
  void SetRaoLaoAngle( double angle );

  /** Set Source to Film Distance. */
  void SetSourceToFilmDistance( double distance );

  /** Set Detector Radius. */
  void SetDetectorRadius( double radius );

  /** Set Detector Position (Up/Down). */
  void SetDetectorPosition( FluoroscopyCArm::DetectorConfiguration pos );

  /** Get Source to Film Distance. */
  double GetSourceToFilmDistance( void ) const;

  /** Get the position of the machine on the floor. */
  const Point2DType & GetPosition( void ) const;

  /** Get the axial position of the machine on the floor. */
  double GetCranioCaudalPosition( void ) const;

  /** Get the axial position of the machine on the floor. */
  double GetLateralPosition( void ) const;

  /** Get the orientation of the machine on the floor. */
  double GetAzimuthAngle( void ) const;

  /** Get the tilt of the axis. */
  double GetAxisTilt( void ) const;

  /** Get Cranio/Caudal angle. */
  double GetCranioCaudalAngle( void ) const;

  /** Get Cranio/Caudal angle. */
  double GetRaoLaoAngle( void ) const;
  
  /** Get the position of the X-rays source. */
  const PointType & GetSourcePosition( void ) const;

  /** Get the position of the X-rays detector. * This is the central point of
   * the image intensifier.  */
  const PointType & GetDetectorPosition( void ) const;

  /** Get the radius of the X-rays detector. * This is the used to compute
   the field of view.  */
  double GetDetectorRadius( void ) const;

  /** Get the vertical direction of the X-rays detector. * This is the
   * central point of the image intensifier.  */
  const VectorType & GetDetectorVerticalDirection( void ) const;

  /** Refresh the observers after a change in * position or in geometry */
  void RefreshObservers( void ) const;

  /** Destructor */
  ~FluoroscopyCArmMobile();

protected:
  /** Constructor */
  FluoroscopyCArmMobile();

private:
  /** Model for the C_Arm supporting the Source and the Intensifier */
  li::FluoroscopyCArm::Pointer     m_CArm;

  /** Model for the horizontal base of Fluoroscopy CArm */
  fltk::Box3D::Pointer       m_Base;
  
  /** Model for the vertical base of Fluoroscopy CArm. */
  fltk::Box3D::Pointer       m_Pile;
  
  /** Position of the machine on the floor. */
  Point2DType                 m_Position;

  /** Orientation of the machine on the floor. */
  double                   m_AzimuthAngle;

  /** Tilt of the axis supporting the C-Arm. */
  double                   m_AxisTilt;
  
};

} // end namespace li


#endif   
