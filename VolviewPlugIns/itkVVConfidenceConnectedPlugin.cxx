/* perform smoothing using an anisotropic diffusion filter */

#include "itkVVFilterModule.h"

#include "itkConfidenceConnectedImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;

  typedef   float       InternalPixelType;
  typedef   itk::Image< InternalPixelType,Dimension > InternalImageType; 

  typedef   itk::ConfidenceConnectedImageFilter< 
                                InternalImageType,  
                                InternalImageType >   FilterType;
 
  typedef  InternalImageType::IndexType   IndexType;

  IndexType seed;

  try 
  {
  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      FilterModule< unsigned char, FilterType > module;
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
      module.SetNeedCasting( true );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      FilterModule< unsigned short, FilterType > module;
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
      module.SetNeedCasting( true );
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
  info->Name = "Curvature Flow Anisotropic Diffusion";
  info->TerseDocumentation = "Anisotropic diffusion smoothing";
  info->FullDocumentation = 
    "This filter applies an edge-preserving smoothing to a volume by computing the evolution of an anisotropic diffusion partial differential equation. Diffusion is regulated by the curvature of iso-contours in the image. This filter processes the whole image in one piece, and does not change the dimensions, data type, or spacing of the volume.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 7;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
