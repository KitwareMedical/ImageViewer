/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkImage2DViewerWindow.h
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

#ifndef fltk_Image2D_Viewer_h
#define fltk_Image2D_Viewer_h

#include <fltkGlWindow.h>
#include <itkRGBPixel.h>


namespace fltk {


class Image2DViewerWindow : public GlWindow 
{

public:

  typedef     unsigned char    ValueType;

  typedef itk::RGBPixel<float>  ColorType;

  Image2DViewerWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~Image2DViewerWindow();

  virtual void draw(void);  // overload fltk methods
  virtual int  handle(int);
  virtual void  handlePopUpMenu( void );
  
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  void Allocate( unsigned int nx, unsigned int ny );
  void Update( void );
  
  const ColorType & GetBackground(void) const;

  ValueType * GetBuffer(void) 
  { return m_Buffer; }

  unsigned int GetNumberOfBytesPerPixel(void) const
  { return m_NumberOfBytesPerPixel; }

  unsigned int GetWidth(void) const;
  unsigned int GetHeight(void) const;
  void SetWidth(unsigned int);
  void SetHeight(unsigned int);
  void FitWindowToImage(void);
  void FitImageToWindow(void);
  void IntensityWindowing(void);
  void SetIntensityWindow( Fl_Window * window );
  void SetParentWindow( Fl_Window * window );
  void PanningEventHandling(int & p1x, int & p1y);
  void ZoomingEventHandling(int & p1x, int & p1y);

private:

  ColorType          m_Background;
  ValueType       *  m_Buffer;
  unsigned int       m_Width;
  unsigned int       m_Height;
  double             m_Zoom;
  int                m_ShiftX;
  int                m_ShiftY;
  Fl_Window       *  m_IntensityWindow;
  Fl_Window       *  m_ParentWindow;
  unsigned int       m_NumberOfBytesPerPixel;

};


} // end namespace fltk

#endif   
