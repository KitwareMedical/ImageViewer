/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSEquivalencyTableReader_h
#define __itkSWSEquivalencyTableReader_h

#include "itkWatershedEquivalencyTable.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{
/** \class EquivalencyTableReader
 * \brief An I/O object for reading EquivalencyTables
 * \sa EquivalencyTable
 * \sa EquivalencyTableWriter */
class EquivalencyTableReader : public ProcessObject
{
public:
  /** Standard class typedefs.  Defines smart pointer for this object. */
  typedef EquivalencyTableReader Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(EquivalencyTableReader, ProcessObject);

  typedef watershed::EquivalencyTable EquivalencyTableType;

  void SetInput(EquivalencyTableType *input)
    { this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(EquivalencyTableType *input)
    { this->ProcessObject::SetNthOutput(0, input); }

  EquivalencyTableType::Pointer GetInput()
    { return static_cast<EquivalencyTableType *>
              (this->ProcessObject::GetInput(0));    }
  
  EquivalencyTableType::Pointer GetOutput()
    { return static_cast<EquivalencyTableType *>
             (this->ProcessObject::GetOutput(0));    }

  /** Set the name of the file to read.  */
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }
  
protected:
  std::string m_FileName;
  EquivalencyTableReader()
    { 
      EquivalencyTableType::Pointer output = EquivalencyTableType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~EquivalencyTableReader() {}

  void  GenerateData();
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSEquivalencyTableReader.txx"
#endif

#endif

