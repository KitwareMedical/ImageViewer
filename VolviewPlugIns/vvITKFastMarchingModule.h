/** Generic interface for protocol communication between an ITK filter
    and the VolView Plugin Interface */

#ifndef _itkVVFastMarchingModule_h
#define _itkVVFastMarchingModule_h

#include "vtkVVPluginAPI.h"

#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkImportImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkCommand.h"

namespace VolView 
{

namespace PlugIn
{

template <class TInputPixelType >
class FastMarchingModule {

public:

   // Pixel type of the input buffer
  typedef TInputPixelType                InputPixelType;
  typedef float                          RealPixelType;
  typedef unsigned char                  SpeedPixelType;
  typedef unsigned char                  OutputPixelType;

  itkStaticConstMacro( Dimension, unsigned int, 3 );

  typedef itk::Image< InputPixelType,  Dimension >  InputImageType;
  typedef itk::Image< RealPixelType,   Dimension >  RealImageType;
  typedef itk::Image< SpeedPixelType,  Dimension >  SpeedImageType;
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

  // Instantiate the ImportImageFilter
  // This filter is used for building an ITK image using 
  // the data passed in a buffer.
  typedef itk::ImportImageFilter< InputPixelType, 
                                  Dimension       > ImportFilterType;

  typedef ImportFilterType::SizeType      SizeType;
  typedef ImportFilterType::IndexType     IndexType;
  typedef ImportFilterType::RegionType    RegionType;

  // Instantiate the GradientMagnitude filter.
  // This filter is the first stage in 
  // the generation of the speed image.
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter< 
                                InputImageType, 
                                RealImageType > GradientMagnitudeFilterType;

  // Instantiate the Sigmoid filter.
  // This is the second stage of the speed image generation.
  // This filter inverts the intensities of the gradient 
  // magnitude in order to generate low values wherever the
  // gradient magnitude is high. For the sake of reducing 
  // memory consumption, the output of this filter is scaled
  // to 8 bits.
  typedef itk::SigmoidImageFilter<  RealImageType, 
                                    SpeedImageType >  SigmoidFilterType;
  


  // Instantiation of the FastMarching filter.
  // This filter computes the propagation of the fron starting
  // at the seed points.  The input of the filter is the speed image.
  // The output is a time-crossing map.
  typedef itk::FastMarchingImageFilter< RealImageType,
                                        SpeedImageType >  FastMarchingFilterType;

   // Instantiation of the Intensity windowing filter.
   // This filter is used to remove infinite times from the non-visited
   // pixels of the time-crossing map. This pixels are set to the stopping value.
   typedef itk::IntensityWindowingImageFilter< 
                                        RealImageType,
                                        OutputImageType >
                                                    IntensityWindowingFilterType;
                                               



   typedef typename FastMarchingFilterType::NodeType         NodeType;
   typedef typename FastMarchingFilterType::NodeContainer    NodeContainerType;



  // Command/Observer intended to update the progress
  typedef itk::MemberCommand< FastMarchingModule >  CommandType;


public:
    FastMarchingModule();
   ~FastMarchingModule();

    void ClearSeeds();
    void AddSeed( float x, float y, float z );
    void SetStoppingValue( float value );
    void SetSigma( float value );
    void SetLowestBorderValue( float value );
    void SetLowestBasinValue(  float value );

    void ProcessData( const vtkVVProcessDataStruct * pds );
    void ProgressUpdate( itk::Object * caller, const itk::EventObject & event );
    void SetPlugInfo( vtkVVPluginInfo * info );

private:
    typename ImportFilterType::Pointer              m_ImportFilter;
    typename GradientMagnitudeFilterType::Pointer   m_GradientMagnitudeFilter;
    typename SigmoidFilterType::Pointer             m_SigmoidFilter;
    typename FastMarchingFilterType::Pointer        m_FastMarchingFilter;
    typename IntensityWindowingFilterType::Pointer  m_IntensityWindowingFilter;

    typename NodeContainerType::Pointer             m_NodeContainer;
    
    double                                          m_InitialSeedValue;
 
    unsigned long                                   m_CurrentNumberOfSeeds;

    typename CommandType::Pointer                   m_CommandObserver;

    vtkVVPluginInfo                               * m_Info;

    std::string                                     m_UpdateMessage;
  
    float                                          m_LowestBasinValue;
    float                                          m_LowestBorderValue;

};

} // end of namespace PlugIn

} // end of namespace Volview

#endif
