/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataWriter.h
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
#ifndef __itkSWSBoundaryDataWriter_h
#define __itkSWSBoundaryDataWriter_h

#include "itkWatershedBoundary.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{

/** This is an I/O object for writing Boundary data objects to disk.
 *  \sa BoundaryDataReader
 *  \sa Boundary */
template <class TScalarType, unsigned int TDimension>
class BoundaryDataWriter : public ProcessObject
{
public:
  typedef BoundaryDataWriter Self;
  typedef ProcessObject Superclass;
  typedef TScalarType ScalarType;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(BoundaryDataWriter, ProcessObject);
  enum { Dimension =TDimension};

  typedef watershed::Boundary<ScalarType, Dimension> BoundaryDataType;

  void SetInput(BoundaryDataType *input)
    {   this->ProcessObject::SetNthInput(0, input); }

  void SetOutput(BoundaryDataType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }

  typename BoundaryDataType::Pointer GetInput()
    {      return static_cast<BoundaryDataType *>
             (this->ProcessObject::GetInput(0).GetPointer());    }

  typename BoundaryDataType::Pointer GetOutput()
    {      return static_cast<BoundaryDataType *>
             (this->ProcessObject::GetOutput(0).GetPointer());    }

  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

  /** This is the method that actually performs the data writing.  It is
   * public so that it can be called without causing the entire pipeline
   * to update and re-execute.
   *
   * A normal Itk Update() call on this filter will update the pipeline
   * and call this method.   */
  void SafeWrite();

protected:
  std::string m_FileName;
  BoundaryDataWriter()
    { 
      BoundaryDataType::Pointer output = BoundaryDataType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~BoundaryDataWriter() {}

  void  GenerateData() { this->SafeWrite(); }
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSBoundaryDataWriter.txx"
#endif

#endif

