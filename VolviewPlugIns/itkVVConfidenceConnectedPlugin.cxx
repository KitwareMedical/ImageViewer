/* perform segmentation using the confidence connected image filter */

#include "itkVVFilterModule.h"

#include "itkConfidenceConnectedImageFilter.h"

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
      typedef  itk::ConfidenceConnectedImageFilter< ImageType,  ImageType >   FilterType;
      IndexType seed;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Curvature Flow...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations( atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetMultiplier(         atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetReplaceValue(       atoi( info->GUIItems[ 2 ].CurrentValue) );
      module.GetFilter()->SetInitialNeighborhoodRadius( 
                                                 atoi( info->GUIItems[ 3 ].CurrentValue) );
      seed[0] =                                  atoi( info->GUIItems[ 4 ].CurrentValue);      
      seed[1] =                                  atoi( info->GUIItems[ 5 ].CurrentValue);      
      seed[2] =                                  atoi( info->GUIItems[ 6 ].CurrentValue);      
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
      typedef  itk::ConfidenceConnectedImageFilter< ImageType,  ImageType >   FilterType;
      IndexType seed;
      FilterModule< FilterType > module;
      module.SetPlugInfo( info );
      module.SetUpdateMessage("Smoothing with Curvature Flow...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations( atoi( info->GUIItems[ 0 ].CurrentValue) );
      module.GetFilter()->SetMultiplier(         atof( info->GUIItems[ 1 ].CurrentValue) );
      module.GetFilter()->SetReplaceValue(       atoi( info->GUIItems[ 2 ].CurrentValue) );
      module.GetFilter()->SetInitialNeighborhoodRadius( 
                                                 atoi( info->GUIItems[ 3 ].CurrentValue) );
      seed[0] =                                  atoi( info->GUIItems[ 4 ].CurrentValue);      
      seed[1] =                                  atoi( info->GUIItems[ 5 ].CurrentValue);      
      seed[2] =                                  atoi( info->GUIItems[ 6 ].CurrentValue);      
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

  info->GUIItems[0].Label = "Number of Iterations";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "5";
  info->GUIItems[0].Help = "Number of times that region growing process should be repeated. If initial statistical estimations are good, further iterations may improve the segmentation. After a certain number of iterations the result will not change. ";
  info->GUIItems[0].Hints = "1 20 1";

  info->GUIItems[1].Label = "Variance Multiplier";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "2.5";
  info->GUIItems[1].Help = "Factor that will multiply the variance of the pixels region in order to define the intensity range for which new pixels will be accepted in the region.";
  info->GUIItems[1].Hints = "0.1 10.0 0.1";

  info->GUIItems[2].Label = "Replace Value";
  info->GUIItems[2].GUIType = VV_GUI_SCALE;
  info->GUIItems[2].Default = "255";
  info->GUIItems[2].Help = "Value to assign to the binary mask of the segmented region. The rest of the image will be set to zero.";
  info->GUIItems[2].Hints = "1 255.0 1.0";

  info->GUIItems[3].Label = "Initial Neighborhood Radius";
  info->GUIItems[3].GUIType = VV_GUI_SCALE;
  info->GUIItems[3].Default = "2";
  info->GUIItems[3].Help = "Size of the initial neighborhood used to compute the statistics of the region. If the region in which the seed point is placed happens to be a homogeneous intensity distribution, increasing this radius will safely improve the statistical estimation of mean and variance. Make sure that the radius is not large enough to make contours participate in the computation of the estimation. That is, from the seed point to the nearest important edge, there should be a distance larger than this radius.";
  info->GUIItems[3].Hints = "1 20.0 1.0";

  info->GUIItems[4].Label = "X coordinate of the seed";
  info->GUIItems[4].GUIType = VV_GUI_SCALE;
  info->GUIItems[4].Default = "0";
  info->GUIItems[4].Help = "X coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[4].Hints = "1 1000.0 1.0";

  info->GUIItems[5].Label = "Y coordinate of the seed";
  info->GUIItems[5].GUIType = VV_GUI_SCALE;
  info->GUIItems[5].Default = "0";
  info->GUIItems[5].Help = "Y coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[5].Hints = "1 1000.0 1.0";

  info->GUIItems[6].Label = "Z coordinate of the seed";
  info->GUIItems[6].GUIType = VV_GUI_SCALE;
  info->GUIItems[6].Default = "0";
  info->GUIItems[6].Help = "Z coordinate of the seed point. The seed should be placed in the middle of the region to be segmented.";
  info->GUIItems[6].Hints = "1 1000.0 1.0";

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
  
void VV_PLUGIN_EXPORT vvConfidenceConnectedInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Confidence Connected Segmentation ";
  info->TerseDocumentation = "Confidence Connected Segmentation";
  info->FullDocumentation = 
    "This filter applies an region growing algorithm for segmentation. The criterion for including new pixels in the region is defined by an intensity range around the mean value of the pixels existing in the region. The extent of the intensity interval is computed as the product of the variance and a multiplier provided by the user. The coordinates of a seed point are used as the initial position for start growing the region.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;

  // Number of bytes required in intermediate memory per voxel
  info->PerVoxelMemoryRequired = 1; 
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 7;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
