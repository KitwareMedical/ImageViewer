/* perform a pixel-wise intensity transformation 
   by computing the median value of a neighborhood  */

#include "itkVVFilterModule.h"

#include "itkMedianImageFilter.h"

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
      typedef  itk::MedianImageFilter< ImageType,   ImageType >   FilterType;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median function...");
      // Set the parameters on it
      ImageType::SizeType radius;
      radius.Fill( atoi( info->GUIItems[ 0 ].CurrentValue ) );
      module.GetFilter()->SetRadius( radius );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::MedianImageFilter< ImageType,  ImageType >   FilterType;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median function...");
      // Set the parameters on it
      ImageType::SizeType radius;
      radius.Fill( atoi( info->GUIItems[ 0 ].CurrentValue ) );
      module.GetFilter()->SetRadius( radius );
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

  info->GUIItems[0].Label = "Radius";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "2";
  info->GUIItems[0].Help = "Integer radius of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed";
  info->GUIItems[0].Hints = "1 20 1";

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
  
void VV_PLUGIN_EXPORT vvMedianInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Median ITK";
  info->TerseDocumentation = "Median Filter ITK";
  info->FullDocumentation = 
    "This filters applies a pixel-wise intensity transform by using a Median function";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 1; // actually depends on the input pixel size
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 1;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
