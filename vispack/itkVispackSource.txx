/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackSource.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVispackSource_txx
#define _itkVispackSource_txx

#include "itkVispackSource.h"
#include "itkObjectFactory.h"

namespace itk
{

/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
VispackSource<TPixel, VImageDimension>
::VispackSource()
{
  unsigned int idx;
  
  for (idx = 0; idx < VImageDimension; ++idx)
    {
    m_Spacing[idx] = 1.0;
    m_Origin[idx] = 0.0;
    }
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
VispackSource<TPixel, VImageDimension>
::PrintSelf(std::ostream& os, Indent indent)
{
  Superclass::PrintSelf(os,indent);

}

/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
VispackSource<TPixel, VImageDimension>
::SetInput(const VispackType &v)
{
  OutputImagePointer outputPtr = this->GetOutput();
  outputPtr->GetPixelContainer()->operator=(v);
  this->Modified();
}

/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
VispackSource<TPixel, VImageDimension>
::EnlargeOutputRequestedRegion(DataObject *output) 
{
  // call the superclass' implementation of this method
  Superclass::EnlargeOutputRequestedRegion(output);

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // set the requested region to the largest possible region (in this case
  // the amount of data that we have)
  outputPtr->SetRequestedRegion( outputPtr->GetLargestPossibleRegion() );
}


/** 
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
VispackSource<TPixel, VImageDimension>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // we need to compute the output spacing, the output origin, the
  // output image size, and the output image start index
  outputPtr->SetSpacing( m_Spacing );
  outputPtr->SetOrigin( m_Origin );
  outputPtr->SetLargestPossibleRegion( m_Region );
}


/**
 *
 */
template <class TPixel, unsigned int VImageDimension>
void 
VispackSource<TPixel, VImageDimension>
::GenerateData()
{
  // Normally, GenerateData() allocates memory.  However, the application
  // provides the memory for this filter via the SetImportPointer() method.
  // Therefore, this filter does not call outputPtr->Allocate().
  
  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // the output buffer size is set to the size specified by the user via the
  // SetRegion() method.
  outputPtr->SetBufferedRegion( outputPtr->GetLargestPossibleRegion() );
}


} // end namespace itk

#endif
