/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSRawFileWriter_h
#define __itkSWSRawFileWriter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include <string>

namespace itk
{
namespace sws
{
/** \class RawFileWriter
 * \brief A simple IO object for writing itk::Images as binary, headerless
 files. */
template <class TOutputImage>
class ITK_EXPORT RawFileWriter : public ImageToImageFilter<TOutputImage, TOutputImage>
{
public:
  /** Standard class typedefs.   */
  typedef RawFileWriter         Self;
  typedef ImageToImageFilter<TOutputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory.   */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).   */
  itkTypeMacro(RawFileWriter, ImageSource);

  /** Image dimension. */
  enum {ImageDimension = TOutputImage::ImageDimension };

  /** Convenient typedefs.   */
  typedef Size<TOutputImage::ImageDimension>  SizeType;
  typedef ImageRegion<TOutputImage::ImageDimension>  RegionType;
  typedef typename TOutputImage::PixelType PixelType;
  
  /** Specify the file to load. This is forwarded to the IO instance. 
   * Either the FileName or FilePrefix plus pattern are used to read
   * files.   */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  RawFileWriter()
    {
      typename TOutputImage::Pointer p = TOutputImage::New();
      this->SetNthOutput(0, p.GetPointer() );
    }
  ~RawFileWriter()    {}
  RawFileWriter(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const {}

  virtual void GenerateData();

  // size of the pixels in the chunk file
  std::string m_FileName;
};

} // namespace ITK
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSRawFileWriter.txx"
#endif

#endif // __itkSWSRawFileWriter_h
