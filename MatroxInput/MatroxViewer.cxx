
#include "MatroxViewer.h"



// FLTK Includes
#include "Fl/fl_file_chooser.H"
#include "Fl/fl_ask.H"


namespace ISIS 
{


MatroxViewer
::MatroxViewer() 
{
  
  m_Renderer      = vtkRenderer::New();
  m_RenderWindow  = vtkRenderWindow::New();
  m_VideoSource   = vtkVideoSource::New();
  m_ImageMapper   = vtkImageMapper::New();
  m_ImageActor    = vtkImageActor::New();

  m_RenderWindow->AddRenderer( m_Renderer );

  m_Renderer->SetBackground( 1.0, 1.0, 1.0 ); 
  m_Renderer->GetActiveCamera()->Zoom( 1.0 ); 
  m_Renderer->GetActiveCamera()->SetPosition(0.0, 0.0, 20.0 ); 

  m_FlRenderWindowInteractor->SetRenderWindow( m_RenderWindow );

  m_ImageMapper->SetInput( m_VideoSource->GetOutput() );



  m_Renderer->AddActor( m_ImageActor );


}



MatroxViewer
::~MatroxViewer() 
{
  m_ImageActor->Delete();
  m_ImageMapper->Delete();
  m_VideoSource->Delete();
  m_RenderWindow->Delete();
  m_Renderer->Delete();
}



void 
MatroxViewer
::Show()
{
  m_FlRenderWindowInteractor->Initialize();
  this->MatroxViewerBase::Show();
  controlWindow->show();
}


void 
MatroxViewer
::Hide()
{
  this->MatroxViewerBase::Hide();
  controlWindow->hide();
}


void 
MatroxViewer
::Grab()
{
  this->MatroxViewerBase::Grab();
}


void 
MatroxViewer
::Stop()
{
  this->MatroxViewerBase::Stop();
}


void 
MatroxViewer
::Quit()
{
  this->Hide();
}


void 
MatroxViewer
::LoadImageFromFile()
{

  const char * filename = fl_file_chooser("Image filename","*.*","");

  if( !filename )
    {
    return;
    }

  try
    {
    this->MatroxViewerBase::LoadImageFromFile( filename );
    }
  catch( ... ) 
    {
    fl_alert("Problem reading the file");
    }

  if( m_ImageIsLoaded )
    {
    m_ImageActor->SetInput( m_ImageCast->GetOutput() );
    }
    
}




}

