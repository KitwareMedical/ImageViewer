/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackToInsightImageAdaptor.txx
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
VispackToInsightImageAdaptor<TOutputImage>
::GenerateOutputInformation()
{
  cout << "VispackToInsightImageAdaptor::GenerateOutputInformation" << endl;
  cout << "input has address " << this->GetInput() << endl;
  TOutputImage *output;
  typename TOutputImage::Index index = {0};
  typename TOutputImage::Size size;
  float m_Spacing[2];
  float m_Origin[2];
  size[0] = this->GetWidth();
  size[1] = this->GetHeight();  

  cout << "Size[0] = " << size[0] << endl;
  cout << "Size[1] = " << size[1] << endl;
  
  output = this->GetOutput(0);

  typename TOutputImage::Region largestPossibleRegion;
  largestPossibleRegion.SetSize( size );
  largestPossibleRegion.SetIndex( index );
  output->SetLargestPossibleRegion( largestPossibleRegion );

  for (int i=0; i<TOutputImage::GetImageDimension(); i++)
    {
      m_Spacing[i] = 1.0;
      m_Origin[i] = 0.0;
    }

  output->SetSpacing(m_Spacing);
  output->SetOrigin(m_Origin);
  
}
  
template <class TOutputImage>
void 
VispackToInsightImageAdaptor<TOutputImage>
::GenerateData()
{
  cout << "VispackToInsightImageAdaptor::GenerateData" << endl;
  typename TOutputImage::Pointer image=this->GetOutput(0);

  image->SetBufferedRegion( image->GetRequestedRegion() );
  cout << "RequestedRegion().Size()[0]=" <<
    image->GetRequestedRegion().GetSize()[0] << endl;

  cout << "RequestedRegion().Size()[1]=" <<
    image->GetRequestedRegion().GetSize()[1] << endl;
  image->Allocate();

  ImageRegionIterator<typename TOutputImage::PixelType,
    TOutputImage::ImageDimension>
    scalarIterator(image, image->GetRequestedRegion());

  const TPixelScalarValueType* inputIterator
    = this->GetInput()->rep()->buffer();
  for ( ; !scalarIterator.IsAtEnd(); ++scalarIterator, ++inputIterator)
    {
      //      ScalarTraits<typename TOutputImage::PixelType>::SetScalar(
      //                                        *scalarIterator,
      //                                        *inputIterator);
      *scalarIterator = *inputIterator;
    }
}
} // end namespace itk
