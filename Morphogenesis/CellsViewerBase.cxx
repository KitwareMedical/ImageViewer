#include "CellsViewerBase.h"


namespace bio {


/**
 *    Constructor
 */ 
CellsViewerBase
::CellsViewerBase()
{
  m_Display.GetGlWindow()->SetBackground( 0.8, 0.8, 0.9 );
  m_Stop = true;
  m_StartTime = 0;

  m_Image = ImageType::New();

  m_SliceDrawer = SliceDrawerType::New();

  m_SliceDrawer->SetInput( m_Image.GetPointer() );

  m_Display.GetNotifier()->AddObserver( fltk::GlDrawEvent,
                             m_SliceDrawer->GetDrawCommand().GetPointer() );

  m_SliceDrawer->AddObserver( fltk::VolumeReslicedEvent,
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

}; // end namespace bio


