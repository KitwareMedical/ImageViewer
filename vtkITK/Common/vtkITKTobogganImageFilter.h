/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKTobogganImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKTobogganImageFilter - Wrapper class around itk::TobogganImageFilterImageFilter
// .SECTION Description
// vtkITKTobogganImageFilter


#ifndef __vtkITKTobogganImageFilter_h
#define __vtkITKTobogganImageFilter_h


#include "vtkITKImageToImageFilterFUL.h"
#include "itkTobogganImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKTobogganImageFilter : public vtkITKImageToImageFilterFUL
{
 public:
  static vtkITKTobogganImageFilter *New();
  vtkTypeRevisionMacro(vtkITKTobogganImageFilter, vtkITKImageToImageFilterFUL);

protected:
  //BTX
  typedef itk::TobogganImageFilter<Superclass::InputImageType> ImageFilterType;
  vtkITKTobogganImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKTobogganImageFilter() {};

  //ETX
  
private:
  vtkITKTobogganImageFilter(const vtkITKTobogganImageFilter&);  // Not implemented.
  void operator=(const vtkITKTobogganImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKTobogganImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKTobogganImageFilter);

#endif




