/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewer.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "CellsViewer.h"

#include "CommandEvents.h"
#include "CellsViewerCommand.h"

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
 *    Hide the Slicer controls
 */ 
void CellsViewer
::HideSlicerControls(void)
{
  this->CellsViewerBase::HideSlicerControls();
  substratesWindow->hide();
}


/**
 *    Hide the clipping plane controls
 */ 
void CellsViewer
::HideClippingPlaneControls(void)
{
  this->CellsViewerBase::HideClippingPlaneControls();
  clippingPlanesWindow->hide();
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
 *    Add a clipping plane
 */ 
void
CellsViewer
::AddClippingPlane( void )
{
  CellsViewerBase::AddClippingPlane();
  clippingPlanesBrowser->clear();
  ClippingPlaneDrawersType::const_iterator plane = m_ClippingPlaneDrawer.begin();
  while ( plane != m_ClippingPlaneDrawer.end() )
    {
    clippingPlanesBrowser->add( plane->first.c_str() );
    ++plane;
    }
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



/**
 *    Connect the cellular aggregate
 */ 
void CellsViewer
::SetCellsAggregate( CellularAggregate * cells )
{

  CellsViewerBase::SetCellsAggregate( cells );

  // Setup Observers
  cells->AddObserver( TimeStepEvent(), this->GetRedrawCommand() );

  CellsViewerCommand::Pointer viewerCommand = CellsViewerCommand::New();

  viewerCommand->SetCellsViewer( this );
  cells->AddObserver( TimeStepEvent(), viewerCommand );
  
  typedef  itk::SimpleMemberCommand<CellsViewer> SimpleCommandType;
  SimpleCommandType::Pointer saveImageCommand = SimpleCommandType::New();
  saveImageCommand->SetCallbackFunction( this, &CellsViewer::SaveCurrentImage );

  cells->AddObserver( TimeStepEvent(), saveImageCommand );

}





 
} // end namespace bio


