/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    RawSliceVolumeReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _RawSliceVolumeReader_h
#define _RawSliceVolumeReader_h

#include <fstream>

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include <string>

namespace itk
{

/** \class RawSliceVolumeReader
 * 
 * This component reads in a raw 3D volume from 
 * a sequence of 2D raw file.
 * This class is activiated by method Execute().
 *
 * Inputs:
 *  - prefix of the raw 2D files
 *  - file pattern of the raw 2D files
 *  - starting slice number
 *  - size of the image
 *  - spacing of the image
 *  - endian-ness of the file
 *
 * Outputs:
 *  - pointer to output image
 *
 */
template <typename TPixel, typename TImage>
class ITK_EXPORT RawSliceVolumeReader : public Object
{
public:

  /** Standard class typedefs. */
  typedef RawSliceVolumeReader Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RawSliceVolumeReader, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  typedef typename ImageType::SizeType SizeType;
  enum { ImageDimension = TImage::ImageDimension };

  /** File format pixel type. */
  typedef TPixel PixelType;

  /** Spacing type. */
  typedef FixedArray<double,ImageDimension> SpacingType;

  /** Set the file name prefix. */
  itkSetStringMacro( FilePrefix );
  
  /** Set the file name pattern. */
  itkSetStringMacro( FilePattern );

  /** Set the image size. */
  itkSetMacro( Size, SizeType );

  /** Set the image spacing. */
  itkSetMacro( Spacing, SpacingType );

  /** Set endian-ness of the file. */
  itkSetMacro( BigEndian, bool );

  /** Set the starting slice number. */
  itkSetMacro( StartSliceNumber, signed long );

  /** Activiate this class. */
  void Execute();

  /** Get the output image. */
  itkGetObjectMacro( Image, ImageType );

protected:
   RawSliceVolumeReader();
   ~RawSliceVolumeReader(){};


private:
  RawSliceVolumeReader( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string  m_FilePrefix;
  std::string  m_FilePattern;
  SizeType     m_Size;
  SpacingType  m_Spacing;
  bool         m_BigEndian;
  signed long  m_StartSliceNumber;

  typename ImageType::Pointer m_Image;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "RawSliceVolumeReader.txx"
#endif

#endif
