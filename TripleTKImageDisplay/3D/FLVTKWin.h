/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FLVTKWin.h
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

#ifndef _FLVTKWin_h
#define _FLVTKWin_h

#include <vtkRenderWindow.h> 
#include <FL/Fl_Box.H>

/** \class FLVTKWin
 * \brief Allow draw of a vtk window into an Fl_Box widget.
 *  
 * Limitations:
 *  1) There is one windows-specific call.
 *  2) All ancestors of the widget are assumed to be FLTK widgets.
 *  3) You can't draw into the window using FLTK calls. Get the pointer to
 *     the vtk window and use as you would vtk.
 */
class FLVTKWin:public Fl_Box
  {      

  protected:

    vtkRenderWindow* mRenWin;
    bool mFirstdraw;
    bool mInitializePosition();    //determine offsetx/y and rootwindow

  public:

    FLVTKWin(int x, int y, int w, int h, const char* l = 0);
    ~FLVTKWin();

    vtkRenderWindow * win();

    //if the Fl_Box widget is resizable, make the vtk window
    //resize along with it
    void size(int w, int h);
    void resize(int x, int y, int w, int h);

    //draw the vtk window, not the box
    virtual void draw();

  };

#endif
