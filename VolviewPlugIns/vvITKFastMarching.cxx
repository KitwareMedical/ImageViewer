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
      typedef  FilterType::NodeContainer            NodeContainer;
      typedef  FilterType::NodeType                 NodeType;
      NodeContainer::Pointer seeds = NodeContainer::New();
      seeds->Initialize();
      const double seedValue = 0.0;
      ImageType::IndexType seedPosition;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing Fast Marching...");
      // Set the parameters on it
      module.GetFilter()->SetStoppingValue(  atof( info->GUIItems[ 0 ].CurrentValue) );
      seedPosition[0] =                      atoi( info->GUIItems[ 1 ].CurrentValue);      
      seedPosition[1] =                      atoi( info->GUIItems[ 2 ].CurrentValue);      
      seedPosition[2] =                      atoi( info->GUIItems[ 3 ].CurrentValue);      
      module.GetFilter()->SetNormalizationFactor(  atof( info->GUIItems[ 4 ].CurrentValue) );
      NodeType node;
      node.SetValue( seedValue );
      node.SetIndex( seedPosition );
      seeds->InsertElement( 0, node );
      module.GetFilter()->SetTrialPoints( seeds );
      ImageType::SizeType size;
      size[0] = info->OutputVolumeDimensions[0];
      size[1] = info->OutputVolumeDimensions[1];
      size[2] = info->OutputVolumeDimensions[2];
      module.GetFilter()->SetOutputSize( size );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::FastMarchingImageFilter< ImageType,  ImageType >   FilterType;
      typedef  FilterType::NodeContainer            NodeContainer;
      typedef  FilterType::NodeType                 NodeType;
      NodeContainer::Pointer seeds = NodeContainer::New();
      seeds->Initialize();
      const double seedValue = 0.0;
      ImageType::IndexType seedPosition;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Computing Fast Marching...");
      // Set the parameters on it
      module.GetFilter()->SetStoppingValue(  atof( info->GUIItems[ 0 ].CurrentValue) );
      seedPosition[0] =                      atoi( info->GUIItems[ 1 ].CurrentValue);      
      seedPosition[1] =                      atoi( info->GUIItems[ 2 ].CurrentValue);      
      seedPosition[2] =                      atoi( info->GUIItems[ 3 ].CurrentValue);      
      module.GetFilter()->SetNormalizationFactor(  atof( info->GUIItems[ 4 ].CurrentValue) );
      NodeType node;
      node.SetValue( seedValue );
      node.SetIndex( seedPosition );
      seeds->InsertElement( 0, node );
      module.GetFilter()->SetTrialPoints( seeds );
      ImageType::SizeType size;
      size[0] = info->OutputVolumeDimensions[0];
      size[1] = info->OutputVolumeDimensions[1];
      size[2] = info->OutputVolumeDimensions[2];
      module.GetFilter()->SetOutputSize( size );
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
  info->GUIItems[0].Help = "Defines a stopping value for the time up to which the front propagation will be computed."; 
  info->GUIItems[0].Hints = "1 100 1";

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

  info->GUIItems[4].Label = "Speed image normalization factor";
  info->GUIItems[4].GUIType = VV_GUI_SCALE;
  info->GUIItems[4].Default = "1";
  info->GUIItems[4].Help = "Factor to be used for dividing the pixel values of the speed image. This allows to use images of integer pixel type for representing the speed. The normalization should map the values of the integer image into the range [0,1]";
  info->GUIItems[4].Hints = "1 1000.0 1.0";



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
  info->NumberOfGUIItems = 5;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
