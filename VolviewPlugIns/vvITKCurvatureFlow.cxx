/* perform smoothing using an anisotropic diffusion filter */

#include "vvITKFilterModuleWithCasting.h"

#include "itkCurvatureFlowImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;

  typedef   float       InternalPixelType;
  typedef   itk::Image< InternalPixelType,Dimension > InternalImageType; 

  typedef   itk::CurvatureFlowImageFilter< 
                                InternalImageType,  
                                InternalImageType >   FilterType;
 
  const unsigned int numberOfIterations = atoi(  info->GetGUIProperty(info, 0, VVP_GUI_VALUE ) );
  const float        timeStep           = atof(  info->GetGUIProperty(info, 1, VVP_GUI_VALUE ) );

  try 
  {
  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      VolView::PlugIn::FilterModuleWithCasting< unsigned char, FilterType > module;
      module.SetPluginInfo( info );
      module.SetUpdateMessage("Smoothing with Curvature Flow...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations( numberOfIterations );
      module.GetFilter()->SetTimeStep( timeStep );
      // Execute the filter
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      VolView::PlugIn::FilterModuleWithCasting< unsigned short, FilterType > module;
      module.SetPluginInfo( info );
      module.SetUpdateMessage("Smoothing with Curvature Flow...");
      // Set the parameters on it
      module.GetFilter()->SetNumberOfIterations( numberOfIterations );
      module.GetFilter()->SetTimeStep( timeStep );
      // Execute the filter
      module.ProcessData( pds );
      break; 
      }
    }
  }
  catch( itk::ExceptionObject & except )
  {
    info->SetProperty( info, VVP_ERROR, except.what() ); 
    return -1;
  }
  return 0;
}


static int UpdateGUI(void *inf)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->SetGUIProperty(info, 0, VVP_GUI_LABEL, "Number of Iterations");
  info->SetGUIProperty(info, 0, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 0, VVP_GUI_DEFAULT, "5");
  info->SetGUIProperty(info, 0, VVP_GUI_HELP, "Number of times that the diffusion approximation will be computed. The more iterations, the stronger the smoothing");
  info->SetGUIProperty(info, 0, VVP_GUI_HINTS , "1 100 1");

  info->SetGUIProperty(info, 1, VVP_GUI_LABEL, "Time Step");
  info->SetGUIProperty(info, 1, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 1, VVP_GUI_DEFAULT, "0.125");
  info->SetGUIProperty(info, 1, VVP_GUI_HELP, "Discretization of time for approximating the diffusion process.");
  info->SetGUIProperty(info, 1, VVP_GUI_HINTS , "0.01 1.0 0.005");

  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP, "0");
  
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
  
void VV_PLUGIN_EXPORT vvITKCurvatureFlowInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->SetProperty(info, VVP_NAME, "Curvature Flow (ITK)");
  info->SetProperty(info, VVP_TERSE_DOCUMENTATION,
                                 "Anisotropic diffusion smoothing");
  info->SetProperty(info, VVP_FULL_DOCUMENTATION,
    "This filter applies an edge-preserving smoothing to a volume by computing the evolution of an anisotropic diffusion partial differential equation. Diffusion is regulated by the curvature of iso-contours in the image. This filter processes the whole image in one piece, and does not change the dimensions, data type, or spacing of the volume.");
  info->SetProperty(info, VVP_SUPPORTS_IN_PLACE_PROCESSING, "0");
  info->SetProperty(info, VVP_SUPPORTS_PROCESSING_PIECES,   "0");
  info->SetProperty(info, VVP_NUMBER_OF_GUI_ITEMS,          "2");
  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP,           "0");
  info->SetProperty(info, VVP_PER_VOXEL_MEMORY_REQUIRED,    "8");
  
}

}

