/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsole2DBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef liFILTERCONSOLE2DBASE
#define liFILTERCONSOLE2DBASE

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkBinaryMagnitudeImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

class liFilterConsole2DBase 
{

public:
  typedef   unsigned short                    InputPixelType;
  typedef   float                             PixelType;
  typedef   unsigned short                    OutputPixelType;

  typedef   float                             ComputationType;

  typedef   itk::Image< InputPixelType, 2 >   InputImageType;
  typedef   itk::Image< PixelType, 2 >        ImageType;
  typedef   itk::Image< OutputPixelType, 2 >  OutputImageType;

  typedef   itk::ImageFileReader< 
                            InputImageType >  VolumeReaderType;

  typedef   itk::ImageFileWriter< 
                      OutputImageType >       VolumeWriterType;


  typedef   itk::RecursiveGaussianImageFilter<
                                    InputImageType,
                                    ImageType
                                          > InputGaussianFilterType;

  typedef   itk::RecursiveGaussianImageFilter<
                                    ImageType,
                                    ImageType > GaussianFilterType;

  typedef   itk::AddImageFilter< ImageType, 
                                 ImageType, 
                                 ImageType >  AddFilterType;


  typedef   itk::BinaryMagnitudeImageFilter< 
                                 ImageType, 
                                 ImageType,
                                 ImageType >  ModulusFilterType;

                                 
  typedef   itk::RescaleIntensityImageFilter<
                               ImageType,
                               OutputImageType
                               >  RescaleIntensityImageFilterType; 
                                 


public:

  liFilterConsole2DBase();
  virtual ~liFilterConsole2DBase();
  virtual void Load(void) = 0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( ComputationType );
  virtual void SaveSmoothed(void) = 0;
  virtual void SaveLaplacian(void) = 0;
  virtual void SaveModulus(void) = 0;
  virtual void SaveSmoothed(const char * filename);
  virtual void SaveLaplacian(const char * filename);
  virtual void SaveModulus(const char * filename);

protected:

  VolumeReaderType::Pointer                       m_Reader;

  InputGaussianFilterType::Pointer                m_Hx;
  InputGaussianFilterType::Pointer                m_Hy;

  GaussianFilterType::Pointer                    m_Hxy;

  GaussianFilterType::Pointer                    m_H1x;
  GaussianFilterType::Pointer                    m_H1y;

  GaussianFilterType::Pointer                    m_H2x;
  GaussianFilterType::Pointer                    m_H2y;

  AddFilterType::Pointer                      m_Laplacian;
  GaussianFilterType::Pointer                 m_Smoothed;
  ModulusFilterType::Pointer                  m_Modulus;

  bool                                        m_ImageFileNameAvailable;

  RescaleIntensityImageFilterType::Pointer    m_RescaleIntensitySmoothed;  
  RescaleIntensityImageFilterType::Pointer    m_RescaleIntensityLaplacian;  
  RescaleIntensityImageFilterType::Pointer    m_RescaleIntensityModulus;  

  VolumeWriterType::Pointer                   m_WriterSmoothed;
  VolumeWriterType::Pointer                   m_WriterLaplacian;
  VolumeWriterType::Pointer                   m_WriterModulus;

};



#endif
