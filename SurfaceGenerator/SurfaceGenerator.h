/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SurfaceGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __SurfaceGenerator_H
#define __SurfaceGenerator_H

#include "SurfaceGeneratorGUI.h"

class SurfaceGenerator : public SurfaceGeneratorGUI 
{
public:
  /** Standard class typedefs. */
  typedef SurfaceGenerator      Self;
  typedef SurfaceGeneratorGUI  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SurfaceGenerator, SurfaceGeneratorGUI);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Mesh class type */
  typedef SurfaceGeneratorBase::MeshType MeshType;
  typedef MeshType::Pointer     MeshPointer;

protected:
  SurfaceGenerator();
  virtual ~SurfaceGenerator();

  void Load();
  void glDrawEllipsoid();

private:
  SurfaceGenerator( const SurfaceGenerator & );           // not defined 
  SurfaceGenerator operator=( const SurfaceGenerator & ); // not defined

};


  


#endif


