/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSEquivalencyTableWriter_h
#define __itkSWSEquivalencyTableWriter_h

#include "itkWatershedEquivalencyTable.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{

/** \class EquivalencyTableWriter
 * \brief An I/O object for writing EquivalencyTables
 * \sa EquivalencyTable
 * \sa EquivalencyTableReader */
class EquivalencyTableWriter : public ProcessObject
{
public:
  typedef EquivalencyTableWriter Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(EquivalencyTableWriter, ProcessObject);

  typedef watershed::EquivalencyTable EquivalencyTableType;
  
  void SetInput(EquivalencyTableType *input)
    {   this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(EquivalencyTableType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }

  EquivalencyTableType::Pointer GetInput()
    {      return static_cast<EquivalencyTableType *>
             (this->ProcessObject::GetInput(0).GetPointer());    }
  
  EquivalencyTableType::Pointer GetOutput()
    {      return static_cast<EquivalencyTableType *>
             (this->ProcessObject::GetOutput(0).GetPointer());    }


  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

  void  SafeWrite();
protected:
  std::string m_FileName;
  EquivalencyTableWriter()
    { 
      EquivalencyTableType::Pointer output = EquivalencyTableType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~EquivalencyTableWriter() {}

  void  GenerateData() { this->SafeWrite(); }
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSEquivalencyTableWriter.txx"
#endif

#endif

