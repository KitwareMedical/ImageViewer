/* Computes the gradient magnitude after convolving with a Gaussian kernel.
   It uses IIR filter for approximating the convolution  */

#include "vvITKFilterModule.h"

#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;

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
      module.GetFilter()->SetSigma(  atof( info->GUIItems[ 0 ].CurrentValue) );
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
      module.GetFilter()->SetSigma(  atof( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetNormalizeAcrossScale( true );
      // Execute the filter
      module.ProcessData( pds );
      break; 
      }
    }
  }
  catch( itk::ExceptionObject & except )
  {
    info->DisplayError( info, except.what() ); 
    return -1;
  }
  return 0;
}


static int UpdateGUI(void *inf)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->GUIItems[0].Label = "Sigma";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "1.0";
  info->GUIItems[0].Help = "Standard deviation of the Gaussian kernel used to smooth the image before computing the gradient";
  info->GUIItems[0].Hints = "0 20 0.1";

  info->RequiredZOverlap = 0;
  
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
  info->Name = "Gradient Magnitude IIR (ITK)";
  info->TerseDocumentation = "Gradient Magnitude Gaussian IIR";
  info->FullDocumentation = 
    "This filter applies IIR filters to compute the equivalent of convolving the input image with the derivatives of a Gaussian kernel and then computing the magnitude of the resulting gradient.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 16; 
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 1;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
