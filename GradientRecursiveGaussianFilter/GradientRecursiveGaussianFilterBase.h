/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GradientRecursiveGaussianFilterBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __GradientRecursiveGaussianFilterBase_H
#define __GradientRecursiveGaussianFilterBase_H

#include "fltkDisplayGlWindowGUI.h"
#include "fltkSlice3DDrawer.h"
#include "fltkVectorImage3D.h"
#include "fltkClippingPlane3DDrawer.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCommand.h"
#include "itkGradientRecursiveGaussianImageFilter.h"

class GradientRecursiveGaussianFilterBase : public itk::Object 
{
public:
  /** Standard class typedefs. */
  typedef GradientRecursiveGaussianFilterBase      Self;
  typedef itk::Object  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GradientRecursiveGaussianFilterBase, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Image Dimension */
   enum { ImageDimension = 3 };

  /** Input Image Type */
   typedef itk::Image< unsigned short, ImageDimension >  ImageType;
   typedef ImageType::Pointer                           ImagePointer;

  /** Gradient Image Type */
   typedef itk::CovariantVector<float, ImageDimension>  GradientVectorType;
   typedef itk::Image<GradientVectorType,ImageDimension> ImageGradientType; 
   typedef ImageGradientType::Pointer                   ImageGradientPointer;

  /**  Gradient filter type  */
  typedef itk::GradientRecursiveGaussianImageFilter<
    ImageType,ImageGradientType,float>              GradientFilterType;
  typedef GradientFilterType::Pointer               GradientFilterPointer; 
  typedef   itk::ImageFileReader<ImageType>         VolumeReaderType;
  typedef   VolumeReaderType::Pointer               VolumeReaderPointer;

  /** Slice drawer. */
  typedef fltk::Slice3DDrawer< ImageType >          ImageSliceDrawerType;
  typedef ImageSliceDrawerType::Pointer             ImageSliceDrawerPointer;

  typedef fltk::VectorImage3D< ImageGradientType >  ImageGradientDrawerType;
  typedef ImageGradientDrawerType::Pointer          ImageGradientDrawerPointer;

protected:
  GradientRecursiveGaussianFilterBase();
  virtual ~GradientRecursiveGaussianFilterBase();

public:

  virtual void Load(void);
  virtual void Save(void) const;
  virtual void Load(const char * filename);
  virtual void Save(const char * filename) const;
  virtual void ShowDisplay(void);
  virtual void HideDisplay(void);
  virtual void ComputeGradient(void);


protected:

  VolumeReaderPointer                 m_Reader;
  ImageGradientPointer                m_ImageGradient;
  GradientFilterPointer               m_GradientFilter;

  ImageSliceDrawerPointer             m_ImageSliceDrawer;
  ImageGradientDrawerPointer          m_ImageGradientDrawer;

  fltkDisplayGlWindowGUI              m_Display;


private:
  GradientRecursiveGaussianFilterBase(
    const GradientRecursiveGaussianFilterBase & ); //dont define
  
  GradientRecursiveGaussianFilterBase 
  operator=(const GradientRecursiveGaussianFilterBase & ); //dont define
  
  bool                                m_ImageLoaded;

};

#endif


