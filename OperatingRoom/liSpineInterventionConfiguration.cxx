//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liSpineInterventionConfiguration.h>

#include <itkFileIOMetaImage.h>
#include <itkWriteMetaImage.h>

#include <liCommandUpdateSpineModelGUI.h>
#include <liCommandGLRedrawModel.h>
#include <liCommandGLRedrawModelFixed.h>
#include <liCommandGLRedrawModelSensitive.h>
#include <liCommandUpdateImageIntensifier.h>
#include <liCommandUpdateRegistratorFieldOfView.h>
#include <liCommandTubesGeneratePointSet.h>

#include <FL/fl_file_chooser.H>
#include <FL/fl_ask.H>




namespace li {
  


///--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
SpineInterventionConfiguration::SpineInterventionConfiguration()
{

  itk::FileIOMetaImageFactory * factory = new itk::FileIOMetaImageFactory;
  itk::ObjectFactoryBase * factoryBase = static_cast<itk::ObjectFactoryBase *>(factory);
  itk::ObjectFactoryBase::RegisterFactory( factoryBase );

  m_FluoroscopyImageReader = ImageReaderType::New();

  m_VolumeImageReader      = VolumeReaderType::New();

  m_OperatingRoomModel = OperatingRoomModel::New();

  m_TableGUI.SetTable( m_OperatingRoomModel->GetSurgeryTable() );

  m_FluoroscopyUnitGUI.SetFluoroscopyUnit(
                        m_OperatingRoomModel->GetFluoroscopyUnit() );



  li::CommandGLRedrawModelSensitive::Pointer commandGLRedrawModelRoom = 
                                            li::CommandGLRedrawModelSensitive::New();

  commandGLRedrawModelRoom->SetModelShape( m_OperatingRoomModel.GetPointer() );

  m_OperatingRoomViewGUI.GetWindow()->GetNotifier()->
              AddObserver( li::GLDrawEvent, commandGLRedrawModelRoom );


  li::CommandGLRedrawModelFixed::Pointer commandGLRedrawModelPatientFixed = 
                                              li::CommandGLRedrawModelFixed::New();

  commandGLRedrawModelPatientFixed->SetModelShape( 
                    m_OperatingRoomModel->GetPatient().GetPointer() );

  m_FluoroscopyUnitViewGUI.fluoroscopyUnitViewWindow->GetNotifier()->
              AddObserver( li::GLDrawEvent, commandGLRedrawModelPatientFixed );


  m_OperatingRoomModel->AddObserver( li::RedrawEvent,
                                     m_OperatingRoomViewGUI.GetRedrawCommand() );


  li::CommandUpdateImageIntensifier::Pointer updateImageIntensifier = 
                                  li::CommandUpdateImageIntensifier::New();

  updateImageIntensifier->SetFluoroscopyUnitView(
                            m_FluoroscopyUnitViewGUI.fluoroscopyUnitViewWindow );


  // Add Observers. The order of addition is relevant

  m_OperatingRoomModel->GetPatient()->AddObserver( 
                              li::PatientMovedEvent,
                              m_FluoroscopyUnitViewGUI.GetRedrawCommand() );
 
  m_OperatingRoomModel->GetPatient()->AddObserver( 
                              li::PatientMovedEvent,
                              m_OperatingRoomViewGUI.GetRedrawCommand()  );

  m_OperatingRoomModel->GetSurgeryTable()->AddObserver( 
                              li::TableMovedEvent,
                              m_OperatingRoomViewGUI.GetRedrawCommand() );

   m_OperatingRoomModel->GetSurgeryTable()->AddObserver( 
                              li::TableMovedEvent,
                              m_FluoroscopyUnitViewGUI.GetRedrawCommand() );
  
  m_OperatingRoomModel->GetFluoroscopyUnit()->AddObserver( 
                              li::FluoroscopyUnitMovedEvent,
                              m_OperatingRoomViewGUI.GetRedrawCommand() );
  
  m_OperatingRoomModel->GetFluoroscopyUnit()->AddObserver(
                              li::FluoroscopyUnitMovedEvent,
                              updateImageIntensifier );

  m_OperatingRoomModel->GetFluoroscopyUnit()->AddObserver( 
                              li::FluoroscopyUnitMovedEvent,
                              m_FluoroscopyUnitViewGUI.GetRedrawCommand() );
 
  m_OperatingRoomModel->GetPatient()->GetVesselsModel()->AddObserver( 
                              li::VesselsChangedEvent,
                              m_FluoroscopyUnitViewGUI.GetRedrawCommand() );
 
  m_OperatingRoomModel->GetPatient()->GetVesselsModel()->AddObserver( 
                              li::VesselsChangedEvent,
                              m_OperatingRoomViewGUI.GetRedrawCommand() );
  
  
  

  m_FluoroscopyUnitViewGUI.SetFluoroscopyImage(
                   m_FluoroscopyImageReader->GetOutput().GetPointer() );

  m_SpineGUI.SetSpineModel( 
      m_OperatingRoomModel->GetPatient()->GetSpineModel().GetPointer() );

  m_VolumeSliceDrawer = VolumeSlicerType::New();

  m_VolumeSliceDrawer->AddObserver( li::VolumeReslicedEvent,
                                    m_VolumeViewGUI.GetRedrawCommand() );

  m_OperatingRoomModel->GetPatient()->GetVesselsModel()->AddObserver( 
                                      li::VesselsChangedEvent,
                                      m_VolumeViewGUI.GetRedrawCommand() );

  m_OperatingRoomModel->GetPatient()->AddObserver( 
                                    li::PatientMovedEvent,
                                    m_VolumeViewGUI.GetRedrawCommand() );

  li::CommandGLRedrawModel::Pointer commandGLRedrawVolumeSlices = 
                                              li::CommandGLRedrawModel::New();

  commandGLRedrawVolumeSlices->SetModel( m_VolumeSliceDrawer.GetPointer() );


  li::CommandGLRedrawModel::Pointer commandGLRedrawModelPatient = 
                                              li::CommandGLRedrawModel::New();

  commandGLRedrawModelPatient->SetModel( 
                    m_OperatingRoomModel->GetPatient().GetPointer() );


  m_VolumeViewGUI.GetNotifier()->
              AddObserver( fltk::GlDrawEvent, commandGLRedrawVolumeSlices );


  m_VolumeViewGUI.GetNotifier()->
              AddObserver( fltk::GlDrawEvent, commandGLRedrawModelPatient );


  li::CommandUpdateSpineModelGUI::Pointer commandUpdateSpineModelGUI =
                                    li::CommandUpdateSpineModelGUI::New();

  commandUpdateSpineModelGUI->SetSpineModelGUI( &m_SpineGUI );

  m_OperatingRoomModel->GetPatient()->GetSpineModel()->AddObserver( 
                                    li::SpineChangedEvent,
                                    commandUpdateSpineModelGUI );


  li::CommandUpdateRegistratorFieldOfView::Pointer
                    commandUpdateRegistratorFieldOfView =
                           li::CommandUpdateRegistratorFieldOfView::New();

  m_OperatingRoomModel->GetFluoroscopyUnit()->AddObserver( 
                              li::FieldOfViewChangedEvent,
                              commandUpdateRegistratorFieldOfView );
 
  commandUpdateRegistratorFieldOfView->SetRegistrator( 
                                          m_VesselsGUI.GetRegistrator() );


  li::CommandTubesGeneratePointSet::Pointer
                   commandTubesGeneratePointSet =
                          li::CommandTubesGeneratePointSet::New();

  commandTubesGeneratePointSet->SetFluoroscopyUnit(
                        m_OperatingRoomModel->GetFluoroscopyUnit() );

  commandTubesGeneratePointSet->SetTubes(
      m_OperatingRoomModel->GetPatient()->GetVesselsModel().GetPointer() );

  m_OperatingRoomModel->GetPatient()->AddObserver( 
                                        li::PatientMovedEvent,
                                        commandTubesGeneratePointSet );

  m_OperatingRoomModel->GetFluoroscopyUnit()->AddObserver(
                                    li::FluoroscopyUnitMovedEvent,
                                    commandTubesGeneratePointSet );

  m_OperatingRoomModel->GetSurgeryTable()->AddObserver( 
                                li::TableMovedEvent,
                                commandTubesGeneratePointSet );
                              

 
}





//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
SpineInterventionConfiguration::~SpineInterventionConfiguration() 
{

}





///--------------------------------------------------
//
//    Show all the windows
//
//--------------------------------------------------
void
SpineInterventionConfiguration::Show() 
{
  // the room has to be displayed in order to 
  // computer the world coordinates of the 
  // fluoroscopy unit source and detector 
  ShowRoomView();
  
//  ShowFluoroscopyView();
//  ShowFluoroscopyControls();
//  ShowTableControls();

}



///--------------------------------------------------
//
//    Show the Operating Room view
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowRoomView() 
{
  m_OperatingRoomViewGUI.Show();

  // This event is sent to inform the Fluoroscopic Unit View
  // of the source and detector position in world coordinates
  m_OperatingRoomModel->GetFluoroscopyUnit()->InvokeEvent(
                          li::FluoroscopyUnitMovedEvent );
}


///--------------------------------------------------
//
//    Show the fluoroscopy unit view
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowFluoroscopyView() 
{
  m_FluoroscopyUnitViewGUI.Show();

  m_OperatingRoomModel->GetFluoroscopyUnit()->InvokeEvent(
                          li::FluoroscopyUnitMovedEvent );
}


///--------------------------------------------------
//
//    Show the volume view
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowVolumeView() 
{

  m_VolumeViewGUI.SetLabel("Volume Registration View");
  m_VolumeViewGUI.Show();

}


///--------------------------------------------------
//
//    Show the volume Slicer controls
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowVolumeSlicer() 
{
  m_VolumeSliceDrawer->Show();
}



///--------------------------------------------------
//
//    Show the Surgical Table controls
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowTableControls() 
{
  m_TableGUI.Show();
}




///--------------------------------------------------
//
//    Show the Spine Model controls
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowSpineControls() 
{
  m_SpineGUI.Show();
}



///--------------------------------------------------
//
//    Show the Vessels Model controls
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowVesselsControls() 
{
  m_VesselsGUI.Show();
}



///--------------------------------------------------
//
//    Show all the graphic interface windows
//
//--------------------------------------------------
void
SpineInterventionConfiguration::ShowFluoroscopyControls() 
{
  m_FluoroscopyUnitGUI.Show();
}





///--------------------------------------------------
//
//    Hide all the graphic interface windows
//
//--------------------------------------------------
void
SpineInterventionConfiguration::Hide() 
{
  m_TableGUI.Hide();
  m_OperatingRoomViewGUI.Hide();
  m_FluoroscopyUnitGUI.Hide();
  m_FluoroscopyUnitViewGUI.Hide();
  m_SpineGUI.Hide();
  m_VesselsGUI.Hide();
  m_VolumeViewGUI.Hide();
  m_VolumeSliceDrawer->Hide();
}


//--------------------------------------------------
//
//    Load Vessels Model
//
//--------------------------------------------------
void
SpineInterventionConfiguration
::LoadVesselsModel( void )
{

  const char * filename = fl_file_chooser("Vessels Model filename","*.tre","");
  if( !filename )
  {
    return;
  }

  try 
  {
    LoadVesselsModel( filename );
  }
  catch( ... ) 
  {
    fl_alert("Problems reading file");
    return;
  }

  m_VesselsGUI.SetVesselsModel(
      m_OperatingRoomModel->GetPatient()->GetVesselsModel().GetPointer() );

  m_VesselsGUI.SetImage( 
             m_FluoroscopyImageReader->GetOutput().GetPointer() );

}



  
///--------------------------------------------------
//
//    Load a Vessels Model from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration 
::LoadVesselsModel( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_OperatingRoomModel->GetPatient()->GetVesselsModel()->Load( filename );
  m_OperatingRoomModel->GetPatient()->GetVesselsModel()->SetDrawingMode( 
                                                            Shape3D::lines );

}


//--------------------------------------------------
//
//    Load a Spine Model
//
//--------------------------------------------------
void
SpineInterventionConfiguration
::LoadSpineModel( void )
{

  const char * filename = fl_file_chooser("Spine Model filename","*.spn","");
  if( !filename )
  {
    return;
  }

  try 
  {
    LoadSpineModel( filename );
  }
  catch( ... ) 
  {
    fl_alert("Problems reading file");
    return;
  }

}



  
///--------------------------------------------------
//
//    Load a Spine Model from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration 
::LoadSpineModel( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_OperatingRoomModel->GetPatient()->GetSpineModel()->Load( filename );

  m_SpineGUI.SetSpineModel( 
      m_OperatingRoomModel->GetPatient()->GetSpineModel().GetPointer() );


  m_SpineGUI.SetImage( 
          m_FluoroscopyImageReader->GetOutput().GetPointer() ) ;
  
}



//--------------------------------------------------
//
//    Save a Spine Model
//
//--------------------------------------------------
void
SpineInterventionConfiguration
::SaveSpineModel( void )
{

  const char * filename = fl_file_chooser("Spine Model filename","*.spn","");
  if( !filename )
  {
    return;
  }

  try 
  {
    SaveSpineModel( filename );
  }
  catch( ... ) 
  {
    fl_alert("Problems writting file");
    return;
  }

}


 
///--------------------------------------------------
//
//    Save a Spine Model to a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration 
::SaveSpineModel( const char * filename )
{
  if( !filename )
  {
    return;
  }

  m_OperatingRoomModel->GetPatient()->GetSpineModel()->Save( filename );

}




///--------------------------------------------------
//
//    Load a fluoroscopy from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration 
::LoadFluoroscopy( const char * filename )
{
  if( !filename )
  {
    return;
  }


  if( !m_FluoroscopyImageReader )
  {
    m_FluoroscopyImageReader = ImageReaderType::New();
  }

  m_FluoroscopyImageReader->SetFileName( filename );
  m_FluoroscopyImageReader->Update();

  m_FluoroscopyUnitViewGUI.SetFluoroscopyImage(
             m_FluoroscopyImageReader->GetOutput().GetPointer() );

  m_SpineGUI.SetImage( 
             m_FluoroscopyImageReader->GetOutput().GetPointer() );

  m_VesselsGUI.SetImage( 
             m_FluoroscopyImageReader->GetOutput().GetPointer() );

  m_OperatingRoomModel->GetFluoroscopyUnit()->InvokeEvent(
                                li::FluoroscopyUnitMovedEvent );

  m_OperatingRoomModel->GetFluoroscopyUnit()->InvokeEvent(
                                li::FluoroscopyUnitMovedEvent );

}



//--------------------------------------------------
//
//    Load a fluoroscopy from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration
::LoadFluoroscopy( void )
{

  const char * filename = fl_file_chooser("Fluoroscopy Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  try 
  {
    LoadFluoroscopy( filename );
  }
  catch( ... ) 
  {
    fl_alert("Problems reading file");
    return;
  }

}




///--------------------------------------------------
//
//    Load a volume from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration 
::LoadVolume( const char * filename )
{
  if( !filename )
  {
    return;
  }


  if( !m_VolumeImageReader )
  {
    m_VolumeImageReader = VolumeReaderType::New();
  }

  m_VolumeImageReader->SetFileName( filename );
  m_VolumeImageReader->Update();

  m_VolumeImageReader->GetOutput()->SetRequestedRegion(
      m_VolumeImageReader->GetOutput()->GetBufferedRegion() );
  
  m_SpineGUI.SetVolume( 
             m_VolumeImageReader->GetOutput().GetPointer() );

  m_VolumeSliceDrawer->SetInput( 
             m_VolumeImageReader->GetOutput().GetPointer() );

  m_VolumeSliceDrawer->InvokeEvent( li::VolumeReslicedEvent );

}



//--------------------------------------------------
//
//    Load a volume from a file
//
//--------------------------------------------------
void
SpineInterventionConfiguration
::LoadVolume( void )
{

  const char * filename = fl_file_chooser("Volume Image filename","*.mh[da]","");
  if( !filename )
  {
    return;
  }

  try 
  {
    LoadVolume( filename );
  }
  catch( ... ) 
  {
    fl_alert("Problems reading file");
    return;
  }

}


 
} // end namespace li




