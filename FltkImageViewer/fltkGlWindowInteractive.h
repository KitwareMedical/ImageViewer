/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindowInteractive.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
  void SetZoom(GLdouble newZoom);
  void SetCenter(const Point3DType & center);
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  void ResetViewingParameters(void);
  
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
