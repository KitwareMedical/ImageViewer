/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMRead.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
    for( int dof=0; dof<(*n)->N(); dof++ )
    {
      std::cout<<(*n)->uDOF(dof)->value;
      if ((dof+1)<(*n)->N())
      {
        std::cout<<",  ";
      }
    }
    std::cout<<"\n";
  }

  return 0;

}
