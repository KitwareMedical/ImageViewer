/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWriteMetaImage.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWriteMetaImage_h
#define __itkWriteMetaImage_h

#include "itkImageFileWriter.h"
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
class ITK_EXPORT WriteMetaImage : public ImageFileWriter<TInputImage>
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef WriteMetaImage       Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef ImageFileWriter<TInputImage>   Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;

  /** 
   * Type of pixel
   */
  typedef   typename TInputImage::PixelType      PixelType;
  typedef            TInputImage                 InputImageType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(WriteMetaImage,ImageFileWriter);

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
