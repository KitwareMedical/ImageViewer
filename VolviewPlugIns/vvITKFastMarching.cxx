/* Applies the FastMarchingImageFilter in order to perform
   segmentation. It requires seed points and a speed image
   as inputs. */

#include "vvITKFilterModule.h"

#include "itkFastMarchingImageFilter.h"

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
      typedef  itk::FastMarchingImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing Fast Marching...");
      // Set the parameters on it
      module.GetFilter()->SetStoppingValue(  atof( info->GUIItems[ 0 ].CurrentValue) );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::FastMarchingImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing Fast Marching...");
      // Set the parameters on it
      module.GetFilter()->SetStoppingValue(  atof( info->GUIItems[ 0 ].CurrentValue) );
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

  info->GUIItems[0].Label = "Stopping Value";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "1.0";
  info->GUIItems[0].Help = "Defines a stopping value for the time up to which the front propagation will be computed. deviation of the Gaussian kernel used to smooth the image before computing the gradient";
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
  
void VV_PLUGIN_EXPORT vvITKFastMarchingInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Fast Marching (ITK)";
  info->TerseDocumentation = "Fast Marching Level Set";
  info->FullDocumentation = 
    "Fast Marcing uses a Level Set representation for propagating a front from user-provided seed points. A user-provided speed image is used for controling the front propagation";
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
