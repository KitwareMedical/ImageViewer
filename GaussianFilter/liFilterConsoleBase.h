/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsoleBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/




#ifndef liFILTERCONSOLEBASE
#define liFILTERCONSOLEBASE

#include <itkPhysicalImage.h>
#include <itkFileIOToImageFilter.h>
#include <itkWriteMetaImage.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkFirstDerivativeRecursiveGaussianImageFilter.h>
#include <itkSecondDerivativeRecursiveGaussianImageFilter.h>
#include <itkTernaryAddImageFilter.h>
#include <itkTernaryMagnitudeImageFilter.h>


class liFilterConsoleBase 
{

public:

  typedef   unsigned short                    InputPixelType;
  typedef   float                             PixelType;
  typedef   float                             ComputationType;

  typedef   itk::PhysicalImage< InputPixelType, 3 >   InputImageType;
  typedef   itk::PhysicalImage< PixelType, 3 >        ImageType;

  typedef   itk::FileIOToImageFilter< 
                            InputImageType >       VolumeReaderType;

  typedef   itk::WriteMetaImage< 
                            ImageType >            VolumeWriterType;

  typedef   itk::RecursiveGaussianImageFilter<
                            InputImageType,
                            ImageType,
                            ComputationType  > InputGaussianFilterType;
  
  typedef   itk::RecursiveGaussianImageFilter<
                            ImageType,
                            ImageType,
                            ComputationType  > GaussianFilterType;

  typedef   itk::FirstDerivativeRecursiveGaussianImageFilter<
                            ImageType,
                            ImageType,
                            ComputationType  > 
                                GaussianFirstDerivativeFilterType;

  typedef   itk::SecondDerivativeRecursiveGaussianImageFilter<
                            ImageType,
                            ImageType,
                            ComputationType  > 
                               GaussianSecondDerivativeFilterType;

  typedef   itk::TernaryAddImageFilter< ImageType, ImageType,
                            ImageType, ImageType >  AddFilterType;


  typedef   itk::TernaryMagnitudeImageFilter< ImageType, ImageType,
                            ImageType, ImageType >  ModulusFilterType;



public:

  liFilterConsoleBase();
  virtual ~liFilterConsoleBase();
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( ComputationType );

  virtual void WriteLaplacian(const char * filename);
  virtual void WriteGradientX(const char * filename);
  virtual void WriteGradientY(const char * filename);
  virtual void WriteGradientZ(const char * filename);
  virtual void WriteGradientModulus(const char * filename);

protected:

  VolumeReaderType::Pointer                  m_Reader;

  InputGaussianFilterType::Pointer               m_Hx;
  InputGaussianFilterType::Pointer               m_Hy;
  InputGaussianFilterType::Pointer               m_Hz;

  GaussianFilterType::Pointer                    m_Hxy;
  GaussianFilterType::Pointer                    m_Hyz;
  GaussianFilterType::Pointer                    m_Hzx;  

  GaussianFirstDerivativeFilterType::Pointer     m_H1x;
  GaussianFirstDerivativeFilterType::Pointer     m_H1y;
  GaussianFirstDerivativeFilterType::Pointer     m_H1z;

  GaussianSecondDerivativeFilterType::Pointer    m_H2x;
  GaussianSecondDerivativeFilterType::Pointer    m_H2y;
  GaussianSecondDerivativeFilterType::Pointer    m_H2z;

  AddFilterType::Pointer                         m_Add;
  ModulusFilterType::Pointer                 m_Modulus;

  bool                                   m_ImageLoaded;

  VolumeWriterType::Pointer         m_Writer_Laplacian;
  VolumeWriterType::Pointer        m_Writer_Gradient_X;
  VolumeWriterType::Pointer        m_Writer_Gradient_Y;
  VolumeWriterType::Pointer        m_Writer_Gradient_Z;
  VolumeWriterType::Pointer   m_Writer_Gradient_Modulus;

};



#endif
