#include "CellsViewerBase.h"


namespace bio {


/**
 *    Constructor
 */ 
CellsViewerBase
::CellsViewerBase()
{
  m_Stop = true;
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
 *    Resume a Stopped simulation
 */ 
void CellsViewerBase
::Resume(void)
{
	m_Stop = false;
}



/**
 *    Connect the cellular aggregate
 */ 
void CellsViewerBase
::SetCellsAggregate( Cell::CellsListType * cells )
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
	return m_Display.GetRedrawCommand().GetPointer();
}



/**
 *    Return and object that accept observers
 */ 
itk::LightObject::Pointer
CellsViewerBase
::GetNotifier(void)
{
	return m_Display.GetNotifier().GetPointer();
}


}; // end namespace bio


