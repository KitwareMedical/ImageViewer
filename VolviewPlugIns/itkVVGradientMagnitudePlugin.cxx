/* Computes the gradient magnitude using finite differences localy */

#include "itkVVFilterModule.h"

#include "itkGradientMagnitudeImageFilter.h"

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
      typedef  itk::GradientMagnitudeImageFilter< ImageType,  ImageType >   FilterType;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing the gradient magnitude...");
      // No parameters to set..

      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::GradientMagnitudeImageFilter< ImageType,  ImageType >   FilterType;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing the gradient magnitude...");
      // No parameters to set..

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
  
void VV_PLUGIN_EXPORT vvGradientMagnitudeInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Gradient Magnitude";
  info->TerseDocumentation = "Gradient Magnitude";
  info->FullDocumentation = 
    "This filter computes the magnitude if the gradient using finite differences. Basically by convolving with masks of type [-1,0,1].";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 1; // actually x the pixel size of the output image. 
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 0;
//  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
