/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

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
             (this->ProcessObject::GetInput(0));    }

  typename BoundaryDataType::Pointer GetOutput()
    {      return static_cast<BoundaryDataType *>
             (this->ProcessObject::GetOutput(0));    }

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

