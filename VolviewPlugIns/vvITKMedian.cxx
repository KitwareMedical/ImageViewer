/* perform an intensity transformation by computing 
   the median value of a neighborhood  */

#include "vvITKFilterModule.h"

#include "itkMedianImageFilter.h"

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{

  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  const unsigned int Dimension = 3;

  itk::Size< Dimension > radius;
  radius[0] = atoi( info->GetGUIProperty(info, 0, VVP_GUI_VALUE ) );
  radius[1] = atoi( info->GetGUIProperty(info, 1, VVP_GUI_VALUE ) );
  radius[2] = atoi( info->GetGUIProperty(info, 2, VVP_GUI_VALUE ) );

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
      module.SetPluginInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median filter...");
      module.GetFilter()->SetRadius( radius );
      module.ProcessData( pds  );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      typedef  unsigned short                       PixelType;
      typedef  itk::Image< PixelType, Dimension >   ImageType; 
      typedef  itk::MedianImageFilter< ImageType,  ImageType >   FilterType;
      VolView::PlugIn::FilterModule< FilterType > module;
      module.SetPluginInfo( info );
      module.SetUpdateMessage("Transforming intensities with a Median filter...");
      module.GetFilter()->SetRadius( radius );
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
  char tmp[1024];
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->SetGUIProperty(info, 0, VVP_GUI_LABEL, "Radius X");
  info->SetGUIProperty(info, 0, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 0, VVP_GUI_DEFAULT, "2");
  info->SetGUIProperty(info, 0, VVP_GUI_HELP, "Integer radius along the X axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.");
  info->SetGUIProperty(info, 0, VVP_GUI_HINTS , "1 5 1");

  info->SetGUIProperty(info, 1, VVP_GUI_LABEL, "Radius Y");
  info->SetGUIProperty(info, 1, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 1, VVP_GUI_DEFAULT, "2");
  info->SetGUIProperty(info, 1, VVP_GUI_HELP, "Integer radius along the Y axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.");
  info->SetGUIProperty(info, 1, VVP_GUI_HINTS , "1 5 1");

  info->SetGUIProperty(info, 2, VVP_GUI_LABEL, "Radius Z");
  info->SetGUIProperty(info, 2, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 2, VVP_GUI_DEFAULT, "2");
  info->SetGUIProperty(info, 2, VVP_GUI_HELP, "Integer radius along the Z axis of the neighborhood used to compute the Median. The neighborhood is a rectangular region that extends this number of pixels around the pixel being computed. Setting a radius of 2 will use a neighborhood of size 5.");
  info->SetGUIProperty(info, 2, VVP_GUI_HINTS , "1 5 1");

  const char * text = info->GetGUIProperty(info,2,VVP_GUI_VALUE);
  if( text )
    {
    sprintf(tmp,"%d", atoi( text ) ); 
    info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP, tmp);
    }
  else
    {
    info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP, "0");
    }
  
  
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
  info->SetProperty(info, VVP_NAME, "Median (ITK)");
  info->SetProperty(info, VVP_TERSE_DOCUMENTATION,
          "Replace each voxel with the median of its neighborhood");
  info->SetProperty(info, VVP_FULL_DOCUMENTATION,
    "This filters applies an intensity transform by replacing the value of every pixel with the median value of their neighborhoods. The neighborhood size is defined by a radius");
  info->SetProperty(info, VVP_SUPPORTS_IN_PLACE_PROCESSING, "0");
  info->SetProperty(info, VVP_SUPPORTS_PROCESSING_PIECES,   "1");
  info->SetProperty(info, VVP_NUMBER_OF_GUI_ITEMS,          "3");
  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP,           "0");
  info->SetProperty(info, VVP_PER_VOXEL_MEMORY_REQUIRED,    "1");
}

}
