/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKBilateralImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKBilateralImageFilter - Wrapper class around itk::BilateralImageFilterImageFilter
// .SECTION Description
// vtkITKBilateralImageFilter


#ifndef __vtkITKBilateralImageFilter_h
#define __vtkITKBilateralImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkBilateralImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKBilateralImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKBilateralImageFilter *New();
  vtkTypeRevisionMacro(vtkITKBilateralImageFilter, vtkITKImageToImageFilterFF);

  void SetDomainSigma ( double v1, double v2, double v3)
    {
    double v[3];
    v[0] = v1; v[1] = v2; v[2] = v3;
    this->GetImageFilterPointer()->SetDomainSigma (v);
    }
  void SetRangeSigma ( double v1 )
    {
    this->GetImageFilterPointer()->SetRangeSigma (v1);
    }
  void SetFilterDimensionality ( unsigned int m)
    {
    this->GetImageFilterPointer()->SetFilterDimensionality (m);
    }

protected:
  //BTX
  typedef itk::BilateralImageFilter<Superclass::InputImageType,Superclass::InputImageType> ImageFilterType;
  vtkITKBilateralImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKBilateralImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }

  //ETX
  
private:
  vtkITKBilateralImageFilter(const vtkITKBilateralImageFilter&);  // Not implemented.
  void operator=(const vtkITKBilateralImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKBilateralImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKBilateralImageFilter);

#endif




