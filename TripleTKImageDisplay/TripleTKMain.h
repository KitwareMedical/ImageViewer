/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTKMain.h
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
#ifndef _TRIPLETKMAIN_H
#define _TRIPLETKMAIN_H

#include "TripleTKGUI.h"
#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>
#include <itkImage.h>
#include <itkFileIOMetaImage.h>
#include <itkFileIOToImageFilter.h>

/** \class TripleTKMain
 * \brief Main for TTKID.
 *
 *   This class is used in conjuction with the TripleTKGUI structure.
 * It holds functions used in the main function for TripleTKGUI and
 * holds the actual main() in TripleTKMain.cxx.  The main function 
 * instantiates this class as an image viewer type and from this
 * type, images are loaded through the functions in this class, and the 
 * function for creating the gui window is called.
 */
class TripleTKMain
  {

  public:
  
    typedef itk::Image<short, 3> ImageType;

    /**Constructor.*/
    TripleTKMain();

    /**Deconstructor.*/
    ~TripleTKMain();

    /**Fully coordinates the 2D system after loading an image.*/
    void synchronize2D(void);
  
    /**Fully coordinates the 3D system after loading an image.*/
    void synchronize3D(void);
    
    /**Sets the image the be viewed in the 2D aspect of the program.*/
    void SetImage(ImageType::Pointer img);
    
    /**Refreshes the display of the GUI.*/
    void show(void);
    
    /**Refreshes the display and structure of the GUI.*/
    void update(void);
    
    /**Redraw the windows -- used at the end of main().*/
    void redraw(void);
    
    /**Initialize any necessary callbacks.*/
    void setCallBacks(void);
    
    /**Return the protected variable window.*/
    Fl_Window * getWindow();

  protected:

    Fl_Window *window;
  
  };


#endif
