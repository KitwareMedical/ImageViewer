/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRawImageReaderViewer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkImage.h"
#include "itkRawImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "fltkImageViewer.h"
#include <FL/fl_file_chooser.H>




class RawImageReaderViewer
{

public:

  enum { Dimension =  3 };

  typedef  unsigned char                              PixelType;
  typedef  itk::Image< PixelType, Dimension >         ImageType;
  typedef  itk::ImageFileReader< ImageType >          FileSourceType;
  typedef  itk::ImageFileWriter< ImageType >          FileSinkType;
  typedef  itk::RawImageIO<PixelType,Dimension>       RawReaderType;
  typedef  itk::RawImageIO<PixelType,Dimension>       RawWriterType;
  typedef  fltk::ImageViewer<PixelType,PixelType>     ViewerType;

public:

  virtual void Load(void);
  virtual void Load(const char * inputFileName);

  virtual void Save(void);
  virtual void Save(const char * outputFileName);

  virtual void SetDimensionX( unsigned int );
  virtual void SetDimensionY( unsigned int );
  virtual void SetDimensionZ( unsigned int );

  virtual void Show(void){};
  virtual void ShowImage(void);

  virtual void HideImage(void);

  virtual void Quit(void);

  RawImageReaderViewer();
  virtual ~RawImageReaderViewer();


private:

  FileSourceType::Pointer     m_RawImageSource;
  FileSinkType::Pointer       m_RawImageSink;
  
  RawReaderType::Pointer      m_RawReader; 
  RawReaderType::Pointer      m_RawWriter; 

  ViewerType                  m_Viewer;

  unsigned int                m_NumberOfPixelsInX;
  unsigned int                m_NumberOfPixelsInY;
  unsigned int                m_NumberOfPixelsInZ;

};



