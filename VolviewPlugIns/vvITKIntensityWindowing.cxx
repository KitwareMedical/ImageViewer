/* perform a pixel-wise intensity transformation using a IntensityWindowing function */

#include "vvITKFilterModule.h"

#include "itkIntensityWindowingImageFilter.h"

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
      typedef  itk::IntensityWindowingImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a IntensityWindowing function...");
      // Set the parameters on it
      module.GetFilter()->SetWindowMinimum(  atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetWindowMaximum(  atoi( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetOutputMinimum(  atoi( info->GUIItems[ 2 ].CurrentValue) );
      module.GetFilter()->SetOutputMaximum(  atoi( info->GUIItems[ 3 ].CurrentValue) );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::IntensityWindowingImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a IntensityWindowing function...");
      // Set the parameters on it
      module.GetFilter()->SetWindowMinimum(  atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetWindowMaximum(  atoi( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetOutputMinimum(  atoi( info->GUIItems[ 2 ].CurrentValue) );
      module.GetFilter()->SetOutputMaximum(  atoi( info->GUIItems[ 3 ].CurrentValue) );
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

  info->GUIItems[0].Label = "Window Minimum";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "0";
  info->GUIItems[0].Help = "Desired value for the minimum intensity of the input window.";
  info->GUIItems[0].Hints = "0 255 1";

  info->GUIItems[1].Label = "Window Maximum";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "255";
  info->GUIItems[1].Help = "Desired value for the maximum intensity of the input window.";
  info->GUIItems[1].Hints = "0 255 1";

  info->GUIItems[2].Label = "Output Minimum";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "0";
  info->GUIItems[2].Help = "Desired value for the minimum intensity of the output image.";
  info->GUIItems[2].Hints = "0 255 1";

  info->GUIItems[3].Label = "Output Maximum";
  info->GUIItems[3].GUIType = VV_GUI_SCALE;
  info->GUIItems[3].Default = "255";
  info->GUIItems[3].Help = "Desired value for the maximum intensity of the output image.";
  info->GUIItems[3].Hints = "0 255 1";

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
  
void VV_PLUGIN_EXPORT vvITKIntensityWindowingInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI   = UpdateGUI;
  info->Name = "Intensity Windowing (ITK)";
  info->TerseDocumentation = "Intensity Windowing Transform";
  info->FullDocumentation = 
    "This filters applies a pixel-wise intensity transform by using a IntensityWindowing function";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 1;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 1; // actually depends on the input pixel size
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 4;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
