/* perform a simple threshold filter */

#include "vtkVVPluginAPI.h"
#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkCurvatureFlowImageFilter.h"



extern "C" {  
/*
  void vvCurvatureFlowProgress(vtkObject *obj, unsigned long, void *inf, 
                        void *vtkNotUsed(prog))
  {
    vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;
    vtkSource *src = vtkSource::SafeDownCast(obj);
    if (src)
      {
      info->UpdateProgress(info,src->GetProgress(),"Smoothing with Curvature Flow..."); 
      }
  }
*/
}

static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;
  int *dim = info->InputVolumeDimensions;

  // Instantiate the image types
  typedef itk::Image< unsigned char, 3 > InputImageType;
  typedef itk::Image< float,         3 > InternalImageType;

  // Instantiate the filter type
  typedef itk::CurvatureFlowImageFilter< 
                  InternalImageType,InternalImageType > FilterType;

  FilterType::Pointer curvaturFlowFilter = FilterType::New();

  // Set the parameters on it
  curvaturFlowFilter->SetNumberOfIterations(
    atoi(info->GUIItems[0].CurrentValue));
  curvaturFlowFilter->SetTimeStep(
    atof(info->GUIItems[1].CurrentValue));


/*
  // setup progress
  vtkCallbackCommand *cc = vtkCallbackCommand::New();
  cc->SetCallback(vvCurvatureFlowProgress);
  cc->SetClientData(inf);
  ig->AddObserver(vtkCommand::ProgressEvent,cc);
  cc->Delete();
  
  // setup the input
  vtkImageImport *ii = vtkImageImport::New();
  ii->SetDataExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
  ii->SetWholeExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1);
  ii->SetDataScalarType(info->InputVolumeScalarType);
  ii->SetNumberOfScalarComponents(
    info->InputVolumeNumberOfComponents);
  ii->SetDataOrigin(info->InputVolumeOrigin);
  ii->SetDataSpacing(info->InputVolumeSpacing);
  ii->SetImportVoidPointer(pds->inData);
  ig->SetInput(ii->GetOutput());
  
  // get the output, would be nice to have VTK write directly 
  // into the output buffer but... VTK is often broken in that regard
  // so we will try, but check afterwards to see if it worked
  vtkImageData *od = ig->GetOutput();
  od->UpdateInformation();
  od->SetExtent(0,0,0,0,0,0);
  od->AllocateScalars();
  int size = dim[0] * dim[1] * pds->NumberOfSlicesToProcess * 
    info->InputVolumeNumberOfComponents;
  od->SetExtent(0, dim[0] - 1, 0, dim[1] - 1,
                pds->StartSlice, 
                pds->StartSlice + pds->NumberOfSlicesToProcess - 1);
  od->GetPointData()->GetScalars()->SetVoidArray(pds->outData,size,1);

  // run the filter
  od->SetUpdateExtent(od->GetExtent());
  od->Update();

  // did VTK not use our memory?
  if (od->GetScalarPointer() != pds->outData)
    {
    memcpy(pds->outData, od->GetScalarPointer(), 
           (od->GetPointData()->GetScalars()->GetMaxId() + 1)*
           od->GetPointData()->GetScalars()->GetDataTypeSize());
    }
  
  // clean up
  ii->Delete();
  ig->Delete();

*/
  return 0;
}

static int UpdateGUI(void *inf)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->GUIItems[0].Label = "Number of Iterations";
  info->GUIItems[0].GUIType = VV_GUI_SCALE;
  info->GUIItems[0].Default = "5";
  info->GUIItems[0].Help = "Number of times that the diffusion approximation will be computed. The more iterations, the stronger the smoothing";
  info->GUIItems[0].Hints = "1 40 1";

  info->GUIItems[1].Label = "Time Step";
  info->GUIItems[1].GUIType = VV_GUI_SCALE;
  info->GUIItems[1].Default = "0.125";
  info->GUIItems[1].Help = "Discretization of time for approximating the diffusion process.";
  info->GUIItems[1].Hints = "0.01 1.0 0.01";

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
  
void VV_PLUGIN_EXPORT vvCurvatureFlowInit(vtkVVPluginInfo *info)
{
  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI = UpdateGUI;
  info->Name = "Curvature Flow Anisotropic Diffusion";
  info->TerseDocumentation = "Smooth the volume by computing anisotropic diffusion regulated by Curvature";
  info->FullDocumentation = 
    "This filter applies a edge-preserving smoothing to a volume by computing the evolution of an anisotropic diffusion partial differential equation. Diffusion is regulated by the curvature of iso-contours in the image. This process the whole image in one piece, and does not change the dimensions, data type, or spacing of the volume.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 2;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
