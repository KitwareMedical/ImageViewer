/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWatershedFilterAndWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWatershedFilterAndWriter_h
#define __itkWatershedFilterAndWriter_h

#include "itkImageFileWriter.h"
#include "itkRawImageIO.h"
#include "itkImageToImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkWatershedSegmentTreeWriter.h"

namespace itk
{

/**
 * \class WatershedFilterAndWriter
 * This is a simple wrapper for the WatershedImageFilter which also writes
 * the segment tree and basic segmentation to disk.  This class is necessary
 * to wrap for vtk using the vtkITK protocol.
 */
template <class TInputImageType>
class WatershedFilterAndWriter : public WatershedImageFilter<TInputImageType>
{
public:
   /** Standard Itk typedefs and smart pointer declaration.   */
  typedef WatershedFilterAndWriter Self;
  typedef WatershedImageFilter<TInputImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(WatershedFilterAndWriter, WatershedImageFilter);

  itkSetStringMacro(TreeFileName);
  itkSetStringMacro(SegmentationFileName);
  
protected:
  std::string m_TreeFileName;
    std::string m_SegmentationFileName;
  WatershedFilterAndWriter()
  {
  }
  ~WatershedFilterAndWriter() {}

  void GenerateData();
};

}//end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWatershedFilterAndWriter.txx"
#endif

#endif
