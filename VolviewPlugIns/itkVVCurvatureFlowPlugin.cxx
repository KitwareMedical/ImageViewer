/* perform a simple threshold filter */

#include "vtkVVPluginAPI.h"

#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImportImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageRegionConstIterator.h"


template <class TPixelType >
class FilterModule {

public:

   // Instantiate the image types
  typedef TPixelType                     InputPixelType;
  typedef float                          InternalPixelType;

  itkStaticConstMacro( Dimension, unsigned int, 3);

  typedef itk::Image< InputPixelType,    Dimension > InputImageType;
  typedef itk::Image< InternalPixelType, Dimension > InternalImageType;

  // Instantiate the ImportImageFilter
  // This filter is used for building an ITK image using 
  // the data passed in a buffer.
  typedef itk::ImportImageFilter< InputPixelType, 
                                  Dimension       > ImportFilterType;

  typedef ImportFilterType::SizeType      SizeType;
  typedef ImportFilterType::IndexType     IndexType;
  typedef ImportFilterType::RegionType    RegionType;

  // Instantiate the CastImageFilter
  // This filter is used for converting the pixel type from the input
  // data buffer into 'float' pixel types. This is required because
  // the CurvatureFlowImageFilter only operates on float and double
  // pixel types.
  typedef itk::CastImageFilter< InputImageType, 
                                InternalImageType > CastFilterType;


  // Instantiate the filter type
  typedef itk::CurvatureFlowImageFilter< 
                                InternalImageType, 
                                InternalImageType > CurvatureFlowFilterType;


public:

  /**  Constructor */
  FilterModule() 
    {
    m_ImportFilter       = ImportFilterType::New();
    m_CastFilter         = CastFilterType::New();
    m_CurvaturFlowFilter = CurvatureFlowFilterType::New();
    }

  /**  Destructor */
  ~FilterModule() 
    {
    }


  /**  ProcessData performs the actual filtering on the data */
  void 
  ProcessData( vtkVVPluginInfo * info, const vtkVVProcessDataStruct * pds )
  {

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


    RegionType region;

    region.SetIndex( start );
    region.SetSize(  size  );
   
    m_ImportFilter->SetSpacing( spacing );
    m_ImportFilter->SetOrigin(  origin  );
    m_ImportFilter->SetRegion(  region  );

    const unsigned int totalNumberOfPixels = region.GetNumberOfPixels();

    const bool         importFilterWillDeleteTheInputBuffer = false;

    m_ImportFilter->SetImportPointer( static_cast< InputPixelType * >( pds->inData ), 
                                    totalNumberOfPixels,
                                    importFilterWillDeleteTheInputBuffer );


    m_CastFilter->SetInput( m_ImportFilter->GetOutput() );


    // Set the parameters on it
    m_CurvaturFlowFilter->SetNumberOfIterations(
      atoi(info->GUIItems[0].CurrentValue));
    m_CurvaturFlowFilter->SetTimeStep(
      atof(info->GUIItems[1].CurrentValue));
    m_CurvaturFlowFilter->SetInput( m_CastFilter->GetOutput() );

    m_CurvaturFlowFilter->Update();

  /*
    // setup progress
    vtkCallbackCommand *cc = vtkCallbackCommand::New();
    cc->SetCallback(vvCurvatureFlowProgress);
    cc->SetClientData(inf);
    ig->AddObserver(vtkCommand::ProgressEvent,cc);
    cc->Delete();
    
  */

    typename InternalImageType::ConstPointer outputImage =
                                               m_CurvaturFlowFilter->GetOutput();

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

  } // end of ProcessData


private:
    typename CastFilterType::Pointer   m_CastFilter;
    typename ImportFilterType::Pointer m_ImportFilter;

    typename CurvatureFlowFilterType::Pointer m_CurvaturFlowFilter;

};



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

  switch( info->InputVolumeScalarType )
    {
    case VTK_UNSIGNED_CHAR:
      {
      FilterModule< unsigned char > module;
      module.ProcessData( info, pds );
      break; 
      }
    case VTK_UNSIGNED_SHORT:
      {
      FilterModule< unsigned short > module;
      module.ProcessData( info, pds );
      break; 
      }
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
