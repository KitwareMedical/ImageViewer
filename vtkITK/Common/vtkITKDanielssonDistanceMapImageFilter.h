/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKDanielssonDistanceMapImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKDanielssonDistanceMapImageFilter - Wrapper class around itk::DanielssonDistanceMapImageFilter
// .SECTION Description
// vtkITKDanielssonDistanceMapImageFilter


#ifndef __vtkITKDanielssonDistanceMapImageFilter_h
#define __vtkITKDanielssonDistanceMapImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_IMAGING_EXPORT vtkITKDanielssonDistanceMapImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKDanielssonDistanceMapImageFilter *New();
  vtkTypeRevisionMacro(vtkITKDanielssonDistanceMapImageFilter, vtkITKImageToImageFilterFF);

  void SetSquaredDistance ( int value )
  {
    DelegateITKInputMacro ( SetSquaredDistance, (bool) value );
  }

  void SquaredDistanceOn()
  {
    this->SetSquaredDistance (true);
  }
  void SquaredDistanceOff()
  {
    this->SetSquaredDistance (false);
  }
  int GetSquaredDistance()
  { DelegateITKOutputMacro ( GetSquaredDistance ); }


  void SetInputIsBinary ( int value )
  {
    DelegateITKInputMacro ( SetInputIsBinary, (bool) value );
  }
  void InputIsBinaryOn()
  {
    this->SetInputIsBinary (true);
  }
  void InputIsBinaryOff()
  {
    this->SetInputIsBinary (false);
  }
  int GetInputIsBinary()
  { DelegateITKOutputMacro ( GetInputIsBinary ); };

protected:
  //BTX
  typedef itk::DanielssonDistanceMapImageFilter<Superclass::InputImageType, Superclass::OutputImageType> ImageFilterType;
  vtkITKDanielssonDistanceMapImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKDanielssonDistanceMapImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  //ETX
  
private:
  vtkITKDanielssonDistanceMapImageFilter(const vtkITKDanielssonDistanceMapImageFilter&);  // Not implemented.
  void operator=(const vtkITKDanielssonDistanceMapImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKDanielssonDistanceMapImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKDanielssonDistanceMapImageFilter);

#endif




