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
  substratesWindow->hide();
  CellsViewerBase::Quit();
}


/**
 *    Show the default components
 */ 
void CellsViewer
::Show(void)
{
  this->ShowDisplay();
  this->ShowControls();
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
 *    Add the egg of a Species to the Browser
 */ 
void CellsViewer
::AddSpeciesEggProducer( 
              EggProducerFunction * producer,  
              const char * name )
{
  CellsViewerBase::AddSpeciesEggProducer( producer );
  speciesBrowser->add( name );
}

  

/**
 *    Show the Species Browser
 */ 
void CellsViewer
::ShowSpecies(void)
{
  speciesWindow->show();
}




/**
 *    Hide the Display
 */ 
void CellsViewer
::HideControls(void)
{
  controlWindow->hide();
  speciesWindow->hide();
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



/**
 *    Load an Image representing the substrate
 */ 
void
CellsViewer
::LoadSubstrate()
{
  CellsViewerBase::LoadSubstrate();
  substratesBrowser->clear();
  SubstratesType::const_iterator substrate = m_Substrates.begin();
  while ( substrate != m_Substrates.end() )
    {
    substratesBrowser->add( substrate->first.c_str() );
    ++substrate;
    }
}


 
} // end namespace bio


