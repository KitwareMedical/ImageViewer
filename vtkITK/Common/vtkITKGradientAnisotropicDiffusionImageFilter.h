/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKGradientAnisotropicDiffusionImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKGradientAnisotropicDiffusionImageFilter - Wrapper class around itk::GradientAnisotropicDiffusionImageFilterImageFilter
// .SECTION Description
// vtkITKGradientAnisotropicDiffusionImageFilter


#ifndef __vtkITKGradientAnisotropicDiffusionImageFilter_h
#define __vtkITKGradientAnisotropicDiffusionImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKGradientAnisotropicDiffusionImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKGradientAnisotropicDiffusionImageFilter *New();
  vtkTypeRevisionMacro(vtkITKGradientAnisotropicDiffusionImageFilter, vtkITKImageToImageFilterFF);

  double GetTimeStep ()
  {
    DelegateITKOutputMacro(GetTimeStep) ;
  };

  double GetConductanceParameter ()
  {
    DelegateITKOutputMacro(GetConductanceParameter) ;
  };

  unsigned int GetIterations ()
  {
    DelegateITKOutputMacro(GetIterations) ;
  };

  void SetIterations ( unsigned int value )
  {
    DelegateITKInputMacro ( SetIterations, value );
  };

  void SetTimeStep ( double value )
  {
    DelegateITKInputMacro ( SetTimeStep, value );
  };

  void SetConductanceParameter ( double value )
  {
    DelegateITKInputMacro ( SetConductanceParameter, value );
  };

protected:
  //BTX
  typedef itk::GradientAnisotropicDiffusionImageFilter<Superclass::InputImageType,Superclass::InputImageType> ImageFilterType;
  vtkITKGradientAnisotropicDiffusionImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKGradientAnisotropicDiffusionImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }

  //ETX
  
private:
  vtkITKGradientAnisotropicDiffusionImageFilter(const vtkITKGradientAnisotropicDiffusionImageFilter&);  // Not implemented.
  void operator=(const vtkITKGradientAnisotropicDiffusionImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKGradientAnisotropicDiffusionImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKGradientAnisotropicDiffusionImageFilter);

#endif




