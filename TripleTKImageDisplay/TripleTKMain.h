/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTKMain.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
