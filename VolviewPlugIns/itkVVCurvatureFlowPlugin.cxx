/* perform a simple threshold filter */

#include "vtkVVPluginAPI.h"
#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImportImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageRegionConstIterator.h"


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
  typedef unsigned char                  InputPixelType;
  typedef float                          InternalPixelType;

  const unsigned int Dimension = 3;

  typedef itk::Image< InputPixelType,    Dimension > InputImageType;
  typedef itk::Image< InternalPixelType, Dimension > InternalImageType;


  // Instantiate the ImportImageFilter
  // This filter is used for building an ITK image using 
  // the data passed in a buffer.
  typedef itk::ImportImageFilter< InputPixelType, 
                                  Dimension       > ImportFilterType;

  ImportFilterType::Pointer importFilter = ImportFilterType::New();

  typedef ImportFilterType::SizeType      SizeType;
  typedef ImportFilterType::IndexType     IndexType;

  SizeType   size;
  IndexType  start;

  double     origin[3];
  double     spacing[3];

  for(unsigned int i=0; i<3; i++)
    {
    size[i]     =  info->InputVolumeDimensions[i];
    origin[i]   =  info->InputVolumeOrigin[i];
    spacing[i]  =  info->InputVolumeSpacing[i];
    start[i]    =  0;
    }

  typedef ImportFilterType::RegionType    RegionType;

  RegionType region;

  region.SetIndex( start );
  region.SetSize(  size  );
 
  importFilter->SetSpacing( spacing );
  importFilter->SetOrigin(  origin  );
  importFilter->SetRegion(  region  );

  const unsigned int totalNumberOfPixels = region.GetNumberOfPixels();

  const bool         importFilterWillDeleteTheInputBuffer = false;

  importFilter->SetImportPointer( static_cast< InputPixelType * >( pds->inData ), 
                                  totalNumberOfPixels,
                                  importFilterWillDeleteTheInputBuffer );

  // Instantiate the CastImageFilter
  // This filter is used for converting the pixel type from the input
  // data buffer into 'float' pixel types. This is required because
  // the CurvatureFlowImageFilter only operates on float and double
  // pixel types.
  typedef itk::CastImageFilter< InputImageType, 
                                InternalImageType > CastFilterType;

  CastFilterType::Pointer castFilter = CastFilterType::New();

  castFilter->SetInput( importFilter->GetOutput() );


  // Instantiate the filter type
  typedef itk::CurvatureFlowImageFilter< 
                  InternalImageType,InternalImageType > FilterType;

  FilterType::Pointer curvaturFlowFilter = FilterType::New();

  // Set the parameters on it
  curvaturFlowFilter->SetNumberOfIterations(
    atoi(info->GUIItems[0].CurrentValue));
  curvaturFlowFilter->SetTimeStep(
    atof(info->GUIItems[1].CurrentValue));
  curvaturFlowFilter->SetInput( castFilter->GetOutput() );

  curvaturFlowFilter->Update();

/*
  // setup progress
  vtkCallbackCommand *cc = vtkCallbackCommand::New();
  cc->SetCallback(vvCurvatureFlowProgress);
  cc->SetClientData(inf);
  ig->AddObserver(vtkCommand::ProgressEvent,cc);
  cc->Delete();
  
*/

  InternalImageType::ConstPointer outputImage = curvaturFlowFilter->GetOutput();

  typedef itk::ImageRegionConstIterator< InternalImageType >  OutputIteratorType;

  OutputIteratorType ot( outputImage, outputImage->GetBufferedRegion() );

  InputPixelType * outData = (InputPixelType *)(pds->outData);

  ot.GoToBegin(); 
  while( !ot.IsAtEnd() )
    {
    *outData = static_cast< InputPixelType >( ot.Get() );
    ++ot;
    ++outData;
    }

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
  info->TerseDocumentation = "Anisotropic diffusion smoothing";
  info->FullDocumentation = 
    "This filter applies an edge-preserving smoothing to a volume by computing the evolution of an anisotropic diffusion partial differential equation. Diffusion is regulated by the curvature of iso-contours in the image. This filter processes the whole image in one piece, and does not change the dimensions, data type, or spacing of the volume.";
  info->SupportsInPlaceProcessing = 0;
  info->SupportsProcessingPieces = 0;
  info->RequiredZOverlap = 0;
  
  /* setup the GUI components */
  info->NumberOfGUIItems = 2;
  info->GUIItems = (vtkVVGUIItem *)malloc(info->NumberOfGUIItems*sizeof(vtkVVGUIItem));
}

}
