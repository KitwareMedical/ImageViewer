/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkGlWindow.h
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

#ifndef fltkGLWINDOW
#define fltkGLWINDOW

#include <FL/gl.h> 
#include <FL/Fl_Gl_Window.H>
#include <fltkWidgetRedrawCommand.h>
#include <fltkCommandEvents.h>
#include <vector>

namespace fltk {


/**
 *  This class implements an OpenGl Window.
 *  it derives from Fl_Gl_Window and add
 *  the elements needed to communicate with
 *  ITK via Command objects
 *
 */
class GlWindow : public Fl_Gl_Window 
{


public:

  typedef WidgetRedrawCommand< Fl_Gl_Window >  RedrawCommandType;
    
public:

  GlWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindow();
  void SaveImage( void );
  void SaveImage( const char * filename );
  RedrawCommandType::Pointer GetRedrawCommand(void);
  itk::Object::Pointer GetNotifier(void);

private:

  RedrawCommandType::Pointer   m_RedrawCommand;
    
  itk::Object::Pointer    m_Notifier;

};

} // end namespace fltk


#endif



