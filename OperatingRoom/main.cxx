//--------------------------------------------
//
//      Project: Operating Room
//
//      Author:  Luis Ibanez
//  
//      Directed by: Elizabeth Bullitt, MD
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liSpineInterventionConfiguration.h>
#include <liSpineInterventionConfigurationGUI.h>

int main(int argc, char *argv[]) 
{
  
  
  li::SpineInterventionConfiguration::Pointer Intervention =
                             li::SpineInterventionConfiguration::New();
  
  SpineInterventionConfigurationGUI InterventionGUI;

  InterventionGUI.SetConfiguration( Intervention.GetPointer() );

  InterventionGUI.Show();
  Intervention->Show();

  Fl::run();


  return 0;

}




   
