/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liImageRegistrationConsoleBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/


#ifndef liIMAGEREGISTRATIONCONSOLEBASE
#define liIMAGEREGISTRATIONCONSOLEBASE

#include <itkPhysicalImage.h>
#include <itkPhysicalImageAdaptor.h>
#include <itkFileIOToImageFilter.h>
#include <itkWriteMetaImage.h>
#include <itkImageToImageAffineMeanSquaresRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffinePatternIntensityRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffineNormalizedCorrelationRegularStepGradientDescentRegistration.h>
#include <itkImageToImageAffineMutualInformationGradientDescentRegistration.h>
#include <itkDataAccessor.h>


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
  typedef   itk::PhysicalImage< InputPixelType, 
                                ImageDimension >          InputImageType;

  // Data Accessor to convert image pixels from 
  // the type in the file to the internal type
  typedef   itk::DataAccessor< InputPixelType, PixelType > DataAccessorType;                                


  // ImageAdaptor that will simulate that the input image
  // is an image with pixels of internal type
  typedef   itk::PhysicalImageAdaptor< InputImageType,
                                       DataAccessorType >   TargetType;
                                       
  // Type of the Reference image                                     
  typedef   itk::PhysicalImage< PixelType, 
                                ImageDimension >          ReferenceType;


   // Type of the Mapped Reference image
  typedef   itk::PhysicalImage< PixelType, 
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

  typedef   itk::Point< double, ImageDimension * (ImageDimension + 1) >   TransformationParametersType;

  typedef   itk::AffineRegistrationTransform< 
                                      double,
                                      ImageDimension,
                                      TransformationParametersType 
                                      >                TransformationType;

  typedef   itk::ImageMapper< TargetType, 
                              TransformationType > TargetMapperType;


  typedef   itk::ImageMapper< ReferenceType, 
                              TransformationType > ReferenceMapperType;


  typedef   itk::SimpleImageRegionIterator< 
                                ReferenceType >    ReferenceIteratorType;


  typedef  TransformationType::PointType          PointType;


  typedef  TransformationType::ParametersType      ParametersType;


public:

  liImageRegistrationConsoleBase();
  virtual ~liImageRegistrationConsoleBase();
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void Stop(void);
  virtual void GenerateReference(void);
  virtual void GenerateMappedReference(void);
  virtual void UpdateTransformationParameters(void);
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
