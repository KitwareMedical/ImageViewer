/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    PGMVolumeWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _PGMVolumeWriter_h
#define _PGMVolumeWriter_h

#include <fstream>

#include "itkObjectFactory.h"
#include "itkObject.h"
#include <string>

namespace itk
{

/** \class PGMVolumeWriter
 * 
 * This component writes out an image as a sequence of 2D slices.
 *
 * Inputs:
 *  - pointer to image to be written out
 *  - the directory name of where to files are to be written
 *  - the file prefix for each file
 *
 */
template <typename TImage>
class ITK_EXPORT PGMVolumeWriter : public Object
{
public:

  /** Standard class typedefs. */
  typedef PGMVolumeWriter Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PGMVolumeWriter, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;
  
  /** Enumeration of the image dimension. */
  enum { ImageDimension = TImage::ImageDimension };

  /** Set the input image. */
  itkSetObjectMacro( Image, ImageType );

  /** Set the output directory name. */
  itkSetStringMacro( DirectoryName );

  /** Set the file prefix. */
  itkSetStringMacro( FilePrefix );

  /** Method to execute the writer. */
  virtual void Execute();
  
protected:
  
   PGMVolumeWriter();
   ~PGMVolumeWriter(){};


private:
  PGMVolumeWriter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string  m_DirectoryName;
  std::string  m_FilePrefix;

  typename ImageType::Pointer m_Image;

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "PGMVolumeWriter.txx"
#endif

#endif
