/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liFilterConsoleBase.h
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




#ifndef liFILTERCONSOLEBASE
#define liFILTERCONSOLEBASE

#include <itkImage.h>
#include <itkImageFileReader.h>
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

  typedef   itk::Image< InputPixelType, 3 >   InputImageType;
  typedef   itk::Image< PixelType, 3 >        ImageType;

  typedef   itk::ImageFileReader< 
                            InputImageType >       VolumeReaderType;

//  typedef   itk::WriteMetaImage< 
//                            ImageType >            VolumeWriterType;

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
  virtual void Load(void) = 0;
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( ComputationType );

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

  GaussianFirstDerivativeFilterType::Pointer     m_H1x;
  GaussianFirstDerivativeFilterType::Pointer     m_H1y;
  GaussianFirstDerivativeFilterType::Pointer     m_H1z;

  GaussianSecondDerivativeFilterType::Pointer    m_H2x;
  GaussianSecondDerivativeFilterType::Pointer    m_H2y;
  GaussianSecondDerivativeFilterType::Pointer    m_H2z;

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
