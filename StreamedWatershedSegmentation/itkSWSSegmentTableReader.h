/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTableReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSSegmentTableReader_h
#define __itkSWSSegmentTableReader_h

#include "itkWatershedSegmentTable.h"
#include "itkProcessObject.h"
#include "itkSWSSegmentTableWriter.h"

namespace itk
{
namespace sws
{

/** \class SegmentTableReader
 * \brief A simple I/O class for reading SegmentTable objects from disk.
 * \sa SegmentTableWriter */
template <class TScalarType>
class SegmentTableReader : public ProcessObject
{
public:
  /** Standard itk typedefs and smart pointer definitions. */
  typedef SegmentTableReader Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(SegmentTableReader, ProcessObject);
  typedef SegmentTableWriter<TScalarType> SegmentTableWriter;

  /** Convenient typedefs. */
  typedef TScalarType ScalarType;
  typedef itk::watershed::SegmentTable<ScalarType> SegmentTableType;
  
  void SetOutput(SegmentTableType *input)
    { this->ProcessObject::SetNthOutput(0, input); }

  typename SegmentTableType::Pointer GetOutput()
    { return static_cast<SegmentTableType *>
             (this->ProcessObject::GetOutput(0).GetPointer());    }

  typedef typename SegmentTableWriter::record_t record_t;
  typedef typename SegmentTableWriter::edge_pair_t edge_pair_t;

  /** Sets the name of the file to read.   */
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

protected:
  std::string m_FileName;
  SegmentTableReader()
    {
      SegmentTableType::Pointer output = SegmentTableType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~SegmentTableReader() {}

  void  GenerateData();
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSSegmentTableReader.txx"
#endif

#endif

