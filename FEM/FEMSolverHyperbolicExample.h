/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMSolverHyperbolicExample.h
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
#include "itkFEMSolverHyperbolic.h"

#include "itkFEMLinearSystemWrappers.h"

#include "itkExceptionObject.h"

#include <iostream>
#include <fstream>
#include <exception>

#define DEFAULT_COMMENT     '.'
#define MATLAB_COMMENT      '%'
#define IDL_COMMENT         ';'

// Only one of the _OUTPUT variables should be nonzero, otherwise
// things will become confusing!  If both are zero, no output will be
// generated.
#define MATLAB_OUTPUT       1
#define IDL_OUTPUT          0
#define DEBUG               0
#define DEBUG_FEM_TESTS     ( ( MATLAB_OUTPUT || IDL_OUTPUT ) && DEBUG )

// Only one of these FORMAT_ variables should be nonzero!

#define FORMAT_OUTPUT_BINARY  1
#define FORMAT_OUTPUT_ASCII   0
#define OUTPUT              ( FORMAT_OUTPUT_BINARY || FORMAT_OUTPUT_ASCII )

using namespace std;
using namespace itk;
using namespace fem;

void PrintK(SolverHyperbolic&, char, ostream&);
void PrintF(SolverHyperbolic&, char, ostream&);
void PrintNodalCoordinates(SolverHyperbolic&, char, ostream&);
void PrintElementCoordinates(SolverHyperbolic&, char, ostream&, int);
void PrintU(SolverHyperbolic&, char, ostream&, int);
