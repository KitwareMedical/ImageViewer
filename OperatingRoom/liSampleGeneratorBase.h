/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liSAMPLEGENERATORBASE
#define liSAMPLEGENERATORBASE

#include "liSample.h"
#include "itkPointSet.h"
#include <vector>

class liSampleGeneratorBase 
{
public:
  typedef  std::vector<liSample>             SamplesType;
  typedef  liSample::PointType               PointType;
  typedef  liSample::VectorType              VectorType;
  typedef  itk::DefaultStaticMeshTraits<double,
                      3,3,double,double>     MeshDefaultType;
  typedef  itk::PointSet<double,3,MeshDefaultType>  PointSetType;

  liSampleGeneratorBase();
  virtual ~liSampleGeneratorBase();
  virtual void GenerateSamples( SamplesType & samples );
  PointSetType::Pointer  GetPointSet(void);
    
  void Test(void) {
    m_PointSet->GetPoints();
  }

protected:
  void TransferSamplesToPointSet( const SamplesType & samples );
  void NormalizeSetOfSamples( SamplesType & samples );

private:  
  PointSetType::Pointer       m_PointSet;

};



#endif
