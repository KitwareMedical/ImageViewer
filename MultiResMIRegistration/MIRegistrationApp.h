/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MIRegistrationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __MIRegistrationApp_h
#define __MIRegistrationApp_h

#include "itkImage.h"
#include "itkMultiResolutionMutualInformationRigidRegistration.h"

#include <string>

/**
 * \class MIRegistrationApp
 * \brief A mutual information registration command line based application
 *
 **/
class MIRegistrationApp
{

public:

  /**
   * ImageDimension enumeration.
   */
  enum{ ImageDimension = 3 };

  /**
   * InputPixelType is the raw image pixel type.
   */
  typedef signed short InputPixelType;

  /**
   * PixelType is the internal image represenation pixel type.
   */
  typedef float PixelType;

  /**
   * InputImageType is the raw image type
   */
  typedef itk::Image<InputPixelType,ImageDimension>
    InputImageType;
  typedef InputImageType::Pointer InputImagePointer;

  /**
   * ImageType is the internal image type
   */
  typedef itk::Image<PixelType,ImageDimension>
    ImageType;
  typedef ImageType::Pointer ImagePointer;

  /**
   * SizeType is the image size type
   */
  typedef InputImageType::SizeType SizeType;

  /**
   * RegistratorType is the type of the registrator used
   */
  typedef itk::MultiResolutionMutualInformationRigidRegistration<
    ImageType, ImageType> RegistratorType;
  typedef RegistratorType::Pointer RegistratorPointer;

  /**
   * InternalRegistratorType is the type of the internal registrator used
   */
  typedef RegistratorType::RegistrationType InternalRegistratorType;

  /**
   * ParametersType is the type of the transform parameters
   */
  typedef InternalRegistratorType::ParametersType ParametersType;

  /**
   * MaxNumberOfLevels enumeration
   */
  enum { MaxNumberOfLevels = 8 };


  /**
   * Constructors
   */
  MIRegistrationApp();
  MIRegistrationApp( const char * );

  /**
   * Destructors
   */
  ~MIRegistrationApp(){};

  /**
   * Execute the application
   */
  void Execute();

  /**
   * Get the target image
   */
  InputImagePointer GetTarget() const
   { return m_Target; }

  /**
   * Get the reference image
   */
  InputImagePointer GetReference() const
    { return m_Reference; }

  /**
   * Get the registered image
   */
  InputImagePointer GetRegisteredImage() const
    { return m_RegisteredImage; }

  /**
   * Get the output parameters
   */
  const ParametersType& GetParameters() const
    { return m_Parameters; }



private:

   /**
    * The registrator
    */
   RegistratorPointer   m_Registrator;

   /**
    * The solution parameters
    */
   ParametersType       m_Parameters;

   /**
    * Target image file name 
    */
   std::string          m_TargetFileName;

   /**
    * Target image size
    */
   SizeType             m_TargetSize;

   /**
    * Target image spacing
    */
   double               m_TargetSpacing[ImageDimension];

   /**
    * Target big endian flag
    */
   bool                 m_TargetBigEndian;

   /**
    * Target image
    */
   InputImagePointer    m_Target;

   /**
    * Normalized target image
    */
   ImagePointer         m_NormalizedTarget;

   /**
    * Reference image file name 
    */
   std::string          m_ReferenceFileName;

   /**
    * Reference image size
    */
   SizeType             m_ReferenceSize;

   /**
    * Reference image spacing
    */
   double               m_ReferenceSpacing[ImageDimension];

   /**
    * Reference big endian flag
    */
   bool                 m_ReferenceBigEndian;

   /**
    * Reference image
    */
   InputImagePointer    m_Reference;

   /**
    * Normalized reference image
    */
   ImagePointer         m_NormalizedReference;

   /**
    * Registered image filename
    */
   std::string          m_RegisteredImageFileName;

   /**
    * Registered image big endian flag
    */
   bool                 m_RegisteredImageBigEndian;

   /**
    * Registerd image
    */
   InputImagePointer    m_RegisteredImage;

   /**
    * Number of computation levels
    */
   unsigned int         m_NumberOfLevels;

   /**
    * Number of iterations to perform at each level
    */
   unsigned int         m_NumberOfIterations[MaxNumberOfLevels];

   /**
    * Learning rate at each level
    */
   double               m_LearningRates[MaxNumberOfLevels];

   /**
    * Translation scaling at each level
    */
   double               m_TranslationScales[MaxNumberOfLevels];

   /**
    * Target starting shrink factors
    */
   unsigned int         m_TargetShrinkFactors[ImageDimension];

   /**
    * Reference starting shrink factors
    */
   unsigned int         m_ReferenceShrinkFactors[ImageDimension];

   /**
    * Dump pgm files flag
    */
   bool                 m_DumpPGMFiles;

   /**
    * PGM directory
    */
   std::string          m_PGMDirectory;

   /**
    * Read in an image
    */
   bool ReadImage( const char *, const SizeType&, const double *, bool,
    InputImageType * );

   /**
    * Write an image out to file
    */
   void WriteImage( InputImageType *, const char *, bool );

   /**
    * Normalize image to have mean of zero and variance of one
    */
   void NormalizeImage( InputImageType *, ImageType * );

   /**
    * Initialize
    */
   void Initialize();

   /**
    * Parse input file
    */
   bool ParseInputFile( const char * );

   /**
    * Setup the registrator
    */
   void SetUpRegistrator();

   /**
    * Transform reference image to form registered image
    */
   void TransformReference();

   /**
    * Write PGM files
    */
   bool WritePGMFiles( InputImageType *, const char *, const char * );

  
};


#endif
