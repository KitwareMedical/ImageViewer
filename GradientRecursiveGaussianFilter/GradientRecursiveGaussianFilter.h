
#ifndef __GradientRecursiveGaussianFilter_H
#define __GradientRecursiveGaussianFilter_H


#include "GradientRecursiveGaussianFilterGUI.h"


class GradientRecursiveGaussianFilter : public GradientRecursiveGaussianFilterGUI {


public:
  /**
   * Standard "Self" typedef.
   */
  typedef GradientRecursiveGaussianFilter      Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef GradientRecursiveGaussianFilterGUI  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(GradientRecursiveGaussianFilter, GradientRecursiveGaussianFilterGUI);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

  /**
   * Mesh class type
   */
  typedef GradientRecursiveGaussianFilterBase::ImageType          ImageType;
  typedef GradientRecursiveGaussianFilterBase::ImageGradientType  ImageGradientType;


protected:

  GradientRecursiveGaussianFilter();
  virtual ~GradientRecursiveGaussianFilter();

  GradientRecursiveGaussianFilter( const GradientRecursiveGaussianFilter & );           // not defined 
  GradientRecursiveGaussianFilter operator=( const GradientRecursiveGaussianFilter & ); // not defined
  

};


  


#endif


