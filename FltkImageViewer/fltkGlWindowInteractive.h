/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractive.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#ifndef fltkGLWINDOWINTERCLASS
#define fltkGLWINDOWINTERCLASS

#include <fltkGlWindow.h>
#include <itkPoint.h>
#include <itkRGBPixel.h>


namespace fltk {


class GlWindowInteractive : public GlWindow 
{

public:

  typedef itk::Point<GLdouble,3>  Point3DType;

  typedef itk::Point<int,2>       ScreenPointType;

  typedef itk::RGBPixel<GLdouble>   ColorType;


  GlWindowInteractive(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindowInteractive();

  virtual void draw(void);  // overload fltk methods
  virtual int  handle(int);
  
  virtual void SetViewPoint(void);

  void SetSensitivity(GLdouble newSensitivity);
  void SetAltitude(GLdouble newAltitude);
  void SetAzimuth(GLdouble newAzimuth);
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  
  const ColorType & GetBackground(void) const;
  GLdouble GetSensitivity(void) const;
  GLdouble GetAltitude(void) const;
  GLdouble GetAzimuth(void) const;
  GLdouble GetZoom(void) const ;
  const Point3DType & GetCenter(void) const;

private:

  ScreenPointType   m_Mouse_a;
  ScreenPointType   m_Mouse_b;
  ColorType         m_Background;


protected:

  GLdouble          m_Altitude;
  GLdouble          m_Azimuth;
  GLdouble          m_Zoom;
  Point3DType       m_Center;

  GLdouble          m_Sensitivity;

};


} // end namespace fltk

#endif   
