/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FuzzyConnectApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __FuzzyConnectApp_h
#define __FuzzyConnectApp_h

#include "itkImage.h"
#include "itkSimpleFuzzyConnectednessScalarImageFilter.h"
#include <string>

/** \class FuzzyConnectApp
 * \brief A simple fuzzy connectedness based object delineation command 
 *        line application
 */
class FuzzyConnectApp
{
public:

  /** ImageDimension enumeration. */
  enum{ ImageDimension = 3 };

  /** InputPixelType is the raw image pixel type. */
  typedef signed short InputPixelType;

  /** InputImageType is the raw image type */
  typedef itk::Image<InputPixelType,ImageDimension> InputImageType;
  typedef InputImageType::Pointer InputImagePointer;

  /** BinaryImageType is the binary image type */
  typedef itk::Image<bool,ImageDimension> BinaryImageType;
  typedef BinaryImageType::Pointer BinaryImagePointer;

  /** SizeType is the image size type. */
  typedef InputImageType::SizeType SizeType;

  /** IndexType is the image index type. */
  typedef InputImageType::IndexType IndexType;

  /** Fuzzy connectedness filter type. */
  typedef itk::SimpleFuzzyConnectednessScalarImageFilter<InputImageType,InputImageType>
          FilterType;
  typedef FilterType::Pointer FilterPointer;

  /** Constructors */
  FuzzyConnectApp();
  FuzzyConnectApp( const char * );

  /** Destructors */
  ~FuzzyConnectApp(){};

  /** Execute the application */
  void Execute();

  /** Get the target image */
  InputImagePointer GetInputImage() const
   { return m_InputImage; }



private:

   /*** Input image file name   */
   std::string          m_InputFileName;

   /*** Input image size  */
   SizeType             m_InputSize;

   /*** Input image spacing  */
   double               m_InputSpacing[ImageDimension];

   /*** Input big endian flag  */
   bool                 m_InputBigEndian;

   /*** Input image  */
   InputImagePointer    m_InputImage;

   /** Fuzzy connectedness filter  */
   FilterPointer        m_Filter;

   /*** The seed  */
   IndexType            m_Seed;

   /*** Threshold value  */
   double               m_Threshold;

   /*** Parameters  */
   double               m_ObjectMean;
   double               m_ObjectVariance;
   double               m_DiffMean;
   double               m_DiffVariance;
   double               m_Weight;

   /*** Dump pgm files flag  */
   bool                 m_DumpPGMFiles;

   /*** PGM directory  */
   std::string          m_PGMDirectory;

   /*** Read in an image  */
   bool ReadImage( const char *, const SizeType&, const double *, bool,
    InputImageType * );

   /*** Compute the fuzzy connectedness map  */
   void ComputeMap();

   /*** Compute segmentation image  */
   void ComputeSegmentationImage();

   /*** Write out segmentation image  */
   void WriteSegmentationImage();

   /*** Initialize  */
   void Initialize();

   /*** Write PGM files  */
   bool WritePGMFiles(InputImageType *, const char *, const char * );

  
};


#endif
