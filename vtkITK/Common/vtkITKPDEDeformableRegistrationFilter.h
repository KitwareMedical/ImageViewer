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


#include "vtkITKImageToImageFilter.h"
#include "itkPDEDeformableRegistrationFilter.h"
#include "itkDemonsRegistrationFilter.h"
#include "vtkObjectFactory.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkVector.h"
#include "vtkITKUtility.h"

class VTK_EXPORT vtkITKPDEDeformableRegistrationFilter : public vtkITKImageToImageFilter
{
 public:
  static vtkITKPDEDeformableRegistrationFilter *New();
  vtkTypeRevisionMacro(vtkITKPDEDeformableRegistrationFilter, vtkITKImageToImageFilter);
  
  void PrintSelf(ostream& os, vtkIndent indent)
  {
    Superclass::PrintSelf ( os, indent );
    os << m_Filter;
  };
  
  void SetNumberOfIterations ( unsigned int n ) { DelegateSetMacro ( NumberOfIterations, n ); };
  unsigned int GetNumberOfIterations () { DelegateGetMacro ( NumberOfIterations ); };
  void SetStandardDeviations ( double d ) { DelegateSetMacro ( StandardDeviations, d ); };
  void SetTargetImage ( vtkImageData *I )
  {
    this->SetFixedImage ( I );
  }
  void SetSourceImage ( vtkImageData *I )
  {
    this->SetMovingImage ( I );
  }
  void SetFixedImage ( vtkImageData *I )
  {
    this->vtkFixedImageExporter->SetInput ( I );
  }
  void SetMovingImage ( vtkImageData *I )
  {
    this->vtkMovingImageExporter->SetInput ( I );
  }
  
  virtual vtkImageData *GetOutput() { return this->DeformationField->GetOutput(); };
  virtual vtkImageData *GetOutput(int idx)
  {
    return (vtkImageData *) this->DeformationField->GetOutput(idx);
  };
  
  // Since we have to jump through a bunch of hoops
  // to run this filter, call RegisterImages
  virtual void RegisterImages ()
  {
    // Update the filter
    this->vtkFixedImageExporter->GetInput()->Update();
    this->GetFilterPointer()->Update();
    // Copy the output data to make our output ready
    vtkImageData *I = this->vtkFixedImageExporter->GetInput();
    this->DeformationField->SetDataScalarTypeToFloat();
    this->DeformationField->SetNumberOfScalarComponents ( 3 );
    this->DeformationField->SetDataExtent ( I->GetExtent() );
    this->DeformationField->SetDataSpacing ( I->GetSpacing() );
    this->DeformationField->SetDataOrigin ( I->GetOrigin() );
    this->DeformationField->SetWholeExtent ( I->GetWholeExtent() );
    int DataExtent[6];
    this->DeformationField->GetDataExtent ( DataExtent );
    int size = 
      (DataExtent[1] - DataExtent[0]+1) *
      (DataExtent[3] - DataExtent[2]+1) *
      (DataExtent[5] - DataExtent[4]+1) *
      3 * sizeof ( float );
    this->DeformationField->CopyImportVoidPointer ( (void*) this->GetFilterPointer()->GetOutput()->GetBufferPointer(), size );
    
  }
  
  
  // If we don't set an input, this filter uses a blank filter
  virtual void SetInput(vtkImageData *Input)
  {
    std::cout << "Doing nothing!!!" << std::endl;
    return;
    /*
    std::cout << "Setting input" << Input << std::endl;
    if ( Input != NULL )
      {
      // Bypass the casting
      this->GetFilterPointer()->SetInput ( this->itkImporter->GetOutput() );
      this->vtkExporter->SetInput ( Input );
      }
    else
      {
      // Unlink it
      this->GetFilterPointer()->SetInput ( NULL );
      }
    */
  };

  
protected:
  //BTX
  typedef itk::Image<float, 3> RegistrationImageType;
  typedef itk::VTKImageImport<RegistrationImageType> FixedImageImportType;
  typedef itk::VTKImageImport<RegistrationImageType> MovingImageImportType;
  
  typedef itk::Vector<float,3> OutputImagePixelType;
  typedef itk::Image<OutputImagePixelType, 3> OutputImageType;

  typedef itk::DemonsRegistrationFilter<RegistrationImageType,RegistrationImageType,OutputImageType> ImageFilterType;

  ImageFilterType::Pointer m_Filter;
  
  vtkITKPDEDeformableRegistrationFilter()
  {
    m_Filter = ImageFilterType::New();
    this->LinkITKProgressToVTKProgress ( m_Filter );
    
    // Set up the filter pipeline

    // Create the import/exports for Fixed and Moving Images
    this->vtkFixedImageExporter = vtkImageExport::New();
    this->itkFixedImageImporter = FixedImageImportType::New();
    ConnectPipelines ( this->vtkFixedImageExporter, this->itkFixedImageImporter );
    this->vtkMovingImageExporter = vtkImageExport::New();
    this->itkMovingImageImporter = MovingImageImportType::New();
    ConnectPipelines ( this->vtkMovingImageExporter, this->itkMovingImageImporter );
    this->GetFilterPointer()->SetFixedImage ( itkFixedImageImporter->GetOutput() );
    this->GetFilterPointer()->SetMovingImage ( itkMovingImageImporter->GetOutput() );
    this->DeformationField = vtkImageImport::New();
    
  };
  ~vtkITKPDEDeformableRegistrationFilter()
  {
    this->vtkFixedImageExporter->Delete();
    this->vtkMovingImageExporter->Delete();
    this->DeformationField->Delete();
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
  vtkImageImport* DeformationField;
  
  FixedImageImportType::Pointer itkFixedImageImporter;
  MovingImageImportType::Pointer itkMovingImageImporter;
  //ETX
  
  
};

vtkCxxRevisionMacro(vtkITKPDEDeformableRegistrationFilter, "$Revision: 1.3 $");
vtkStandardNewMacro(vtkITKPDEDeformableRegistrationFilter);

#endif




