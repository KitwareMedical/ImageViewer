/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralPedicle.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liVERTEBRALPEDICLECLASS
#define liVERTEBRALPEDICLECLASS

#include "fltkCylinder3D.h"

namespace li {

/** This class implements a 3D model of a generic Vertebral Pedicle. */
class ITK_EXPORT VertebralPedicle : public fltk::Shape3D 
{
public:
  /** Standard class typedefs. */
  typedef VertebralPedicle   Self;
  typedef fltk::Shape3D  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( VertebralPedicle, fltk::Shape3D );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Destructor */
  ~VertebralPedicle();

  /** Set Vertical Position of the pedicle. */
  void    SetVerticalPosition(double val);

  /** Set Horizontal Position of the pedicle. */
  void    SetHorizontalPosition(double val);

  /** Set Frontal Position of the pedicle. */
  void    SetFrontalPosition(double val);

  /** Set Vertical Angle of the pedicle. */
  void    SetVerticalAngle(double val);

  /** Set Horizontal Angle of the pedicle. */
  void    SetHorizontalAngle(double val);

  /** Set Vertical Radius of the pedicle. */
  void    SetVerticalRadius(double val);

  /** Set Horizontal Radius of the pedicle. */
  void    SetHorizontalRadius(double val);

  /** Set the Length of the pedicle. */
  void    SetLength(double val);

  /** Set the number of sectors on the cylinder. */
  void    SetNumberOfSectors(unsigned int num);

  /** Get Vertical Position of the pedicle. */
  double  GetVerticalPosition(void) const;

  /** Get Horizontal Position of the pedicle. */
  double  GetHorizontalPosition(void) const;

  /** Get Frontal Position of the pedicle. */
  double  GetFrontalPosition(void) const;

  /** Get Vertical Angle of the pedicle. */
  double  GetVerticalAngle(void) const;

  /** Get Horizontal Angle of the pedicle. */
  double  GetHorizontalAngle(void) const;

  /** Get Vertical Radius of the pedicle. */
  double  GetVerticalRadius(void) const;

  /** Get Horizontal Radius of the pedicle. */
  double  GetHorizontalRadius(void) const;

  /** set the Length of the pedicle. */
  double  GetLength(void) const;

  /** Get the number of sectors on the cylinder. */
  unsigned int  GetNumberOfSectors(void) const;

  /** Draw the model using OpenGL commands */
  void    DrawGeometry(void) const;

  /** Write the serialized object to a stream. */
  int     Write( fltk::Shape3D::OfstreamType &os, int level ) const;

  /** Read the serialized object from a stream. */
  int     Read( fltk::Shape3D::IfstreamType &is);

protected:
  VertebralPedicle();
  VertebralPedicle(const VertebralPedicle & b);
  const VertebralPedicle & operator=(const VertebralPedicle & b);

private:
  double                      m_VerticalPosition;
  double                      m_HorizontalPosition;
  double                      m_FrontalPosition;
  double                      m_VerticalAngle;
  double                      m_HorizontalAngle;
  double                      m_VerticalRadius;
  double                      m_HorizontalRadius;
  double                      m_Length;
  fltk::Cylinder3D::Pointer   m_LeftTube;
  fltk::Cylinder3D::Pointer   m_RightTube;
  unsigned int                m_NumberOfSectors;

};



}   // end namespace li


#endif




