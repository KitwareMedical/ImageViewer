/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    EnergyFunction.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __ENERGYFUNCTION_TXX
#define __ENERGYFUNCTION_TXX

#include "EnergyFunction.h"

template<class TImage, class TImageMask>
EnergyFunction<TImage, TImageMask>
::EnergyFunction(std::vector<double> classMeans, 
                 std::vector<double> classSigmas)
{
  m_Image = 0 ;
  m_Mask = 0 ;
  m_BiasField = 0 ;
  m_SliceNo = -1 ;
  m_EnergyTable = new EnergyTable(classMeans, classSigmas) ;
}

template<class TImage, class TImageMask>
EnergyFunction<TImage, TImageMask>
::~EnergyFunction()
{
  delete m_EnergyTable ;
  m_EnergyTable = 0 ;
}

template<class TImage, class TImageMask>
void
EnergyFunction<TImage, TImageMask>
::SetImage(ImagePointer image)
{
  m_Image = image ;
}


template<class TImage, class TImageMask>
void
EnergyFunction<TImage, TImageMask>
::SetMask(MaskPointer mask)
{
  m_Mask = mask ;
}

template<class TImage, class TImageMask>
void
EnergyFunction<TImage, TImageMask>
::SetSliceNo(int sliceNo)
{
  m_SliceNo = sliceNo ;
}

template<class TImage, class TImageMask>
void
EnergyFunction<TImage, TImageMask>
::SetBiasField(BiasField* biasField)
{
  m_BiasField = biasField ;
}



template<class TImage, class TImageMask>
EnergyFunction<TImage, TImageMask>::MeasureType
EnergyFunction<TImage, TImageMask>
::GetValue(ParametersType parameters, MeasureType& ret) 
{

  if (m_Image == 0 || m_EnergyTable == 0 || m_BiasField == 0)
    exit(0) ;

  double  total = 0.0;
  
  typename TImage::RegionType region = m_Image->GetLargestPossibleRegion() ;

  typename TImage::IndexType index = region.GetIndex() ;
  typename TImage::SizeType size = region.GetSize() ;

  if (TImage::ImageDimension == 3 && m_SliceNo > -1)
    {
      index[2] = m_SliceNo ;
      size[2] = 1 ;

      region.SetSize(size) ;
      region.SetIndex(index) ;
    }

  itk::ImageRegionIteratorWithIndex<ImageType> iIter(m_Image, region) ;

  m_BiasField->SetCoefficients(parameters) ;
  //dump(m_BiasField->GetCoefficients()) ;
  BiasField::SimpleForwardIterator bIter(m_BiasField) ;
  bIter.Begin() ;

  if (m_Mask->GetLargestPossibleRegion().GetSize() != 
      m_Image->GetLargestPossibleRegion().GetSize())
    {
      while (!iIter.IsAtEnd())
        {
          total += m_EnergyTable->GetEnergy0(iIter.Get() - bIter.Get());
          ++iIter ;
          ++bIter ;
        }
    }
  else
    {
      itk::ImageRegionIteratorWithIndex<MaskType> 
        mIter(m_Mask, region) ;
      mIter.Begin() ;
      while (!iIter.IsAtEnd())
        {
          if (mIter.Get() > 0.0) 
            {
              total += m_EnergyTable->GetEnergy0(iIter.Get() - bIter.Get());
            }
          ++iIter ;
          ++bIter;
          ++mIter ;
        }
    }

  return ret = total ;
}

#endif
