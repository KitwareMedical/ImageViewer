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
#include <itkImageToImageTranslationMeanSquaresRegistration.h>
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

 
  //  Registration method to use
  typedef   itk::ImageToImageTranslationMeanSquaresRegistration<
                                                ReferenceType,
                                                TargetType> 
                                                  RegistrationMethodType;

  typedef   itk::FileIOToImageFilter< 
                            InputImageType >             ImageReaderType;

  typedef   itk::Vector< double, ImageDimension >   TransformationParametersType;

  typedef   itk::TranslationRegistrationTransform< 
                                      double,
                                      ImageDimension,
                                      TransformationParametersType 
                                      >                TransformationType;

  typedef   itk::ImageMapper< TargetType, 
                              TransformationType > MapperType;


  typedef   itk::SimpleImageRegionIterator< 
                                ReferenceType >    ReferenceIteratorType;


  typedef  TransformationType::PointType          PointType;


public:

  liImageRegistrationConsoleBase();
  virtual ~liImageRegistrationConsoleBase();
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void GenerateReference(void);
  virtual void GenerateMappedReference(void);

protected:

  ImageReaderType::Pointer               m_Reader;

  RegistrationMethodType::Pointer        m_RegistrationMethod;

  TargetType::Pointer                    m_TargetImage;

  ReferenceType::Pointer                 m_ReferenceImage;

  MappedReferenceType::Pointer           m_MappedReferenceImage;

  MapperType::Pointer                    m_ImageMapper;

  bool                                   m_ImageLoaded;


};



#endif
