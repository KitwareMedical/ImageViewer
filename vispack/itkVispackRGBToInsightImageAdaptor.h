/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackRGBToInsightImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkVispackRGBPixelToInsightImageAdaptor_h
#define __itkVispackRGBPixelToInsightImageAdaptor_h

#include <iostream.h>
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"
#include "image.h"
#include "imageRGBPixelA.h"
#include "itkVispackToInsightImageAdaptor.h"
#include "itkVectorComponentDataAccessor.h"
#include "itkVector.h"

namespace itk {

template <class TOutputImage>
class ITK_EXPORT VispackRGBPixelToInsightImageAdaptor
  : public VispackToInsightImageAdaptor<TOutputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef VispackRGBPixelToInsightImageAdaptor Self;

  /**
   * Standard Superclass typedef
   */
  typedef VispackToInsightImageAdaptor<TOutputImage> Superclass;

  /**
   * Smart pointer typedef support
   */
  typedef SmartPointer<Self> Pointer;

  /**
   * Typename typedef for the output image scalar pixel type
   */
  typedef typename TOutputImage::ScalarValueType  TPixelScalarValueType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VispackRGBPixelToInsightImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image
   */
  void SetInput(VISImageRGBPixelA* img)
  {
    cout << "Setting input to " << img << endl;
    m_Input = img;
    this->Modified();
  }
  
  /**
   * Get the input image
   */
  VISImageRGBPixelA* GetInput()
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
  VispackRGBPixelToInsightImageAdaptor() {}
  ~VispackRGBPixelToInsightImageAdaptor() {}
  VispackRGBPixelToInsightImageAdaptor(const VispackRGBPixelToInsightImageAdaptor&) {}
  void operator=(const VispackRGBPixelToInsightImageAdaptor&) {}
  void GenerateData();

private:
  VISImageRGBPixelA *m_Input;
  
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVispackRGBPixelToInsightImageAdaptor.txx"
#endif


#endif
