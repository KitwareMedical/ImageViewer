/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole3DBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef liFILTERCONSOLE3DBASE
#define liFILTERCONSOLE3DBASE

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkGradientAnisotropicDiffusionImageFilter.h>
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>

#include <itkAddImageFilter.h>
#include <itkBinaryMagnitudeImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

class liFilterConsole3DBase 
{

public:
  typedef   float                             InputPixelType;
  typedef   float                             PixelType;
  typedef   float                             OutputPixelType;
  typedef   unsigned char                     WritePixelType;

  typedef   float                             ComputationType;

  typedef   itk::Image< InputPixelType, 3 >   InputImageType;
  typedef   itk::Image< PixelType, 3 >        ImageType;
  typedef   itk::Image< OutputPixelType, 3 >  OutputImageType;
  typedef   itk::Image< WritePixelType, 3 >   WriteImageType;

  typedef   itk::ImageFileReader< 
                            InputImageType >  VolumeReaderType;

  typedef   itk::ImageFileWriter< 
                      WriteImageType >        VolumeWriterType;


  typedef itk::GradientAnisotropicDiffusionImageFilter<
                      InputImageType,
                      OutputImageType >       GradientAnisotropicDiffusionFilterType;

  typedef itk::CurvatureAnisotropicDiffusionImageFilter<
                      InputImageType,
                      OutputImageType >        CurvatureAnisotropicDiffusionFilterType;

  
  typedef   itk::RescaleIntensityImageFilter<
                      OutputImageType,
                      WriteImageType >        RescaleIntensityImageFilterType; 
                                 

public:

  liFilterConsole3DBase();
  virtual ~liFilterConsole3DBase();
  virtual void Load(void) = 0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void ShowGradientImage(void);
  virtual void ShowCurvatureImage(void);
  virtual void SetIterationsForGradient( double );
  virtual void SetConductanceForGradient( double );
  virtual void SetIterationsForCurvature( double );
  virtual void SetConductanceForCurvature( double );
  virtual void SaveGradientImage(void) = 0;
  virtual void SaveGradientImage(const char * filename);
  virtual void SaveCurvatureImage(void) = 0;
  virtual void SaveCurvatureImage(const char * filename);

protected:

  VolumeReaderType::Pointer                                      m_Reader;

  GradientAnisotropicDiffusionFilterType::Pointer                m_FilterG;

  CurvatureAnisotropicDiffusionFilterType::Pointer               m_FilterC;

  bool                                        m_ImageFileNameAvailable;

  RescaleIntensityImageFilterType::Pointer    m_Rescaler;  

  VolumeWriterType::Pointer                   m_Writer;

};



#endif
