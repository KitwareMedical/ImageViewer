/** Base class, non-templated, for the generic interface for 
    protocol communication between an ITK filter
    and the VolView Plugin Interface */

#ifndef _vvITKFilterModuleBase_h
#define _vvITKFilterModulBasee_h

#include "vtkVVPluginAPI.h"

#include "itkCommand.h"
#include "itkProcessObject.h"

#include <string.h>
#include <stdlib.h>


namespace VolView
{

namespace PlugIn
{

class FilterModuleBase {

public:
  // Command/Observer intended to update the progress
  typedef itk::MemberCommand< FilterModuleBase >  CommandType;

public:

  /**  Constructor */
  FilterModuleBase() 
    {
    m_CommandObserver    = CommandType::New();
    m_Info               = 0;
    m_UpdateMessage      = "Processing the filter...";
    m_CommandObserver->SetCallbackFunction( this, &FilterModuleBase::ProgressUpdate );
    }


  /**  Destructor */
  ~FilterModuleBase() 
    {
    }


  static 
  const char * GetInputVolumeScalarRange( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float lower = info->InputVolumeScalarRange[0];
    const float upper = info->InputVolumeScalarRange[1];
    sprintf(tmp,"%f %f %f", lower, upper, 1.0 ); 
    return tmp;
  }


  static 
  const char * GetInputVolumeScalarTypeRange( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float lower = info->InputVolumeScalarTypeRange[0];
    const float upper = info->InputVolumeScalarTypeRange[1];
    sprintf(tmp,"%f %f %f", lower, upper, 1.0 ); 
    return tmp;
  }


  static 
  const char * GetInputVolumeScalarMinimum( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float value = info->InputVolumeScalarRange[0];
    sprintf(tmp,"%f", value); 
    return tmp;
  }


  static 
  const char * GetInputVolumeScalarMaximum( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float value = info->InputVolumeScalarRange[1];
    sprintf(tmp,"%f", value); 
    return tmp;
  }

  static 
  const char * GetInputVolumeScalarTypeMinimum( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float value = info->InputVolumeScalarTypeRange[0];
    sprintf(tmp,"%f", value); 
    return tmp;
  }

  static 
  const char * GetInputVolumeScalarTypeMaximum( const vtkVVPluginInfo  * info )
  {
    static char tmp[1024];
    const float value = info->InputVolumeScalarTypeRange[1];
    sprintf(tmp,"%f", value); 
    return tmp;
  }


  static 
  const char * GetInputVolumeDimension( const vtkVVPluginInfo  * info, int axis )
  {
    static char tmp[1024];
    const float limit = info->InputVolumeDimensions[ axis ];
    sprintf(tmp,"%f %f %f", 0.0, limit, 1.0 ); 
    return tmp;
  }

  void SetUpdateMessage( const char * message )
  {
     m_UpdateMessage = message;
  }


  CommandType::Pointer
  GetCommandObserver()
  {
     return m_CommandObserver;
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
  SetPluginInfo( vtkVVPluginInfo * info )
  {
    m_Info = info;
  }


  /**  Get the Plugin Info structure */
  vtkVVPluginInfo *
  GetPluginInfo()
  {
    return m_Info;
  }


private:
    CommandType::Pointer         m_CommandObserver;
    vtkVVPluginInfo            * m_Info;
    std::string                  m_UpdateMessage;
};


} // end namespace PlugIn

} // end namespace VolView

#endif
