/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltValidationOutput.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltValidationOutput_h
#define _VanderbiltValidationOutput_h

#include "itkObject.h"
#include "itkAffineTransform.h"
#include <string>

#include "VanderbiltModality.h"

namespace itk
{

/** \class VanderbiltValidationOutput
 *
 * This component writes out the overall transform in
 * Vanderbilt format.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - pointer to the fixed image
 *  - pointer to the moving image
 *  - pointer to an affine tranform
 *  - flag to indicate whether or not the transform should be inverted
 *    ( i.e. exchange the roles of the moving and fixed images )
 *  - name of the output file
 *  - flag to indicate whether or not to append to output file
 *  - string containing the name of investigator(s)
 *  - string containing the investigation site
 *  - string containing a description of the method used
 *  - string containing the date
 *  - patient id number { 0 = practice, 1-9, 101-109 }
 *  - the "From" modality
 *  - the "To" modality
 *
 *
 */
template <typename TImage>
class ITK_EXPORT VanderbiltValidationOutput : public Object
{
public:

  /** Standard class typedefs. */
  typedef VanderbiltValidationOutput Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VanderbiltValidationOutput, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TImage ImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TImage::ImageDimension };

  /** Affine transform type. */
  typedef AffineTransform<double,ImageDimension> AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  /** Modality type. */
  typedef VanderbiltModality::ModalityType ModalityType;

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, ImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, ImageType );

  /** Set the transform. */
  itkSetObjectMacro( Transform, AffineTransformType );

  /** Set the flag to indicate whether or not to invert the transform. */
  itkSetMacro( InvertTransform, bool );
  itkGetMacro( InvertTransform, bool );
  itkBooleanMacro( InvertTransform );

  /** Set output file name. */
  itkSetStringMacro( OutputFileName );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

  /** Set header information. */
  itkSetStringMacro( InvestigatorString1 );
  itkSetStringMacro( InvestigatorString2);
  itkSetStringMacro( SiteString1 );
  itkSetStringMacro( SiteString2 );
  itkSetStringMacro( MethodString );
  itkSetStringMacro( DateString );

  /** Set patient number. */
  itkSetMacro( PatientNumber, unsigned int );

  /** Set the from modality. */
  itkSetMacro( FromModality, ModalityType );

  /** Set the to modality. */
  itkSetMacro( ToModality, ModalityType );

  /** Activiate the process. */
  void Execute();
  

protected:
  VanderbiltValidationOutput();
  ~VanderbiltValidationOutput(){};

private:
  VanderbiltValidationOutput( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename ImageType::Pointer   m_FixedImage;
  typename ImageType::Pointer   m_MovingImage;
  typename ImageType::Pointer   m_ResampledImage;

  AffineTransformPointer        m_Transform;
  bool                          m_InvertTransform;

  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

  std::string                   m_InvestigatorString1;
  std::string                   m_InvestigatorString2;
  std::string                   m_SiteString1;
  std::string                   m_SiteString2;
  std::string                   m_MethodString;
  std::string                   m_DateString;

  unsigned int                  m_PatientNumber;
  ModalityType                  m_FromModality;
  ModalityType                  m_ToModality;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "VanderbiltValidationOutput.txx"
#endif

#endif
