/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CellsViewerBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "CellsViewerBase.h"
#include <FL/fl_file_chooser.H>
#include <FL/fl_ask.H>

namespace bio {


/**
 *    Constructor
 */ 
CellsViewerBase
::CellsViewerBase()
{
  m_Display.GetGlWindow()->SetBackground( 0.8, 0.8, 0.9 );
  m_Display.GetGlWindow()->SetZoom( 5.0 );
  m_Display.SetLabel("Cell Viewer");
  m_Stop =            true;
  m_StartTime =          0;

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
  this->HideClippingPlaneControls();
  this->HideCellularAggregateControls();
  this->HideDisplay();

  // Hide the substrate controls
  SubstratesDrawersType::const_iterator slicer = 
                            m_SubstrateSliceDrawer.begin();

  while ( slicer != m_SubstrateSliceDrawer.end() )
    {
    slicer->second->Hide();
    ++slicer;
    }

  // Hide the clipping plane controls
  ClippingPlaneDrawersType::const_iterator plane = 
                            m_ClippingPlaneDrawer.begin();

  while ( plane != m_ClippingPlaneDrawer.end() )
    {
    plane->second->Hide();
    ++plane;
    }


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
 *    Show the Set of Registered Species
 */ 
void CellsViewerBase
::ShowSpecies(void)
{
  // To be overrided
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
  if( Cell::Dimension==3 )
    {
    //m_SliceDrawer->Show();
    }
}



/**
 *    Hide the Slicer controls
 */ 
void CellsViewerBase
::HideSlicerControls(void)
{
}


/**
 *    Hide the clipping plane controls
 */ 
void CellsViewerBase
::HideClippingPlaneControls(void)
{
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
 *    Clear the Aggregate
 */ 
void CellsViewerBase
::ClearAggregate(void)
{
  m_Cells->KillAll();
  m_Display.Redraw();
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
 *    Add the egg of a species to the container 
 */ 
void
CellsViewerBase
::AddSpeciesEggProducer( EggProducerFunction * producer ) 
{
  m_SpeciesProducer.push_back( producer );
}



/**
 *    Return the container of species eggs
 */ 
CellsViewerBase::SpeciesContainerType &
CellsViewerBase
::GetSpecies(void) 
{
  return m_SpeciesProducer;
}



/**
 *   Select the egg of one of the Registered Species
 */ 
void CellsViewerBase
::SelectSpecies( unsigned int species )
{
  if( species < m_SpeciesProducer.size() )
    {
    m_Cells->KillAll();
    Cell * theEgg = m_SpeciesProducer[ species ]();
    m_Cells->SetEgg( theEgg );
    }
  m_Display.Redraw();
}



/**
 *    Add a clipping plane
 */ 
void
CellsViewerBase
::AddClippingPlane( void )
{
  std::string planeName;
  const char * name = fl_input("Name for the Clipping Plane","Plane 1");
  if( !name )
    {
    return;
    }
  planeName = name;
  ClippingPlaneDrawerPointer plane = ClippingPlaneDrawerType::New();
  m_ClippingPlaneDrawer[ planeName ] = plane.GetPointer();
  
  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             plane->GetDrawCommand().GetPointer() );

  plane->AddObserver( 
            fltk::ClippingPlaneEvent(), 
            m_Display.GetRedrawCommand() );
 
  plane->SetLabel( planeName.c_str() );
  plane->Show();

}



/**
 *    Remove a clipping plane
 */ 
void
CellsViewerBase
::RemoveClippingPlane( void )
{
  
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
 *    Show the clipping planes controls
 */ 
void
CellsViewerBase
::ShowClippingPlane( const char * name )
{
  std::string clippingPlaneName = name;

  ClippingPlaneDrawersType::const_iterator clippingPlane = 
                      m_ClippingPlaneDrawer.find( clippingPlaneName );

  if( clippingPlane != m_ClippingPlaneDrawer.end() )
    {
    clippingPlane->second->Show();
    }

}



/**
 *    Export positions and radii of cells
 *    To be overloaded in a subclass
 */ 
void
CellsViewerBase
::ExportDrawing( void ) const
{
  const char * filename = fl_file_chooser("","","");
  if( !filename )
    {
    return;
    }
  m_Cells->ExportDrawing( filename );
}



/**
 *    Export positions and radii of cells in XFIG format
 *    To be overloaded in a subclass
 */ 
void
CellsViewerBase
::ExportXFIG( void ) const
{
  const char * filename = fl_file_chooser("","","");
  if( !filename )
    {
    return;
    }
  m_Cells->ExportXFIG( filename );
}




/**
 *    Show the substrate controls
 */ 
void
CellsViewerBase
::ShowSubstrate( const char * name )
{
  std::string substrateName = name;
  
  SubstratesDrawersType::const_iterator slice = 
                  m_SubstrateSliceDrawer.find( substrateName );
    
  if( slice != m_SubstrateSliceDrawer.end() )
    {
    slice->second->Show();
    }

}



/**
 *    Load an Image representing the substrate
 */ 
void
CellsViewerBase
::LoadSubstrate()
{
  const char * filename = fl_file_chooser("","","");
  if( !filename )
  {
    return;
  }

  ImageReaderPointer imageReader = ImageReaderType::New();

  imageReader->SetFileName( filename );
  try {
    imageReader->Update();
  }
  catch ( itk::ExceptionObject & ex )
  {
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
  
  char buff[500];
  sprintf( buff,"Substrate%03d",m_Substrates.size() );
  std::string substrateName = buff;

  const char * useranswer = fl_input( "Substrate Name", 
                                    substrateName.c_str() );

  if( !useranswer ) 
    {
    return;
    } 

  substrateName = useranswer;

  SliceDrawerPointer substrateSliceDrawer = SliceDrawerType::New();
    
  substrateSliceDrawer->SetInput( imageReader->GetOutput().GetPointer() );

  m_Display.GetNotifier()->AddObserver( 
             fltk::GlDrawEvent(),
             substrateSliceDrawer->GetDrawCommand().GetPointer() );

  substrateSliceDrawer->AddObserver( 
                           fltk::VolumeReslicedEvent(),
                           m_Display.GetRedrawCommand() );
 
  
  m_Substrates[ substrateName ] 
                  = imageReader->GetOutput().GetPointer();
  
  m_SubstrateSliceDrawer[ substrateName ] 
                  = substrateSliceDrawer.GetPointer();

  m_Cells->AddSubstrate( imageReader->GetOutput().GetPointer() );

  substrateSliceDrawer->SetLabel( substrateName.c_str() );
  substrateSliceDrawer->Show();
  
  
}

  
 


} // end namespace bio


