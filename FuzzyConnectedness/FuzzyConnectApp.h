/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FuzzyConnectApp.h
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
#ifndef __FuzzyConnectApp_h
#define __FuzzyConnectApp_h

#include "itkImage.h"
#include "itkFuzzyConnectednessImageFilter.h"
#include <string>

/**
 * \class FuzzyConnectApp
 * \brief A simple fuzzy connectedness based object delineation command 
 *        line application
 *
 **/
class FuzzyConnectApp
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
   * InputImageType is the raw image type
   */
  typedef itk::Image<InputPixelType,ImageDimension>
    InputImageType;
  typedef InputImageType::Pointer InputImagePointer;

  /**
   * BinaryImageType is the binary image type
   */
  typedef itk::Image<bool,ImageDimension>
    BinaryImageType;
  typedef BinaryImageType::Pointer BinaryImagePointer;

  /**
   * SizeType is the image size type
   */
  typedef InputImageType::SizeType SizeType;

  /**
   * IndexType is the image index type
   */
  typedef InputImageType::IndexType IndexType;

  /**
   * Fuzzy connectedness filter type
   */
  typedef itk::FuzzyConnectednessImageFilter<
    InputImageType,InputImageType>  FilterType;
  typedef FilterType::Pointer FilterPointer;

  /**
   * Constructors
   */
  FuzzyConnectApp();
  FuzzyConnectApp( const char * );

  /**
   * Destructors
   */
  ~FuzzyConnectApp(){};

  /**
   * Execute the application
   */
  void Execute();

  /**
   * Get the target image
   */
  InputImagePointer GetInputImage() const
   { return m_InputImage; }



private:

   /**
    * Input image file name 
    */
   std::string          m_InputFileName;

   /**
    * Input image size
    */
   SizeType             m_InputSize;

   /**
    * Input image spacing
    */
   double               m_InputSpacing[ImageDimension];

   /**
    * Input big endian flag
    */
   bool                 m_InputBigEndian;

   /**
    * Input image
    */
   InputImagePointer    m_InputImage;


   /**
    * Fuzzy connectedness filter
    */
   FilterPointer        m_Filter;

   /**
    * The seed
    */
   IndexType            m_Seed;

   /**
    * Threshold value
    */
   double               m_Threshold;

   /**
    * Parameters
    */
   double               m_ObjectMean;
   double               m_ObjectVariance;
   double               m_DiffMean;
   double               m_DiffVariance;
   double               m_Weight;

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
    * Compute the fuzzy connectedness map
    */
   void ComputeMap();

   /**
    * Compute segmentation image
    */
   void ComputeSegmentationImage();

   /**
    * Write out segmentation image
    */
   void WriteSegmentationImage();

   /**
    * Initialize
    */
   void Initialize();

   /**
    * Write PGM files
    */
   bool WritePGMFiles(InputImageType *, const char *, const char * );

  
};


#endif
