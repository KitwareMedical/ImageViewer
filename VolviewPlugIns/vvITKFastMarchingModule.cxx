/* This module encapsulates the full preprocessing required for
   applying the FastMarching image filter for segmenting the 
   volume.  It requires seed points and the original image as inputs. */


#include "vvITKFastMarchingModule.txx"


static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  try 
  {
  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      typedef  unsigned char                              PixelType;
      typedef VolView::PlugIn::FastMarchingModule< 
                                            PixelType >   ModuleType;
      ModuleType  module;

      module.SetPlugInfo( info );
      module.SetStoppingValue(  atof( info->GUIItems[ 0 ].CurrentValue) );
      module.AddSeed( atof( info->GUIItems[ 1 ].CurrentValue),
                      atof( info->GUIItems[ 2 ].CurrentValue),
                      atof( info->GUIItems[ 3 ].CurrentValue)  );
      module.SetSigma(             atof( info->GUIItems[4].CurrentValue ) );
      module.SetLowestBasinValue(  atof( info->GUIItems[5].CurrentValue ) );
      module.SetLowestBorderValue( atof( info->GUIItems[6].CurrentValue ) );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      // ... when ready, copy/paste from U-Char implementation.
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
  info->GUIItems[0].Default = "100.0";
  info->GUIItems[0].Help = "Defines a stopping value for the time up to which the front propagation will be computed."; 
  info->GUIItems[0].Hints = "1 500 1";

  info->GUIItems[1].Label = "X coordinate of the seed";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "0";
  info->GUIItems[1].Help = "X coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[1].Hints = "1 1000.0 1.0";

  info->GUIItems[2].Label = "Y coordinate of the seed";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "0";
  info->GUIItems[2].Help = "Y coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[2].Hints = "1 1000.0 1.0";

  info->GUIItems[3].Label = "Z coordinate of the seed";
  info->GUIItems[3].GUIType = VV_GUI_SCALE;
  info->GUIItems[3].Default = "0";
  info->GUIItems[3].Help = "Z coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[3].Hints = "1 1000.0 1.0";

  info->GUIItems[4].Label = "Sigma for gradient magnitude.";
  info->GUIItems[4].GUIType = VV_GUI_SCALE;
  info->GUIItems[4].Default = "1";
  info->GUIItems[4].Help = "The input image is smoothed with a Gaussian during the computation of the Gradient Magnitude. This sigma value should be large enough to attenuate image noise, but not as large as to prevent the level set front from getting close to the edges of objects in the image.";
  info->GUIItems[4].Hints = "0.1 10.0 0.1";

  info->GUIItems[5].Label = "Bottom of basin.";
  info->GUIItems[5].GUIType = VV_GUI_SCALE;
  info->GUIItems[5].Default = "0";
  info->GUIItems[5].Help = "The lowest value of the gradient magnitude in the inside of the region to be segmented. This value will be mapped by the Sigmoid into the fastest propagation in the speed image.";
  info->GUIItems[5].Hints = "0 10.0 0.1";

  info->GUIItems[6].Label = "Lowest of basin border.";
  info->GUIItems[6].GUIType = VV_GUI_SCALE;
  info->GUIItems[6].Default = "6";
  info->GUIItems[6].Help = "The lowest value of the gradient magnitude in the border of the region to be segmented. This value will be mapped by the Sigmoid into the slowest propagation in the speed image.";
  info->GUIItems[6].Hints = "1 50.0 1.0";



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
  
void VV_PLUGIN_EXPORT vvITKFastMarchingModuleInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI   = UpdateGUI;
  info->Name = "Fast Marching Module (ITK)";
  info->TerseDocumentation = "Fast Marching Module";
  info->FullDocumentation =  "This module applies a Fast Marching level set method for segmenting a volume. All the necessary  preprocessing is packaged in this module. This makes it a good choice when you are already familiar with the parameters settings requires for you particular data set. When you are applying FastMarching to a new data set, you may want to rather go step by step using each one the individual filters.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 16; 
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 7;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
