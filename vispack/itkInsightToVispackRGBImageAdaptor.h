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
#ifndef __itkInsightToVispackRGBImageAdaptor_h
#define __itkInsightToVispackRGBImageAdaptor_h

#include <iostream.h>
#include "itkWriteImage.h"
#include "itkImageRegionIterator.h"
#include "image.h"

namespace itk {

template <class TInputImage>
class ITK_EXPORT InsightToVispackRGBImageAdaptor : public WriteImage<TInputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef InsightToVispackRGBImageAdaptor Self;

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
  itkTypeMacro(InsightToVispackRGBImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image.
   */
  void SetOutput(VISImageRGBA *img)
  {
    m_Output = img;
    this->Modified();
  }

  /**
   * Get the output image.
   */
  VISImageRGBA* GetOutput()
  {
    return m_Output;
  }
  
protected:
  InsightToVispackRGBImageAdaptor() {}
  ~InsightToVispackRGBImageAdaptor() {}
  InsightToVispackRGBImageAdaptor(const InsightToVispackRGBImageAdaptor&) {}
  void operator=(const InsightToVispackRGBImageAdaptor&) {}
  void WriteData();

private:
  VISImageRGBA *m_Output;
  
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkInsightToVispackRGBImageAdaptor.txx"
#endif


#endif
