/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkmyRawFileWriter.h
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
#ifndef __itkmyRawFileWriter_h
#define __itkmyRawFileWriter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include <string>

namespace itk
{

/**
 *
 *
 */
template <class TOutputImage>
class ITK_EXPORT myRawFileWriter : public ImageToImageFilter<TOutputImage, TOutputImage>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef myRawFileWriter         Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ImageToImageFilter<TOutputImage, TOutputImage>  Superclass;

  /**
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Run-time type information (and related methods).
   */
  itkTypeMacro(myRawFileWriter, ImageSource);


  enum {ImageDimension = TOutputImage::ImageDimension };
  /**
   * typedef for Size.
   */
  typedef Size<TOutputImage::ImageDimension>  SizeType;

  /**
   * typedef for Region.
   */
  typedef ImageRegion<TOutputImage::ImageDimension>  RegionType;

  typedef typename TOutputImage::PixelType PixelType;
  
  /**
   * Specify the file to load. This is forwarded to the IO instance. 
   * Either the FileName or FilePrefix plus pattern are used to read
   * files.
   */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  //  itkSetMacro(PixelSize, unsigned int);
  //  itkGetMacro(PixelSize, unsigned int);

  //  itkSetMacro(Region, RegionType);
  //  itkGetMacro(Region, RegionType);

  /** 
   * Specify file prefix for the image file(s). You should specify either
   * a FileName or FilePrefix. Use FilePrefix if the data is stored
   * in multiple files. (Note: the FileName ivar is available from the
   * superclass.)
   */
  //  itkSetStringMacro(FilePrefix);
  //  itkGetStringMacro(FilePrefix);

  /**
   * The sprintf format used to build filename from FilePrefix and number.
   */
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
