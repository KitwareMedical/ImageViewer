

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMImageRegEx.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for detailm_Solver.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "FEMImageRegEx.h"
#include <strstream>

namespace itk {
namespace fem {
   
// Register load class.
typedef  ImageMetricLoad<ImageRegEx::ImageType,ImageRegEx::ImageType> LMClass2;
template class itk::fem::ImageMetricLoadImplementation<LMClass2>;

ImageRegEx::ImageRegEx( )
{
  m_FileCount=0;    
  m_Nx=256;
  m_Ny=256;
  m_MeshResolution=16*2;  // determines the 'resolution' of the grid
 
  m_MinE=9.e44;  // FIXME

  m_DescentDirection=positive;
  m_E=1.; 
  m_Maxiters=1;  
  m_dT=1;
  m_Rho=1;

  m_NumberOfIntegrationPoints=4;
  m_MetricWidth=3;
  m_DoLineSearch=false;
  m_SearchForMinAtEachLevel=false;
  m_LineSearchStep=0.05;

  m_MaxSmoothing=1;
  m_MinSmoothing=1;
  m_SmoothingStep=2;
  m_DoMultiRes=false;
}


void ImageRegEx::RunRegistration()
{
    std::cout << "beginning \n";  
    m_Solver.SetDeltatT(m_dT);  
    m_Solver.SetRho(m_Rho);    

    ReadImages();
    
    GenRegMesh(); 

    m_Solver.GenerateGFN();

    m_Load=LMClass2::New();

    m_Load->SetReferenceImage(m_RefImg);
    m_Load->SetTargetImage(m_TarImg);
//  m_Load->SetMetric(MeanSquaresImageToImageMetric<ImageType,ImageType>::New());
    m_Load->InitializeMetric();
    ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
    m_Load->SetMetricRadius(r);
    m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
    m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
    m_Load->SetSign((Float)m_DescentDirection);
    m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
    m_Load=dynamic_cast<LMClass2*> (&*m_Solver.load.Find(m_Solver.load.size()));  
  
   
    //FIXME - experiment with these values and choices for solver
    LinearSystemWrapperItpack itpackWrapper; 
    LinearSystemWrapperDenseVNL vnld;  
    itpackWrapper.SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
    itpackWrapper.SetMaximumNumberIterations(m_Solver.GetNGFN()); 
    itpackWrapper.SetTolerance(1.e-6);

    // select solution type
    // did converge in test
    
    itpackWrapper.JacobianSemiIterative(); // err 23 500 its
    
    // did not converge below here:  ordered best to worst
   // itpackWrapper.SymmetricSuccessiveOverrelaxationSuccessiveOverrelaxation(); // err 53
   // itpackWrapper.SymmetricSuccessiveOverrelaxationConjugateGradient();// err 43 
   // itpackWrapper.SuccessiveOverrelaxation(); // err 33 500 its

    // These methods failed!!
    //itpackWrapper.JacobianConjugateGradient();  // err 13 500 its
    //itpackWrapper.ReducedSystemSemiIteration(); // err 201 
    //itpackWrapper.ReducedSystemConjugateGradient(); // err 201 

     m_Solver.SetLinearSystemWrapper(&itpackWrapper); 
    //m_Solver.SetLinearSystemWrapper(&vnld); 

  /*
  * Assemble the global mass and stiffness matrix. In order to do this
  * the GFN's should already be assigned to every DOF.
  */  
  m_Solver.AssembleKandM();
 
    
  /* Solve the system in time */

  if (!m_DoMultiRes) IterativeSolve(); else MultiResSolve();

  std::cout << " interpolating vector field " << std::endl;

  GetVectorField();

  
//  ReadImages(); //FIXME
  WarpImage();

  std::cout<<"\n E " << m_E << " dt " << m_dT << " rho " << m_Rho << "\n";
  
}

void ImageRegEx::ReadImages()
{
  // DEFINE INPUT FILES
 
  const unsigned int nx = m_Nx;
  const unsigned int ny = m_Ny;

  // Read a Raw File
  typedef  itk::ImageFileReader< ImageType >      FileSourceType;
  typedef  itk::RawImageIO<ImageType::PixelType,ImageType::ImageDimension>   RawReaderType;


  FileSourceType::Pointer reffilter = FileSourceType::New();
  reffilter->SetFileName( m_ReferenceFileName );
  FileSourceType::Pointer tarfilter = FileSourceType::New();
  tarfilter->SetFileName( m_TargetFileName );

  RawReaderType::Pointer  rawReader  = RawReaderType::New();
  rawReader->SetDimensions( 0, nx );
  rawReader->SetDimensions( 1, ny );

  reffilter->SetImageIO( rawReader );
  tarfilter->SetImageIO( rawReader );

  try
    {
    reffilter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during reference file reading " << std::endl;
    std::cerr << e << std::endl;
    return;
    }
  try
    {
    tarfilter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during target file reading " << std::endl;
    std::cerr << e << std::endl;
    return;
    }
  
  std::cout << "File succesfully read ! " << std::endl;

  // now set variables
  m_RefImg=reffilter->GetOutput();
  m_TarImg=tarfilter->GetOutput();
  m_Rregion = m_RefImg->GetLargestPossibleRegion();
  m_Tregion = m_TarImg->GetLargestPossibleRegion();

}


void ImageRegEx::WarpImage()
{
 // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;

  typedef itk::WarpImageFilter<ImageType,ImageType,FieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();

  typedef WarperType::CoordRepType CoordRepType;
  typedef itk::NearestNeighborInterpolateImageFunction<ImageType,CoordRepType>
    InterpolatorType0;
  typedef itk::LinearInterpolateImageFunction<ImageType,CoordRepType>
    InterpolatorType1;
  typedef itk::BSplineInterpolateImageFunction<ImageType,CoordRepType>
    InterpolatorType2;
  InterpolatorType1::Pointer interpolator = InterpolatorType1::New();
  

  warper = WarperType::New();
  warper->SetInput( m_RefImg );
  warper->SetDeformationField( m_Field );
  warper->SetInterpolator( interpolator );
  warper->SetOutputSpacing( m_TarImg->GetSpacing() );
  warper->SetOutputOrigin( m_TarImg->GetOrigin() );
  ImageType::PixelType padValue = 1;
  warper->SetEdgePaddingValue( padValue );
  warper->Update();

  m_WarpedImage=warper->GetOutput();  

}

void ImageRegEx::GenRegMesh()
{

  
  /* We'll need these pointers to create and initialize the objectm_Solver. */
  Node::Pointer n1;
  
  // We create a uniform mesh over the image
  
  unsigned int i,j, ct=0;  Float X, Y;
  for (j=0; j<=m_Ny;j=j+m_MeshResolution){
  for (i=0; i<=m_Nx;i=i+m_MeshResolution){
    if ( i < m_Nx-1) X=Float(i); else X=Float(m_Nx-1);
    if ( j < m_Ny-1) Y=Float(j); else Y=Float(m_Ny-1); 
    n1=new Node(X,Y);
    n1->GN=ct;
    ct++;
    m_Solver.node.push_back( FEMP<Node>(&*n1) );
    //cout << " ct " << ct << " pos " << i << " " << j << endl;
  }
  }
 
  /*
   * Then we have to create the materials that will define
   * the elementm_Solver.  Only E is used in the membrane.
   */
  MaterialLinearElasticity::Pointer m;
  m=MaterialLinearElasticity::New();
  m->GN=0;       /* Global number of the material */
  m->E=m_E;  /* Young modulus -- used in the membrane */
  m->A=1.0;     /* Crossection area */
  m->h=1.0;     /* Crossection area */
  m->I=1.0;    /* Momemt of inertia */
  m->nu=0.; //.0;    /* poissons -- DONT CHOOSE 1.0!!*/
  m->RhoC=1.0;
  m_Solver.mat.push_back( FEMP<Material>(&*m) );

 

  /*
   * Next we create the finite elements that use the above
   * created nodem_Solver. 
   */
//  MembraneC02D::Pointer e1;
  Element2DC0LinearQuadrilateralStress::Pointer e1;
  
  unsigned int ctGN=0,jct=0,ict=0;
    for (unsigned int j=0; j<=m_Ny-m_MeshResolution;j=j+m_MeshResolution){ 
      ict=0;
      for (unsigned int i=0; i<=m_Nx-m_MeshResolution;i=i+m_MeshResolution){ 
        e1=Element2DC0LinearQuadrilateralStress::New();
        e1->GN=ctGN;  ctGN++;
        e1->SetNode(0,dynamic_cast<Node*>( &*m_Solver.node.Find(ctGN+jct-1) ));
        e1->SetNode(1,dynamic_cast<Node*>( &*m_Solver.node.Find(ctGN+jct) ));
        e1->SetNode(2,dynamic_cast<Node*>( &*m_Solver.node.Find((ctGN+m_Nx/m_MeshResolution)+jct+1) ));
        e1->SetNode(3,dynamic_cast<Node*>( &*m_Solver.node.Find((ctGN+m_Nx/m_MeshResolution)+jct) ));
        e1->m_mat=dynamic_cast<MaterialLinearElasticity*>( &*m_Solver.mat.Find(0) );
        m_Solver.el.push_back( FEMP<Element>(&*e1) );
        ict++;
       // std::cout << ctGN << "  elt nodes : a  " << ctGN+jct -1 << " b " <<
       //   ctGN+jct << " c " << (ctGN+nx/step)+jct+1 << " d " << (ctGN+nx/step)+jct<< endl;
      }
      jct++;
      
    }

    
 /*
  * Apply the boundary conditions.  We pin the image corners.
  * First compute which elements these will be.
  */


  LoadBC::Pointer l1;

  // elements for the corner of the image only valid if nx=ny
  unsigned int ind0=0, ind1=(unsigned int)( (float)m_Nx/(float)m_MeshResolution-1.0), ind2=ind1*(ind1+1), ind3=ind2+ind1;

  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind0));
  l1->m_dof=0;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 
  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind0));
  l1->m_dof=1;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 


  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind1));
  l1->m_dof=2;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 
  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind1));
  l1->m_dof=3;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 


  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind2));
  l1->m_dof=6;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 
  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind2));
  l1->m_dof=7;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 

  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind3));
  l1->m_dof=4;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 
  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind3));
  l1->m_dof=5;
  l1->m_value=vnl_vector<double>(1,0.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 

/*
  unsigned int ind4=128;
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind4));
  l1->m_dof=4;
  l1->m_value=vnl_vector<double>(1,10.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); 
  
  l1=LoadBC::New();
  l1->m_element=( &*m_Solver.el.Find(ind4));
  l1->m_dof=5;
  l1->m_value=vnl_vector<double>(1,10.0);
  m_Solver.load.push_back( FEMP<Load>(&*l1) ); */

  return;
}


void ImageRegEx::IterativeSolve()
{
//  m_Energy=9.e9;
if (m_SearchForMinAtEachLevel) m_MinE=9.e9;
  
  // iterative solve  
  for (unsigned int iters=0; iters<m_Maxiters; iters++){

//    float radparam=1.-(float)iters/((float)m_Maxiters-1.);
//    unsigned int maxrad=9,minrad=1;
//    SetWidthOfMetricRegion(minrad+maxrad*(unsigned int)radparam);
    /*
     * Assemble the master force vector (from the applied loads)
     */
     m_Solver.AssembleFforTimeStep();

    /*
     * Solve the system of equations for displacements (u=K^-1*F)
     */
    m_Solver.Solve();
   
    Float cure=0.0,mint=0.0,tstep=m_LineSearchStep;
  
    //m_Solver.AverageLastTwoDisplacements();

   if (m_DoLineSearch) 
   {
     if (!m_SearchForMinAtEachLevel) m_Load->SetMetricReferenceImage(m_RefImg);// in case the current img is multi-res
     if (!m_SearchForMinAtEachLevel) m_Load->SetMetricTargetImage(m_TarImg);// in case the current img is multi-res
     for (Float t=tstep; t <= 1.0; t=t+tstep) 
     {
        cure=m_Load->EvaluateMetricGivenSolution(&(m_Solver.el), t);
        if (cure <= m_MinE)
        {
          m_MinE=cure;
          mint=t;       
        }
     }
     m_Solver.AddToDisplacements(mint); 
   }
   else m_Solver.AddToDisplacements(1.);
   
   std::cout << " min E " << m_MinE << "  t " << mint << " iter " << iters << std::endl;
   //m_Solver.ZeroVector(2);
   //m_Solver.ZeroVector(3);
   // uncomment to write out every deformation SLOW due to interpolating vector field everywhere
   GetVectorField();
   WarpImage();
   WriteWarpedImage("E:\\Avants\\MetaImages\\result");
  } 
  
}


void ImageRegEx::GetVectorField()
{

  m_FieldSize[0] = m_Nx;
  m_FieldSize[1] = m_Ny;
   
  m_FieldRegion.SetSize( m_FieldSize );
  m_Field = FieldType::New();
  m_Field->SetLargestPossibleRegion( m_FieldRegion );
  m_Field->SetBufferedRegion( m_FieldRegion );
  m_Field->Allocate(); 
 
  m_Wregion.SetSize( m_FieldSize );
  m_WarpedImage = ImageType::New();
  m_WarpedImage->SetLargestPossibleRegion( m_Wregion );
  m_WarpedImage->SetBufferedRegion( m_Wregion );

  Element::ArrayType* el = &(m_Solver.el);
  vnl_vector_fixed<double,2> Pos(0.0);  // solution at the point
  vnl_vector_fixed<double,2> Sol(0.0);  // solution at the local point
  vnl_vector_fixed<double,2> Gpt(0.0);  // global position given by local point
  FieldIterator m_FieldIter( m_Field, m_FieldRegion );
  m_FieldIter.GoToBegin();
  ImageType::IndexType rindex = m_FieldIter.GetIndex();
  ImageType::IndexType tindex = m_FieldIter.GetIndex();

  for(  Element::ArrayType::iterator elt=el->begin(); elt!=el->end(); elt++) 
  {
  
    unsigned int sfsz= (*elt)->GetNumberOfNodes();
    vnl_vector<double> shapeF( sfsz );

    for (double r=-1.; r <= 1.; r=r+ 1./(float)m_MeshResolution)
    for (double s=-1.; s <= 1.; s=s+ 1./(float)m_MeshResolution)
    {
      Pos[0]=r; 
      Pos[1]=s;;
      VectorType disp; 
      vnl_vector<double> ncoord(2); 
      Gpt=(*elt)->GetGlobalFromLocalCoordinates(Pos);
      Sol=(*elt)->InterpolateSolution(Pos,*(m_Solver.GetLS()),1); // for total solution index
      for (unsigned int ii=0; ii < ImageDimension; ii++)
      { 
        rindex[ii]=(long int) Gpt[ii];
        disp[ii] =(Float) 1.*Sol[ii];
        tindex[ii]=(long int) rindex[ii]+(long int)(disp[ii]+0.5);
      }
      
      m_Field->SetPixel(rindex, disp );
    }    
  }
}


void ImageRegEx::WriteWarpedImage(const char* fname)
{

  // for image output
  std::string exte=".raw";
  std::string fnum;
  m_FileCount++;

  std::ostrstream buf;
  buf.clear();
  buf<<(m_FileCount+10)<<'\0';
  fnum=std::string(buf.str());
  buf.freeze(false);

  std::string fullfname=(fname+fnum+exte);

/*  
  FILE *fbin; 
  ImgIterator wimIter( m_WarpedImage,m_Wregion );

  fbin=fopen(fullfname.c_str(),"wb");
  ImageDataType t=0;
  // for arbitrary dimensionality
  wimIter.GoToBegin();  
  for( ; !wimIter.IsAtEnd(); ++wimIter )
  {
    t=(ImageDataType) wimIter.Get();
    fwrite(&t,sizeof(t),1,fbin); 
  }
  fclose(fbin);     
*/
 
  // Write a Raw File
  typedef  itk::ImageFileWriter< ImageType >      FileSinkType;
  typedef  itk::RawImageIO<ImageType::PixelType,ImageType::ImageDimension>   RawWriterType;

  FileSinkType::Pointer   fileSink   = FileSinkType::New();
  RawWriterType::Pointer  rawWriter  = RawWriterType::New();

  fileSink->SetImageIO( rawWriter );
  fileSink->SetFileName( fullfname.c_str() );
  fileSink->SetInput( m_WarpedImage );

  
  try
    {
    fileSink->Write();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception caught during Raw file writing " << std::endl;
    std::cerr << e << std::endl;
    return;
    }

  
}

void ImageRegEx::MultiResSolve()
{
     
  typedef DiscreteGaussianImageFilter<ImageType, ImageType>  SmootherType;
//  typedef MeanImageFilter<ImageType, ImageType>  SmootherType;
  SmootherType::Pointer filter = SmootherType::New();
  //itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::Pointer  edgefilter = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::New();
  //itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::Pointer edgefilter
  //  = itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::New();
  itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::Pointer
    edgefilter = itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  edgefilter->SetVariance(1.0f);
  edgefilter->SetMaximumError(.01f);

  ImageType::SizeType neighRadius;
  neighRadius[0] = 1;
  neighRadius[1] = 1;
  //filter->SetRadius(neighRadius);

  // run the algorithm
  filter->SetVariance(1.0f);
  filter->SetMaximumError(.01f);
      
  float smoothing;
  // run the algorithm
  for (smoothing=m_MaxSmoothing; smoothing >= m_MinSmoothing; smoothing/=m_SmoothingStep)
  {

//    neighRadius[0] =(unsigned long) smoothing;
//    neighRadius[1] =(unsigned long) smoothing;
//    filter->SetRadius(neighRadius);

    filter->SetVariance(smoothing);
    edgefilter->SetVariance(smoothing);

    filter->SetInput(m_TarImg);
    filter->Update();

    edgefilter->SetInput(m_TarImg);
    edgefilter->Update();    

    m_Load->SetMetricTargetImage(filter->GetOutput());    
    filter->SetInput(m_RefImg);
    filter->Update();
    edgefilter->SetInput(m_RefImg);
    edgefilter->Update();    
    m_Load->SetMetricReferenceImage(filter->GetOutput());
    IterativeSolve();
  }
// now full res
  m_Load->SetMetricTargetImage(m_TarImg); 
  m_Load->SetMetricReferenceImage(m_RefImg);
  IterativeSolve();
  //if (smoothing <= m_MinSmoothing) m_TarImg=edgefilter->GetOutput(); // //FIXME for testing

  return;
}


}} // end namespace itk::fem


int main() 
{
  const char* m_ReferenceFileName;
  const char* m_TargetFileName;

  itk::fem::ImageRegEx X; // Declare the registration clasm_Solver.
 
  X.m_Solver.SetAlpha(0.5);
  X.SetDescentDirectionMinimize();// for Mean Squares
  X.DoLineSearch(true);// Perform line search at each iteration


/* typically Set by user interaction */  
  X.SetMaximumIterations(5);// Iterations at each resolution
  X.SetTimeStep(0.01);// Time step (controls solution size)
  X.SetElasticity(1.0);// Physical property of elasticity
  X.SetRho(1.0);// Controls damping

  std::cout << "input E  , m_Maxiters , dt , rho :" ;  
  std::cin >> X.m_E >> X.m_Maxiters >> X.m_dT >> X.m_Rho; 
  
//    m_ReferenceFileName="E:\\Avants\\MetaImages\\callosa_seg2.im"; 
//    m_TargetFileName="E:\\Avants\\MetaImages\\callosa_seg1.im"; 
//    m_ReferenceFileName="E:\\Avants\\MetaImages\\callosa1_dt_shift.im";  
//    m_ReferenceFileName="E:\\Avants\\MetaImages\\callosa1_dt.im"; 
//    m_TargetFileName="E:\\Avants\\MetaImages\\callosa2_dt.im"; 
  X.m_Nx=64;   // set image size
  X.m_Ny=64;
 
//  m_ReferenceFileName="E:\\Avants\\MetaImages\\gauss_im1.im"; 
//  m_TargetFileName="E:\\Avants\\MetaImages\\gauss_im2.im";
  m_ReferenceFileName="E:\\Avants\\MetaImages\\brain_slice1smth.im"; // good params E=1 its=10 dt=1. rho= 1.e7
  m_TargetFileName="E:\\Avants\\MetaImages\\brain_slice2smth.im";
    
  X.SetReferenceFile(m_ReferenceFileName);
  X.SetTargetFile(m_TargetFileName);
  
  X.SetMeshResolution(8);//  Number of voxels per element

  X.SetNumberOfIntegrationPoints(2);// Resolution of energy integration
  X.SetWidthOfMetricRegion(2);
  X.DoMultiRes(false);// Use multi-resolution strategy
  X.DoSearchForMinAtEachResolution(true);// Minimize at each resolution
  X.m_MaxSmoothing=12.0; // set multi-res parameters
  X.m_MinSmoothing=12.;
  X.m_SmoothingStep=4.0;
  X.RunRegistration();
  X.WriteWarpedImage("E:\\Avants\\MetaImages\\result");

  return 0;
}
