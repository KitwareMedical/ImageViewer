/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMImageRegLMEx.cxx
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

#include "FEMImageRegLMEx.h"

namespace itk {
namespace fem {
   
// Register load class.
typedef  ImageMetricLoad<ImageRegLMEx::ImageType,ImageRegLMEx::ImageType> LMClass2;
template class itk::fem::ImageMetricLoadImplementation<LMClass2>;

ImageRegLMEx::ImageRegLMEx( )
{
  m_FileCount=0; 
  m_MeshResolution=16*2;  // determines the 'resolution' of the grid
 
  m_MinE=9.e44;  // FIXME

  m_DescentDirection=positive;
  m_E=1.; 
  m_Maxiters=1;  
  m_dT=1;
  m_Rho=1;

  m_NumberOfIntegrationPoints=4;
  m_MetricWidth=3;
  m_DoLineSearchOnImageEnergy=false;
  m_SearchForMinAtEachLevel=false;
  m_LineSearchStep=0.025;

  m_MaxSmoothing=1;
  m_MinSmoothing=1;
  m_SmoothingStep=2;
  m_DoMultiRes=false;
  m_UseLandmarks=false;

  m_ReferenceFileName = NULL;
  m_TargetFileName = NULL;
  m_LandmarkFileName = NULL;
  m_TotalIterations=0;

  for (unsigned int i=0; i < ImageDimension; i++)
  {
     m_ImageScaling[i]=1;
     m_ImageSize[i]=0;
     m_ImageOrigin[i]=0;
  }
}


void ImageRegLMEx::RunRegistration()
{
    std::cout << "beginning \n";  
    m_Solver.SetDeltatT(m_dT);  
    m_Solver.SetRho(m_Rho);    

    ReadImages();
 
   
  // Solve the system in time 

  if (!m_DoMultiRes) 
  {
    
    CreateMesh(m_ImageOrigin,m_ImageSize,(double)m_MeshResolution,m_Solver); 
    ApplyLoads();

    m_Solver.GenerateGFN(); 

    LinearSystemWrapperItpack itpackWrapper; 
    itpackWrapper.SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
    itpackWrapper.SetMaximumNumberIterations(m_Solver.GetNGFN()); 
    itpackWrapper.SetTolerance(1.e-6);
    itpackWrapper.JacobianSemiIterative(); 
    m_Solver.SetLinearSystemWrapper(&itpackWrapper); 

    m_Load=LMClass2::New();

    m_Load->SetReferenceImage(m_RefImg);
    m_Load->SetTargetImage(m_TarImg);
    MetricType::Pointer msqp=MetricType::New();
    msqp->SetScaleGradient(1.0); // this is the default(?)
    m_Load->SetMetric(msqp.GetPointer());
    m_Load->InitializeMetric();
    ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
    m_Load->SetMetricRadius(r);
    m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
    m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
    m_Load->SetSign((Float)m_DescentDirection);
    m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
    m_Load=dynamic_cast<LMClass2*> (&*m_Solver.load.Find(m_Solver.load.size()));  
  
 
    m_Solver.AssembleKandM();
 
    IterativeSolve();

    GetVectorField();
    WarpImage(); 
  }
  else 
  {
    MultiResSolve();
  }
    

  std::cout<<"\n E " << m_E << " dt " << m_dT << " rho " << m_Rho << "\n";
  
}

void ImageRegLMEx::ReadImages()
{
  // DEFINE INPUT FILES
 
  // Read a Raw File
  typedef  itk::ImageFileReader< ImageType >      FileSourceType;
  typedef  itk::RawImageIO<ImageType::PixelType,ImageType::ImageDimension>   RawReaderType;


  FileSourceType::Pointer reffilter = FileSourceType::New();
  reffilter->SetFileName( m_ReferenceFileName );
  FileSourceType::Pointer tarfilter = FileSourceType::New();
  tarfilter->SetFileName( m_TargetFileName );

  RawReaderType::Pointer  rawReader  = RawReaderType::New();
  
  for (int ii=0; ii<ImageDimension; ii++) rawReader->SetDimensions( ii, m_ImageSize[ii] );

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
  m_ImageSize = m_TarImg->GetLargestPossibleRegion().GetSize();
  for (unsigned int i=0; i < ImageDimension; i++) m_ImageOrigin[i]=0;
}

bool ImageRegLMEx::ReadConfigFile(const char* fname)
  // Reads the parameters necessary to configure the example & returns
  // false if no configuration file is found
{
  std::ifstream f;
  char buffer[80] = {'\0'};
  float fbuf = 0.0;
  unsigned int ibuf = 0;
  char* sbuf = new char[256];

  std::cout << "Reading config file..." << fname << std::endl;
  f.open(fname);
  if (f) {
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->m_Solver.SetAlpha(fbuf);
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (ibuf == 0) { this->SetDescentDirectionMinimize(); }
    else { this->SetDescentDirectionMaximize(); }

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (ibuf == 1) { this->DoLineSearch(true); }
    else { this->DoLineSearch(false); }

    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->SetElasticity(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->SetMaximumIterations(ibuf);
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->SetTimeStep(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    this->SetRho(fbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
  sbuf = new char[256];
  strcpy(sbuf, buffer);
    this->SetReferenceFile(sbuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
  sbuf = new char[256];
  strcpy(sbuf, buffer);
    this->SetTargetFile(sbuf);
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;

    if (ibuf == 1) {
      this->UseLandmarks(true);
    sbuf = new char[256];
    strcpy(sbuf, buffer);
      this->SetLandmarkFile(sbuf);
    }
    else { this->UseLandmarks(false); }

    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;
  sbuf = new char[256];
  strcpy(sbuf, buffer);
    this->SetResultsFile(sbuf);
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_ImageSize[0] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_ImageSize[1] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->SetMeshResolution(ibuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->SetNumberOfIntegrationPoints(ibuf);

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->SetWidthOfMetricRegion(ibuf);
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (ibuf == 1) {
      this->DoMultiRes(true);

      FEMLightObject::SkipWhiteSpace(f);
      f >> ibuf;
      if (ibuf == 1) { this->DoSearchForMinAtEachResolution(true); }
      else { this->DoSearchForMinAtEachResolution(true); }

      FEMLightObject::SkipWhiteSpace(f);
      f >> fbuf;
      this->m_MaxSmoothing = fbuf;

      FEMLightObject::SkipWhiteSpace(f);
      f >> fbuf;
      this->m_MinSmoothing = fbuf;
    
      FEMLightObject::SkipWhiteSpace(f);
      f >> fbuf;
      this->m_SmoothingStep = fbuf;
    }
    else { this->DoMultiRes(false); }


    f.close();
    std::cout << "Example configured." << std::endl;
    return true;
  }
  else { 
    std::cout << "No configuration file specified...quitting.\n"; 
    return false;
  }  
}

void ImageRegLMEx::WriteDispField()
{
  ;
}


void ImageRegLMEx::WarpImage()
{
 // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;

   // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;
  bool InverseWarp=true;


  if (InverseWarp)
  {
    bool InImage=true;
    m_WarpedImage->Allocate();
    FieldIterator m_FieldIter( m_Field, m_FieldRegion );
    m_FieldIter.GoToBegin();
    ImageType::IndexType rindex = m_FieldIter.GetIndex();
    ImageType::IndexType tindex = m_FieldIter.GetIndex();

    m_FieldIter.GoToBegin();  
    for( ; !m_FieldIter.IsAtEnd(); ++m_FieldIter )
    {
      rindex=m_FieldIter.GetIndex();
      tindex=m_FieldIter.GetIndex();
      VectorType disp=m_FieldIter.Get();

      for (unsigned int ii=0; ii < ImageDimension; ii++)
      { 
        tindex[ii]+=(long int)(disp[ii]);  //+0.5);
        if (tindex[ii] >=0 && tindex[ii] < m_FieldSize[ii]) InImage=true;
          else 
          {
            InImage=false;
            ii=ImageDimension;
          }
      }

      if (InImage)
      {
        ImageDataType t = (ImageDataType) m_RefImg->GetPixel(tindex);
        m_WarpedImage->SetPixel(rindex, t );
      }
      else m_WarpedImage->SetPixel(rindex,1);
    }
  }
  else 
  {
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
  warper->SetOutputSpacing( m_RefImg->GetSpacing() );
  warper->SetOutputOrigin( m_RefImg->GetOrigin() );
  ImageType::PixelType padValue = 1;
  warper->SetEdgePaddingValue( padValue );
  warper->Update();

  m_WarpedImage=warper->GetOutput();  
  }

}


void ImageRegLMEx::CreateMesh(ImageSizeType MeshOrigin, ImageSizeType MeshSize, 
                              double ElementsPerSide, Solver& S)
{

  //
   // Create the materials that will define
   // the element.  Only E is used in the membrane.
   ///
  MaterialLinearElasticity::Pointer m;
  m=MaterialLinearElasticity::New();
  m->GN=0;       // Global number of the material ///
  m->E=m_E;  // Young modulus -- used in the membrane ///
  m->A=1.0;     // Crossection area ///
  m->h=1.0;     // Crossection area ///
  m->I=1.0;    // Momemt of inertia ///
  m->nu=0.; //.0;    // poissons -- DONT CHOOSE 1.0!!///
  m->RhoC=1.0;
  m_Solver.mat.push_back( FEMP<Material>(&*m) ); 

  e1=ElementType::New();
  e1->m_mat=dynamic_cast<MaterialLinearElasticity*>( &*m_Solver.mat.Find(0) );

  vnl_vector<double> MeshOriginV; MeshOriginV.resize(ImageDimension); 
  vnl_vector<double> MeshSizeV;   MeshSizeV.resize(ImageDimension); 
  vnl_vector<double> ElementsPerDimension;  ElementsPerDimension.resize(ImageDimension); 
  for (unsigned int i=0; i<ImageDimension; i++)
  { 
    MeshSizeV[i]=(double)MeshSize[i]-1; // FIX ME  make more general
    MeshOriginV[i]=(double)MeshOrigin[i];// FIX ME make more general
    ElementsPerDimension[i]=(double)ElementsPerSide;
  }

  if (ImageDimension == 2)
  GenerateMesh<Element2DC0LinearQuadrilateral>::
    Rectangular(e1,S,MeshOriginV,MeshSizeV,ElementsPerDimension);   
  delete e1;

}

void ImageRegLMEx::ApplyLoads()
{
 //
  // Apply the boundary conditions.  We pin the image corners.
  // First compute which elements these will be.
  ///

  LoadBC::Pointer l1;

  // elements for the corner of the image only valid if nx=ny
  unsigned int ind0=0; 
  unsigned int ind1=(unsigned int)(m_MeshResolution-1.0);
  unsigned int ind2=m_MeshResolution*(m_MeshResolution-1); 
  unsigned int ind3=m_MeshResolution*m_MeshResolution-1; 
 

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

  if (m_UseLandmarks)
  {
  // Landmark loads
  std::ifstream f;
  std::cout << m_LandmarkFileName << std::endl;
  std::cout << "Loading landmarks...";
  f.open(m_LandmarkFileName);
  if (f) {

    try { 
      m_Solver.Read(f); 
    }
    catch (itk::ExceptionObject &err) { 
      std::cerr << "Exception: " << err;
      f.close();
    }
    
    f.close();
    std::cout << "done" << std::endl;
  }
  else { std::cout << "no landmark file specified." << std::endl; }
  }
  
  m_Load=LMClass2::New();

  m_Load->SetReferenceImage(m_RefImg);
  m_Load->SetTargetImage(m_TarImg);
  MetricType::Pointer msqp=MetricType::New();
  msqp->SetScaleGradient(1.0); // this is the default(?)
  m_Load->SetMetric(msqp.GetPointer());
  m_Load->InitializeMetric();
  ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
  m_Load->SetMetricRadius(r);
  m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
  m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
  m_Load->SetSign((Float)m_DescentDirection);
  m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
  m_Load=dynamic_cast<LMClass2*> (&*m_Solver.load.Find(m_Solver.load.size()));  
  
  return;
}


void ImageRegLMEx::IterativeSolve()
{
//  m_Energy=9.e9;
if (m_SearchForMinAtEachLevel) m_MinE=9.e9;
  
  // iterative solve  
  for (unsigned int iters=0; iters<m_Maxiters; iters++){
    /*
     * Assemble the master force vector (from the applied loads)
     */
     m_Solver.AssembleFforTimeStep();

    /*
     * Solve the system of equations for displacements (u=K^-1*F)
     */
    m_Solver.Solve();  
    m_Solver.GoldenSection();
    Float cure=0.0,mint=0.0,tstep=m_LineSearchStep;
  

   if (m_DoLineSearchOnImageEnergy) 
   {
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
   else 
   {
     cure=m_Load->EvaluateMetricGivenSolution(&(m_Solver.el), 1.0);
     if (cure <= m_MinE)
     {
        m_MinE=cure;
        mint=iters;   
        m_Solver.AddToDisplacements();  
        m_Solver.PrintDisplacements();
        GetVectorField();
        SampleVectorFieldAtNodes(); 
        m_Solver.PrintDisplacements();
        std::cout;
     } else iters=m_Maxiters;
   }
   
   std::cout << " min E " << m_MinE << " Cur E " << cure << "  t " << mint << " iter " << iters << std::endl;
 
   // uncomment to write out every deformation SLOW due to interpolating vector field everywhere.
   //GetVectorField();
   //WarpImage();
   //WriteWarpedImage(m_ResultsFileName);
   
   m_TotalIterations++;
  } 
  
}


void ImageRegLMEx::GetVectorField()
{

  for (unsigned int i=0; i<ImageDimension; i++) m_FieldSize[i] = m_ImageSize[i];
   
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
        rindex[ii]=(long int) (Gpt[ii]+0.5);
        disp[ii] =(Float) 1.*Sol[ii];
      }
      m_Field->SetPixel(rindex, disp );
    }    
  }
}


void ImageRegLMEx::WriteWarpedImage(const char* fname)
{

  // for image output
  FILE *fbin; 
  std::string exte=".raw";
  std::string fnum;
  m_FileCount++;

  OStringStream buf;
  buf<<(m_FileCount+10);
  fnum=std::string(buf.str().c_str());

  std::string fullfname=(fname+fnum+exte);

  ImageIterator wimIter( m_WarpedImage,m_Wregion );

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

  
}


void ImageRegLMEx::SampleVectorFieldAtNodes()
{

  // Here, we need to iterate through the nodes, get the nodal coordinates,
  // sample the VF at the node and place the values in the SolutionVector.

  Node::ArrayType* nodes = &(m_Solver.node);
  Element::VectorType coord;  
  VectorType SolutionAtNode;

  FieldIterator m_FieldIter( m_Field, m_FieldRegion );
  m_FieldIter.GoToBegin();
  ImageType::IndexType rindex = m_FieldIter.GetIndex();

  for(  Node::ArrayType::iterator node=nodes->begin(); node!=nodes->end(); node++) 
  {
    coord=(*node)->GetCoordinates();
    for (unsigned int ii=0; ii < ImageDimension; ii++)
    { 
      rindex[ii]=(long int) (coord[ii]+0.5);
    }
    SolutionAtNode=m_Field->GetPixel(rindex);
    // Now put it into the solution!
    for (unsigned int ii=0; ii < ImageDimension; ii++)
    { 
      Float Sol=SolutionAtNode[ii]*((Float)m_ImageScaling[ii]);
      Float OldSol=m_Solver.GetLinearSystemWrapper()->
        GetSolutionValue((*node)->GetDegreeOfFreedom(ii),m_Solver.TotalSolutionIndex);   
      Float diff=Sol-OldSol;
      m_Solver.GetLinearSystemWrapper()->
        SetSolutionValue((*node)->GetDegreeOfFreedom(ii),Sol,m_Solver.TotalSolutionIndex);   
    }
  }

}


void ImageRegLMEx::MultiResSolve()
{

  vnl_vector<Float> LastResolutionSolution;
//   Setup a multi-resolution pyramid
  typedef itk::MultiResolutionPyramidImageFilter<FloatImageType,FloatImageType>
    PyramidType;
  typedef PyramidType::ScheduleType ScheduleType;
  PyramidType::Pointer pyramid1 = PyramidType::New();
  PyramidType::Pointer pyramid2 = PyramidType::New();

  typedef itk::CastImageFilter<ImageType,FloatImageType> CasterType1;
  CasterType1::Pointer caster1 = CasterType1::New();
  typedef itk::CastImageFilter<FloatImageType,ImageType> CasterType2;
  CasterType2::Pointer caster2 = CasterType2::New();

  caster1->SetInput(m_RefImg); caster1->Update();
  pyramid1->SetInput( caster1->GetOutput());
  caster1->SetInput(m_TarImg); caster1->Update();
  pyramid2->SetInput( caster1->GetOutput());

// set schedule by specifying the number of levels;
  unsigned int numLevels = 2;
  itk::Vector<unsigned int,ImageDimension> factors;
  factors.Fill( 1 << (numLevels - 1) );
  pyramid1->SetNumberOfLevels( numLevels );
  pyramid2->SetNumberOfLevels( numLevels );

//  ImageType::SizeType Isz=m_RefImg->GetLargestPossibleRegion().GetSize();
  ScheduleType SizeReduction=pyramid1->GetSchedule();

  for (unsigned int i=0; i<numLevels; i++)
  {
    pyramid1->Update();
    pyramid1->GetOutput( i )->Update();
    pyramid2->Update();
    pyramid2->GetOutput( i )->Update();

    ImageType::SizeType Isz=pyramid2->GetOutput( i )->GetLargestPossibleRegion().GetSize();
//  m_MeshResolution=

    CreateMesh(m_ImageOrigin,Isz,(double)m_MeshResolution,m_Solver); 
    ApplyLoads();
    //if ( i == 0) {
    m_Solver.GenerateGFN(); 

    LinearSystemWrapperItpack itpackWrapper; 
    itpackWrapper.SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
    itpackWrapper.SetMaximumNumberIterations(m_Solver.GetNGFN()); 
    itpackWrapper.SetTolerance(1.e-6);
    itpackWrapper.JacobianSemiIterative(); 
    m_Solver.SetLinearSystemWrapper(&itpackWrapper); 

    m_Load=LMClass2::New();

    caster2->SetInput(pyramid1->GetOutput(i)); caster2->Update();
    m_Load->SetReferenceImage(m_RefImg);
    caster2->SetInput(pyramid2->GetOutput(i)); caster2->Update();
    m_Load->SetTargetImage(m_TarImg);
    MetricType::Pointer msqp=MetricType::New();
    msqp->SetScaleGradient(1.0); // this is the default(?)
    m_Load->SetMetric(msqp.GetPointer());
    m_Load->InitializeMetric();
    ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
    m_Load->SetMetricRadius(r);
    m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
    m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
    m_Load->SetSign((Float)m_DescentDirection);
    m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
    m_Load=dynamic_cast<LMClass2*> (&*m_Solver.load.Find(m_Solver.load.size()));  
    //}
    caster2->SetInput(pyramid1->GetOutput(i)); caster2->Update();
    m_Load->SetReferenceImage(caster2->GetOutput());  
    m_Load->SetMetricReferenceImage(caster2->GetOutput());  

    caster2->SetInput(pyramid2->GetOutput(i)); caster2->Update();
    m_Load->SetTargetImage(caster2->GetOutput());  
    m_Load->SetMetricTargetImage(caster2->GetOutput());  

    m_Solver.AssembleKandM();
 
    if (i > 0)
    {
      for (unsigned int j=0; j<m_Solver.GetNGFN(); j++)
      {
        m_Solver.GetLinearSystemWrapper()->
          SetSolutionValue(j,LastResolutionSolution[j],m_Solver.TotalSolutionIndex);
      }
    }


    IterativeSolve();

    if (i < numLevels-1)
    {
      Float s1=(Float)SizeReduction[i][0];  
      Float s2=(Float)SizeReduction[i+1][0];  
      Float Magnification=s1/s2;

      LastResolutionSolution.clear();
      LastResolutionSolution.resize(m_Solver.GetNGFN());
      unsigned int TotalSolutionIndex=1; // from SolverType
      for (unsigned int j=0; j<m_Solver.GetNGFN(); j++)
      {
        double temp=m_Solver.GetLinearSystemWrapper()->
          GetSolutionValue(j,TotalSolutionIndex);
        LastResolutionSolution[j]=temp*Magnification;
      } 
    }
    
    if ( i == numLevels-1 ) 
    { 
      GetVectorField();
      WarpImage();     
    }
  }
  //m_RefImg=caster2->GetOutput(); // //FIXME for testing

  //LinearSystemSolverType* temp=
  //    dynamic_cast<LinearSystemSolverType*>(m_Solver.GetLinearSystemWrapper());
  //delete temp;

  return;
  
}


}} // end namespace itk::fem


int main() 
{
  itk::fem::ImageRegLMEx X; // Declare the registration clasm_Solver.
 
  if (!X.ReadConfigFile("U:\\itk\\Insight\\Examples\\FEM\\FEMregLMparams.txt")) { return -1; }

  X.RunRegistration();
  X.WriteWarpedImage(X.m_ResultsFileName);

  return 0;
}

/*


void ImageRegLMEx::CreateLinearSystemSolver()
{
    //FIXME - experiment with these values and choices for solver
    LinearSystemSolverType* itpackWrapper=new LinearSystemSolverType; 
    itpackWrapper->SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
    itpackWrapper->SetMaximumNumberIterations(m_Solver.GetNGFN()); 
    itpackWrapper->SetTolerance(1.e-6);

    // select solution type
    // did converge in test
    
    itpackWrapper->JacobianSemiIterative(); // err 23 500 its
    m_Solver.SetLinearSystemWrapper(itpackWrapper); 

    // did not converge below here:  ordered best to worst
   // itpackWrapper->SymmetricSuccessiveOverrelaxationSuccessiveOverrelaxation(); // err 53
   // itpackWrapper->SymmetricSuccessiveOverrelaxationConjugateGradient();// err 43 
   // itpackWrapper->SuccessiveOverrelaxation(); // err 33 500 its

    // These methods failed!!
    //itpackWrapper->JacobianConjugateGradient();  // err 13 500 its
    //itpackWrapper->ReducedSystemSemiIteration(); // err 201 
    //itpackWrapper->ReducedSystemConjugateGradient(); // err 201 

    
}



void ImageRegLMEx::MultiResSolve()
{
  typedef itk::CastImageFilter<ImageType,FloatImageType> CasterType1;
  CasterType1::Pointer caster1 = CasterType1::New();
  typedef itk::CastImageFilter<FloatImageType,ImageType> CasterType2;
  CasterType2::Pointer caster2 = CasterType2::New();

  typedef DiscreteGaussianImageFilter<FloatImageType,FloatImageType>  SmootherType;
  SmootherType::Pointer smoothfilter = SmootherType::New();
  //itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::Pointer  edgefilter = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>::New();
  //itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::Pointer edgefilter
  //  = itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>::New();
  itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::Pointer
    edgefilter = itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType>::New();
  
  edgefilter->SetVariance(1.0f);
  edgefilter->SetMaximumError(.01f);

  smoothfilter->SetVariance(1.0f);
  smoothfilter->SetMaximumError(.01f);
      
  float smoothing;
  // run the algorithm

  CreateMesh(); 
  ApplyLoads();
  m_Solver.GenerateGFN();
  CreateLinearSystemSolver();
  m_Solver.AssembleKandM();
  
  for (smoothing=m_MaxSmoothing; smoothing >= m_MinSmoothing; smoothing/=m_SmoothingStep)
  {
    smoothfilter->SetVariance(smoothing);
    edgefilter->SetVariance(smoothing);

    // take care of target image filtering 
    caster1->SetInput(m_TarImg); caster1->Update();
    smoothfilter->SetInput(caster1->GetOutput());
    smoothfilter->Update();
    edgefilter->SetInput(m_TarImg);
    edgefilter->Update();    
    caster2->SetInput(smoothfilter->GetOutput()); caster2->Update();
    m_Load->SetMetricTargetImage(caster2->GetOutput());  

    // take care of ref image filtering  
    caster1->SetInput(m_RefImg); caster1->Update();
    smoothfilter->SetInput(caster1->GetOutput());
    smoothfilter->Update();
    edgefilter->SetInput(m_RefImg);
    edgefilter->Update();    
    caster2->SetInput(smoothfilter->GetOutput()); caster2->Update();
    m_Load->SetMetricReferenceImage(caster2->GetOutput());
 
    IterativeSolve();

  }
// now full res
  m_Load->SetMetricTargetImage(m_TarImg); 
  m_Load->SetMetricReferenceImage(m_RefImg);
  IterativeSolve();
  //if (smoothing <= m_MinSmoothing) m_RefImg=caster2->GetOutput(); // //FIXME for testing

  return;
}


void ImageRegLMEx::MultiResPyramidSolve()
{

  vnl_vector<Float> LastResolutionSolution;

//   Setup a multi-resolution pyramid
  typedef itk::MultiResolutionPyramidImageFilter<FloatImageType,FloatImageType>
    PyramidType;
  typedef PyramidType::ScheduleType ScheduleType;
  PyramidType::Pointer pyramid1 = PyramidType::New();
  PyramidType::Pointer pyramid2 = PyramidType::New();

  typedef itk::CastImageFilter<ImageType,FloatImageType> CasterType1;
  CasterType1::Pointer caster1 = CasterType1::New();
  typedef itk::CastImageFilter<FloatImageType,ImageType> CasterType2;
  CasterType2::Pointer caster2 = CasterType2::New();

  caster1->SetInput(m_RefImg); caster1->Update();
  pyramid1->SetInput( caster1->GetOutput());
  caster1->SetInput(m_TarImg); caster1->Update();
  pyramid2->SetInput( caster1->GetOutput());

// set schedule by specifying the number of levels;
  unsigned int numLevels = 1;
  itk::Vector<unsigned int,ImageDimension> factors;
  factors.Fill( 1 << (numLevels - 1) );
  pyramid1->SetNumberOfLevels( numLevels );
  pyramid2->SetNumberOfLevels( numLevels );

//  ImageType::SizeType Isz=m_RefImg->GetLargestPossibleRegion().GetSize();
  ScheduleType SizeReduction=pyramid1->GetSchedule();

  for (unsigned int i=0; i<numLevels; i++)
  {
    pyramid1->Update();
    pyramid1->GetOutput( i )->Update();
    pyramid2->Update();
    pyramid2->GetOutput( i )->Update();

    ImageType::SizeType Isz=pyramid2->GetOutput( i )->GetLargestPossibleRegion().GetSize();
//  m_MeshResolution=
    m_Nx=(double) Isz[0];

//    if ( i == numLevels-1 ) { m_E=1.e16; m_Rho=1.e16;}
    CreateMesh(0.0,(double) (Isz[0]-1),(double)m_MeshResolution,m_Solver); 
    m_Solver.GenerateGFN();

    ApplyLoads();
    
    caster2->SetInput(pyramid1->GetOutput(i)); caster2->Update();
    m_Load->SetMetricReferenceImage(caster2->GetOutput());  

    caster2->SetInput(pyramid2->GetOutput(i)); caster2->Update();
    m_Load->SetMetricTargetImage(caster2->GetOutput());  

    if (i==0) CreateLinearSystemSolver(); // for when we change # of gfns (remesh)
   
    m_Solver.AssembleKandM();

    if (i > 0)
    {
      unsigned int TotalSolutionIndex=1,SolutionTMinus1Index=3; // from SolverType
      for (unsigned int j=0; j<m_Solver.GetNGFN(); j++)
      {
        m_Solver.GetLinearSystemWrapper()->
          SetSolutionValue(j,LastResolutionSolution[j],TotalSolutionIndex);
   //     m_Solver.GetLinearSystemWrapper()->
   //       SetVectorValue(j,LastResolutionSolution[j],SolutionTMinus1Index); 
      }
    }


    IterativeSolve();

    if (i < numLevels-1)
    {
      float s1=(float)SizeReduction[i][0];  
      float s2=(float)SizeReduction[i+1][0];  
      float Magnification=1.0;//s1/s2;
      LastResolutionSolution.clear();
      LastResolutionSolution.resize(m_Solver.GetNGFN());
      unsigned int TotalSolutionIndex=1; // from SolverType
      for (unsigned int j=0; j<m_Solver.GetNGFN(); j++)
      {
        double temp=m_Solver.GetLinearSystemWrapper()->
          GetSolutionValue(j,TotalSolutionIndex);
        LastResolutionSolution[j]=temp*Magnification;
      } 
    }
  }
  //m_RefImg=caster2->GetOutput(); // //FIXME for testing
  GetVectorField();
  WarpImage();

  //LinearSystemSolverType* temp=
  //    dynamic_cast<LinearSystemSolverType*>(m_Solver.GetLinearSystemWrapper());
  //delete temp;

  return;
}


}} // end namespace itk::fem


*/
