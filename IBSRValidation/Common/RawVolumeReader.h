/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawVolumeReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawVolumeReader_h
#define _RawVolumeReader_h

#include <fstream>

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include <string>

namespace itk
{

/** \class RawVolumeReader
 * 
 * This component reads in a raw volume from file.
 * This class is activiated by method Execute().
 *
 * Inputs:
 *  - name of the raw image file
 *  - size of the image
 *  - spacing of the image
 *  - endian-ness of the file
 *
 * Outputs:
 *  - pointer to output image
 *
 */
template <typename TPixel, typename TImage>
class ITK_EXPORT RawVolumeReader : public Object
{
public:

  /** Standard class typedefs. */
  typedef RawVolumeReader Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RawVolumeReader, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::SizeType SizeType;
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** File format pixel type. */
  typedef TPixel PixelType;

  /** Spacing type. */
  typedef FixedArray<double,itkGetStaticConstMacro(ImageDimension)> SpacingType;
  
  /** Set the filename. */
  itkSetStringMacro( FileName );

  /** Set the image size. */
  itkSetMacro( Size, SizeType );

  /** Set the image spacing. */
  itkSetMacro( Spacing, SpacingType );

  /** Set endian-ness of the file. */
  itkSetMacro( BigEndian, bool );

  /** Activiate this class. */
  void Execute();

  /** Get the output image. */
  itkGetObjectMacro( Image, ImageType );

protected:
   RawVolumeReader();
   ~RawVolumeReader(){};


private:
  RawVolumeReader( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string  m_FileName;
  SizeType     m_Size;
  SpacingType  m_Spacing;
  bool         m_BigEndian;

  typename ImageType::Pointer m_Image;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "RawVolumeReader.txx"
#endif

#endif
