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
#include <itkRecursiveGaussianImageFilter.h>
#include <itkFirstDerivativeRecursiveGaussianImageFilter.h>
#include <itkSecondDerivativeRecursiveGaussianImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkBinaryMagnitudeImageFilter.h>

class liFilterConsole2DBase 
{

public:
  typedef   unsigned short                    InputPixelType;
  typedef   float                             PixelType;
  typedef   float                             ComputationType;

  typedef   itk::Image< InputPixelType, 2 >   InputImageType;
  typedef   itk::Image< PixelType, 2 >        ImageType;

  typedef   itk::ImageFileReader< 
                            InputImageType >       VolumeReaderType;


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

  typedef   itk::AddImageFilter< ImageType, 
                                 ImageType, 
                                 ImageType >  AddFilterType;


  typedef   itk::BinaryMagnitudeImageFilter< 
                                 ImageType, 
                                 ImageType,
                                 ImageType >  ModulusFilterType;



public:

  liFilterConsole2DBase();
  virtual ~liFilterConsole2DBase();
  virtual void Load(void) = 0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( ComputationType );

protected:

  VolumeReaderType::Pointer                   m_Reader;

  InputGaussianFilterType::Pointer                m_Hx;
  InputGaussianFilterType::Pointer                m_Hy;

  GaussianFilterType::Pointer                    m_Hxy;

  GaussianFirstDerivativeFilterType::Pointer     m_H1x;
  GaussianFirstDerivativeFilterType::Pointer     m_H1y;

  GaussianSecondDerivativeFilterType::Pointer    m_H2x;
  GaussianSecondDerivativeFilterType::Pointer    m_H2y;

  AddFilterType::Pointer                         m_Add;
  ModulusFilterType::Pointer                 m_Modulus;

  bool                                   m_ImageLoaded;

};



#endif
