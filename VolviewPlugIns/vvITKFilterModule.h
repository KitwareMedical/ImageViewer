/** Generic interface for protocol communication between an ITK filter
    and the VolView Plugin Interface */

#ifndef _vvITKFilterModule_h
#define _vvITKFilterModule_h

#include "vvITKFilterModuleBase.h"

#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkImportImageFilter.h"
#include "itkImageRegionConstIterator.h"


namespace VolView
{

namespace PlugIn
{

template <class TFilterType >
class FilterModule : public FilterModuleBase {

public:

   // Instantiate the image types
  typedef TFilterType                             FilterType;
  typedef typename FilterType::InputImageType     InputImageType;
  typedef typename FilterType::OutputImageType    OutputImageType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename OutputImageType::PixelType     OutputPixelType;

  itkStaticConstMacro( Dimension, unsigned int, 
         itk::GetImageDimension< InputImageType >::ImageDimension );

  // Instantiate the ImportImageFilter
  // This filter is used for building an ITK image using 
  // the data passed in a buffer.
  typedef itk::ImportImageFilter< InputPixelType, 
                                  Dimension       > ImportFilterType;

  typedef ImportFilterType::SizeType      SizeType;
  typedef ImportFilterType::IndexType     IndexType;
  typedef ImportFilterType::RegionType    RegionType;



public:

  /**  Constructor */
  FilterModule() 
    {
    m_ImportFilter       = ImportFilterType::New();
    m_Filter             = FilterType::New();
    m_Filter->ReleaseDataFlagOn();
    }



  /**  Destructor */
  ~FilterModule() 
    {
    }


  FilterType * GetFilter()
  {
    return m_Filter.GetPointer();
  }


  /**  ProcessData performs the actual filtering on the data */
  void 
  ProcessData( const vtkVVProcessDataStruct * pds )
  {

    SizeType   size;
    IndexType  start;

    double     origin[3];
    double     spacing[3];

    size[0]     =  this->GetPluginInfo()->InputVolumeDimensions[0];
    size[1]     =  this->GetPluginInfo()->InputVolumeDimensions[1];
    size[2]     =  pds->NumberOfSlicesToProcess;

    for(unsigned int i=0; i<3; i++)
      {
      origin[i]   =  this->GetPluginInfo()->InputVolumeOrigin[i];
      spacing[i]  =  this->GetPluginInfo()->InputVolumeSpacing[i];
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

    const unsigned int numberOfPixelsPerSlice = size[0] * size[1];

    InputPixelType *   dataBlockStart = 
                          static_cast< InputPixelType * >( pds->inData )  
                        + numberOfPixelsPerSlice * pds->StartSlice;

    m_ImportFilter->SetImportPointer( dataBlockStart, 
                                      totalNumberOfPixels,
                                      importFilterWillDeleteTheInputBuffer );

    m_Filter->SetInput( m_ImportFilter->GetOutput() );

    // Set the Observer for updating progress in the GUI
    m_Filter->AddObserver( itk::ProgressEvent(), this->GetCommandObserver() );

    // Execute the filter
    m_Filter->Update();

    // Copy the data (with casting) to the output buffer provided by the Plug In API
    typename OutputImageType::ConstPointer outputImage =
                                               m_Filter->GetOutput();

    typedef itk::ImageRegionConstIterator< OutputImageType >  OutputIteratorType;

    OutputIteratorType ot( outputImage, outputImage->GetBufferedRegion() );

    OutputPixelType * outData = (OutputPixelType *)(pds->outData);

    ot.GoToBegin(); 
    while( !ot.IsAtEnd() )
      {
      *outData = ot.Get();
      ++ot;
      ++outData;
      }

  } // end of ProcessData


private:
    typename ImportFilterType::Pointer    m_ImportFilter;
    typename FilterType::Pointer          m_Filter;
};


} // end namespace PlugIn

} // end namespace VolView

#endif
