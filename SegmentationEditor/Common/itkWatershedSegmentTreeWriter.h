/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWatershedSegmentTreeWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWatershedSegmentTreeWriter_h
#define __itkWatershedSegmentTreeWriter_h

#include "itkWatershedSegmentTree.h"
#include "itkProcessObject.h"

namespace itk
{

/** \class SegmentTreeWriter
 * \brief A simple I/O object for writer SegmentTree data objects to disk.
 **/
template <class TScalarType>
class WatershedSegmentTreeWriter : public ProcessObject
{
public:
  /** Standard Itk typedefs and smart pointer declaration.   */
  typedef WatershedSegmentTreeWriter Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(WatershedSegmentTreeWriter, ProcessObject);

  /** Cnvenient typedefs. */
  typedef TScalarType ScalarType;
  typedef itk::watershed::SegmentTree<ScalarType> SegmentTreeType;
  
  void SetInput(SegmentTreeType *input)
    {   this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(SegmentTreeType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }

  typename SegmentTreeType::Pointer GetInput()
    {      return static_cast<SegmentTreeType *>
             (this->ProcessObject::GetInput(0));    }
  
  typename SegmentTreeType::Pointer GetOutput()
    {      return static_cast<SegmentTreeType *>
             (this->ProcessObject::GetOutput(0));    }

  /** Set the file name of the SegmentTree data to read.   */
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

    void  Write();

protected:
  std::string m_FileName;
  WatershedSegmentTreeWriter()
    { 
      SegmentTreeType::Pointer output = SegmentTreeType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~WatershedSegmentTreeWriter() {}

  void  GenerateData() { this->Write(); }
};

}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWatershedSegmentTreeWriter.txx"
#endif

#endif

