/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMSolverHyperbolicExample.cxx
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

#include "FEMSolverHyperbolicExample.h"

using namespace std;
using namespace itk;
using namespace fem;

#if DEBUG_FEM_TESTS

void PrintK(SolverHyperbolic& S, char comment, ostream& of)
// Print K - the global stiffness matrix
{
  LinearSystemWrapper::Pointer lsw = S.GetLinearSystemWrapper();

  std::cout << std::endl << "k" << "=[";
  for (unsigned int j=0; j < lsw->GetSystemOrder(); j++) {
    if (IDL_OUTPUT) { std::cout << " ["; }
    for (unsigned int k=0; k < lsw->GetSystemOrder(); k++) {
      if (k > 0) { std::cout << ",  "; }
      std::cout << lsw->GetMatrixValue(j,k);
    }
    if (IDL_OUTPUT) {
      if (j < lsw->GetSystemOrder()-1) { std::cout << " ], $" << std::endl; }
      else  { std::cout << "]"; }
    } 
    else if (MATLAB_OUTPUT) { std::cout << std::endl; }
  }
  std::cout << "];" << std::endl;
}  

void PrintF(SolverHyperbolic& S, char comment, ostream& of)
// Print F - the global load vector
{
  LinearSystemWrapper::Pointer lsw = S.GetLinearSystemWrapper();
  
  std::cout << std::endl << "f" << "=[";
  for (unsigned int j=0; j < lsw->GetSystemOrder(); j++) {
    if (j > 0) { std::cout << ",  "; }
    std::cout << lsw->GetVectorValue(j);
  }
  std::cout << "];" << std::endl;
}

#endif

#if FORMAT_OUTPUT_ASCII

void PrintNodalCoordinates(SolverHyperbolic& S, char comment, ostream& of)
// Print the nodal coordinates
{
  of << std::endl << comment << "Nodal coordinates: " << std::endl;

  of << "xyz" << "=[";
  for (Solver::NodeArray::iterator n = S.node.begin(); n != S.node.end(); n++) {
    if (IDL_OUTPUT) { of << " ["; }
    // FIXME: this will generate errors in IDL - needs to be comma-delimited
    of << (*n)->GetCoordinates();
    if (IDL_OUTPUT) { 
      if ((n+1) != S.node.end()) { of << " ], $" << std::endl; }
      else { of << "]"; }
    }
    else if (MATLAB_OUTPUT) { of << std::endl; }
  }
   of << "];" << std::endl;
}

void PrintElementCoordinates(SolverHyperbolic& S, char comment, ostream& of, int iter)
// Useful for display purposes - lets you draw each element
// individually, instead of just a stream of nodes
{
  of << std::endl << comment << "Element coordinates: " << std::endl;
  int ct = 1;

  for (Solver::ElementArray::iterator e = S.el.begin(); e != S.el.end(); e++) {
    of << "e(" << ct << "," << (iter+1) << ",:,:)=[";
    if (IDL_OUTPUT) { of << " ["; }
    for (unsigned int n=0; n < (*e)->GetNumberOfNodes(); n++) {

      // FIXME: this will generate errors in IDL - needs to be comma-delimited
      Element::VectorType nc = (*e)->GetNodeCoordinates(n);

      for (unsigned int d=0, dof; ( dof = (*e)->GetNode(n)->GetDegreeOfFreedom(d) ) != Element::InvalidDegreeOfFreedomID; d++) {
  std::cout << S.GetSolution(dof) << std::endl;
        nc[d] += S.GetSolution(dof);
      }
      of << nc;

      if (IDL_OUTPUT) {
        if ((e+1) != S.el.end()) { of << " ], $" << std::endl; }
        else { of << "]"; }
      }
      else if (MATLAB_OUTPUT) { of << std::endl; }
    }
    of << "];" << std::endl;
    ct++;
  }
}

#endif

#if OUTPUT 

void PrintU(SolverHyperbolic& S, char comment, ostream& of, int iter)
// Print the displacements
{
  of << std::endl << comment << "Displacements: " << std::endl;

  of << "u(" << (iter+1) << ",:,:)=[";
  for(Solver::NodeArray::iterator n = S.node.begin(); n!=S.node.end(); n++) {
    if (IDL_OUTPUT) { of << " ["; }
    for( unsigned int d=0, dof; (dof=(*n)->GetDegreeOfFreedom(d))!=Element::InvalidDegreeOfFreedomID; d++ ) {
      if (d > 0 && d != Element::InvalidDegreeOfFreedomID) { of <<", "; }
      of << S.GetSolution(dof);
    }
    if (IDL_OUTPUT) { 
      if ((n+1) != S.node.end()) { of << " ], $" << std::endl; }
      else { of << "]"; }
    }
    else if (MATLAB_OUTPUT) { of << std::endl; }
  }
  of << "];" << std::endl;
}
      
#endif    


int main(int ac, char** av)
{
  // NOTE TO THE USER: if you would like to run the menu-based test,
  // you will need to change the two paths below to point to the
  // appropriate directory in your ITK tree from your executable
  // folder.

  // File I/O streams
  ifstream f;
  ofstream of;

  // Filename containing list of possible input files
  char listloc[] = "../../Insight/Testing/Data/Input/FEM/input-list";

  // Path to input files
  char filepath[] = "../../Insight/Testing/Data/Input/FEM/";

  // Path to output (either Matlab or IDL)
  char outpath[] = "";

  // Storage for list of or user-specified input file(s)
  char** filelist; 
  char buffer[80] = {'\0'};
  int numfiles = 0;
  char *fname = NULL;

  // Number of solver iterations
  int niter = 100;

  // Choose a linear system wrapper (0 - sparse VNL, 1 - dense VNL, 2 - Itpack)
  int w = 2;  

  // Output comments and file extension
  char comment;
  char* outfile = new char[strlen(outpath)+20];
  if (MATLAB_OUTPUT) { comment = MATLAB_COMMENT; }
  else if (IDL_OUTPUT) { comment = IDL_COMMENT;  }
  else { comment = DEFAULT_COMMENT; }

  strcpy(outfile, outpath);
  strcat(outfile, "shout.data");

  if (ac < 2)
  // Display the menu
  {
    std::cout << "Loading menu..." << std::endl;
    
    f.open(listloc);
    if (!f) {
      std::cout << "ERROR: null file handle - couldn't read input file list" << std::endl;
      return EXIT_FAILURE;
    }
    
    f >> numfiles;
    filelist = new char*[numfiles];
    for (int k=0; k < numfiles; k++) {
      f >> buffer;
      filelist[k] = new char[strlen(buffer)+1];
      strcpy(filelist[k], buffer);
    }
    f.close();
    
    // Prompt the user to select a problem
    int ch = -1;
    while (ch < 0 || ch >= numfiles) {
      for (int j=0; j < numfiles; j++) { std::cout << j << ": " << filelist[j] << std::endl; }
      std::cout << std::endl << "Select an FEM problem to solve:  ";
      cin >> ch;
    }
    
    // Print the name of the selected problem
    std::cout << std::endl << comment << "FEM Problem: " << filelist[ch] << std::endl;
    
    // Construct the file name appropriately from the list
    fname = new char[strlen(filepath)+strlen(filelist[ch])+5];
    strcpy(fname, filepath);
    strcat(fname, filelist[ch]);
  }
  // Accept a user-specified file
  else {
    std::cout << "User-specified file..." << std::endl;
      
    fname = new char[strlen(av[1])+5];
    strcpy(fname, av[1]);
    
    // Print the name of the user-specified problem
    std::cout << std::endl << comment << "FEM Input: " << fname << std::endl;
  }
    
  // Open a file handle & associate it with the input file
  f.open(fname);
  if (!f)
  {
    std::cout << "ERROR: null file handle...terminating." << std::endl;
    return EXIT_FAILURE;
  }

  try {

    // Declare the *hyperbolic* FEM solver & associated input stream
    // and read the input file

    std::cout << comment << "Solver()" << std::endl;
    SolverHyperbolic SH;
    SH.SetTimeStep(.05);

    std::cout << comment << "Read()" << std::endl;
    SH.Read(f);
    f.close();
    delete(fname);
    
    // Call the appropriate sequence of Solver methods to solve the
    // problem
    
    std::cout << comment << "GenerateGFN()" << std::endl;
    SH.GenerateGFN();          // Generate global freedom numbers for system DOFs

    LinearSystemWrapperItpack lsw_itpack;
    LinearSystemWrapperDenseVNL lsw_dvnl;
    LinearSystemWrapperVNL lsw_vnl;

    switch(w) {
    case 2:
      // Itpack 
      std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperItpack" << std::endl;
      lsw_itpack.SetMaximumNonZeroValuesInMatrix(1000000);
      SH.SetLinearSystemWrapper(&lsw_itpack);
      break;
    case 1:
      // Dense VNL
      std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperDenseVNL" << std::endl;
      SH.SetLinearSystemWrapper(&lsw_dvnl);
      break;
    default:
      // Sparse VNL - default
      std::cout << std::endl << comment << ">>>>>Using LinearSystemWrapperVNL" << std::endl;
      SH.SetLinearSystemWrapper(&lsw_vnl);
      break;
    }
    
    std::cout << comment << "AssembleK()" << std::endl;
    SH.AssembleK();            // Assemble the global stiffness matrix K
    
    std::cout << comment << "DecomposeK()" << std::endl;
    SH.DecomposeK();           // Invert K
    
    // If output is expected, open the output stream
    of.open(outfile);
    if (!f) {
      std::cout << "ERROR: null file handle - couldn't open output file" << std::endl;
      std::cout << outfile << std::endl;
      return EXIT_FAILURE;
    }

    // Find out the dimensions of the array that will be sent to Matlab/IDL 
    // In Matlab, the dimensions of the array will be [ndof, nndel, nelems, niter]
    Solver::ElementArray::iterator elit = SH.el.begin();
    unsigned char nelems = SH.el.size();
    unsigned char nndel = (*elit)->GetNumberOfNodes();
    unsigned char ndof = (*elit)->GetNumberOfDegreesOfFreedomPerNode();

    of << static_cast<unsigned char>(niter) << nelems << nndel << ndof;

    for (int nit = 0; nit < niter; nit++) {
      SH.AssembleF();            // Assemble the global load vector F
      SH.Solve();                // Iteratively solve the system Mu'' + Cu' + Ku=F for u

      int ctr = 0;

      // Output the new positions of the elements in the mesh ** in binary format **
#if FORMAT_OUTPUT_BINARY
      for (Solver::ElementArray::iterator ee = SH.el.begin(); ee != SH.el.end(); ee++) {
  for (unsigned int n=0; n < (*ee)->GetNumberOfNodes(); n++) {
    Element::VectorType nc = (*ee)->GetNode(n)->GetCoordinates();
    for (unsigned int dof = 0; dof < (*ee)->GetNumberOfDegreesOfFreedomPerNode(); dof++) {
      double ans = nc[dof] + SH.GetSolution((*ee)->GetNode(n)->GetDegreeOfFreedom(dof));
      for (unsigned i = 0; i < sizeof(double); i++) {
        of << reinterpret_cast<unsigned char*>(&ans)[i];
      }
    }
  }
  ctr++;
      }
#endif
      
      std::cout << comment << " Iteration " << nit << std::endl;
#if DEBUG_FEM_TESTS
      PrintK(SH, comment, of);
      PrintF(SH, comment, of);
#endif

#if OUTPUT
      //PrintElementCoordinates(SH, comment, of, nit);
#endif
    }

    std::cout << comment << "Done" << std::endl;
  }

  catch (::itk::ExceptionObject &err) {
    std::cerr << "ITK exception detected: "  << err;
    of.close();
    delete(outfile);

    return EXIT_FAILURE;
  }

  // Clean up and get out
  of.close();
  delete(outfile);

  return EXIT_SUCCESS;
}





