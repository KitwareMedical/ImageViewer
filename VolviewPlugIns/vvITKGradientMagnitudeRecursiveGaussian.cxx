/* Computes the gradient magnitude after convolving with a Gaussian kernel.
   It uses IIR filter for approximating the convolution  */

#include "vvITKFilterModule.h"

#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;
 
  const float sigma     = atof( info->GetGUIProperty(info, 0, VVP_GUI_VALUE ));

  try 
  {
  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      typedef  unsigned char                        PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::GradientMagnitudeRecursiveGaussianImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing the gradient magnitude...");
      // Set the parameters on it
      module.GetFilter()->SetSigma( sigma ); 
      module.GetFilter()->SetNormalizeAcrossScale( true );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::GradientMagnitudeRecursiveGaussianImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing the gradient magnitude...");
      // Set the parameters on it
      module.GetFilter()->SetSigma( sigma ); 
      module.GetFilter()->SetNormalizeAcrossScale( true );
      // Execute the filter
      module.ProcessData( pds );
      break; 
      }
    }
  }
  catch( itk::ExceptionObject & except )
  {
    info->SetProperty( info, VVP_ERROR, except.what() ); 
    return -1;
  }
  return 0;
}


static int UpdateGUI(void *inf)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->SetGUIProperty(info, 0, VVP_GUI_LABEL, "Sigma");
  info->SetGUIProperty(info, 0, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 0, VVP_GUI_DEFAULT, "1,0");
  info->SetGUIProperty(info, 0, VVP_GUI_HELP, "Standard deviation of the Gaussian kernel used to smooth the image before computing the gradient");
  info->SetGUIProperty(info, 0, VVP_GUI_HINTS , "0 20 0.1");

  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP, "0");

  info->OutputVolumeScalarType = info->InputVolumeScalarType;
  info->OutputVolumeNumberOfComponents = 
    info->InputVolumeNumberOfComponents;
  memcpy(info->OutputVolumeDimensions,info->InputVolumeDimensions,
         3*sizeof(int));
  memcpy(info->OutputVolumeSpacing,info->InputVolumeSpacing,
         3*sizeof(float));
  memcpy(info->OutputVolumeOrigin,info->InputVolumeOrigin,
         3*sizeof(float));

  return 1;
}


extern "C" {
  
void VV_PLUGIN_EXPORT vvITKGradientMagnitudeRecursiveGaussianInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->SetProperty(info, VVP_NAME, "Gradient Magnitude IIR (ITK)");
  info->SetProperty(info, VVP_TERSE_DOCUMENTATION,
                                "Gradient Magnitude Gaussian IIR");
  info->SetProperty(info, VVP_FULL_DOCUMENTATION,
    "This filter applies IIR filters to compute the equivalent of convolving the input image with the derivatives of a Gaussian kernel and then computing the magnitude of the resulting gradient.");

  info->SetProperty(info, VVP_SUPPORTS_IN_PLACE_PROCESSING, "0");
  info->SetProperty(info, VVP_SUPPORTS_PROCESSING_PIECES,   "0");
  info->SetProperty(info, VVP_NUMBER_OF_GUI_ITEMS,          "1");
  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP,           "0");
  info->SetProperty(info, VVP_PER_VOXEL_MEMORY_REQUIRED,    "8"); 
}

}
