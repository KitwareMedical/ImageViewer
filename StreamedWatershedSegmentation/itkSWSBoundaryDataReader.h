/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSBoundaryDataReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSWSBoundaryDataReader_h
#define __itkSWSBoundaryDataReader_h

#include "itkWatershedBoundary.h"
#include "itkProcessObject.h"

namespace itk
{
namespace sws
{

/** This is a simple I/O object for reading Boundary data objects from disk.
 *
 * \sa watershed::BoundaryDataWriter
 * \sa watershed::Boundary */
template <class TScalarType, unsigned int TDimension>
class BoundaryDataReader : public ProcessObject
{
public:
  /** Convenient types and smart pointer definitions. */
  typedef BoundaryDataReader Self;
  typedef ProcessObject Superclass;
  typedef TScalarType ScalarType;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(BoundaryDataReader, ProcessObject);

  /** Expose the dimensionality template parameter at run time. */
  enum { Dimension =TDimension};

  /** Convenience typedef for the Boundary data type this filter
   * will read.   */
  typedef watershed::Boundary<ScalarType, Dimension> BoundaryDataType;
  
  void SetOutput(BoundaryDataType *input)
    {   this->ProcessObject::SetNthOutput(0, input); }
  typename BoundaryDataType::Pointer GetOutput()
    {      return static_cast<BoundaryDataType *>
             (this->ProcessObject::GetOutput(0).GetPointer());    }

  /** Set the name of the file to read.   */
  void SetFileName(const char *fn)
    {
      m_FileName = fn;
      this->Modified();
    }

protected:
  std::string m_FileName;
  BoundaryDataReader()
    { 
      BoundaryDataType::Pointer output = BoundaryDataType::New();
      this->ProcessObject::SetNumberOfRequiredOutputs(1);
      this->ProcessObject::SetNthOutput(0, output.GetPointer());
    }
  ~BoundaryDataReader() {}

  void  GenerateData();
};

}// end namespace sws
}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSWSBoundaryDataReader.txx"
#endif

#endif
