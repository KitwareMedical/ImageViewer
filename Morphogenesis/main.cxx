
#include "BacterialColony.h"
#include "BacterialColonyRedrawCommand.h"
#include "CellsViewer.h"
#include "CommandEvents.h"


int main()
{



  bio::CellsViewer               viewer;

  bio::BacterialColony::Pointer  colony = bio::BacterialColony::New();


  itk::Command::Pointer redraw = viewer.GetRedrawCommand();
  colony->AddObserver( bio::TimeStepEvent, redraw );


  viewer.SetCellsAggregate( colony );
  
  bio::BacterialColonyRedrawCommand::Pointer colonyRedrawCommand =
                                    bio::BacterialColonyRedrawCommand::New();

  colonyRedrawCommand->SetBacterialColony( colony );

  viewer.GetNotifier()->AddObserver( fltk::GlDrawEvent, colonyRedrawCommand );


  colony->SetGrowthRadiusIncrement( 0.01 );


  bio::Bacteria * first = new bio::Bacteria;

  colony->Add( first );

  viewer.Show();
  
  Fl::run();


  return 0;

}
