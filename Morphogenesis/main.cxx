
#include "BacterialColony.h"
#include "BacterialColonyRedrawCommand.h"
#include "CellsViewer.h"
#include "CommandEvents.h"


int main()
{

  bio::BacterialColony::Pointer  colony = bio::BacterialColony::New();

  bio::CellsViewer      viewer;

  viewer.SetCellsAggregate( colony );

  colony->AddObserver( itk::Command::ModifiedEvent, 
                       viewer.GetRedrawCommand()    );

  bio::BacterialColonyRedrawCommand::Pointer colonyRedrawCommand =
                                    bio::BacterialColonyRedrawCommand::New();

  colonyRedrawCommand->SetBacterialColony( colony );

  viewer.GetNotifier()->AddObserver( fltk::GlDrawEvent, colonyRedrawCommand );

  colony->SetGrowthRadiusIncrement( 0.01 );

  viewer.Show();
  
  Fl::run();


  return 0;

}
