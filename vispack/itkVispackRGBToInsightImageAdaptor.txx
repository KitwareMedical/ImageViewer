/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackRGBToInsightImageAdaptor.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
namespace itk
{

template <class TOutputImage>
void 
VispackRGBToInsightImageAdaptor<TOutputImage>
::GenerateData()
{
  VectorComponentDataAccessor<Vector<TPixelVectorValueType, 3>,
    TPixelVectorValueType>  accessor; 
  
  cout << "VispackRGBToInsightImageAdaptor::GenerateData" << endl;
  typename TOutputImage::Pointer image=this->GetOutput(0);

  image->SetBufferedRegion( image->GetRequestedRegion() );
  cout << "RequestedRegion().Size()[0]=" <<
    image->GetRequestedRegion().GetSize()[0] << endl;

  cout << "RequestedRegion().Size()[1]=" <<
    image->GetRequestedRegion().GetSize()[1] << endl;
  image->Allocate();

  ImageRegionIterator<typename TOutputImage::PixelType,
    TOutputImage::ImageDimension>
    it(image, image->GetRequestedRegion());
  
  VISImageRGBA I = *(this->GetInput());
  
  VISImage<TPixelVectorValueType> Ir
    = (VISImage<TPixelVectorValueType>)I.channel(R);
  VISImage<TPixelVectorValueType> Ig
    = (VISImage<TPixelVectorValueType>)I.channel(G);
  VISImage<TPixelVectorValueType> Ib
    = (VISImage<TPixelVectorValueType>)I.channel(B);
  
  const TPixelVectorValueType* input   = Ir.rep()->buffer();
  accessor.SetVisibleComponent(0);
  for ( it = it.Begin(); !it.IsAtEnd(); ++it, ++input)
    {
      accessor.Set(*it, *input);
    }

  input   = Ig.rep()->buffer();
  accessor.SetVisibleComponent(1);
  for ( it = it.Begin(); !it.IsAtEnd(); ++it, ++input)
    {
      accessor.Set(*it, *input);
    }
  
  input   = Ib.rep()->buffer();
  accessor.SetVisibleComponent(2);
  for ( it = it.Begin(); !it.IsAtEnd(); ++it, ++input)
    {
      accessor.Set(*it, *input);
    }
}
} // end namespace itk
