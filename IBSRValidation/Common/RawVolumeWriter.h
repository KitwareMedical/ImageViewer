/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawVolumeWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawVolumeWriter_h
#define _RawVolumeWriter_h

#include <fstream>

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include <string>

namespace itk
{

/** \class RawVolumeWriter
 * 
 * This component writes in a raw volume to file.
 * This class is activiated by method Execute().
 *
 * Inputs:
 *  - the input image
 *  - name of the raw image file
 *  - endian-ness of the file
 *
 */
template <typename TImage>
class ITK_EXPORT RawVolumeWriter : public Object
{
public:

  /** Standard class typedefs. */
  typedef RawVolumeWriter Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RawVolumeWriter, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** File format pixel type. */
  typedef typename ImageType::PixelType PixelType;

  /** Set the input image. */
  virtual void SetInputImage( const ImageType * ptr )
    { m_InputImage = ptr; }
      
  /** Set the filename. */
  itkSetStringMacro( FileName );

  /** Set endian-ness of the file. */
  itkSetMacro( BigEndian, bool );

  /** Activiate this class. */
  void Execute();


protected:
   RawVolumeWriter();
   ~RawVolumeWriter(){};


private:
  RawVolumeWriter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string  m_FileName;
  bool         m_BigEndian;

  typename ImageType::ConstPointer m_InputImage;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "RawVolumeWriter.txx"
#endif

#endif
