/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKDiscreteGaussianImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKDiscreteGaussianImageFilter - Wrapper class around itk::DiscreteGaussianImageFilterImageFilter
// .SECTION Description
// vtkITKDiscreteGaussianImageFilter


#ifndef __vtkITKDiscreteGaussianImageFilter_h
#define __vtkITKDiscreteGaussianImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKDiscreteGaussianImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKDiscreteGaussianImageFilter *New();
  vtkTypeRevisionMacro(vtkITKDiscreteGaussianImageFilter, vtkITKImageToImageFilterFF);

  void SetVariance ( double v1, double v2, double v3)
    {
    double v[3];
    v[0] = v1; v[1] = v2; v[2] = v3;
    this->GetImageFilterPointer()->SetVariance (v);
    }
  void SetMaximumError (double e1, double e2, double e3)
    {
    double e[3];
    e[0] = e1; e[1] = e2; e[2] = e3;
    this->GetImageFilterPointer()->SetMaximumError (e);
    }
  void SetMaximumKernelWidth ( int m)
    {
    this->GetImageFilterPointer()->SetMaximumKernelWidth (m);
    }

  void SetFilterDimensionality ( unsigned int m)
    {
    this->GetImageFilterPointer()->SetFilterDimensionality (m);
    }

protected:
  //BTX
  typedef itk::DiscreteGaussianImageFilter<Superclass::InputImageType,Superclass::InputImageType> ImageFilterType;
  vtkITKDiscreteGaussianImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKDiscreteGaussianImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }

  //ETX
  
private:
  vtkITKDiscreteGaussianImageFilter(const vtkITKDiscreteGaussianImageFilter&);  // Not implemented.
  void operator=(const vtkITKDiscreteGaussianImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKDiscreteGaussianImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKDiscreteGaussianImageFilter);

#endif




