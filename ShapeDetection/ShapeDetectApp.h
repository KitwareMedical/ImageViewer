/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __ShapeDetectApp_h
#define __ShapeDetectApp_h

#include "itkImage.h"
#include "itkCovariantVector.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include <string>

/** \class ShapeDetectApp
 * \brief A simple level-set shape detection command line application
 */
class ShapeDetectApp
{
public:
  /** ImageDimension enumeration. */
  enum{ ImageDimension = 3 };

  /** InputPixelType is the raw image pixel type. */
  typedef signed short InputPixelType;

  /** InputImageType is the raw image type. */
  typedef itk::Image<InputPixelType,ImageDimension> InputImageType;
  typedef InputImageType::Pointer InputImagePointer;

  /** ImageType is the internal image type. */
  typedef itk::Image<float,ImageDimension> ImageType;
  typedef ImageType::Pointer ImagePointer;

  /** SizeType is the image size type, */
  typedef InputImageType::SizeType SizeType;

  /** IndexType is the image index type. */
  typedef InputImageType::IndexType IndexType;

  /** DerivativeImageType is the derivative image type. */
  typedef itk::CovariantVector<float,ImageDimension> VectorType;
  typedef itk::Image<VectorType, ImageDimension> DerivativeImageType;

  /** Derivative filter type, */
  typedef itk::GradientRecursiveGaussianImageFilter<
    InputImageType,DerivativeImageType,double>  DerivativeFilterType;
  typedef DerivativeFilterType::Pointer DerivativeFilterPointer;

  /** Shape Detection filter type. */
  typedef itk::FastMarchingImageFilter<ImageType,ImageType> DetectionFilterType;
  typedef DetectionFilterType::Pointer DetectionFilterPointer;
    

  /** Constructors */
  ShapeDetectApp();
  ShapeDetectApp( const char * );

  /** Destructors */
  ~ShapeDetectApp(){};

  /** Execute the application.  */
  void Execute();

  /** Get the target image */
  InputImagePointer GetInputImage() const
    { return m_InputImage; }

private:
   /** Input image file name.  */
   std::string          m_InputFileName;

   /** Input image size.  */
   SizeType             m_InputSize;

   /** Input big endian flag.  */
   bool                 m_InputBigEndian;

   /** Input image.  */
   InputImagePointer    m_InputImage;

   /** Dump pgm files flag  */
   bool                 m_DumpPGMFiles;

   /** PGM directory.  */
   std::string          m_PGMDirectory;

   /** Seed point.  */
   IndexType            m_Seed;

   /** Time crossing threshold.  */
   double               m_Threshold;

   /** Sigma for computing derivative of gaussian.  */
   double               m_Sigma;

   /** Edge potential map.  */
   ImagePointer         m_EdgePotentialImage;

   /** Shape detection filter.  */
   DetectionFilterPointer  m_DetectionFilter;

   /** Segmentation mask.  */
   ImagePointer          m_SegmentationMask;

   /** Read in an image.  */
   bool ReadImage( const char *, const SizeType&, bool, InputImageType * );

   /** Initialize  */
   void Initialize();

   /** Compute edge potential map  */
   void ComputeEdgePotentialMap();

   /** Compute time crossing map using fast marching  */
   void ComputeTimeCrossingMap();

   /** Threshold time crossing map  */
   void ThresholdTimeCrossingMap();

   /** Write segmentation mask  */
  void WriteSegmentationImage();
  
};


#endif
