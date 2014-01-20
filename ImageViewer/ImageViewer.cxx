/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageViewer.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>

#include <itkImage.h>
#include <itkImageFileReader.h>

#define USE_FLTK 1
#if USE_FLTK
# include <FL/Fl.H>
# include <FL/Fl_Window.H>
# include <FL/Fl_File_Chooser.H>
# include <GLSliceView.h>
# include "ImageViewerGUI.h"
Fl_Window *form;
#else
# include <QApplication>
# include "qtmainwindow.h"
#include "ctkVTKSliceView.h"
#endif

int usage(void)
   {
   std::cout << "ImageViewer" << std::endl;
   std::cout << std::endl;
   std::cout << "ImageViewer <Filename>" << std::endl;
   std::cout << std::endl;

   return 1;
   }

int main(int argc, char **argv)
{
  typedef itk::Image< float, 3 > ImageType;

#if !USE_FLTK
  QApplication a(argc, argv);
  MainWindow form;
#else
  char *fName;
#endif
  
  if(argc > 2)
    {
    return usage();
    }
#if USE_FLTK
  else
    {
    if(argc == 1)
      {
      fName = fl_file_chooser("Pick an image file", "*.*", ".");
      if(fName == NULL || strlen(fName)<1)
        {
        return 0;
        }
      }
    else
      {
      if(argv[1][0] != '-')
        {
        fName = argv[argc-1];
        }
      else
        {
        return usage();
        }
      }
    }

  std::cout << "Loading File: " << fName << std::endl;
  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  VolumeReaderType::Pointer reader = VolumeReaderType::New();


  reader->SetFileName(fName);

  ImageType::Pointer imP;
  imP = reader->GetOutput();

  try
    {
    reader->Update();
    }
 catch (itk::ExceptionObject &e)
    {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "...Done Loading File" << std::endl;

  char mainName[255];
  sprintf(mainName, "metaView: %s", fName);

  std::cout << std::endl;
  std::cout << "For directions on interacting with the window," << std::endl;
  std::cout << "   type 'h' within the window" << std::endl;

  form = make_window();

  tkMain->label(mainName);

  tkWin->SetInputImage(imP);
  tkWin->flipY(true);

  form->show();
  tkWin->show();
  tkWin->update();

  // force a first redraw
  Fl::check();
  tkWin->update();

  Fl::run();

  return 1;
#else
  form.show();
  return a.exec();
#endif
}
