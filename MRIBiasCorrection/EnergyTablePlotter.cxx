/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    EnergyTablePlotter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <string>
#include <vector>
#include <vnl/vnl_math.h>

#include "myutils.h"
#include "OptionList.h"
#include "itkCompositeValleyFunction.h"

typedef itk::CompositeValleyFunction EnergyFunction ;

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: EnergyTablePlotter --class-mean mean(1) ... mean(i)" ) ;
  print_line("       --class-sigma sigma(1) ... sigma(i)" ) ;
  print_line("       --use-log [yes|no] --interval int" ) ;

  print_line("") ;

  print_line("--class-mean mean(1),...,mean(i)" ) ;
  print_line("        intensity means  of the differen i tissue classes") ;
  print_line("--class-sigma sig(1),...,sig(i)" ) ; 
  print_line("        intensity sigmas of the different i tissue clases") ;
  print_line("        NOTE: The sigmas should be listed in the SAME ORDER") ;
  print_line("              of means");
  print_line("        and each value should be SEPERATED BY A SPACE)") ;
  print_line("--use-log [yes|no]") ;
  print_line("        if yes, assume a multiplicative bias field") ;
  print_line("        [default yes]") ;
  print_line("        (use log of image, class-mean, class-sigma,") ;
  print_line("         and init-step-size)" );
  print_line("--interval int") ;
  print_line("        sampling interval of intensity from the energy table") ;

  print_line("") ;

  print_line("example: EnergyTablePlotter --class-mean 1500 570") ;
  print_line("         --class-sigma 100 78 --use-log yes --interval 10") ;
}

int main(int argc, char* argv[])
{
  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  itk::Array <double> classMeans ;
  itk::Array<double> classSigmas ;
  bool useLog ;
  int interval ;
  try
    {
      options.GetMultiDoubleOption("class-mean", &classMeans, true) ;
      options.GetMultiDoubleOption("class-sigma", &classSigmas, true) ; 
      useLog = options.GetBooleanOption("use-log", true, true) ;
      interval = options.GetIntOption("interval", 10, true) ;
      
      if (classMeans.size() == 0 || classSigmas.size() == 0)
        exit(0) ;
      
      if (classMeans.size() != classSigmas.size())
        {
          std::cout << "The number of means and sigma mismatch."
                    << std::endl ;
          exit(0) ;
        }
      
    }      
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
    }
  
  if (useLog)
    { 
      for (int i = 0 ; i < classMeans.size() ; i++) 
        {
          classSigmas[i] = log(1.0 + classSigmas[i] / (classMeans[i] + 1.0)) ;
          classMeans[i] = log(classMeans[i] + 1.0) ;
        }
    }

  EnergyFunction energy(classMeans, classSigmas) ;
  
  double higher = energy.GetHigherBound() ;
  double lower = energy.GetLowerBound() ;
  long noOfSamples = energy.GetNumberOfSamples() ; 
  double TableInc = (double) ((higher - lower) / 
                              (noOfSamples - 1));

  double d = lower;
  std::cout << "intensity\tenergy" << std::endl ;
  int i = 0 ;
  while(i < noOfSamples)
    {
      std::cout << d << "\t" << energy(d) << std::endl ;
      i += interval ;
      d += TableInc * interval ;
    }

  return 0 ;
}
