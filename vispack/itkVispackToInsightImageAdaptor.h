/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackToInsightImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkVispackToInsightImageAdaptor_h
#define __itkVispackToInsightImageAdaptor_h

#include <iostream.h>
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"
#include "image.h"

namespace itk {

template <class TOutputImage>
class ITK_EXPORT VispackToInsightImageAdaptor : public ImageSource<TOutputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef VispackToInsightImageAdaptor Self;

  /**
   * Standard Superclass typedef
   */
  typedef ImageSource<TOutputImage> Superclass;

  /**
   * Smart pointer typedef support
   */
  typedef SmartPointer<Self> Pointer;

  /**
   * Typename typedef for the output image scalar & vector pixel type
   */
  typedef typename TOutputImage::VectorValueType  TPixelVectorValueType;

  typedef typename TOutputImage::ScalarValueType  TPixelScalarValueType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VispackToInsightImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image
   */
  void SetInput(VISImage<TPixelScalarValueType>* img)
  {
    m_Input = img;
    this->Modified();
  }

  /**
   * Get the input image
   */
  VISImage<TPixelScalarValueType>* GetInput()
  {
    return m_Input;
  }

  /**
   *
   */
  virtual unsigned int GetHeight()
  {
    return m_Input->height();
  }

  /**
   *
   */
  virtual unsigned int GetWidth()
  {
    return m_Input->width();
  }
  
protected:
  VispackToInsightImageAdaptor() {}
  ~VispackToInsightImageAdaptor() {}
  VispackToInsightImageAdaptor(const VispackToInsightImageAdaptor&) {}
  void operator=(const VispackToInsightImageAdaptor&) {}
  void GenerateData();
  void GenerateOutputInformation();

private:
  VISImage<TPixelScalarValueType> *m_Input;
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVispackToInsightImageAdaptor.txx"
#endif


#endif
