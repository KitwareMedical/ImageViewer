/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTableWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTableWriter_h
#define __itkSWSSegmentTableWriter_h

#include "itkWatershedSegmentTable.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{
/** \class SegmentTableWriter
 * \brief A simple I/O class for writing SegmentTable objects to disk.
 * \sa SegmentTableReader */
template <class TScalarType>
class SegmentTableWriter : public ProcessObject
{
public:
  /** Standard Itk typedefs and smart pointer declaration   */
  typedef SegmentTableWriter Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(SegmentTableWriter, ProcessObject);

  typedef TScalarType ScalarType;
  
  typedef itk::watershed::SegmentTable<ScalarType> SegmentTableType;
  
  void SetInput(SegmentTableType *input)
    {   this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(SegmentTableType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }

  typename SegmentTableType::Pointer GetInput()
    {      return static_cast<SegmentTableType *>
             (this->ProcessObject::GetInput(0));    }
  
  typename SegmentTableType::Pointer GetOutput()
    {      return static_cast<SegmentTableType *>
             (this->ProcessObject::GetOutput(0));    }

  struct record_t
  {
    unsigned long label;
    ScalarType    min;
    unsigned long edge_list_sz;
  };

  typedef typename SegmentTableType::edge_pair_t edge_pair_t;
  
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

  void  SafeWrite();

protected:
  std::string m_FileName;
  SegmentTableWriter()
    { 
      SegmentTableType::Pointer output = SegmentTableType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~SegmentTableWriter() {}

  void  GenerateData() {}
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSSegmentTableWriter.txx"
#endif

#endif

