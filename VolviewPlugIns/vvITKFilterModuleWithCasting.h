/** Generic interface for protocol communication between an ITK filter
    and the VolView Plugin Interface */

#ifndef _itkVVFilterModuleWithCasting_h
#define _itkVVFilterModuleWithCasting_h

#include "vtkVVPluginAPI.h"

#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkImportImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkCommand.h"

namespace VolView 
{

namespace PlugIn
{

template <class TPixelType, class TFilterType >
class FilterModuleWithCasting {

public:

   // Instantiate the image types
  typedef TPixelType                     InputPixelType;
  typedef TFilterType                    FilterType;

  typedef typename FilterType::InputImageType     InternalImageType;
  typedef typename InternalImageType::PixelType   InternalPixelType;

  itkStaticConstMacro( Dimension, unsigned int, 
         itk::GetImageDimension< InternalImageType >::ImageDimension );

  typedef itk::Image< InputPixelType,    Dimension > InputImageType;

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
  // data buffer into the input pixel type of the filter.
  typedef itk::CastImageFilter< InputImageType, 
                                InternalImageType > CastFilterType;


  // Instantiate the filter type
  typedef TFilterType           FilterType;


  // Command/Observer intended to update the progress
  typedef itk::MemberCommand< FilterModuleWithCasting >  CommandType;


public:

  /**  Constructor */
  FilterModuleWithCasting() 
    {
    m_ImportFilter       = ImportFilterType::New();
    m_CastFilter         = CastFilterType::New();
    m_Filter             = FilterType::New();
    m_CommandObserver    = CommandType::New();
    m_Info               = 0;
    m_UpdateMessage      = "Processing the filter...";
    m_CastFilter->ReleaseDataFlagOn();
    m_Filter->ReleaseDataFlagOn();
    }



  /**  Destructor */
  ~FilterModuleWithCasting() 
    {
    }


  FilterType * GetFilter()
  {
    return m_Filter.GetPointer();
  }



  void SetUpdateMessage( const char * message )
  {
     m_UpdateMessage = message;
  }

  void 
  ProgressUpdate( itk::Object * caller, const itk::EventObject & event )
  {

    if( typeid( itk::ProgressEvent ) != typeid( event ) )
      {
      return;
      }

    itk::ProcessObject::Pointer process =
              dynamic_cast< itk::ProcessObject *>( caller );

    const float progress = process->GetProgress();

    m_Info->UpdateProgress(m_Info, progress, m_UpdateMessage.c_str()); 

  }



  /**  Set the Plugin Info structure */
  void
  SetPlugInfo( vtkVVPluginInfo * info )
  {
    m_Info = info;
  }

  /**  ProcessData performs the actual filtering on the data */
  void 
  ProcessData( const vtkVVProcessDataStruct * pds )
  {

    SizeType   size;
    IndexType  start;

    double     origin[3];
    double     spacing[3];

    size[0]     =  m_Info->InputVolumeDimensions[0];
    size[1]     =  m_Info->InputVolumeDimensions[1];
    size[2]     =  pds->NumberOfSlicesToProcess;

    for(unsigned int i=0; i<3; i++)
      {
      origin[i]   =  m_Info->InputVolumeOrigin[i];
      spacing[i]  =  m_Info->InputVolumeSpacing[i];
      start[i]    =  0;
      }

    std::cout << "Size = " << size << std::endl;

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

    m_CastFilter->SetInput( m_ImportFilter->GetOutput() );
    m_Filter->SetInput( m_CastFilter->GetOutput() );

    // Set the Observer for updating progress in the GUI
    m_CommandObserver->SetCallbackFunction( this, &FilterModuleWithCasting::ProgressUpdate );
    m_Filter->AddObserver( itk::ProgressEvent(), m_CommandObserver );

    // Execute the filter
    m_Filter->Update();

    // Copy the data (with casting) to the output buffer provided by the Plug In API
    typename InternalImageType::ConstPointer outputImage =
                                               m_Filter->GetOutput();

    typedef itk::ImageRegionConstIterator< InternalImageType >  OutputIteratorType;

    OutputIteratorType ot( outputImage, outputImage->GetBufferedRegion() );

    InputPixelType * outData = (InputPixelType *)(pds->outData);

    ot.GoToBegin(); 
    while( !ot.IsAtEnd() )
      {
      // NOTE: some combination of ClampMacro and 
      // NumericTraits should be used here. 
      // The code below is ok only for unsigned types in the InputPixelType...
      InternalPixelType value = ot.Get();
      if( value < itk::NumericTraits<InternalPixelType>::Zero ) 
        {
        value = itk::NumericTraits<InternalPixelType>::Zero;
        }
      *outData = static_cast< InputPixelType >( value );
      ++ot;
      ++outData;
      }

  } // end of ProcessData


private:
    typename ImportFilterType::Pointer    m_ImportFilter;
    typename CastFilterType::Pointer      m_CastFilter;
    typename FilterType::Pointer          m_Filter;

    typename CommandType::Pointer         m_CommandObserver    ;

    vtkVVPluginInfo                     * m_Info;

    std::string                           m_UpdateMessage;
  
};

} // end of namespace PlugIn

} // end of namespace Volview

#endif
