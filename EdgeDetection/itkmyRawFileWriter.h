/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkmyRawFileWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkmyRawFileWriter_h
#define __itkmyRawFileWriter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include <string>

namespace itk
{

template <class TOutputImage>
class ITK_EXPORT myRawFileWriter : public ImageToImageFilter<TOutputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef myRawFileWriter         Self;
  typedef ImageToImageFilter<TOutputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(myRawFileWriter, ImageSource);

  /** The dimension of the image. */
  enum {ImageDimension = TOutputImage::ImageDimension };

  /** Typedefs. */
  typedef Size<TOutputImage::ImageDimension>  SizeType;
  typedef ImageRegion<TOutputImage::ImageDimension>  RegionType;
  typedef typename TOutputImage::PixelType PixelType;
  
  /** Specify the file to load. This is forwarded to the IO instance.  Either
   * the FileName or FilePrefix plus pattern are used to read files.  */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /*** Specify file prefix for the image file(s). You should specify either *
   * a FileName or FilePrefix. Use FilePrefix if the data is stored in
   * multiple files. (Note: the FileName ivar is available from the
   * superclass.)  */
  //  itkSetStringMacro(FilePrefix);
  //  itkGetStringMacro(FilePrefix);

  /** The sprintf format used to build filename from FilePrefix and number. */
  //  itkSetStringMacro(FilePattern);
  //  itkGetStringMacro(FilePattern);


protected:
  myRawFileWriter()
    { std::cout << "myRawFileWriter()" << std::endl; }
  ~myRawFileWriter()    {}
  myRawFileWriter(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const {}

  virtual void GenerateData();

  // size of the pixels in the chunk file
  //  unsigned int m_PixelSize;  
  std::string m_FileName;
  //  RegionType m_Region;
  //  std::string m_FilePrefix;
  //  std::string m_FilePattern;
  

};

} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkmyRawFileWriter.txx"
#endif

#endif // __itkmyRawFileWriter_h
