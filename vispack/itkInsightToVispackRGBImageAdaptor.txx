/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkInsightToVispackRGBImageAdaptor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef _itkInsightToVispackRGBImageAdaptor_txx
#define _itkInsightToVispackRGBImageAdaptor_txx

#include "itkVectorComponentDataAccessor.h"

namespace itk
{
  
template <class TInputImage>
void 
InsightToVispackRGBImageAdaptor<TInputImage>
::WriteData()
{
  TPixelVectorValueType *outputIterator;
  const unsigned int w  =
    this->GetInput()->GetRequestedRegion().GetSize()[0]; 
  const unsigned int h =
    this->GetInput()->GetRequestedRegion().GetSize()[1];
  
  cout << "InsightToVispackRGBImageAdaptor::Writing data " << endl;

  VectorComponentDataAccessor<typename TInputImage::PixelType,
    TPixelVectorValueType> accessor;
  
  ImageRegionIterator<typename TInputImage::PixelType,
    TInputImage::ImageDimension> it(this->GetInput(),
                                 this->GetInput()->GetRequestedRegion());

  VISImage<TPixelVectorValueType> Ir(w, h);
  VISImage<TPixelVectorValueType> Ig(w, h);
  VISImage<TPixelVectorValueType> Ib(w, h);

  outputIterator = Ir.repRef()->bufferRef();
  accessor.SetVisibleComponent(0);
  for (it = it.Begin() ; !it.IsAtEnd(); ++it, ++outputIterator)
    {      *outputIterator = accessor.Get(*it);    }

  outputIterator = Ig.repRef()->bufferRef();
  accessor.SetVisibleComponent(1);
  for (it = it.Begin() ; !it.IsAtEnd(); ++it, ++outputIterator)
    {      *outputIterator = accessor.Get(*it);    }

  outputIterator = Ib.repRef()->bufferRef();
  accessor.SetVisibleComponent(2);
  for (it = it.Begin() ; !it.IsAtEnd(); ++it, ++outputIterator)
    {      *outputIterator = accessor.Get(*it);    }


  m_Output = new VISImageRGBA(w, h);
  m_Output->putColor((VISImage<byte>)Ir, R);
  m_Output->putColor((VISImage<byte>)Ig, G);
  m_Output->putColor((VISImage<byte>)Ib, B);
  m_Output->putColor((VISImage<byte>)Ir, A);
}

} // end namespace itk

#endif
