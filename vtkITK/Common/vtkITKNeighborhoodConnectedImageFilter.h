/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKNeighborhoodConnectedImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKNeighborhoodConnectedImageFilter - Wrapper class around itk::NeighborhoodConnectedImageFilter
// .SECTION Description
// vtkITKNeighborhoodConnectedImageFilter


#ifndef __vtkITKNeighborhoodConnectedImageFilter_h
#define __vtkITKNeighborhoodConnectedImageFilter_h


#include "vtkITKImageToImageFilterUSUS.h"
#include "itkNeighborhoodConnectedImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKNeighborhoodConnectedImageFilter : public vtkITKImageToImageFilterUSUS
{
 public:
  static vtkITKNeighborhoodConnectedImageFilter *New();
  vtkTypeRevisionMacro(vtkITKNeighborhoodConnectedImageFilter, vtkITKImageToImageFilterUSUS);

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

  void SetRadius ( unsigned int x, unsigned int y, unsigned int z )
  {
    ImageFilterType::InputImageSizeType size;
    size[0] = x;
    size[1] = y;
    size[2] = z;
    DelegateITKInputMacro ( SetRadius, size );
  }
  unsigned int GetRadius ()
  {
    return this->GetImageFilterPointer()->GetRadius()[0];
  }
  void SetSeed ( int x, int y, int z )
  {
    ImageFilterType::IndexType seed;
    seed[0] = x;
    seed[1] = y;
    seed[2] = z;
    this->GetImageFilterPointer()->SetSeed ( seed );
  }
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
  typedef itk::NeighborhoodConnectedImageFilter<Superclass::InputImageType, Superclass::OutputImageType> ImageFilterType;
  vtkITKNeighborhoodConnectedImageFilter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKNeighborhoodConnectedImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  //ETX
  
private:
  vtkITKNeighborhoodConnectedImageFilter(const vtkITKNeighborhoodConnectedImageFilter&);  // Not implemented.
  void operator=(const vtkITKNeighborhoodConnectedImageFilter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKNeighborhoodConnectedImageFilter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKNeighborhoodConnectedImageFilter);

#endif




