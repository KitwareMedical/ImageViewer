/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkInsightToVispackImageAdaptor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
namespace itk
{

template <class TInputImage>
void 
InsightToVispackImageAdaptor<TInputImage>
::WriteData()
{
  cout << "Writing data " << endl;
  ImageRegionIterator<typename TInputImage::PixelType,
    TInputImage::ImageDimension> it(this->GetInput(),
                                 this->GetInput()->GetRequestedRegion());

  TPixelScalarValueType* outputIterator
    = this->GetOutput()->repRef()->bufferRef();
  
  for (it.Begin() ; !it.IsAtEnd(); ++it, ++outputIterator)
    {
      *outputIterator =  ScalarTraits<typename TInputImage::PixelType>
        ::GetScalar( *it );
    }
}

} // end namespace itk
