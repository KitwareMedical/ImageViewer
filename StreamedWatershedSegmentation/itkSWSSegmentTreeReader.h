/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTreeReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTreeReader_h
#define __itkSWSSegmentTreeReader_h

#include "itkWatershedSegmentTree.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{

/** \class SegmentTreeReader
 * \brief A simple I/O object for reading SegmentTree data objects from disk.
 * \sa SegmentTreeWriter */
template <class TScalarType>
class SegmentTreeReader : public ProcessObject
{
public:
  /** Standard Itk typedefs and smart pointer declaration.   */
  typedef SegmentTreeReader Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(SegmentTreeReader, ProcessObject);

  /** Convenient typedefs. */
  typedef TScalarType ScalarType;
  typedef itk::watershed::SegmentTree<ScalarType> SegmentTreeType;
  
  void SetInput(SegmentTreeType *input)
    {   this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(SegmentTreeType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }

  typename SegmentTreeType::Pointer GetInput()
    {      return static_cast<SegmentTreeType *>
             (this->ProcessObject::GetInput(0).GetPointer());    }
  
  typename SegmentTreeType::Pointer GetOutput()
    {      return static_cast<SegmentTreeType *>
             (this->ProcessObject::GetOutput(0).GetPointer());    }

  /** Set the file name of the SegmentTree data to read.   */
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }
  
protected:
  std::string m_FileName;
  SegmentTreeReader()
    { 
      SegmentTreeType::Pointer output = SegmentTreeType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~SegmentTreeReader() {}

  void  GenerateData();
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSSegmentTreeReader.txx"
#endif

#endif

