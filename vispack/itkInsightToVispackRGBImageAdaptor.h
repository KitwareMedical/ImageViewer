/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkInsightToVispackRGBImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkInsightToVispackRGBPixelImageAdaptor_h
#define __itkInsightToVispackRGBPixelImageAdaptor_h

#include <iostream.h>
#include "itkImageWriter.h"
#include "itkImageRegionIterator.h"
#include "image.h"

namespace itk {

template <class TInputImage>
class ITK_EXPORT InsightToVispackRGBPixelImageAdaptor : public ImageWriter<TInputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef InsightToVispackRGBPixelImageAdaptor Self;

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
  typedef typename TInputImage::VectorValueType  TPixelVectorValueType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(InsightToVispackRGBPixelImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image.
   */
  void SetOutput(VISImageRGBPixelA *img)
  {
    m_Output = img;
    this->Modified();
  }

  /**
   * Get the output image.
   */
  VISImageRGBPixelA* GetOutput()
  {
    return m_Output;
  }
  
protected:
  InsightToVispackRGBPixelImageAdaptor() {}
  ~InsightToVispackRGBPixelImageAdaptor() {}
  InsightToVispackRGBPixelImageAdaptor(const InsightToVispackRGBPixelImageAdaptor&) {}
  void operator=(const InsightToVispackRGBPixelImageAdaptor&) {}
  void WriteData();

private:
  VISImageRGBPixelA *m_Output;
  
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInsightToVispackRGBPixelImageAdaptor.txx"
#endif


#endif
