/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkInsightToVispackImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkInsightToVispackImageAdaptor_h
#define __itkInsightToVispackImageAdaptor_h

#include <iostream.h>
#include "itkWriteImage.h"
#include "itkImageRegionIterator.h"
#include "image.h"

namespace itk {

template <class TInputImage>
class ITK_EXPORT InsightToVispackImageAdaptor : public WriteImage<TInputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef InsightToVispackImageAdaptor Self;

  /**
   * Standard Superclass typedef
   */
  typedef ImageSource<TInputImage> Superclass;

  /**
   * Smart pointer typedef support
   */
  typedef SmartPointer<Self> Pointer;

  /**
   * Typename typedef for the output image scalar pixel type
   */
  typedef typename TInputImage::ScalarValueType  TPixelScalarValueType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(InsightToVispackImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image.
   */
  void SetOutput(VISImage<TPixelScalarValueType> *img)
  {
    m_Output = img;
    this->Modified();
  }

  /**
   * Get the output image.
   */
  VISImage<TPixelScalarValueType>* GetOutput()
  {
    return m_Output;
  }
  
protected:
  InsightToVispackImageAdaptor() {}
  ~InsightToVispackImageAdaptor() {}
  InsightToVispackImageAdaptor(const InsightToVispackImageAdaptor&) {}
  void operator=(const InsightToVispackImageAdaptor&) {}
  void WriteData();

private:
  VISImage<TPixelScalarValueType> *m_Output;
  
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInsightToVispackImageAdaptor.txx"
#endif


#endif
