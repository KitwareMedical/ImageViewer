/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMRead.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEM.h"
#include <iostream>
#include <fstream>
#include <string>




/**
 * This example reads a FEM problem from a *.fem file that is specified on
 * command line. If no argument is provided on command line, file
 * 'truss.fem' is read from the current folder.
 *
 * External forces, which are also specified inside a *.fem, are applied
 * and the problem is solved for displacements. Solution vector is output
 * on stdout.
 *
 * You'll probably have to check the problem definiton in a file, in order
 * to understand what these displacements mean.
 */
int main( int argc, char *argv[] ) {

char* filename;

  if (argc>=2)
  {
    filename=argv[1];
  }
  else
  {
    /**
     * If the argument was not specified, we try default file name.
     */
    filename="truss.fem";
  }


  /**
   * First we create the FEM solver object.
   */
  ::itk::fem::Solver S;

  /**
   * Open the file and assign it to stream object f
   */
  std::cout<<"Reading FEM problem from file: "<<std::string(filename)<<"\n";
  std::ifstream f;
  f.open(filename);
  if (!f)
  {
    std::cout<<"File "<<filename<<" not found!\n";
    return 1;
  }

  /**
   * Read the fem problem from the stream f
   */
  try 
  {
    S.Read(f);
  }
  catch (::itk::fem::FEMException e)
  {
    std::cout<<"Error reading FEM problem: "<<filename<<"!\n";
    e.Print(std::cout);
    return 1;
  }

  f.close();

  /**
   * Now the whole problem is stored inside the Solver class.
   * We can begin solving for displacements.
   */
  std::cout<<"Solving..."<<std::string(filename)<<"\n";

  /**
   * Assign a unique id (global freedom number - GFN)
   * to every degree of freedom (DOF) in a system.
   */
  S.GenerateGFN();

  /**
   * Assemble the master stiffness matrix. In order to do this
   * the GFN's should already be assigned to every DOF.
   */
  S.AssembleK();

  /**
   * Invert the master stiffness matrix
   */
  S.DecomposeK();

  /**
   * Assemble the master force vector (from the applied loads)
   */
  S.AssembleF();

  /**
   * Solve the system of equations for displacements (u=K^-1*F)
   */
  S.Solve();

  /**
   * Copy the displacemenets which are now stored inside
   * the solver class back to nodes, where they belong.
   */
  S.UpdateDisplacements();

  /**
   * Output displacements of all nodes in a system;
   */
  std::cout<<"\nNodal displacements:\n";
  for( ::itk::fem::Solver::NodeArray::iterator n = S.node.begin(); n!=S.node.end(); n++)
  {
    std::cout<<"Node#: "<<(*n)->GN<<": ";
    /** For each DOF in the node... */
    for( unsigned int d=0, dof; (dof=(*n)->GetDegreeOfFreedom(d))!=::itk::fem::Element::InvalidDegreeOfFreedomID; d++ )
    {
      std::cout<<S.GetSolution(dof);
      std::cout<<",  ";
    }
    std::cout<<"\b\b\b \b\n";
  }

  return 0;

}
