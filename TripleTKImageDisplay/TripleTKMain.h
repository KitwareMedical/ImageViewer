/*=====================================================================
Summer 2001

  This class is used in conjuction with the TripleTKGUI structure.
  It holds functions used in the main function for TripleTKGUI and
  holds the actual main() in TripleTKMain.cxx.  The main function 
  instantiates this class as an image viewer type and from this
  type, images are loaded through the functions in this class, and the 
  function for creating the gui window is called.

  Author: Andrew MacKelfresh
  CADDLab
  UNC-CH
======================================================================*/
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


/*
*   Class TripleTKMain 
*/
class TripleTKMain
  {

  public:
  
    typedef itk::PhysicalImage<short, 3> ImageType;

    TripleTKMain();
    ~TripleTKMain();
    void synchronize2D(void);
    void synchronize3D(void);
    void SetImage(ImageType::Pointer img);
    void show(void);
    void update(void);
    void redraw(void);
    void setCallBacks(void);
    Fl_Window * getWindow();

  protected:

    Fl_Window *window;
  
  };


#endif
