/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSRawFileReader_h
#define __itkSWSRawFileReader_h

#include "itkImageSource.h"
#include "itkImage.h"
#include "itkExceptionObject.h"
#include <string>

namespace itk
{

namespace sws
{
  
/** \class RawFileReader
 * \brief A very simple reader for raw binary image data without headers.
 * This class reads a binary file into an itk::Image.  The size of the image
 * is given by the SetRegion macro. */
template <class TOutputImage>
class ITK_EXPORT RawFileReader : public ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs.  */
  typedef RawFileReader         Self;
  typedef ImageSource<TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory.   */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).   */
  itkTypeMacro(RawFileReader, ImageSource);

  /** Convenient typedefs.   */
  typedef Size<TOutputImage::ImageDimension>  SizeType;
  typedef ImageRegion<TOutputImage::ImageDimension>  RegionType;
  typedef typename TOutputImage::PixelType PixelType;
  
  /** Specify the name of the file to load.  */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Declare the size of each pixel.  This method is not used.  Pixel size is
   * determined from the templated image type.   */
  itkSetMacro(PixelSize, unsigned int);
  itkGetMacro(PixelSize, unsigned int);

  /** Declare the size of the image region contained in the file.   */
  itkSetMacro(Region, RegionType);
  itkGetMacro(Region, RegionType);

  /** Execute the reader.  This public method allows the filter to be updated
   * without propagating updates through the pipeline.   */
  void SafeRead();

protected:
  RawFileReader()
    {
      typename TOutputImage::Pointer p = TOutputImage::New();
      this->SetNthOutput(0, p.GetPointer() );
    }
  ~RawFileReader()    {}
  RawFileReader(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const {}

  virtual void GenerateOutputInformation();
  virtual void GenerateData() {   this->SafeRead(); }

  // size of the pixels in the chunk file
  unsigned int m_PixelSize;  
  std::string m_FileName;
  RegionType m_Region;
};

} // namespace SWS
  
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSRawFileReader.txx"
#endif

#endif // __itkRawFileReader_h
