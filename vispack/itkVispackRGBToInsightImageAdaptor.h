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
#ifndef __itkVispackRGBToInsightImageAdaptor_h
#define __itkVispackRGBToInsightImageAdaptor_h

#include <iostream.h>
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"
#include "image.h"
#include "imageRGBA.h"
#include "itkVispackToInsightImageAdaptor.h"
#include "itkVectorComponentDataAccessor.h"
#include "itkVector.h"

namespace itk {

template <class TOutputImage>
class ITK_EXPORT VispackRGBToInsightImageAdaptor
  : public VispackToInsightImageAdaptor<TOutputImage>
{
public:
  /**
   * Standard "Self" typdef.
   */
  typedef VispackRGBToInsightImageAdaptor Self;

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
  itkTypeMacro(VispackRGBToInsightImageAdaptor,ImageSource);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  /**
   * Set the output image
   */
  void SetInput(VISImageRGBA* img)
  {
    cout << "Setting input to " << img << endl;
    m_Input = img;
    this->Modified();
  }
  
  /**
   * Get the input image
   */
  VISImageRGBA* GetInput()
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
  VispackRGBToInsightImageAdaptor() {}
  ~VispackRGBToInsightImageAdaptor() {}
  VispackRGBToInsightImageAdaptor(const VispackRGBToInsightImageAdaptor&) {}
  void operator=(const VispackRGBToInsightImageAdaptor&) {}
  void GenerateData();

private:
  VISImageRGBA *m_Input;
  
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVispackRGBToInsightImageAdaptor.txx"
#endif


#endif
