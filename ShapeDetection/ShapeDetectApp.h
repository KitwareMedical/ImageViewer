/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ShapeDetectApp.h
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
#ifndef __ShapeDetectApp_h
#define __ShapeDetectApp_h

#include "itkPhysicalImage.h"
#include "itkCovariantVector.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include <string>


/**
 * \class ShapeDetectApp
 * \brief A simple level-set shape detection command line application
 *
 **/
class ShapeDetectApp
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
   * ImageType is the internal image type
   */
  typedef itk::Image<float,ImageDimension>
    ImageType;
  typedef ImageType::Pointer ImagePointer;

  /**
   * SizeType is the image size type
   */
  typedef InputImageType::SizeType SizeType;

  /**
   * IndexType is the image index type
   */
  typedef InputImageType::IndexType IndexType;

  /**
   * DerivativeImageType is the derivative image type
   */
  typedef itk::CovariantVector<float,ImageDimension> VectorType;
  typedef itk::Image<VectorType, ImageDimension>
    DerivativeImageType;

  /**
   * Derivative filter type
   */
  typedef itk::GradientRecursiveGaussianImageFilter<
    InputImageType,DerivativeImageType,double>  DerivativeFilterType;
  typedef DerivativeFilterType::Pointer DerivativeFilterPointer;

  /**
   * Shape Detection filter type
   */
  typedef itk::FastMarchingImageFilter<ImageType,ImageType> DetectionFilterType;
  typedef DetectionFilterType::Pointer DetectionFilterPointer;
    

  /**
   * Constructors
   */
  ShapeDetectApp();
  ShapeDetectApp( const char * );

  /**
   * Destructors
   */
  ~ShapeDetectApp(){};

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
    * Input big endian flag
    */
   bool                 m_InputBigEndian;

   /**
    * Input image
    */
   InputImagePointer    m_InputImage;

   /**
    * Dump pgm files flag
    */
   bool                 m_DumpPGMFiles;

   /**
    * PGM directory
    */
   std::string          m_PGMDirectory;

   /**
    * Seed point
    */
   IndexType            m_Seed;

   /**
    * Time crossing threshold
    */
   double               m_Threshold;

   /**
    * Sigma for computing derivative of gaussian
    */
   double               m_Sigma;

   /**
    * Edge potential map
    */
   ImagePointer         m_EdgePotentialImage;

   /**
    * Shape detection filter
    */
   DetectionFilterPointer  m_DetectionFilter;

   /**
    * Segmentation mask
    */
   ImagePointer          m_SegmentationMask;

   /**
    * Read in an image
    */
   bool ReadImage( const char *, const SizeType&, bool,
    InputImageType * );

   /**
    * Initialize
    */
   void Initialize();

   /**
    * Compute edge potential map
    */
   void ComputeEdgePotentialMap();

   /**
    * Compute time crossing map using fast marching
    */
   void ComputeTimeCrossingMap();

   /**
    * Threshold time crossing map
    */
   void ThresholdTimeCrossingMap();

   /**
    * Write segmentation mask
    */
  void WriteSegmentationImage();

  
};


#endif
