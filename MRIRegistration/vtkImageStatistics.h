/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkImageStatistics.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// .NAME vtkImageStatistics -  Compute image statististics
// .SECTION Description
// vtkImageStatistics computes min, max, mean and standrad deviation
// of an image.

#ifndef __vtkImageStatistics_h
#define __vtkImageStatistics_h


#include "vtkImageToImageFilter.h"

class vtkImageStatistics : public vtkImageToImageFilter
{
public:
  static vtkImageStatistics *New();
  vtkTypeMacro(vtkImageStatistics,vtkImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  double MinValue;
  double MaxValue;
  double MeanValue;
  double StandardDeviationValue;

  // Get min, max, & mean
  vtkGetMacro(MinValue,double);
  vtkGetMacro(MaxValue,double);
  vtkGetMacro(MeanValue,double);
  vtkGetMacro(StandardDeviationValue,double);

protected:
  vtkImageStatistics();
  ~vtkImageStatistics() {};
  vtkImageStatistics(const vtkImageStatistics&) {};
  void operator=(const vtkImageStatistics&) {};

  void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);
  void ExecuteData (vtkDataObject *);
};

#endif




