/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRawImageSequenceReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRawImageSequenceReader_h
#define __itkRawImageSequenceReader_h

#include "itkImageIOBase.h"
#include "itkImageSource.h"
#include "itkExceptionObject.h"
#include "itkSize.h"
#include "itkImageRegion.h"
#include "itkDefaultConvertPixelTraits.h"

namespace itk
{

/** \brief Base exception class for IO conflicts. */
class RawImageSequenceReaderException : public ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro( RawImageSequenceReaderException, ExceptionObject );

  /** Constructor. */
  RawImageSequenceReaderException(char *file, unsigned int line, 
                           const char* message = "Error in IO") : 
    ExceptionObject(file, line)
    {
      SetDescription(message);
    }

  /** Constructor. */
  RawImageSequenceReaderException(const std::string &file, unsigned int line, 
                           const char* message = "Error in IO") : 
    ExceptionObject(file, line)
    {
      SetDescription(message);
    }
};


/** \brief Data source that reads image data from disk files.
 *
 * This source object is a general filter to read raw slice by
 * slice data from file. It works with a RawImageIO class
 * to actually do the reading of the data.
 *
 * TOutputImage is the type expected by the external users of the
 * filter. If data stored in the file is stored in a different format
 * then specified by TOutputImage, than this filter converts data 
 * between the file type and the external expected type.  The 
 * ConvertTraits template argument is used to do the conversion.
 *
 * \warning This source assumes that TOutputImage::ImageDimension is 3
 * and the image data are stored as 2D slices, one per file.
 *
 */
template <class TOutputImage,
  class ConvertPixelTraits = 
DefaultConvertPixelTraits< ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT RawImageSequenceReader : public ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef RawImageSequenceReader         Self;
  typedef ImageSource<TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RawImageSequenceReader, ImageSource);

  /** Image dimension enumeration. */
  itkStaticConstMacro (ImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** The size of the output image. */
  typedef Size<itkGetStaticConstMacro(ImageDimension)>  Size;

  /** The region of the output image. */
  typedef ImageRegion<itkGetStaticConstMacro(ImageDimension)>  Region;

  /** The pixel type of the output image. */
  typedef typename TOutputImage::PixelType OutputImagePixelType;
    
  /** Specify file prefix for the image file(s). */
  itkSetStringMacro(FilePrefix);
  itkGetStringMacro(FilePrefix);
  
  /** The sprintf format used to build filename from FilePrefix and number. */
  itkSetStringMacro(FilePattern);
  itkGetStringMacro(FilePattern);
  
  /** Set/Get the ImageIO helper class. */
  itkSetObjectMacro(ImageIO,ImageIOBase);
  itkGetObjectMacro(ImageIO,ImageIOBase);

  /** Set/Get the starting slice number. */
  itkSetMacro( StartSliceNumber, signed long );
  itkGetMacro( StartSliceNumber, signed long );

  /** Set/Get the ending slice number. */
  itkSetMacro( EndSliceNumber, signed long );
  itkGetMacro( EndSliceNumber, signed long );
  
protected:
  RawImageSequenceReader();
  ~RawImageSequenceReader();
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Convert a block of pixels from one type to another. */
  void DoConvertBuffer(void* buffer, OutputImagePixelType * outputData, unsigned long numberOfPixels);

  /** Does the real work. */
  virtual void GenerateData();

  ImageIOBase::Pointer m_ImageIO;

  std::string m_FilePrefix;
  std::string m_FilePattern;

  signed long m_StartSliceNumber;
  signed long m_EndSliceNumber;
  
private:
  RawImageSequenceReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};


} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRawImageSequenceReader.txx"
#endif

#endif // __itkRawImageSequenceReader_h
