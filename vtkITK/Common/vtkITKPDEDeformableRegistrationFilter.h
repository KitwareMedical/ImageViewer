/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKPDEDeformableRegistrationFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKPDEDeformableRegistrationFilter - Wrapper class around itk::PDEDeformableRegistrationImageFilter
// .SECTION Description
// vtkITKPDEDeformableRegistrationFilter


#ifndef __vtkITKPDEDeformableRegistrationFilter_h
#define __vtkITKPDEDeformableRegistrationFilter_h


#include "vtkITKImageToImageFilterF3F3.h"
#include "itkPDEDeformableRegistrationFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKPDEDeformableRegistrationFilter : public vtkITKImageToImageFilterF3F3
{
 public:
  static vtkITKPDEDeformableRegistrationFilter *New();
  vtkTypeRevisionMacro(vtkITKPDEDeformableRegistrationFilter, vtkITKImageToImageFilterF3F3);
  //BTX
  typedef itk::Image<float, 3> RegistrationImageType;
  typedef itk::VTKImageImport<RegistrationImageType> FixedImageImportType;
  typedef itk::VTKImageImport<RegistrationImageType> MovingImageImportType;
  //ETX
  
  void SetNumberOfIterations ( unsigned int n ) { DelegateSetMacro ( NumberOfIterations, n ); };
  unsigned int GetNumberOfIterations () { DelegateGetMacro ( NumberOfIterations ); };
  void SetStandardDeviations ( double d ) { DelegateSetMacro ( StandardDeviations, d ); };
  void SetTargetImage ( vtkImageData *I )
  {
    this->vtkFixedImageExporter->SetInput ( I );
  }
  void SetSourceImage ( vtkImageData *I )
  {
    this->vtkMovingImageExporter->SetInput ( I );
  }
  
protected:
  //BTX
  typedef itk::PDEDeformableRegistrationFilter<RegistrationImageType,RegistrationImageType,Superclass::OutputImageType> ImageFilterType;
  vtkITKPDEDeformableRegistrationFilter() :
    Superclass ( dynamic_cast<Superclass::GenericFilterType*> ( ImageFilterType::New().GetPointer() ) )
  {
    // Create the import/exports for Fixed and Moving Images
    this->vtkFixedImageExporter = vtkImageExport::New();
    this->itkFixedImageImporter = FixedImageImportType::New();
    ConnectPipelines ( this->vtkFixedImageExporter, this->itkFixedImageImporter );
    this->vtkMovingImageExporter = vtkImageExport::New();
    this->itkMovingImageImporter = MovingImageImportType::New();
    ConnectPipelines ( this->vtkMovingImageExporter, this->itkMovingImageImporter );
    this->GetFilterPointer()->SetFixedImage ( itkFixedImageImporter->GetOutput() );
    this->GetFilterPointer()->SetMovingImage ( itkMovingImageImporter->GetOutput() );
    
  };
  ~vtkITKPDEDeformableRegistrationFilter()
  {
    this->vtkFixedImageExporter->Delete();
    this->vtkMovingImageExporter->Delete();
  };

  //ETX
  
private:
  vtkITKPDEDeformableRegistrationFilter(const vtkITKPDEDeformableRegistrationFilter&);  // Not implemented.
  void operator=(const vtkITKPDEDeformableRegistrationFilter&);  // Not implemented.

  // To get the filter as the proper type
  ImageFilterType* GetFilterPointer ()
  {
    return dynamic_cast<ImageFilterType*> ( this->m_Filter.GetPointer() );
  }

  //BTX
  // Need to have two vtkExporters
  vtkImageExport* vtkFixedImageExporter;
  vtkImageExport* vtkMovingImageExporter;
  
  FixedImageImportType::Pointer itkFixedImageImporter;
  MovingImageImportType::Pointer itkMovingImageImporter;
  //ETX
  
  
};

vtkCxxRevisionMacro(vtkITKPDEDeformableRegistrationFilter, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkITKPDEDeformableRegistrationFilter);

#endif




