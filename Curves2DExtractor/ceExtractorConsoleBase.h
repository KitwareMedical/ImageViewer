/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ceExtractorConsoleBase.h
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




#ifndef ce_FilterConsoleBase_h
#define ce_FilterConsoleBase_h

#include <itkImage.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkFirstDerivativeRecursiveGaussianImageFilter.h>
#include <itkSecondDerivativeRecursiveGaussianImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkBinaryMagnitudeImageFilter.h>
#include <itkEigenAnalysis2DImageFilter.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkImageToParametricSpaceFilter.h>
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkJoinImageFilter.h>


class ceExtractorConsoleBase 
{

public:

  typedef   double                            InputPixelType;
  typedef   double                            PixelType;
  typedef   double                            ComputationType;

  typedef   itk::Vector< PixelType, 2 >           VectorType;
  
  typedef   itk::CovariantVector< PixelType, 2 >  CovariantVectorType;

  typedef   itk::Image< InputPixelType, 2 >       InputImageType;
  typedef   itk::Image< PixelType, 2 >            ImageType;
  typedef   itk::Image< VectorType, 2 >           VectorImageType;
  typedef   itk::Image< CovariantVectorType, 2 >  CovariantVectorImageType;

  typedef   itk::Mesh< int, 3 >                   MeshType;
    
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
                            ImageType, ImageType >  AddFilterType;


  typedef   itk::BinaryMagnitudeImageFilter< ImageType, 
                            ImageType, ImageType >  ModulusFilterType;

  typedef   itk::EigenAnalysis2DImageFilter< ImageType, 
                            ImageType, VectorImageType >  EigenFilterType;

  typedef   itk::GradientRecursiveGaussianImageFilter< 
                            InputImageType,
                            CovariantVectorImageType, 
                            ComputationType >           GradientFilterType;

  typedef   itk::MultiplyImageFilter< VectorImageType,
                                      VectorImageType,
                                      ImageType >  ScalarProductFilterType;

  typedef   itk::ImageToParametricSpaceFilter< ImageType, MeshType >
                                                  ParametricSpaceFilterType;

  typedef   itk::JoinImageFilter< ImageType, ImageType >      JoinFilterType;


public:

  ceExtractorConsoleBase();
  virtual ~ceExtractorConsoleBase();
  virtual void Load(const char * filename);
  virtual void ShowProgress(float);
  virtual void ShowStatus(const char * text);
  virtual void Execute(void);
  virtual void SetSigma( ComputationType );


protected:

  VolumeReaderType::Pointer                  m_Reader;

  InputGaussianFilterType::Pointer               m_Hx;
  InputGaussianFilterType::Pointer               m_Hy;

  GaussianFilterType::Pointer                    m_Hxy;
  GaussianFilterType::Pointer                    m_H1xy;

  GaussianFirstDerivativeFilterType::Pointer     m_H1x;
  GaussianFirstDerivativeFilterType::Pointer     m_H1y;

  GaussianSecondDerivativeFilterType::Pointer    m_H2x;
  GaussianSecondDerivativeFilterType::Pointer    m_H2y;

  AddFilterType::Pointer                         m_Add;
  ModulusFilterType::Pointer                 m_Modulus;

  EigenFilterType::Pointer                     m_Eigen;

  GradientFilterType::Pointer               m_Gradient;

  ScalarProductFilterType::Pointer     m_ScalarProduct;

  JoinFilterType::Pointer                       m_Join;
    
  ParametricSpaceFilterType::Pointer m_ParametricSpace;


  bool                                   m_ImageLoaded;

};



#endif
