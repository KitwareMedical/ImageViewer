/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMImageRegLMEx.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFEM.h"

#include "itkImageFileReader.h" 
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkRawImageIO.h"
#include "itkRawImageWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"


namespace itk {
namespace fem {

/** \class ImageRegLMEx 
    \brief FEM Image registration example class.

     The image registration problem is modeled here with the finite element method.
     Image registration is, in general, an ill-posed problem.  Thus, we use an optimization
     scheme where the optimization criterion is given by a regularized variational energy.
     The variational energy arises from modeling the image as a physical body on which 
     external forces act.  The body is allowed to deform so as to minimize the 
     applied force.  The resistance of the physical body to deformation, determined by 
     the physics associated with the body, serves to regularize the solution.
     The forces applied to the body are, generally, highly non-linear and so the 
     body is allowed to deform slowly and incrementally.  The direction it deforms
     follows the gradient of the potential energy (the force) we define.  The potential
     energies we may choose from are given by the itk image-to-image metrics. 
     The choices and the associated direction of descent are : 
        Mean Squares (minimize), 
        Normalized Cross-Correlation (maximize)
        Pattern Intensity  (maximize)
        Mutual Information (maximize).
     Note that we have to set the direction (SetDescentDirection) when we choose a metric. 
     The forces driving the problem may also be given by user-supplied landmarks.  
     The corners of the image, in this example, are always pinned.  This example is 
     designed for 2D or 3D images.  A rectilinear mesh is generated automatically 
     given the correct element type (Quadrilateral or Hexahedral).

     Our specific Solver for this example uses trapezoidal time stepping.  This is 
     a method for solving a second-order PDE in time.  The solution is penalized 
     by the zeroth (mass matrix) and first derivatives (stiffness matrix) of the 
     shape functions.  There is an option to perform a line search on the energy 
     after each iteration.  Optimal parameter settings require experimentation.
     The following approach tends to work well :  
        Choose the relative size of density  to elasticity (e.g. Rho / E ~= 1.)
          such that the image deforms locally and slowly.
          This also affects the stability of the solution.
        Choose the time step to control the size of the deformation at each step.
        Choose enough iterations to allow the solution to converge (this may be automated).

     Reading images is up to the user.  Either set the images using 
     SetReference/TargetImage or see the ReadImages function.  Outputs are raw images
     of the same type as the reference image.

     \note This code works for only 2 or 3 dimensions.
*/

}} // end namespace itk::fem
