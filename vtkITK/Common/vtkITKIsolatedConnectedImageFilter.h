/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKIsolatedConnectedImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKIsolatedConnectedImageFilter - Wrapper class around itk::IsolatedConnectedImageFilter
// .SECTION Description
// vtkITKIsolatedConnectedImageFilter


#ifndef __vtkITKIsolatedConnectedImageFilter_h
#define __vtkITKIsolatedConnectedImageFilter_h


#include "vtkITKImageToImageFilterUSUS.h"
#include "itkIsolatedConnectedImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKIsolatedConnectedImageFilter : public vtkITKImageToImageFilterUSUS
{
 public:
  static vtkITKIsolatedConnectedImageFilter *New();
  vtkTypeRevisionMacro(vtkITKIsolatedConnectedImageFilter, vtkITKImageToImageFilterUSUS);

  void SetReplaceValue ( double value )
  {
    InputImagePixelType d = static_cast<InputImagePixelType> ( value );
    DelegateITKInputMacro ( SetReplaceValue, d );
  };
  void SetLower ( double ind )
  {
    InputImagePixelType d = static_cast<InputImagePixelType> ( ind );
    DelegateITKInputMacro ( SetLower, d );
  };
  double GetLower()
  { DelegateITKOutputMacro ( GetLower ); };
  
  double GetIsolatedValue()
  { DelegateITKOutputMacro ( GetIsolatedValue ); };
  
  void SetSeed1 ( int x, int y, int z )
  {
    ImageFilterType::IndexType seed;
    seed[0] = x;
    seed[1] = y;
    seed[2] = z;
    this->GetImageFilterPointer()->SetSeed1 ( seed );
  }

  void SetSeed2 ( int x, int y, int z )
  {
    ImageFilterType::IndexType seed;
    seed[0] = x;
    seed[1] = y;
    seed[2] = z;
    this->GetImageFilterPointer()->SetSeed2 ( seed );
  }

protected:
  //BTX
  typedef itk::IsolatedConnectedImageFilter<Superclass::InputImageType, Superclass::OutputImageType> ImageFilterType;
  vtkITKIsolatedConnectedImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKIsolatedConnectedImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  //ETX
  
private:
  vtkITKIsolatedConnectedImageFilter(const vtkITKIsolatedConnectedImageFilter&);  // Not implemented.
  void operator=(const vtkITKIsolatedConnectedImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKIsolatedConnectedImageFilter, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkITKIsolatedConnectedImageFilter);

#endif




