
#include "BacterialColony.h"
#include "BacterialColonyRedrawCommand.h"
#include "CellsViewer.h"
#include "CommandEvents.h"
#include "CellsViewerCommand.h"


int main()
{



  bio::CellsViewer * viewer = new bio::CellsViewer;

  bio::BacterialColony::Pointer  colony = bio::BacterialColony::New();


  bio::CellsViewerCommand::Pointer viewerCommand = 
                                          bio::CellsViewerCommand::New();

  viewerCommand->SetCellsViewer( viewer );

  itk::Command::Pointer redrawCommand = viewer->GetRedrawCommand();
  colony->AddObserver( bio::TimeStepEvent, redrawCommand );

  colony->AddObserver( bio::TimeStepEvent, viewerCommand );

  viewer->SetCellsAggregate( colony );
  
  bio::BacterialColonyRedrawCommand::Pointer colonyRedrawCommand =
                                    bio::BacterialColonyRedrawCommand::New();

  colonyRedrawCommand->SetBacterialColony( colony );

  viewer->GetNotifier()->AddObserver( fltk::GlDrawEvent, colonyRedrawCommand );

  colony->SetGrowthRadiusIncrement( 0.01 );


  bio::Bacteria * first = new bio::Bacteria;

  colony->Add( first );

  viewer->Show();
  
  Fl::run();

  delete viewer;

  return 0;

}
