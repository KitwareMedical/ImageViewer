/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKThresholdSegmentationLevelSetImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKThresholdSegmentationLevelSetImageFilter - Wrapper class around itk::ThresholdSegmentationLevelSetImageFilterImageFilter
// .SECTION Description
// vtkITKThresholdSegmentationLevelSetImageFilter


#ifndef __vtkITKThresholdSegmentationLevelSetImageFilter_h
#define __vtkITKThresholdSegmentationLevelSetImageFilter_h


#include "vtkITKImageToImageFilterFF.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKThresholdSegmentationLevelSetImageFilter : public vtkITKImageToImageFilterFF
{
 public:
  static vtkITKThresholdSegmentationLevelSetImageFilter *New();
  vtkTypeRevisionMacro(vtkITKThresholdSegmentationLevelSetImageFilter, vtkITKImageToImageFilterFF);

  float GetUpperThreshold ()
  {
    DelegateITKOutputMacro(GetUpperThreshold) ;
  };

  float GetLowerThreshold ()
  {
    DelegateITKOutputMacro(GetLowerThreshold) ;
  };

  float GetIsoSurfaceValue ()
  {
    DelegateITKOutputMacro(GetIsoSurfaceValue) ;
  };

  void SetUpperThreshold (float value )
  {
     DelegateITKInputMacro ( SetUpperThreshold, value );
  };

  void SetLowerThreshold (float value )
  {
     DelegateITKInputMacro ( SetLowerThreshold, value );
  };
  
  void SetIsoSurfaceValue ( float value )
  {
     DelegateITKInputMacro ( SetIsoSurfaceValue, value );
  };
  
  void SetMaximumIterations ( int value )
  {
    DelegateITKInputMacro ( SetMaximumIterations, value );
  };

  void SetMaximumRMSError ( float value )
  {
    DelegateITKInputMacro ( SetMaximumRMSError, value );
  };

  float GetMaximumRMSError()
  {
    DelegateITKOutputMacro ( GetMaximumRMSError );
  };

  void SetUseNegativeFeatures (int value )
  {
    DelegateITKInputMacro( SetUseNegativeFeatures, value);
  }
  
  void SetFeatureImage ( vtkImageData *value )
  {
    this->vtkFeatureExporter->SetInput(value);
  }
  
  void SetFeatureScaling ( float value )
  {
    DelegateITKInputMacro ( SetFeatureScaling, value );
  };

  float GetFeatureScaling ()
  {
    DelegateITKOutputMacro(GetFeatureScaling) ;
  };

   float GetRMSChange ()
  {
    DelegateITKOutputMacro(GetRMSChange);
  };

  int GetElapsedIterations()
  {
    DelegateITKOutputMacro(GetElapsedIterations);
  };

  
protected:
  //BTX
  typedef itk::ThresholdSegmentationLevelSetImageFilter<Superclass::InputImageType,Superclass::InputImageType> ImageFilterType;
  typedef itk::VTKImageImport<InputImageType> FeatureImageImportType;
  
  vtkITKThresholdSegmentationLevelSetImageFilter() : Superclass ( ImageFilterType::New() )
  {
    this->vtkFeatureExporter = vtkImageExport::New();
    this->itkFeatureImporter = FeatureImageImportType::New();
    ConnectPipelines ( this->vtkFeatureExporter, this->itkFeatureImporter );
    this->GetImageFilterPointer()->SetFeatureImage ( this->itkFeatureImporter->GetOutput() );
  };
  
  ~vtkITKThresholdSegmentationLevelSetImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  
  FeatureImageImportType::Pointer itkFeatureImporter;
  //ETX

  vtkImageExport *vtkFeatureExporter;
  // vtkImageImport *vtkSpeedImporter;
  
private:
  vtkITKThresholdSegmentationLevelSetImageFilter(const vtkITKThresholdSegmentationLevelSetImageFilter&);  // Not implemented.
  void operator=(const vtkITKThresholdSegmentationLevelSetImageFilter&);  //
                                                                          // Not implemented
  
};

vtkCxxRevisionMacro(vtkITKThresholdSegmentationLevelSetImageFilter, "$Revision: 1.3 $");
vtkStandardNewMacro(vtkITKThresholdSegmentationLevelSetImageFilter);

#endif




