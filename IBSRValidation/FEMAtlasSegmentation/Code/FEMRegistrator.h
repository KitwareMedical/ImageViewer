/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMRegistrator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMRegistrator_h
#define _FEMRegistrator_h

#include "itkObject.h"
#include "itkVector.h"
#include "itkImage.h"
#include "itkFEM.h"
#include "itkFEMRegistrationFilter.h"

#include "itkArray.h"

namespace itk
{

/** \class FEMRegistrator
 *
 * This component computes the transform to register a
 * moving image onto a fixed image.
 *
 * In particular, it uses the deformable FEM registration
 * algorithm.
 *
 * The registration is done using a multiresolution strategy.
 * At each resolution level, the downsampled images are obtained
 * using a RecursiveMultiResolutionPyramidImageFilter.
 * 
 * \warning This class requires both images to be 3D and with
 * pixels of known type.
 *
 * The registration process is activated by method Execute().
 *
 * Inputs:
 *   - pointer to fixed image
 *   - pointer to moving image
 *   - number of resolution levels
 *   - number of optimization iterations at each level
 *   - the initial rigid (quaternion) transform parameters
 *   - the coarest level shrink factors for the fixed image
 *   - the coarest level shrink factors for the moving image
 *
 * Outputs:
 *   - pointer to output deformation field
 *
 */ 
template <
typename TFixedImage, 
typename TMovingImage,
typename TFieldValue = float>
class FEMRegistrator : public Object
{
public:

  /** Standard class typedefs. */
  typedef FEMRegistrator Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMRegistrator, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Label Image Type */
  typedef Image<unsigned char,3> LabelImageType;

  /** Fixed Image Type. */
  typedef TFixedImage FixedImageType;

  /** Moving Image Type. */
  typedef TMovingImage MovingImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TFixedImage::ImageDimension };

  /** Deformation field value type. */
  typedef TFieldValue FieldValueType;

  /** Deformation field pixel type. */
  typedef Vector<FieldValueType,ImageDimension> FieldPixelType;

  /** Deformation field type. */
  typedef Image<FieldPixelType,ImageDimension> DeformationFieldType;

  /** Fixed Image Pyramid Type. */
  typedef RecursiveMultiResolutionPyramidImageFilter<
                                    FixedImageType,
                                    FixedImageType  >    FixedImagePyramidType;

  /** Moving Image Pyramid Type. */
  typedef RecursiveMultiResolutionPyramidImageFilter<
                                    MovingImageType,
                                    MovingImageType  >   MovingImagePyramidType;

  /** Registration Method. */
  typedef fem::FEMRegistrationFilter<MovingImageType,FixedImageType> RegistrationType; 

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, FixedImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, MovingImageType );

  /** Set the moving image. */
  itkSetObjectMacro( AtlasLabelImage, LabelImageType );

  /** Set the moving image. */
  itkGetObjectMacro( AtlasLabelImage, LabelImageType );

  /** Set the moving image. */
  itkSetObjectMacro( WarpedAtlasLabelImage, LabelImageType );

  /** Set the moving image. */
  itkGetObjectMacro( WarpedAtlasLabelImage, LabelImageType );

  /** Set FEM registration configuratrion filename. */
  itkSetStringMacro( FEMConfigurationFileName );

  /** Method to execute the registration. */
  virtual void Execute();

  /** Get the deformation field. */
  itkGetObjectMacro( DeformationField, DeformationFieldType );

protected:
  FEMRegistrator();
  ~FEMRegistrator();

private:
  FEMRegistrator( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename FixedImageType::Pointer            m_FixedImage;
  typename MovingImageType::Pointer           m_MovingImage;
  typename LabelImageType::Pointer            m_AtlasLabelImage;
  typename LabelImageType::Pointer            m_WarpedAtlasLabelImage;
  RegistrationType                            m_Registration;

  typename MovingImageType::Pointer           m_DeformedImage;
  typename DeformationFieldType::Pointer      m_DeformationField;

  unsigned long                               m_Tag;

  std::string                   m_FEMConfigurationFileName;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "FEMRegistrator.txx"
#endif

#endif
