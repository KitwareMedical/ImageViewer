/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CriticalPoint.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __CriticalPoint_H
#define __CriticalPoint_H

#include "itkVector.h"
#include "itkPoint.h"
#include "itkCovariantVector.h"

class CriticalPoint 
{
public:
  typedef itk::Point<double,3>              PointType;
  typedef itk::Vector<double,3>             VectorType;
  typedef itk::CovariantVector<double,3>    NormalType;
  typedef double                            CurvatureType;
  typedef unsigned long            IdentifierType;
    
public:
  CriticalPoint();
  virtual ~CriticalPoint();

  virtual void Draw(const PointType & position) const;

  IdentifierType GetSelfIdentifier(void) const 
    { return m_SelfIdentifier; }
  void SetSelfIdentifier( IdentifierType id )
    { m_SelfIdentifier = id;  }

  VectorType GetMaximumDirection( void)
    { return m_MaximumDirection; }
  void SetMaximumDirection( VectorType  maximumDirection )
    { m_MaximumDirection = maximumDirection; }

  VectorType GetMinimumDirection( void)
    { return m_MinimumDirection; }
  void SetMinimumDirection( VectorType  minimumDirection )
    { m_MinimumDirection = minimumDirection; }

  CurvatureType GetMaximumCurvature( void )
    { return m_MaximumCurvature; }
  void SetMaximumCurvature( CurvatureType maximumCurvature )
    { m_MaximumCurvature = maximumCurvature; }

  CurvatureType GetMinimumCurvature( void )
    { return m_MinimumCurvature; }
  void SetMinimumCurvature( CurvatureType minimumCurvature )
    { m_MinimumCurvature = minimumCurvature; }

  NormalType GetNormal ( void )
    { return m_Normal; }
  void SetNormal( NormalType normal)
    { m_Normal = normal; }

private:
  VectorType      m_MaximumDirection;
  VectorType      m_MinimumDirection;
  NormalType      m_Normal;
  CurvatureType   m_MaximumCurvature;
  CurvatureType   m_MinimumCurvature;
  IdentifierType  m_SelfIdentifier;

};

#endif


