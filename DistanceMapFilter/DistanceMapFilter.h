/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DistanceMapFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __DistanceMapFilter_H
#define __DistanceMapFilter_H

#include "DistanceMapFilterGUI.h"

class DistanceMapFilter : public DistanceMapFilterGUI 
{
public:
  /** Standard class typedefs. */
  typedef DistanceMapFilter      Self;
  typedef DistanceMapFilterGUI  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DistanceMapFilter, DistanceMapFilterGUI);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Image types */
  typedef DistanceMapFilterBase::ImageType            ImageType;
  typedef DistanceMapFilterBase::DistanceImageType    DistanceImageType;

  virtual void ComputeDistance(void);

protected:
  DistanceMapFilter();
  virtual ~DistanceMapFilter();

private:
  DistanceMapFilter( const DistanceMapFilter & );           // not defined 
  DistanceMapFilter operator=( const DistanceMapFilter & ); // not defined

};

#endif


