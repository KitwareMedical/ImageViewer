/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsoleBase.h
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


#ifndef liIMAGEREGISTRATIONCONSOLEBASE
#define liIMAGEREGISTRATIONCONSOLEBASE

#include <itkImage.h>
#include <itkImageAdaptor.h>
#include <itkFileIOToImageFilter.h>
#include <itkWriteMetaImage.h>
#include <itkImageToImageAffineMeanSquaresRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffinePatternIntensityRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffineNormalizedCorrelationRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffineMutualInformationGradientDescentRegistration.h>
#include <itkPixelAccessor.h>


/**
 * \brief ImageRegistrationConsoleBase class that instantiate
 * the elements required for a registration method without GUI
 *
 */

class liImageRegistrationConsoleBase
{

public:

  enum { ImageDimension = 3 };

  typedef enum {
    mutualInformation,
    normalizedCorrelation,
    patternIntensity,
    meanSquares
  } RegistrationMethodType;
  
  // Type of the image as it is read from a file
  typedef   unsigned short     InputPixelType;


  // Pixel type to be used internally during registration
  typedef   float              PixelType;

  
  // Type of the image to be read from the file
  typedef   itk::Image< InputPixelType, 
                                ImageDimension >          InputImageType;

  // Data Accessor to convert image pixels from 
  // the type in the file to the internal type
  typedef   itk::PixelAccessor< InputPixelType, PixelType > PixelAccessorType;                                


  // ImageAdaptor that will simulate that the input image
  // is an image with pixels of internal type
  typedef   itk::ImageAdaptor< InputImageType,
                                       PixelAccessorType >   TargetType;
                                       
  // Type of the Reference image                                     
  typedef   itk::Image< PixelType, 
                                ImageDimension >          ReferenceType;


   // Type of the Mapped Reference image
  typedef   itk::Image< PixelType, 
                                ImageDimension >      MappedReferenceType;

 
  //  Registration methods to use
  typedef   itk::ImageToImageAffineMeanSquaresRegularStepGradientDescentRegistration<
                                                ReferenceType,
                                                TargetType> 
                                                  MeanSquaresRegistrationMethodType;

  typedef   itk::ImageToImageAffineNormalizedCorrelationRegularStepGradientDescentRegistration<
                                                ReferenceType,
                                                TargetType> 
                                                  NormalizedCorrelationRegistrationMethodType;

  typedef   itk::ImageToImageAffinePatternIntensityRegularStepGradientDescentRegistration<
                                                ReferenceType,
                                                TargetType> 
                                                  PatternIntensityRegistrationMethodType;

  typedef   itk::ImageToImageAffineMutualInformationGradientDescentRegistration<
                                                ReferenceType,
                                                TargetType> 
                                                  MutualInformationRegistrationMethodType;

  typedef   itk::FileIOToImageFilter< 
                            InputImageType >             ImageReaderType;

  typedef   itk::Point< double, ImageDimension * (ImageDimension + 1) >   TransformParametersType;

  typedef   itk::AffineTransform< 
                                      double,
                                      ImageDimension,
                                      TransformParametersType 
                                      >                TransformType;

  typedef   itk::ImageMapper< TargetType, 
                              TransformType > TargetMapperType;


  typedef   itk::ImageMapper< ReferenceType, 
                              TransformType > ReferenceMapperType;


  typedef   itk::ImageRegionIteratorWithIndex< 
                                ReferenceType >    ReferenceIteratorType;


  typedef  TransformType::InputPointType          PointType;


  typedef  TransformType::ParametersType      ParametersType;


public:

  liImageRegistrationConsoleBase();
  virtual ~liImageRegistrationConsoleBase();
  virtual void Load(void)=0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void Stop(void);
  virtual void GenerateReference(void);
  virtual void GenerateMappedReference(void);
  virtual void UpdateTransformParameters(void);
  virtual void SelectRegistrationMethod( RegistrationMethodType method );

protected:

  ImageReaderType::Pointer               m_Reader;

  MeanSquaresRegistrationMethodType::Pointer
                                                  m_MeansSquaresMethod;
  
  MutualInformationRegistrationMethodType::Pointer
                                                  m_MutualInformationMethod;

  PatternIntensityRegistrationMethodType::Pointer
                                                  m_PatternIntensityMethod;
  
  NormalizedCorrelationRegistrationMethodType::Pointer
                                                  m_NormalizedCorrelationMethod;
  
  TargetType::Pointer                             m_TargetImage;

  ReferenceType::Pointer                          m_ReferenceImage;

  MappedReferenceType::Pointer                    m_MappedReferenceImage;

  TargetMapperType::Pointer                       m_TargetMapper;

  ReferenceMapperType::Pointer                    m_ReferenceMapper;

  bool                                            m_ImageLoaded;

  RegistrationMethodType                          m_SelectedMethod;

};



#endif
