/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWatershedFilterAndWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWatershedFilterAndWriter_txx
#define __itkWatershedFilterAndWriter_txx

#include "itkWatershedFilterAndWriter.h"
#include "itkImageRegionIterator.h"

namespace itk
{

template <class TInputImageType>
void
WatershedFilterAndWriter<TInputImageType>
::GenerateData()
{
  // First execute the superclass filter
  Superclass::GenerateData();

  // Now set up the writers for this data
  typedef typename Superclass::ScalarType ScalarType;
  typedef typename Superclass::OutputImageType OutputImageType;
  
  typename WatershedSegmentTreeWriter<ScalarType>::Pointer treeWriter
    = WatershedSegmentTreeWriter<ScalarType>::New();

  typename RawImageIO<unsigned long, TInputImageType::ImageDimension>::Pointer
    io = RawImageIO<unsigned long, TInputImageType::ImageDimension>::New();
  
  typename ImageFileWriter< OutputImageType >::Pointer writer
    = ImageFileWriter< OutputImageType >::New();
  
  io->SetFileDimensionality(TInputImageType::ImageDimension);
  io->SetByteOrderToLittleEndian();

  //
  // Crop the border of the basic segmentation.
  //
  typename  OutputImageType::Pointer output = OutputImageType::New();
  output->SetRegions(this->GetBasicSegmentation()->GetRequestedRegion());
  output->Allocate();
  
  ImageRegionIterator<OutputImageType> it_from(this->GetBasicSegmentation(), output->GetRequestedRegion());
  ImageRegionIterator<OutputImageType> it_to(output, output->GetRequestedRegion());

  for (; !it_to.IsAtEnd(); ++it_to, ++it_from)
    {      it_to.Set(it_from.Get());    }
 
  writer->SetImageIO(io);
  writer->SetFileName(this->m_SegmentationFileName.c_str());
  writer->SetInput(output);
  writer->Write();

  treeWriter->SetInput(this->GetSegmentTree());
  treeWriter->SetFileName(this->m_TreeFileName.c_str());
  treeWriter->Write();
}

  
}// end namespace itk

#endif
