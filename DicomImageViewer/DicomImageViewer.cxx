/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DicomImageViewer.cxx
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

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "fltkImage2DViewer.h"

#include "FL/fl_file_chooser.H"


int usage(void)
   {
   std::cout << "DicomImageViewer" << std::endl;
   std::cout << std::endl;
   std::cout << "DicomImageViewer <Filename>" << std::endl;
   std::cout << std::endl;

   return 1;
   }

int main(int argc, char **argv)
  {
  typedef itk::Image< float, 2 > ImageType;

  char *fName;
  
  if(argc > 2)
    {
    return usage();
    }
   else
    if(argc == 1)
      {
      fName = fl_file_chooser("Pick a DicomImage file", "*.dcm", ".");
      if(fName == NULL || strlen(fName)<1)
        {
        return 0;
        }
      }
    else
      if(argv[1][0] != '-')
        {
        fName = argv[argc-1];
        }
      else
        {
        return usage();
        }

  std::cout << "Loading File: " << fName << std::endl;
  typedef itk::ImageFileReader< ImageType > VolumeReaderType;
  VolumeReaderType::Pointer reader = VolumeReaderType::New();

  reader->SetFileName(fName);

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & exception )
    {
    std::cout << "Problems reading file format" << std::endl;
    std::cout << exception << std::endl;
    return 1;
    }
  std::cout << "...Done Loading File" << std::endl;

  typedef fltk::Image2DViewer<ImageType::PixelType> ImageViewerType;
  ImageViewerType::Pointer viewer = ImageViewerType::New();

  viewer->SetLabel( fName );
  viewer->SetImage( reader->GetOutput() );
  viewer->Show();

  Fl::run();
  
  return 1;
  }
