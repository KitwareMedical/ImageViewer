


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
  //m_SliceDrawer->Hide();
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
 *    Return the time from the start of the simulation
 */ 
clock_t
CellsViewerBase
::GetStartTime(void) const
{
  return m_StartTime;
}


/**
 *    Show the substrate controls
 */ 
void
CellsViewerBase
::ShowSubstrate( const char * name )
{
  std::string substrateName = name;
  SliceDrawerPointer slice = m_SubstrateSliceDrawer[ substrateName ];
  if( slice )
    {
    slice->Show();
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


