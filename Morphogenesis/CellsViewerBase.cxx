


#include "CellsViewerBase.h"
#include <Fl/fl_file_chooser.H>
#include <Fl/fl_ask.H>

namespace bio {


/**
 *    Constructor
 */ 
CellsViewerBase
::CellsViewerBase()
{
  m_Display.GetGlWindow()->SetBackground( 0.8, 0.8, 0.9 );
  m_Stop =            true;
  m_StartTime =          0;
  m_ImageIsLoaded =  false;

  m_SliceDrawer = SliceDrawerType::New();

  m_ImageReader = ImageReaderType::New();
    
  m_Image = m_ImageReader->GetOutput();

  m_SliceDrawer->SetInput( m_Image.GetPointer() );

  m_Display.GetNotifier()->AddObserver( fltk::GlDrawEvent,
                             m_SliceDrawer->GetDrawCommand().GetPointer() );

  m_SliceDrawer->AddObserver( fltk::VolumeReslicedEvent,
                               m_Display.GetRedrawCommand() );
  
  m_ImageReader->AddObserver( itk::Command::EndEvent,
                              m_Display.GetRedrawCommand() );
                               

}





/**
 *    Destructor
 */ 
CellsViewerBase
::~CellsViewerBase()
{

	this->HideDisplay();
}



/**
 *    Quit
 */ 
void CellsViewerBase
::Quit(void)
{
  this->Stop();
  this->HideSlicerControls();
  this->HideCellularAggregateControls();
	this->HideDisplay();
}


/**
 *    Show the Display
 */ 
void CellsViewerBase
::ShowDisplay(void)
{
	m_Display.Show();
}



/**
 *    Hide the Display
 */ 
void CellsViewerBase
::HideDisplay(void)
{
	m_Display.Hide();
}




/**
 *    Show the Slicer controls
 */ 
void CellsViewerBase
::ShowSlicerControls(void)
{
  if( m_ImageIsLoaded && Cell::Dimension==3 )
    {
  	m_SliceDrawer->Show();
    }
}



/**
 *    Hide the Slicer controls
 */ 
void CellsViewerBase
::HideSlicerControls(void)
{
  m_SliceDrawer->Hide();
}


/**
 *    Show the Cellular Aggregate controls
 */ 
void CellsViewerBase
::ShowCellularAggregateControls(void)
{
  m_Cells->Show();
}



/**
 *    Hide the Cellular Aggregate controls
 */ 
void CellsViewerBase
::HideCellularAggregateControls(void)
{
  m_Cells->Hide();
}




/**
 *    Initiate the execution of the simulation
 */ 
void
CellsViewerBase
::Run(void)
{
	m_Stop = false;
  if( !m_StartTime ) 
  {
    m_StartTime = clock();
  }
  while( !m_Stop )
    {
    m_Cells->AdvanceTimeStep();
    Fl::check();
    }
}



/**
 *    Stop the execution of the simulation
 */ 
void 
CellsViewerBase
::Stop(void)
{
	m_Stop = true;
}




/**
 *    Restart a simulation
 */ 
void CellsViewerBase
::Restart(void)
{
	m_Stop = false;
}



/**
 *    Connect the cellular aggregate
 */ 
void CellsViewerBase
::SetCellsAggregate( CellularAggregate * cells )
{
	m_Cells = cells;
}


/**
 *    Return the Command assigned to redraw
 */ 
itk::Command::Pointer
CellsViewerBase
::GetRedrawCommand(void)
{
  itk::Command::Pointer command;
  
  command = m_Display.GetRedrawCommand();

	return command.GetPointer();
}



/**
 *    Return and object that accept observers
 */ 
itk::Object::Pointer
CellsViewerBase
::GetNotifier(void)
{
	return m_Display.GetNotifier().GetPointer();
}


/**
 *    Return the time from the start of the simulation
 */ 
clock_t
CellsViewerBase
::GetStartTime(void) const
{
	return m_StartTime;
}


/**
 *    Load an Image representing the substrate
 */ 
void
CellsViewerBase
::LoadImage()
{
  const char * filename = fl_file_chooser("","","");
  if( !filename )
  {
    return;
  }
  m_ImageReader->SetFileName( filename );
  try {
    m_ImageReader->Update();
  }
  catch ( itk::ExceptionObject & ex )
  {
    m_ImageIsLoaded = false;
    std::string message;
    message = "Problem found while reading image ";
    message += filename;
    message += "\n";
    message += ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    fl_alert( message.c_str() );
    return;
  }
  
  m_SliceDrawer->SetInput( m_Image.GetPointer() );

  m_ImageIsLoaded = true;
}

  
 


}; // end namespace bio


