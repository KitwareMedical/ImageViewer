/* perform an intensity transformation by computing 
   the median value of a neighborhood  */

#include "vvITKFilterModule.h"

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
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median filter...");
      // Set the parameters on it
      ImageType::SizeType radius;
      radius[0] = atoi( info->GUIItems[ 0 ].CurrentValue );
      radius[1] = atoi( info->GUIItems[ 1 ].CurrentValue );
      radius[2] = atoi( info->GUIItems[ 2 ].CurrentValue );
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
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median filter...");
      // Set the parameters on it
      ImageType::SizeType radius;
      radius[0] = atoi( info->GUIItems[ 0 ].CurrentValue );
      radius[1] = atoi( info->GUIItems[ 1 ].CurrentValue );
      radius[2] = atoi( info->GUIItems[ 2 ].CurrentValue );
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

  info->GUIItems[0].Label = "Radius X";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "2";
  info->GUIItems[0].Help = "Integer radius along the X axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.";
  info->GUIItems[0].Hints = "1 10 1";

  info->GUIItems[1].Label = "Radius Y";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "2";
  info->GUIItems[1].Help = "Integer radius along the Y axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.";
  info->GUIItems[1].Hints = "1 10 1";

  info->GUIItems[2].Label = "Radius Z";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "2";
  info->GUIItems[2].Help = "Integer radius along the Z axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.";
  info->GUIItems[2].Hints = "1 10 1";


  info->RequiredZOverlap = atoi( info->GUIItems[ 2 ].CurrentValue ); // radius along z
  
  
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
  
void VV_PLUGIN_EXPORT vvITKMedianInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI   = UpdateGUI;
  info->Name = "Median (ITK)";
  info->TerseDocumentation = "Median Filter ITK";
  info->FullDocumentation = 
    "This filters applies an intensity transform by replacing the value of every pixel with the median value of their neighborhoods. The neighborhood size is defined by a radius";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces  = 1;
  info->PerVoxelMemoryRequired    = 1;

  /* setup the GUI components */
  info->NumberOfGUIItems = 3;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
