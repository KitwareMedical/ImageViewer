/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GradientRecursiveGaussianFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __GradientRecursiveGaussianFilter_H
#define __GradientRecursiveGaussianFilter_H

#include "GradientRecursiveGaussianFilterGUI.h"

class GradientRecursiveGaussianFilter : public GradientRecursiveGaussianFilterGUI 
{
public:
  /** Standard class typedefs. */
  typedef GradientRecursiveGaussianFilter     Self;
  typedef GradientRecursiveGaussianFilterGUI  Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GradientRecursiveGaussianFilter,GradientRecursiveGaussianFilterGUI);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Mesh class type */
  typedef GradientRecursiveGaussianFilterBase::ImageType          ImageType;
  typedef GradientRecursiveGaussianFilterBase::ImageGradientType  ImageGradientType;

protected:
  GradientRecursiveGaussianFilter();
  virtual ~GradientRecursiveGaussianFilter();

private:
  GradientRecursiveGaussianFilter( const GradientRecursiveGaussianFilter & );
  GradientRecursiveGaussianFilter 
          operator=( const GradientRecursiveGaussianFilter & );

};


  


#endif


