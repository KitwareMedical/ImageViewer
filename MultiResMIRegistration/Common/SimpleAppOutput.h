/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SimpleAppOutput.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _SimpleAppOutput_h
#define _SimpleAppOutput_h

#include "itkObject.h"
#include "itkAffineTransform.h"
#include <string>

namespace itk
{

/** \class SimpleAppOutput
 *
 * This component resamples the moving image using the user
 * specified transform. This class then writes out the 
 * fixed, moving and resampled image as PGM files.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - pointer to the fixed image
 *  - pointer to the moving image
 *  - pointer to an affine tranform
 *  - name of directory to output PGM files
 *
 * Outputs:
 *  - pointer to the resampled image
 */
template <typename TImage>
class ITK_EXPORT SimpleAppOutput : public Object
{
public:

  /** Standard class typedefs. */
  typedef SimpleAppOutput Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SimpleAppOutput, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };

  /** Affine transform type. */
  typedef AffineTransform<double,ImageDimension> AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, ImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, ImageType );

  /** Set the transform. */
  itkSetObjectMacro( Transform, AffineTransformType );

  /** Set the output directory name. */
  itkSetStringMacro( DirectoryName );

  /** Activiate the process. */
  void Execute();
  

protected:
  SimpleAppOutput();
  ~SimpleAppOutput(){};

private:
  SimpleAppOutput( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename ImageType::Pointer  m_FixedImage;
  typename ImageType::Pointer  m_MovingImage;
  typename ImageType::Pointer  m_ResampledImage;

  AffineTransformPointer  m_Transform;

  std::string m_DirectoryName;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "SimpleAppOutput.txx"
#endif

#endif
