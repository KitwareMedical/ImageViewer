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
#include "itkFEMGenerateMesh.h"
//#include "itkFEMLoadLandmark.h"
#include "itkFEMLinearSystemWrapperItpack.h"
#include "itkFEMLinearSystemWrapperDenseVNL.h"

#include "itkImage.h"
#include "itkFileIOToImageFilter.h"
#include "itkImageFileReader.h" 
#include "itkCastImageFilter.h"
#include "itkMultiResolutionPyramidImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkRawImageIO.h"
#include "itkRawImageWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"

#include "itkVector.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkVectorCastImageFilter.h"
#include "itkWarpImageFilter.h"
#include "itkImageFileReader.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"

#include "itkTranslationTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_math.h"
#include "vnl/vnl_vector_fixed.h"

#include <iostream>
#include <string>
#include <fstream>



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
     designed for 2D images.  The mesh is the only dimension dependent part of the
     class.  If a mesh for any other dimension can be supplied and the correct elements
     and nodes can be chosen, then the example will work in other dimensions.

     Our specific Solver for this example uses the Crank Nicolson method.  This is 
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
     SetReferenceImage or see the ReadImages() function.  Outputs are raw images.
     Change the ImageDataType typedef to output your type or keep it the same
     to always output unsigned char.

I am no longer using the old (non-LM) example, but added a UseLandmarks 
boolean to this one.  I've also changed the Load so that the metric can be set.
Still have to recompile to make that change, but nonetheless a step forward.
The multi-res option is improving.  Some parameters I've been
using :   E = (7000, 4000) , iters=(5,10), dt (1.e-6, 1), rho=(1.e-4,50),

The first in the pair is for single res, the 2nd for multi-res.  I've also
begun using alpha = 1.0 as it privileges the new forces more than the old,
making the registration more flexible (less "memory".)  
*/
class ImageRegLMEx
{
public:
  
  typedef LinearSystemWrapperItpack LinearSystemSolverType;
  typedef SolverCrankNicolson SolverType;
  enum Sign { positive = 1, negative = -1 };
  typedef double Float;
  typedef unsigned char ImageDataType;
  typedef Image< unsigned char, 2 > ImageType;
  typedef Image< float, 2 > FloatImageType;
  typedef ImageType::SizeType ImageSizeType;
  enum { ImageDimension = 2 };
  typedef itk::fem::Element2DC0LinearQuadrilateralMembrane ElementType;
  typedef itk::MeanSquaresImageToImageMetric<ImageType,ImageType> MetricType;
  typedef itk::Vector<Float,ImageDimension> VectorType;
  typedef itk::Image<VectorType,ImageDimension> FieldType;
  typedef itk::WarpImageFilter<ImageType,ImageType,FieldType> WarperType; 
  typedef itk::ImageRegionIteratorWithIndex<ImageType> ImageIterator; 
  typedef itk::ImageRegionIteratorWithIndex<FieldType> FieldIterator; 

  /**
   * Easy access to the FEMObjectFactory. We create a new class
   * whose name is shorter and it's not templated...
   */
  class FEMOF : public FEMObjectFactory<FEMLightObject>{};
  
  /* Main functions */
 
  /** Read the configuration file to set up the example parameters */
  bool ReadConfigFile(const char*);

  /** Call this to register two images. */
  void  RunRegistration(); 
  
 /** Call this to write out images - a counter is attached to the 
  *  file name so we can output a numbered sequence tracking the deformation.
  */
  void  WriteWarpedImage(const char* fn);

  /** Helper functions */
  /** Reads images from m_ReferenceFileName and m_TargetFileName */
  void  ReadImages();  
  /** This function generates a rectangular mesh of ElementsPerSide^D size */
  void CreateMesh(ImageSizeType MeshOrigin, ImageSizeType MeshSize, 
                              double ElementsPerSide, Solver& S);
  /** The loads are entered into the solver. */
  void ApplyLoads(); 
  /**  Builds the itpack linear system wrapper with appropriate parameters. */
  void CreateLinearSystemSolver();
  /** The solution loop */
  void  IterativeSolve();  
  /** The solution loop for a simple multi-resolution strategy. */
  void  MultiResSolve();
  void  MultiResPyramidSolve();
  /** Evaluates the energy by calling the image metric */
  Float EvaluateEnergy();
  /** Interpolates the vector field over the domain */
  void  GetVectorField(); 
  /** This is used for changing between mesh resolutions */
  void SampleVectorFieldAtNodes();
  /** Applies the warp to the reference image */
  void  WarpImage();      

  /** Writes the displacement field to a file */
  void WriteDispField();

  /** Set the following parameters to run the example */
  void SetReferenceFile(const char* r) {m_ReferenceFileName=r;}
  void SetTargetFile(const char* t) {m_TargetFileName=t;}
  void SetLandmarkFile(const char* l) {m_LandmarkFileName=l; }
  void SetResultsFile(const char* r) {m_ResultsFileName=r;}
  void SetMeshResolution(unsigned int i){ m_MeshResolution=i;}
  void SetNumberOfIntegrationPoints(unsigned int i){ m_NumberOfIntegrationPoints=i;}
  void SetWidthOfMetricRegion(unsigned int i) { m_MetricWidth=i;}
  void SetMaximumIterations(unsigned int i) { m_Maxiters=i;}
  void SetTimeStep(Float i) { m_dT=i;}
  void SetElasticity(Float i) { m_E=i;} /** Stiffness Matrix weight */
  void SetRho(Float r) { m_Rho=r;} /** Mass matrix weight */  
  void SetDescentDirectionMinimize() { m_DescentDirection=positive;} /** Tries to minimize energy */
  void SetDescentDirectionMaximize() { m_DescentDirection=negative;} /** Tries to maximize energy */
  void DoLineSearch(bool b) { m_DoLineSearchOnImageEnergy=b; } /** Finds the minimum energy between the current and next solution by linear search.*/
  void DoMultiRes(bool b) { m_DoMultiRes=b; } 
  void DoSearchForMinAtEachResolution(bool b) { m_SearchForMinAtEachLevel=b; } 
  void UseLandmarks(bool b) {m_UseLandmarks=b;}

  const char* m_ReferenceFileName;  
  const char* m_TargetFileName;
  const char* m_LandmarkFileName;
  const char* m_ResultsFileName;

  unsigned int m_MeshResolution; // determines resolution of mesh
  unsigned int m_NumberOfIntegrationPoints;
  unsigned int m_MetricWidth;
  unsigned int m_Maxiters; // max iterations
  unsigned int m_TotalIterations;
  float m_MaxSmoothing; // max smoothing
  float m_MinSmoothing; // min smoothing
  float m_SmoothingStep;// smoothing step size
  Float m_dT; // time step
  Float m_E;  // elasticity 
  ImageSizeType m_ImageSize; // image size
  ImageSizeType m_ImageOrigin; // image size
  /** Gives the ratio of original image size to current image size - for dealing with multi-res.*/
  ImageSizeType m_ImageScaling; 
  Float m_Energy; // current value of energy
  Float m_MinE;  // minimum recorded energy
  Float m_Rho;   // mass matrix weight
  bool  m_DoLineSearchOnImageEnergy;  
  bool  m_DoMultiRes;
  bool  m_SearchForMinAtEachLevel;
  bool  m_UseLandmarks;
  Float m_LineSearchStep;
  Sign  m_DescentDirection;
  int m_FileCount; // keeps track of number of files written

  SolverType m_Solver; // Defines the solver to use
  SolverType* GetSolver(){return &m_Solver;}
  ImageMetricLoad<ImageType,ImageType>* m_Load; // Defines the load to use
   
  ImageType::RegionType m_FieldRegion;
  ImageType::SizeType m_FieldSize;
  FieldType::Pointer m_Field;

  // define the warper
  WarperType::Pointer m_Warper; 
  
 // declare a new image to hold the warped  reference
  ImageType::Pointer  m_WarpedImage;
  ImageType::RegionType m_Wregion; 
  ImageType::IndexType m_Windex;
 
 // declare images for target and reference
  ImageType::Pointer  m_RefImg;
  ImageType::Pointer  m_TarImg;
  ImageType::RegionType m_Rregion;
  ImageType::RegionType m_Tregion;
  ImageType::IndexType m_Rindex;
  ImageType::IndexType m_Tindex;

  ElementType::Pointer e1;
/** for quadrature rules, see
"Exact Integrations of Polynomials and Symmetric 
Quadrature Formulas over Arbitrary Polyhedral Grids", journal
of computational physics, 140, 122-147 (1998)

  
A. Stroud (1971), Approximate Calculation of Multiple Integrals,
Prentice-Hall.
  */

  ImageRegLMEx( ); 
 

};

}} // end namespace itk::fem
