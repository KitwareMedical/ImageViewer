/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSRawFileReader.h
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
  /** Standard "Self" typedef.  */
  typedef RawFileReader         Self;

  /** Standard "Superclass" typedef. */
  typedef ImageSource<TOutputImage>  Superclass;

  /** Smart pointer typedef support.   */
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory.   */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).   */
  itkTypeMacro(RawFileReader, ImageSource);

  /** typedef for Size.   */
  typedef Size<TOutputImage::ImageDimension>  SizeType;

  /** typedef for Region.   */
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
