/* perform segmentation using the confidence connected image filter */

#include "vvITKFilterModule.h"

#include "itkConnectedThresholdImageFilter.h"

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
      typedef  ImageType::IndexType                 IndexType;
      typedef  itk::ConnectedThresholdImageFilter<  ImageType,  ImageType >   FilterType;
      IndexType seed;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Threshold Connected Region Growing...");
      // Set the parameters on it
      module.GetFilter()->SetUpper(              atof( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetLower(              atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetReplaceValue(       atoi( info->GUIItems[ 2 ].CurrentValue) );
      seed[0] =                                  atoi( info->GUIItems[ 3 ].CurrentValue);      
      seed[1] =                                  atoi( info->GUIItems[ 4 ].CurrentValue);      
      seed[2] =                                  atoi( info->GUIItems[ 5 ].CurrentValue);      
      module.GetFilter()->SetSeed( seed );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  ImageType::IndexType                 IndexType;
      typedef  itk::ConnectedThresholdImageFilter< ImageType,  ImageType >   FilterType;
      IndexType seed;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Threshold Connected Region Growing...");
      // Set the parameters on it
      module.GetFilter()->SetUpper(              atof( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetLower(              atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetReplaceValue(       atoi( info->GUIItems[ 2 ].CurrentValue) );
      seed[0] =                                  atoi( info->GUIItems[ 3 ].CurrentValue);      
      seed[1] =                                  atoi( info->GUIItems[ 4 ].CurrentValue);      
      seed[2] =                                  atoi( info->GUIItems[ 5 ].CurrentValue);      
      module.GetFilter()->SetSeed( seed );
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

  info->GUIItems[0].Label = "Upper Threshold";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "250";
  info->GUIItems[0].Help = "Upper threshold. Only pixels with intensities lower than this threshold will be considered to be added to the region.";
  info->GUIItems[0].Hints = "0 255 1";

  info->GUIItems[1].Label = "Lower Threshold";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "50";
  info->GUIItems[1].Help = "Lower threshold. Only pixels with intensities higher than this threshold will be considered to be added to the region.";
  info->GUIItems[1].Hints = "0 255 1";

  info->GUIItems[2].Label = "Replace Value";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "255";
  info->GUIItems[2].Help = "Value to assign to the binary mask of the segmented region. The rest of the image will be set to zero.";
  info->GUIItems[2].Hints = "1 255.0 1.0";

  info->GUIItems[3].Label = "X coordinate of the seed";
  info->GUIItems[3].GUIType = VV_GUI_SCALE;
  info->GUIItems[3].Default = "0";
  info->GUIItems[3].Help = "X coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[3].Hints = "1 1000.0 1.0";

  info->GUIItems[4].Label = "Y coordinate of the seed";
  info->GUIItems[4].GUIType = VV_GUI_SCALE;
  info->GUIItems[4].Default = "0";
  info->GUIItems[4].Help = "Y coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[4].Hints = "1 1000.0 1.0";

  info->GUIItems[5].Label = "Z coordinate of the seed";
  info->GUIItems[5].GUIType = VV_GUI_SCALE;
  info->GUIItems[5].Default = "0";
  info->GUIItems[5].Help = "Z coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[5].Hints = "1 1000.0 1.0";

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
  
void VV_PLUGIN_EXPORT vvITKConnectedThresholdInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Connected Threshold (ITK) ";
  info->TerseDocumentation = "Connected Threshold Segmentation";
  info->FullDocumentation = 
    "This filter applies an region growing algorithm for segmentation. The criterion for including new pixels in the region is defined by an intensity range whose bound are provided by the user. These bounds are described as the lower and upper thresholds.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 4;  // It is actually dependent of the complexity of the shape to segment
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 6;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
