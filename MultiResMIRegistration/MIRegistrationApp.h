/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIRegistrationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MIRegistrationApp_h
#define __MIRegistrationApp_h

#include "itkImage.h"
#include "itkArray.h"
#include "MIMRegistrator.h"

#include <string>

/** \class MIRegistrationApp
 * \brief A mutual information registration command line based application. */
class MIRegistrationApp
{
public:
  /** ImageDimension enumeration. */
  enum{ ImageDimension = 3 };

  /** InputPixelType is the raw image pixel type. */
  typedef signed short InputPixelType;

  /** PixelType is the internal image represenation pixel type. */
  typedef float PixelType;

  /** InputImageType is the raw image type */
  typedef itk::Image<InputPixelType,ImageDimension> InputImageType;
  typedef InputImageType::Pointer InputImagePointer;

  /** ImageType is the internal image type */
  typedef itk::Image<PixelType,ImageDimension> ImageType;
  typedef ImageType::Pointer ImagePointer;

  /** SizeType is the image size type */
  typedef InputImageType::SizeType SizeType;

  /** RegistratorType is the type of the registrator used */
  typedef itk::MIMRegistrator<
    ImageType, ImageType> RegistratorType;
  typedef RegistratorType::Pointer RegistratorPointer;

  /** ParametersType is the type of the transform parameters. */
  typedef RegistratorType::ParametersType ParametersType;

  /** MaxNumberOfLevels enumeration. */
  enum { MaxNumberOfLevels = 8 };

  /** Constructors. */
  MIRegistrationApp();
  MIRegistrationApp( const char * );

  /** Destructors. */
  ~MIRegistrationApp(){};

  /** Execute the application. */
  void Execute();

  /** Get the target image */
  InputImagePointer GetTarget() const
   { return m_Target; }

  /** Get the reference image. */
  InputImagePointer GetReference() const
    { return m_Reference; }

  /** Get the registered image. */
  InputImagePointer GetRegisteredImage() const
    { return m_RegisteredImage; }

  /** Get the output parameters. */
  const ParametersType& GetParameters() const
    { return m_Parameters; }

private:
   /*** The registrator  */
   RegistratorPointer   m_Registrator;

   /*** The solution parameters  */
   ParametersType       m_Parameters;

   /*** Target image file name   */
   std::string          m_TargetFileName;

   /*** Target image size  */
   SizeType             m_TargetSize;

   /*** Target image spacing  */
   double               m_TargetSpacing[ImageDimension];

   /*** Target big endian flag  */
   bool                 m_TargetBigEndian;

   /*** Target image  */
   InputImagePointer    m_Target;

   /*** Normalized target image  */
   ImagePointer         m_NormalizedTarget;

   /*** Reference image file name   */
   std::string          m_ReferenceFileName;

   /*** Reference image size  */
   SizeType             m_ReferenceSize;

   /*** Reference image spacing  */
   double               m_ReferenceSpacing[ImageDimension];

   /*** Reference big endian flag  */
   bool                 m_ReferenceBigEndian;

   /*** Reference image  */
   InputImagePointer    m_Reference;

   /*** Normalized reference image  */
   ImagePointer         m_NormalizedReference;

   /*** Registered image filename  */
   std::string          m_RegisteredImageFileName;

   /*** Registered image big endian flag  */
   bool                 m_RegisteredImageBigEndian;

   /*** Registerd image  */
   InputImagePointer    m_RegisteredImage;

   /*** Number of computation levels  */
   unsigned int         m_NumberOfLevels;

   /*** Number of iterations to perform at each level  */
   itk::Array<unsigned int> m_NumberOfIterations;

   /*** Learning rate at each level  */
   itk::Array<double>       m_LearningRates;

   /*** Translation scaling at each level  */
   itk::Array<double>       m_TranslationScales;

   /*** Target starting shrink factors  */
   unsigned int         m_TargetShrinkFactors[ImageDimension];

   /*** Reference starting shrink factors  */
   unsigned int         m_ReferenceShrinkFactors[ImageDimension];

   /*** Dump pgm files flag  */
   bool                 m_DumpPGMFiles;

   /*** PGM directory  */
   std::string          m_PGMDirectory;

   /*** Read in an image  */
   bool ReadImage( const char *, const SizeType&, const double *, bool,
    InputImageType * );

   /*** Write an image out to file  */
   void WriteImage( InputImageType *, const char *, bool );

   /*** Normalize image to have mean of zero and variance of one.  */
   void NormalizeImage( InputImageType *, ImageType::Pointer & );

   /*** Initialize  */
   void Initialize();

   /*** Parse input file  */
   bool ParseInputFile( const char * );

   /*** Setup the registrator  */
   void SetUpRegistrator();

   /*** Transform reference image to form registered image  */
   void TransformReference();

   /*** Write PGM files  */
   bool WritePGMFiles( InputImageType *, const char *, const char * );
  
};

#endif
