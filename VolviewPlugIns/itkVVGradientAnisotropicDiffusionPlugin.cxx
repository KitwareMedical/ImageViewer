/* perform smoothing using an anisotropic diffusion filter */

#include "itkVVFilterModule.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;

  typedef   float       InternalPixelType;
  typedef   itk::Image< InternalPixelType,Dimension > InternalImageType; 

  typedef   itk::GradientAnisotropicDiffusionImageFilter< 
                                InternalImageType,  
                                InternalImageType >   FilterType;
 
  try 
  {
  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      FilterModule< unsigned char, FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Gradient Anisotropic Diffusion...");
      // Set the parameters on it
      module.GetFilter()->SetIterations(             atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetConductanceParameter(   atof( info->GUIItems[ 1 ].CurrentValue) );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      FilterModule< unsigned short, FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Gradient Anisotropic Diffusion...");
      // Set the parameters on it
      module.GetFilter()->SetIterations(             atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetConductanceParameter(   atof( info->GUIItems[ 1 ].CurrentValue) );
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

  info->GUIItems[0].Label = "Number of Iterations";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "5";
  info->GUIItems[0].Help = "Number of times that the diffusion approximation will be computed. The more iterations, the stronger the smoothing";
  info->GUIItems[0].Hints = "1 100 1";

  info->GUIItems[1].Label = "Conductance";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "3.0";
  info->GUIItems[1].Help = "Factor that multiplies the image gradient in order to compute the effective conductance locally. The higher the value of this parameter, the stronger the diffusion will be";
  info->GUIItems[1].Hints = "0.1 10.0 0.1";

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
  
void VV_PLUGIN_EXPORT vvGradientAnisotropicDiffusionInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Gradient Anisotropic Diffusion";
  info->TerseDocumentation = "Anisotropic diffusion smoothing";
  info->FullDocumentation = 
    "This filter applies an edge-preserving smoothing to a volume by computing the evolution of an anisotropic diffusion partial differential equation. Diffusion is regulated by the gradient of the image. This filter processes the whole image in one piece, and does not change the dimensions, data type, or spacing of the volume.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 2;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
