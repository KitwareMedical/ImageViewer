/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWriteMetaImage.h
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
#ifndef __itkWriteMetaImage_h
#define __itkWriteMetaImage_h

#include "itkImageWriter.h"
#include "MetaUtils.h"
#include <vector>

namespace itk
{

/** \class WriteMetaImage
 * \brief Write an image in Meta Image format.
 *
 * WriteMetaImage writes N-D images in Meta Image file format. 
 */
template <class TInputImage>
class ITK_EXPORT WriteMetaImage : public ImageWriter<TInputImage>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef WriteMetaImage       Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ImageWriter<TInputImage>   Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;

  /** 
   * Type of pixel
   */
  typedef   typename TInputImage::PixelType      PixelType;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(WriteMetaImage,ImageWriter);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /** 
   * Specify the name of the input file.
   */
  itkSetStringMacro(FileName);
  
  /** 
   * Get the name of the input file.
   */
  itkGetStringMacro(FileName);

   /** 
   * GenerateData the filter (write the image)
   */
   void GenerateData(void);
 
   /** 
   * Write Data (abstract function from Writer)
   */
   void WriteData(void);


   /**
    * Return the MetaImage type code corresponding to the pixel type
    */
    MET_Type GetTypeCode(void) const;
 

protected:
  WriteMetaImage();
  ~WriteMetaImage() {}
  WriteMetaImage(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ofstream& os, Indent indent) const;

private:
  std::string m_FileName;

};

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWriteMetaImage.txx"
#endif

#endif
