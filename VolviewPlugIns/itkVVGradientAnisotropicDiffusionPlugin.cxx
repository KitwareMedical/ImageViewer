/* perform smoothing using an anisotropic diffusion filter */

#include "itkVVFilterModuleWithCasting.h"

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
      FilterModuleWithCasting< unsigned char, FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Gradient Anisotropic Diffusion...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations(     atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetTimeStep(               atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetConductanceParameter(   atof( info->GUIItems[ 2 ].CurrentValue) );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      FilterModuleWithCasting< unsigned short, FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Gradient Anisotropic Diffusion...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations(     atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetTimeStep(               atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetConductanceParameter(   atof( info->GUIItems[ 2 ].CurrentValue) );
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

  info->GUIItems[1].Label = "Time Step";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "0.125";
  info->GUIItems[1].Help = "Discretization of time for approximating the diffusion process.";
  info->GUIItems[1].Hints = "0.01 1.0 0.005";

  info->GUIItems[2].Label = "Conductance";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "3.0";
  info->GUIItems[2].Help = "Factor that multiplies the image gradient in order to compute the effective conductance locally. The higher the value of this parameter, the stronger the diffusion will be";
  info->GUIItems[2].Hints = "0.1 10.0 0.1";

  const unsigned int numberOfIterations = atoi( info->GUIItems[ 0 ].CurrentValue );

  info->RequiredZOverlap = numberOfIterations;
  
  info->OutputVolumeScalarType = info->InputVolumeScalarType;
  info->OutputVolumeNumberOfComponents = info->InputVolumeNumberOfComponents;

  info->OutputVolumeDimensions[0] = info->InputVolumeDimensions[0]; // + 2 * numberOfIterations;
  info->OutputVolumeDimensions[1] = info->InputVolumeDimensions[1]; // + 2 * numberOfIterations;
  info->OutputVolumeDimensions[2] = info->InputVolumeDimensions[2]; // + 2 * numberOfIterations;

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
  info->SupportsProcessingPieces = 1;
  info->RequiredZOverlap = 0;
  
  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 8;

  /* setup the GUI components */
  info->NumberOfGUIItems = 3;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
