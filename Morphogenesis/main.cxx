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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "BacterialColony.h"
#include "BacterialColonyGUI.h"
#include "Eukariote.h"
#include "PressureSensitiveBacteria.h"
#include "GradientEatingBacteria.h"
#include "BilayerOrganism.h"
#include "CellsViewer.h"


int main()
{


  bio::CellsViewer * viewer = new bio::CellsViewer;

  bio::BacterialColony::Pointer  colony = bio::BacterialColony::New();


  viewer->GetNotifier()->AddObserver( fltk::GlDrawEvent(), 
                       colony->GetRedrawCommand().GetPointer() );

  viewer->SetCellsAggregate( colony );

  // Add All the known species here
  viewer->AddSpeciesEggProducer( 
            & bio::Prokariote::CreateEgg, 
              bio::Prokariote::GetSpeciesName() );

  viewer->AddSpeciesEggProducer( 
            & bio::Bacteria::CreateEgg,   
              bio::Bacteria::GetSpeciesName() );

  viewer->AddSpeciesEggProducer( 
            & bio::PressureSensitiveBacteria::CreateEgg,   
              bio::PressureSensitiveBacteria::GetSpeciesName() );

  viewer->AddSpeciesEggProducer( 
            & bio::GradientEatingBacteria::CreateEgg,   
              bio::GradientEatingBacteria::GetSpeciesName() );

  viewer->AddSpeciesEggProducer( 
            & bio::Eukariote::CreateEgg,  
              bio::Eukariote::GetSpeciesName() );

  viewer->AddSpeciesEggProducer( 
            & bio::BilayerOrganism::CreateEgg,  
              bio::BilayerOrganism::GetSpeciesName() );




  viewer->Show();
  

  try 
    {
    Fl::run();
    delete viewer;
    }
  catch( itk::ExceptionObject & exception )
    {
    std::cout << "Exception caught !" << std::endl;
    std::cout << "Description : " << exception.GetDescription() << std::endl;
    std::cout << "Location    : " << exception.GetLocation()    << std::endl;
    }

  return 0;

}



