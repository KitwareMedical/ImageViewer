/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsoleBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef liFILTERCONSOLEBASE
#define liFILTERCONSOLEBASE

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkTernaryAddImageFilter.h>
#include <itkTernaryMagnitudeImageFilter.h>

class liFilterConsoleBase 
{

public:
  typedef   unsigned short                    InputPixelType;
  typedef   float                             PixelType;

  typedef   itk::Image< InputPixelType, 3 >   InputImageType;
  typedef   itk::Image< PixelType, 3 >        ImageType;

  typedef   itk::ImageFileReader< 
                            InputImageType >       VolumeReaderType;

//  typedef   itk::WriteMetaImage< 
//                            ImageType >            VolumeWriterType;

  typedef   itk::RecursiveGaussianImageFilter<
                                    InputImageType,
                                    ImageType > InputGaussianFilterType;
  
  typedef   itk::RecursiveGaussianImageFilter<
                                    ImageType,
                                    ImageType > GaussianFilterType;

  typedef   itk::TernaryAddImageFilter< ImageType, ImageType,
                            ImageType, ImageType >  AddFilterType;


  typedef   itk::TernaryMagnitudeImageFilter< ImageType, ImageType,
                            ImageType, ImageType >  ModulusFilterType;


  typedef   GaussianFilterType::RealType    RealType;

public:

  liFilterConsoleBase();
  virtual ~liFilterConsoleBase();
  virtual void Load(void) = 0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( RealType );

  virtual void WriteLaplacian(void) = 0;
  virtual void WriteGradientX(void) = 0;
  virtual void WriteGradientY(void) = 0;
  virtual void WriteGradientZ(void) = 0;
  virtual void WriteGradientModulus(void) = 0;

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

  GaussianFilterType::Pointer                    m_H1x;
  GaussianFilterType::Pointer                    m_H1y;
  GaussianFilterType::Pointer                    m_H1z;

  GaussianFilterType::Pointer                    m_H2x;
  GaussianFilterType::Pointer                    m_H2y;
  GaussianFilterType::Pointer                    m_H2z;

  AddFilterType::Pointer                         m_Add;
  ModulusFilterType::Pointer                 m_Modulus;

  bool                                   m_ImageLoaded;

  /*
  VolumeWriterType::Pointer         m_Writer_Laplacian;
  VolumeWriterType::Pointer        m_Writer_Gradient_X;
  VolumeWriterType::Pointer        m_Writer_Gradient_Y;
  VolumeWriterType::Pointer        m_Writer_Gradient_Z;
  VolumeWriterType::Pointer   m_Writer_Gradient_Modulus;
  */

};



#endif
