/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKConnectedThresholdImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKConnectedThresholdImageFilter - Wrapper class around itk::ConnectedThresholdImageFilter
// .SECTION Description
// vtkITKConnectedThresholdImageFilter


#ifndef __vtkITKConnectedThresholdImageFilter_h
#define __vtkITKConnectedThresholdImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkConnectedThresholdImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKConnectedThresholdImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKConnectedThresholdImageFilter *New();
  vtkTypeRevisionMacro(vtkITKConnectedThresholdImageFilter, vtkITKImageToImageFilterFF);

  void SetReplaceValue ( double value )
  {
    InputImagePixelType d = static_cast<InputImagePixelType> ( value );
    DelegateITKInputMacro ( SetReplaceValue, d );
  };
  void SetUpper ( double ind )
  {
    InputImagePixelType d = static_cast<InputImagePixelType> ( ind );
    DelegateITKInputMacro ( SetUpper, d );
  };
  double GetUpper()
  { DelegateITKOutputMacro ( GetUpper ); };
  void SetLower ( double ind )
  {
    InputImagePixelType d = static_cast<InputImagePixelType> ( ind );
    DelegateITKInputMacro ( SetLower, d );
  };
  double GetLower()
  { DelegateITKOutputMacro ( GetLower ); };

  void ClearSeeds () { this->GetImageFilterPointer()->ClearSeeds(); };
  void AddSeed ( int x, int y, int z )
  {
    ImageFilterType::IndexType seed;
    seed[0] = x;
    seed[1] = y;
    seed[2] = z;
    this->GetImageFilterPointer()->AddSeed ( seed );
  }

protected:
  //BTX
  typedef itk::ConnectedThresholdImageFilter<Superclass::InputImageType, Superclass::OutputImageType> ImageFilterType;
  vtkITKConnectedThresholdImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKConnectedThresholdImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  //ETX
  
private:
  vtkITKConnectedThresholdImageFilter(const vtkITKConnectedThresholdImageFilter&);  // Not implemented.
  void operator=(const vtkITKConnectedThresholdImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKConnectedThresholdImageFilter, "$Revision: 1.3 $");
vtkStandardNewMacro(vtkITKConnectedThresholdImageFilter);

#endif




