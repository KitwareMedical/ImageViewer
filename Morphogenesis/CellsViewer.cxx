#include "CellsViewer.h"


namespace bio {


/**
 *    Constructor
 */ 
CellsViewer
::CellsViewer()
{
}





/**
 *    Destructor
 */ 
CellsViewer
::~CellsViewer()
{
  this->HideControls();
}



/**
 *    Quit
 */ 
void CellsViewer
::Quit(void)
{
	this->HideControls();
  CellsViewerBase::Quit();
}


/**
 *    Show the default components
 */ 
void CellsViewer
::Show(void)
{
	this->ShowControls();
  this->ShowDisplay();
}



/**
 *    Show the Display
 */ 
void CellsViewer
::ShowControls(void)
{
	controlWindow->show();
}




/**
 *    Hide the Display
 */ 
void CellsViewer
::HideControls(void)
{
	controlWindow->hide();
}



/**
 *    Initiate the execution of the simulation
 */ 
void
CellsViewer
::Run(void)
{
  CellsViewerBase::Run();
}



/**
 *    Stop the execution of the simulation
 */ 
void 
CellsViewer
::Stop(void)
{
  CellsViewerBase::Stop();
}




/**
 *    Restart a Simulation
 */ 
void CellsViewer
::Restart(void)
{
  CellsViewerBase::Restart();
}



}; // end namespace bio


