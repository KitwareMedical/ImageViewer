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

  void SetUseNegativeFeatures (int value )
  {
    DelegateITKInputMacro( SetUseNegativeFeatures, value);
  }
  
  void SetFeatureImage ( vtkImageData *value)
  {
    this->vtkFeatureExporter->SetInput(value);
  }

  vtkImageData *GetSpeedImage()
  {
    this->vtkSpeedImporter->Update();
    return this->vtkSpeedImporter->GetOutput();
  }

  void SetFeatureScaling ( float value )
  {
    DelegateITKInputMacro ( SetFeatureScaling, value );
  };

   float GetRMSChange ()
  {
    DelegateITKOutputMacro(GetRMSChange);
  };

  int GetElapsedIterations()
  {
    DelegateITKOutputMacro(GetElapsedIterations);
  };

  
  // Description: Override vtkSource's Update so that we can access this class's GetOutput(). vtkSource's GetOutput is not virtual.
  void Update()
  {
    if (this->vtkFeatureExporter->GetInput())
      {
        this->itkFeatureImporter->Update();
        
        if (this->GetOutput(0))
          {
            this->GetOutput(0)->Update();
            if ( this->GetOutput(0)->GetSource() )
              {
                //          this->SetErrorCode( this->GetOutput(0)->GetSource()->GetErrorCode() );
              }
          }
      }
  }
    
protected:
  //BTX
  typedef itk::ThresholdSegmentationLevelSetImageFilter<Superclass::InputImageType,Superclass::InputImageType> ImageFilterType;
  typedef itk::VTKImageImport<InputImageType> FeatureImageImportType;
  typedef itk::VTKImageExport<InputImageType> SpeedImageExportType;
  
  vtkITKThresholdSegmentationLevelSetImageFilter() : Superclass ( ImageFilterType::New() )
  {
    this->vtkFeatureExporter = vtkImageExport::New();
    this->itkFeatureImporter = FeatureImageImportType::New();
    this->itkSpeedExporter = SpeedImageExportType::New();
    this->vtkSpeedImporter = vtkImageImport::New();
    ConnectPipelines(this->itkSpeedExporter, this->vtkSpeedImporter);
    ConnectPipelines(this->vtkFeatureExporter, this->itkFeatureImporter);
    (dynamic_cast<ImageFilterType*>(m_Filter.GetPointer()))->SetFeatureImage(this->itkFeatureImporter->GetOutput());
    this->itkSpeedExporter->SetInput((dynamic_cast<ImageFilterType*>(m_Filter.GetPointer()))->GetSpeedImage());
  };
  ~vtkITKThresholdSegmentationLevelSetImageFilter() {};
  ImageFilterType* GetImageFilterPointer() { return dynamic_cast<ImageFilterType*> ( m_Filter.GetPointer() ); }
  
  FeatureImageImportType::Pointer itkFeatureImporter;
  SpeedImageExportType::Pointer itkSpeedExporter;
  //ETX

  vtkImageExport *vtkFeatureExporter;
  vtkImageImport *vtkSpeedImporter;
  
private:
  vtkITKThresholdSegmentationLevelSetImageFilter(const vtkITKThresholdSegmentationLevelSetImageFilter&);  // Not implemented.
  void operator=(const vtkITKThresholdSegmentationLevelSetImageFilter&);  //
                                                                          // Not implemented
  
};

vtkCxxRevisionMacro(vtkITKThresholdSegmentationLevelSetImageFilter, "$Revision: 1.5 $");
vtkStandardNewMacro(vtkITKThresholdSegmentationLevelSetImageFilter);

#endif




