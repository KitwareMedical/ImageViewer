/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    main.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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




   
