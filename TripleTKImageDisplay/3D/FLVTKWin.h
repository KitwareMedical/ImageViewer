/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FLVTKWin.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
