/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DemonsRegistrator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _DemonsRegistrator_h
#define _DemonsRegistrator_h

#include "itkObject.h"
#include "itkVector.h"
#include "itkImage.h"
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"

#include "itkArray.h"

namespace itk
{

/** \class DemonsRegistrator
 *
 * This component computes the transform to register a
 * moving image onto a fixed image.
 *
 * In particular, it uses the deformable demons registration
 * algorithm.
 *
 * The registration is done using a multiresolution strategy.
 * At each resolution level, the downsampled images are obtained
 * using a RecursiveMultiResolutionPyramidImageFilter.
 * 
 * \warning This class requires both images to be 3D and with
 * pixels of a real type.
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
class DemonsRegistrator : public Object
{
public:

  /** Standard class typedefs. */
  typedef DemonsRegistrator Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DemonsRegistrator, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

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
  typedef MultiResolutionPDEDeformableRegistration< 
                                    FixedImageType, 
                                    MovingImageType,
                                    DeformationFieldType >    RegistrationType;

  /** UnsignedIntArray type. */
  typedef Array<unsigned int> UnsignedIntArray;

  /** ShrinkFactorsArray type. */
  typedef FixedArray<unsigned int,ImageDimension> ShrinkFactorsArray;

  /** Set the fixed image. */
  itkSetObjectMacro( FixedImage, FixedImageType );

  /** Set the moving image. */
  itkSetObjectMacro( MovingImage, MovingImageType );

  /** Set the number of resolution levels. */
  itkSetClampMacro( NumberOfLevels, unsigned short, 1,
    NumericTraits<unsigned short>::max() );

  /** Set the number of iterations per level. */
  itkSetMacro( NumberOfIterations, UnsignedIntArray );

  /** Set the fixed and moving image shrink factors. */
  itkSetMacro( FixedImageShrinkFactors, ShrinkFactorsArray );
  itkSetMacro( MovingImageShrinkFactors, ShrinkFactorsArray );

  /** Method to execute the registration. */
  virtual void Execute();

  /** Get the deformation field. */
  itkGetObjectMacro( DeformationField, DeformationFieldType );

  /** Initialize registration at the start of new level. */
  void StartNewLevel();

protected:
  DemonsRegistrator();
  ~DemonsRegistrator();

private:
  DemonsRegistrator( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename FixedImageType::Pointer            m_FixedImage;
  typename MovingImageType::Pointer           m_MovingImage;
  typename FixedImagePyramidType::Pointer     m_FixedImagePyramid;
  typename MovingImagePyramidType::Pointer    m_MovingImagePyramid;
  typename RegistrationType::Pointer          m_Registration;

  unsigned short                              m_NumberOfLevels;
  UnsignedIntArray                            m_NumberOfIterations;

  ShrinkFactorsArray                          m_MovingImageShrinkFactors;
  ShrinkFactorsArray                          m_FixedImageShrinkFactors;

  typename DeformationFieldType::Pointer          m_DeformationField;

  unsigned long                               m_Tag;


};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "DemonsRegistrator.txx"
#endif

#endif
