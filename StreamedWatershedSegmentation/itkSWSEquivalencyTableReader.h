/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableReader.h
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
  /** Standard Itk pipeline typedefs.  Defines smart pointer for this object.
   */
  typedef EquivalencyTableReader Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(EquivalencyTableReader, ProcessObject);

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

