/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMImageRegLMEx.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for detail.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif
#include <fstream>
#include "FEMImageRegLMEx.h"

namespace itk {
namespace fem {



/*
 * Generate a rectangular mesh of quadrilateral elements
 */
void Generate2DRectilinearMesh(itk::fem::Element::ConstPointer e0, Solver& S, vnl_vector<double>& orig, vnl_vector<double>& size, vnl_vector<double>& Nel)
{

  // Check for correct number of dimensions
  if(orig.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions ||
     size.size() != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions ||
     Nel.size()  != Element2DC0LinearQuadrilateral::NumberOfSpatialDimensions)
  {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
  }

  // Clear existing elements, loads and nodes in Solver
  S.load.clear();
  S.el.clear();
  S.node.clear();

  // Number of nodes in each dimension
  Nel[0]=floor(Nel[0]);
  Nel[1]=floor(Nel[1]);
  unsigned int Ni=static_cast<unsigned int>(Nel[0]);
  unsigned int Nj=static_cast<unsigned int>(Nel[1]);

  // Create nodes
  Node::Pointer n;
  int gn=0; // number of node
  for(unsigned int j=0; j<=Nj; j++)
  {
    for(unsigned int i=0; i<=Ni; i++)  
    {
      n=new Node(orig[0]+i*size[0]/Nel[0], orig[1]+j*size[1]/Nel[1]);
      n->GN=gn;
      gn++;
      S.node.push_back(FEMP<Node>(n));
    }
  }

  // Create elements  
  gn=0; // global number of the element
  Element2DC0LinearQuadrilateral::Pointer e;
  for(unsigned int j=0; j<Nj; j++)
  {
    for(unsigned int i=0; i<Ni; i++)
    {
      e=dynamic_cast<Element2DC0LinearQuadrilateral*>(e0->Clone());
      e->SetNode(0,S.node.Find( i+  (Ni+1)*j     ));
      e->SetNode(1,S.node.Find( i+1+(Ni+1)*j     ));
      e->SetNode(2,S.node.Find( i+1+(Ni+1)*(j+1) ));
      e->SetNode(3,S.node.Find( i+  (Ni+1)*(j+1) ));
      e->GN=gn;
      gn++;
      S.el.push_back(FEMP<Element>(e));
    }
  }

}


/*
 * Generate a rectangular mesh of hexahedron elements
 */
void Generate3DRectilinearMesh
(itk::fem::Element::ConstPointer e0, Solver& S, vnl_vector<double>& orig, 
 vnl_vector<double>& size, vnl_vector<double>& Nel)
{

  // Check for correct number of dimensions
  if(orig.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions ||
     size.size() != Element3DC0LinearHexahedron::NumberOfSpatialDimensions ||
     Nel.size()  != Element3DC0LinearHexahedron::NumberOfSpatialDimensions)
  {
    throw FEMException(__FILE__, __LINE__, "GenerateMesh<Element2DC0LinearQuadrilateral>::Rectangular");
  }

  // Number of nodes in each dimension
  Nel[0]=floor(Nel[0]);
  Nel[1]=floor(Nel[1]);
  Nel[2]=floor(Nel[2]);
  unsigned int Ni=static_cast<unsigned int>(Nel[0]);
  unsigned int Nj=static_cast<unsigned int>(Nel[1]);
  unsigned int Nk=static_cast<unsigned int>(Nel[2]);

  // Create nodes
  Node::Pointer n;
  int gn=0; // number of node
  for(unsigned int k=0; k<=Nk; k++)
  {
    for(unsigned int j=0; j<=Nj; j++)
    {
      for(unsigned int i=0; i<=Ni; i++)
      {
        n=new Node(orig[0]+i*size[0]/Nel[0],
                   orig[1]+j*size[1]/Nel[1],
                   orig[2]+k*size[2]/Nel[2]);
        n->GN=gn;
        gn++;
        S.node.push_back(FEMP<Node>(n));
      }
    }
  }

  // Create elements  
  gn=0; // global number of the element
  itk::fem::Element3DC0LinearHexahedron::Pointer e;
  for(unsigned int k=0; k<Nk; k++)
  {
    for(unsigned int j=0; j<Nj; j++)
    {
      for(unsigned int i=0; i<Ni; i++)
      {
        e=dynamic_cast<Element3DC0LinearHexahedron*>(e0->Clone());
        e->SetNode(0,S.node.Find( i+  (Ni+1)*(j  +(Nj+1)*k) ));
        e->SetNode(1,S.node.Find( i+1+(Ni+1)*(j  +(Nj+1)*k) ));
        e->SetNode(2,S.node.Find( i+1+(Ni+1)*(j+1+(Nj+1)*k) ));
        e->SetNode(3,S.node.Find( i+  (Ni+1)*(j+1+(Nj+1)*k) ));
        e->SetNode(4,S.node.Find( i+  (Ni+1)*(j  +(Nj+1)*(k+1)) ));
        e->SetNode(5,S.node.Find( i+1+(Ni+1)*(j  +(Nj+1)*(k+1)) ));
        e->SetNode(6,S.node.Find( i+1+(Ni+1)*(j+1+(Nj+1)*(k+1)) ));
        e->SetNode(7,S.node.Find( i+  (Ni+1)*(j+1+(Nj+1)*(k+1)) ));

        e->GN=gn;
        gn++;
        S.el.push_back(FEMP<Element>(e));
      }
    }
  }

}

template<class TReference,class TTarget>
ImageRegLMEx<TReference,TTarget>::~ImageRegLMEx( )
{
 
}

template<class TReference,class TTarget>
ImageRegLMEx<TReference,TTarget>::ImageRegLMEx( )
{
  m_FileCount=0; 
  m_MeshResolution=16*2;  // determines the 'resolution' of the grid
 
  m_MinE=vnl_huge_val(m_MinE);  

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

  m_NumLevels=1;
  m_MaxLevel=1;
  m_MeshStep=2;
  m_MeshLevels=1;
  m_DoMultiRes=false;
  m_UseLandmarks=false;

  m_ReferenceFileName = NULL;
  m_TargetFileName = NULL;
  m_LandmarkFileName = NULL;
  m_Field=NULL;
  m_TotalIterations=0;

  for (unsigned int i=0; i < ImageDimension; i++)
  {
     m_ImageScaling[i]=1;
     m_ImageSize[i]=0;
     m_ImageOrigin[i]=0;
  }
  
 

}


template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::RunRegistration()
{
    std::cout << "beginning \n";  
    m_Solver.SetDeltatT(m_dT);  
    m_Solver.SetRho(m_Rho);    

    ReadImages();
 
   
  // Solve the system in time 

  if (!m_DoMultiRes) 
  {
    
    CreateMesh(m_ImageOrigin,m_ImageSize,(double)m_MeshResolution,m_Solver); 
    m_Solver.GenerateGFN(); 
    ApplyLoads(m_Solver,m_MeshResolution);


    LinearSystemWrapperItpack itpackWrapper; 
    itpackWrapper.SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
    itpackWrapper.SetMaximumNumberIterations(m_Solver.GetNGFN()); 
    itpackWrapper.SetTolerance(1.e-6);
    itpackWrapper.JacobianSemiIterative(); 
    m_Solver.SetLinearSystemWrapper(&itpackWrapper); 

    m_Load=ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType::New();

    m_Load->SetReferenceImage(m_RefImg);
    m_Load->SetTargetImage(m_TarImg);
    m_Load->SetMetric(m_Metric);
    m_Load->InitializeMetric();
    typename ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
    m_Load->SetMetricRadius(r);
    m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
    m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
    m_Load->SetSign((Float)m_DescentDirection);
    m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
    m_Load=dynamic_cast<ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType*> (&*m_Solver.load.Find(m_Solver.load.size()));  
  
 
    m_Solver.AssembleKandM();
 
    IterativeSolve(m_Solver);

    GetVectorField(m_Solver);
    WarpImage(); 
  }
  else 
  {
    MultiResSolve();
  }
    

  std::cout<<"\n E " << m_E << " dt " << m_dT << " rho " << m_Rho << "\n";
  
}

template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::ReadImages()
{
  // DEFINE INPUT FILES
 
  // Read a Raw File
  typedef  itk::ImageFileReader< ImageType >      FileSourceType;
  typedef typename ImageType::PixelType PixType;
  typedef  itk::RawImageIO< PixType,
                            ImageType::ImageDimension>   RawReaderType;

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
  
  VectorType disp;
  for (unsigned int i=0; i < ImageDimension; i++) 
  {
    disp[i]=0.0;
    m_ImageOrigin[i]=0;
    m_FieldSize[i] = m_ImageSize[i];
  }

  m_FieldRegion.SetSize( m_FieldSize );
  m_Field = FieldType::New();
  m_Field->SetLargestPossibleRegion( m_FieldRegion );
  m_Field->SetBufferedRegion( m_FieldRegion );
  m_Field->Allocate(); 
 
  m_Wregion.SetSize( m_FieldSize );
  m_WarpedImage = ImageType::New();
  m_WarpedImage->SetLargestPossibleRegion( m_Wregion );
  m_WarpedImage->SetBufferedRegion( m_Wregion );
  m_WarpedImage->Allocate();

  FieldIterator m_FieldIter( m_Field, m_FieldRegion );
  m_FieldIter.GoToBegin();
  for( ; !m_FieldIter.IsAtEnd(); ++m_FieldIter )
  {
    m_FieldIter.Set(disp);
  }

}


template<class TReference,class TTarget>
bool ImageRegLMEx<TReference,TTarget>::ReadConfigFile(const char* fname, SolverType& mySolver)
  // Reads the parameters necessary to configure the example & returns
  // false if no configuration file is found
{
  std::ifstream f;
  char buffer[80] = {'\0'};
  float fbuf = 0.0;
  unsigned int ibuf = 0;
  char* sbuf;

  std::cout << "Reading config file..." << fname << std::endl;
  f.open(fname);
  if (f) {
    
    FEMLightObject::SkipWhiteSpace(f);
    f >> fbuf;
    mySolver.SetAlpha(fbuf);
    
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
    unsigned int dim = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_ImageSize[0] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    this->m_ImageSize[1] = ibuf;

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (dim == 3) this->m_ImageSize[2] = ibuf;

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
    FEMLightObject::SkipWhiteSpace(f);
    f >> buffer;

    if (ibuf == 1) {
      this->SetWriteDisplacements(true);
      sbuf = new char[256];
      strcpy(sbuf, buffer);
      this->SetDisplacementsFile(sbuf);
    }
    else { this->SetWriteDisplacements(false); }

    FEMLightObject::SkipWhiteSpace(f);
    f >> ibuf;
    if (ibuf == 1) {
      this->DoMultiRes(true);

      FEMLightObject::SkipWhiteSpace(f);
      f >> ibuf;
      if (ibuf == 1) { this->DoSearchForMinAtEachResolution(true); }
      else { this->DoSearchForMinAtEachResolution(true); }

      FEMLightObject::SkipWhiteSpace(f);
      f >> ibuf;
      this->m_NumLevels = (unsigned int) ibuf;

      FEMLightObject::SkipWhiteSpace(f);
      f >> ibuf;
      this->m_MaxLevel = ibuf;
    
      FEMLightObject::SkipWhiteSpace(f);
      f >> ibuf;
      this->m_MeshLevels = ibuf;
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


template<class TReference,class TTarget>
int ImageRegLMEx<TReference,TTarget>::WriteDisplacementField(unsigned int index)
  // Outputs the displacement field for the index provided (0=x,1=y,2=z)
{
  // Initialize the caster to the displacement field
  IndexSelectCasterType::Pointer fieldCaster = IndexSelectCasterType::New();
  fieldCaster->SetInput( m_Field );
  fieldCaster->SetIndex( index );
  
  // Define the output of the caster
  FloatImageType::Pointer fieldImage = FloatImageType::New();
  fieldCaster->Update();
  fieldImage = fieldCaster->GetOutput();

  // Set up the output filename
  char* outfile = new char[strlen(m_DisplacementsFileName+10)];
  sprintf(outfile, "%s%c.raw", m_DisplacementsFileName, 'x'+index);
  std::cout << "Writing displacements to " << outfile;

  // Write the single-index field to a file
  //   itk::ImageRegionIteratorWithIndex<FloatImageType> it( fieldImage, fieldImage->GetLargestPossibleRegion() );
  //   for (; !it.IsAtEnd(); ++it) { std::cout << it.Get() << "\t"; }

  itk::RawImageIO<Float,ImageDimension>::Pointer io = itk::RawImageIO<Float,ImageDimension>::New();
  itk::ImageFileWriter<FloatImageType>::Pointer writer = itk::ImageFileWriter<FloatImageType>::New();
  writer->SetInput(fieldImage);
  writer->SetImageIO(io);
  writer->SetFileName(outfile);
  writer->Write();

  std::cout << "...done" << std::endl;
  return 0;
}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::WarpImage()
{
 // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;

   // -------------------------------------------------------
  std::cout << "Warping image" << std::endl;
  bool InverseWarp=true;


  if (InverseWarp)
  {
    bool InImage=true;
    FieldIterator m_FieldIter( m_Field, m_FieldRegion );
    m_FieldIter.GoToBegin();
    typename ImageType::IndexType rindex = m_FieldIter.GetIndex();
    typename ImageType::IndexType tindex = m_FieldIter.GetIndex();

    m_FieldIter.GoToBegin();  
    for( ; !m_FieldIter.IsAtEnd(); ++m_FieldIter )
    {
      rindex=m_FieldIter.GetIndex();
      tindex=m_FieldIter.GetIndex();
      VectorType disp=m_FieldIter.Get();

      for (unsigned int ii=0; ii < ImageDimension; ii++)
      { 
        tindex[ii]+=(long int)(disp[ii]+0.5);
        if ( tindex[ii] >= 0 && (unsigned int) tindex[ii] < (unsigned int) m_FieldSize[ii])  InImage=true;
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
  typename ImageType::PixelType padValue = 1;
  warper->SetEdgePaddingValue( padValue );
  warper->Update();

  m_WarpedImage=warper->GetOutput();  
  }

}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::CreateMesh(ImageSizeType MeshOrigin, ImageSizeType MeshSize, 
                              double ElementsPerSide, Solver& mySolver)
{

  vnl_vector<double> MeshOriginV; MeshOriginV.resize(ImageDimension); 
  vnl_vector<double> MeshSizeV;   MeshSizeV.resize(ImageDimension); 
  vnl_vector<double> ElementsPerDimension;  ElementsPerDimension.resize(ImageDimension); 
  for (unsigned int i=0; i<ImageDimension; i++)
  { 
    MeshSizeV[i]=(double)MeshSize[i]-1; // FIX ME  make more general
    MeshOriginV[i]=(double)MeshOrigin[i];// FIX ME make more general
    ElementsPerDimension[i]=(double)ElementsPerSide;
  }

// FIXME when mesh input is available
  if (dynamic_cast<Element2DC0LinearQuadrilateral*> (e1)&& ImageDimension == 2) 
  Generate2DRectilinearMesh(e1,mySolver,MeshOriginV,MeshSizeV,ElementsPerDimension); 
  else if (dynamic_cast<Element3DC0LinearHexahedron*> (e1) && ImageDimension == 3) 
    Generate3DRectilinearMesh(e1,mySolver,MeshOriginV,MeshSizeV,ElementsPerDimension); 
  else {  throw FEMException(__FILE__, __LINE__, "CreateMesh - wrong image or element type ");}
  delete e1;

}


template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::ApplyLoads(SolverType& mySolver,unsigned int Resolution)
{
 //
  // Apply the boundary conditions.  We pin the image corners.
  // First compute which elements these will be.
  ///

  LoadBC::Pointer l1;

  // Pin image corners - we find the corners by using Origin and Size of the images
  unsigned int CornerCounter=0;
  Node::ArrayType* nodes = &(mySolver.node);
  Element::VectorType coord;  
  for(  Node::ArrayType::iterator node=nodes->begin(); node!=nodes->end(); node++) 
  {
    coord=(*node)->GetCoordinates();
    CornerCounter=0;
    for (unsigned int ii=0; ii < ImageDimension; ii++)
  { 
    if (coord[ii] == m_ImageOrigin[ii] || coord[ii] == m_ImageSize[ii]-1 ) CornerCounter++;
  }
  if (CornerCounter == ImageDimension) // the node is located at a true corner
  {
      unsigned int tgn=(*((*node)->m_elements.begin()))->GN;
    unsigned int ndofpernode=(*((*node)->m_elements.begin()))->GetNumberOfDegreesOfFreedomPerNode();
    unsigned int numnodesperelt=(*((*node)->m_elements.begin()))->GetNumberOfNodes();
    unsigned int whichnode=0;
    for (unsigned int nn=0; nn<numnodesperelt; nn++)
    {
      if ((*node) == (*((*node)->m_elements.begin()))->GetNode(nn)) whichnode=nn;
    }
    // now, cycle through all elements in the node's element set and fix each dimension
    for (unsigned int jj=0; jj<ndofpernode; jj++)
    {
        l1=LoadBC::New();
      // now we get the element from the node -- we assume we need fix the dof only once
      // even if more than one element shares it.
        l1->m_element=( &*mySolver.el.Find(tgn)); 
    unsigned int localdof=whichnode*ndofpernode+jj;
        l1->m_dof=localdof; // FIXME should be correct for each element
        l1->m_value=vnl_vector<double>(1,0.0);
        mySolver.load.push_back( FEMP<Load>(&*l1) );
    }
  }
  }
  

  if (m_UseLandmarks)
  {
  // Landmark loads
  std::ifstream f;
  std::cout << m_LandmarkFileName << std::endl;
  std::cout << "Loading landmarks...";
  f.open(m_LandmarkFileName);
  if (f) {

    try { 
      mySolver.Read(f); 
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
  
  m_Load=ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType::New();

  m_Load->SetReferenceImage(m_RefImg);
  m_Load->SetTargetImage(m_TarImg);
  
  m_Load->SetMetric(m_Metric);
  m_Load->InitializeMetric();
  typename ImageType::SizeType r;
  for (unsigned int i=0; i < ImageDimension; i++) r[i]=m_MetricWidth;
  m_Load->SetMetricRadius(r);
  m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
  m_Load->GN=mySolver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
  m_Load->SetSign((Float)m_DescentDirection);
  mySolver.load.push_back( FEMP<Load>(&*m_Load) );    
  m_Load=dynamic_cast<ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType*> (&*mySolver.load.Find(mySolver.load.size()));  
  
  return;
}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::IterativeSolve(SolverType& mySolver)
{

  unsigned int minct=0,NumMins=2;
  if (!m_DoMultiRes) NumMins=1;
  if (m_SearchForMinAtEachLevel) m_MinE=9.e9;
  m_MinE=10.e9;
  Float LastE=9.e9 , deltE=1.e9, ETol=1.e-5;
  // iterative solve  
  //for (unsigned int iters=0; iters<m_Maxiters; iters++){
  unsigned int iters=0;
  while ( (iters < m_Maxiters && m_MinE >= LastE && deltE > ETol )  || minct < NumMins){
  /*
     * Assemble the master force vector (from the applied loads)
     */
     mySolver.AssembleFforTimeStep();

    /*
     * Solve the system of equations for displacements (u=K^-1*F)
     */
    mySolver.Solve();  
    mySolver.GoldenSection();
    Float cure=0.0,mint=0.0,tstep=m_LineSearchStep;
  

   if (m_DoLineSearchOnImageEnergy) 
   {
     for (Float t=tstep; t <= 1.0; t=t+tstep) 
     {
        cure=m_Load->EvaluateMetricGivenSolution(&(mySolver.el), t);
        if (cure <= m_MinE)
        {
          m_MinE=cure;
          mint=t;       
        }
     }
     mySolver.AddToDisplacements(mint); 
   }
   else 
   {
     LastE=m_Solver.EvaluateResidual(1.0);
     deltE=fabs(LastE-m_MinE);
     if (LastE <= m_MinE  || minct < NumMins )
     {
        mint=iters;   
        mySolver.AddToDisplacements();  
        if (LastE > m_MinE) minct++; 
        m_MinE=LastE;
     } //else iters=m_Maxiters;
   }
   
   std::cout << " min E " << m_MinE << " Cur E " << LastE << "  t " << mint << " iter " << iters << std::endl;
   iters++;
   // uncomment to write out every deformation SLOW due to interpolating vector field everywhere.
   //GetVectorField();
   //WarpImage();
   //WriteWarpedImage(m_ResultsFileName);
   
   m_TotalIterations++;
  } 
  
}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::GetVectorField(SolverType& mySolver)
{

  Element::ArrayType* el = &(mySolver.el);
  vnl_vector_fixed<double,ImageDimension> Pos(0.0);  // solution at the point
  vnl_vector_fixed<double,ImageDimension> Sol(0.0);  // solution at the local point
  vnl_vector_fixed<double,ImageDimension> Gpt(0.0);  // global position given by local point
  FieldIterator m_FieldIter( m_Field, m_FieldRegion );
  m_FieldIter.GoToBegin();
  typename ImageType::IndexType rindex = m_FieldIter.GetIndex();

  for(  Element::ArrayType::iterator elt=el->begin(); elt!=el->end(); elt++) 
  {
  
    unsigned int sfsz= (*elt)->GetNumberOfNodes();
    vnl_vector<double> shapeF( sfsz );

// this code works only for 2 and 3 dimensions !!
   if (ImageDimension == 2) 
   {
    for (double r=-1.0; r <= 1.; r=r+ 1./(1.0* (double)m_ImageSize[0] / (double)m_MeshResolution) )
    for (double s=-1.0; s <= 1.; s=s+ 1./(1.0* (double)m_ImageSize[1] / (double)m_MeshResolution) )
    {
      Pos[0]=r; 
      Pos[1]=s;
      VectorType disp; 
 
      Gpt=(*elt)->GetGlobalFromLocalCoordinates(Pos);
      Sol=(*elt)->InterpolateSolution(Pos,*(mySolver.GetLS()),mySolver.TotalSolutionIndex); // for total solution index
      for (unsigned int ii=0; ii < ImageDimension; ii++)
      { 
        Float x=Gpt[ii];
        long int temp;
        if (r == -1.0 || s==-1.0) temp=(long int) ((x)*(Float)m_ImageScaling[ii]+0.5);// BUG FIX ME
        else temp=(long int) ((x+0.5)*(Float)m_ImageScaling[ii]);
        rindex[ii]=temp;
        disp[ii] =(Float) 1.0*Sol[ii]*((Float)m_ImageScaling[ii]);
      }
     
      m_Field->SetPixel(rindex, disp );
    }
    } 
    else if (ImageDimension==3){
    for (double r=-1.0; r <= 1.; r=r+ 1./(1.0* (double)m_ImageSize[0] / (double)m_MeshResolution) )
    for (double s=-1.0; s <= 1.; s=s+ 1./(1.0* (double)m_ImageSize[1] / (double)m_MeshResolution) )
    for (double t=-1.0; t <= 1.; t=t+ 1./(1.0* (double)m_ImageSize[2] / (double)m_MeshResolution) )
    {
      Pos[0]=r; 
      Pos[1]=s;
      Pos[2]=t; 
      VectorType disp; 
 
      Gpt=(*elt)->GetGlobalFromLocalCoordinates(Pos);
      Sol=(*elt)->InterpolateSolution(Pos,*(mySolver.GetLS()),mySolver.TotalSolutionIndex); // for total solution index
      for (unsigned int ii=0; ii < ImageDimension; ii++)
      { 
        Float x=Gpt[ii];
        long int temp;
        if (r == -1.0 || s==-1.0) temp=(long int) ((x)*(Float)m_ImageScaling[ii]+0.5);// BUG FIX ME
        else temp=(long int) ((x+0.5)*(Float)m_ImageScaling[ii]);
        rindex[ii]=temp;
        disp[ii] =(Float) 1.0*Sol[ii]*((Float)m_ImageScaling[ii]);
//        if (disp[ii] < -100.) std:: cout << " Num Error " << disp[ii] << std::endl; 
      }
     
      m_Field->SetPixel(rindex, disp );
    }}
  }
  /* Insure that the values are exact at the nodes. They won't necessarily be unless we use this code.
  Node::ArrayType* nodes = &(mySolver.node);
  Element::VectorType coord;  
  VectorType SolutionAtNode;
  for(  Node::ArrayType::iterator node=nodes->begin(); node!=nodes->end(); node++) 
  {
    coord=(*node)->GetCoordinates();
    for (unsigned int ii=0; ii < ImageDimension; ii++)
    { 
      rindex[ii]=(long int) (coord[ii]*(Float)m_ImageScaling[ii]+0.5);
      Float OldSol=mySolver.GetLinearSystemWrapper()->
        GetSolutionValue((*node)->GetDegreeOfFreedom(ii),mySolver.TotalSolutionIndex);
      SolutionAtNode[ii]=OldSol*((Float)m_ImageScaling[ii]);    
    }
    m_Field->SetPixel(rindex, SolutionAtNode );
  }*/
}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::WriteWarpedImage(const char* fname)
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



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::SampleVectorFieldAtNodes(SolverType& mySolver)
{

  // Here, we need to iterate through the nodes, get the nodal coordinates,
  // sample the VF at the node and place the values in the SolutionVector.

  Node::ArrayType* nodes = &(mySolver.node);
  Element::VectorType coord;  
  VectorType SolutionAtNode;

  FieldIterator m_FieldIter( m_Field, m_FieldRegion );
  m_FieldIter.GoToBegin();
  typename ImageType::IndexType rindex = m_FieldIter.GetIndex();

  for(  Node::ArrayType::iterator node=nodes->begin(); node!=nodes->end(); node++) 
  {
    coord=(*node)->GetCoordinates();
    for (unsigned int ii=0; ii < ImageDimension; ii++)
    { 
      rindex[ii]=(long int) (coord[ii]*(Float)m_ImageScaling[ii]+0.5);
    }
    SolutionAtNode=m_Field->GetPixel(rindex);
    // Now put it into the solution!
    for (unsigned int ii=0; ii < ImageDimension; ii++)
    { 
      Float Sol=SolutionAtNode[ii]/((Float)m_ImageScaling[ii]); // Scale back to current scale
      mySolver.GetLinearSystemWrapper()->
        SetSolutionValue((*node)->GetDegreeOfFreedom(ii),Sol,mySolver.TotalSolutionIndex);    
    }
  }

}



template<class TReference,class TTarget>
void ImageRegLMEx<TReference,TTarget>::MultiResSolve()
{

  Float LastScaleEnergy=0.0, ThisScaleEnergy=0.0;
  vnl_vector<Float> LastResolutionSolution;
//   Setup a multi-resolution pyramid
  typedef itk::RecursiveMultiResolutionPyramidImageFilter<FloatImageType,FloatImageType>
    PyramidType;
  typedef PyramidType::ScheduleType ScheduleType;
  PyramidType::Pointer pyramidR = PyramidType::New();
  PyramidType::Pointer pyramidT = PyramidType::New();

  typedef itk::CastImageFilter<ImageType,FloatImageType> CasterType1;
  typedef itk::CastImageFilter<FloatImageType,ImageType> CasterType2;
  CasterType1::Pointer Rcaster1 = CasterType1::New();
  CasterType1::Pointer Tcaster1 = CasterType1::New();
  CasterType2::Pointer Rcaster2 = CasterType2::New();
  CasterType2::Pointer Tcaster2 = CasterType2::New();

  Rcaster1->SetInput(m_RefImg); Rcaster1->Update();
  pyramidR->SetInput( Rcaster1->GetOutput());
  Tcaster1->SetInput(m_TarImg); Tcaster1->Update();
  pyramidT->SetInput( Tcaster1->GetOutput());

// set schedule by specifying the number of levels;
  unsigned int numLevels = (unsigned int) m_NumLevels;
  pyramidR->SetNumberOfLevels( numLevels );
  pyramidT->SetNumberOfLevels( numLevels );

//  ImageType::SizeType Isz=m_RefImg->GetLargestPossibleRegion().GetSize();
  ScheduleType SizeReductionR=pyramidR->GetSchedule();
  ScheduleType SizeReductionT=pyramidT->GetSchedule();
  //for (unsigned int ii=0; ii<numLevels; ii++) for (unsigned int jj=0; jj<ImageDimension; jj++) 
  //{ SizeReductionR[ii][jj]=1;SizeReductionT[ii][jj]=1;}
  pyramidR->SetSchedule(SizeReductionR); pyramidT->SetSchedule(SizeReductionT);
  std::cout << SizeReductionR << std::endl;
  pyramidR->Update();
  pyramidT->Update();

/*
  itk::RawImageIO<typename ImageDataType,typename ImageDimension>::Pointer io;
  itk::ImageFileWriter<ImageType>::Pointer writer;
  io = itk::RawImageIO<typename ImageDataType,typename ImageDimension>::New();
  writer = itk::ImageFileWriter<ImageType>::New();
  writer->SetImageIO(io);
  writer->SetFileName("E:\\Avants\\MetaImages\\junk64x64.raw");
*/
  for (unsigned int i=0; i<m_MaxLevel; i++)
  {
    pyramidR->GetOutput( i )->Update();
    pyramidT->GetOutput( i )->Update();

//    Tcaster2->SetInput(pyramidT->GetOutput(i)); Tcaster2->Update(); writer->SetInput(Tcaster2->GetOutput()); writer->Write();
    
    typename ImageType::SizeType Isz=pyramidT->GetOutput( i )->GetLargestPossibleRegion().GetSize();

    for (unsigned int d=0; d < ImageDimension; d++)
    {
      m_ImageScaling[d]=m_ImageSize[d]/Isz[d];
    }

    //for (unsigned int m=0; m < m_MeshLevels; m++) // mesh resolution loop
    {
      double MeshResolution=m_MeshResolution/ m_ImageScaling[0]; //pow((double)m_MeshStep,(double)m_MeshLevels-1.0-(double)m);

      Rcaster2 = CasterType2::New();// Weird - don't know why but this worked
      Tcaster2 = CasterType2::New();// and declaring the casters outside the loop did not.
 
      CreateMesh(m_ImageOrigin,Isz,MeshResolution,m_Solver); 
      m_Solver.GenerateGFN();
      ApplyLoads(m_Solver,(unsigned int)MeshResolution);
 

      LinearSystemWrapperItpack itpackWrapper; 
      itpackWrapper.SetMaximumNonZeroValuesInMatrix(25*m_Solver.GetNGFN());
      itpackWrapper.SetMaximumNumberIterations(m_Solver.GetNGFN()); 
      itpackWrapper.SetTolerance(1.e-6);
      itpackWrapper.JacobianSemiIterative(); 
      m_Solver.SetLinearSystemWrapper(&itpackWrapper); 

      m_Load=ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType::New();

      Rcaster2->SetInput(pyramidR->GetOutput(i)); Rcaster2->Update();
      m_Load->SetReferenceImage(Rcaster2->GetOutput()); 

      Tcaster2->SetInput(pyramidT->GetOutput(i)); Tcaster2->Update();
      m_Load->SetTargetImage(Tcaster2->GetOutput());  

      m_Load->SetMetric(m_Metric);
      m_Load->InitializeMetric();
      typename ImageType::SizeType r={{m_MetricWidth,m_MetricWidth}};
      m_Load->SetMetricRadius(r);
      m_Load->SetNumberOfIntegrationPoints(m_NumberOfIntegrationPoints);
      m_Load->GN=m_Solver.load.size()+1; //NOTE SETTING GN FOR FIND LATER
      m_Load->SetSign((Float)m_DescentDirection);
      m_Solver.load.push_back( FEMP<Load>(&*m_Load) );    
      m_Load=dynamic_cast<ImageRegLMEx<TReference,TTarget>::ImageMetricLoadType*> (&*m_Solver.load.Find(m_Solver.load.size()));  
   
      m_Solver.AssembleKandM();
 
      if ( i > 0) 
      {
        SampleVectorFieldAtNodes(m_Solver);
        LastScaleEnergy=ThisScaleEnergy; ThisScaleEnergy=LastScaleEnergy; // need to resolve this
        ThisScaleEnergy=m_Solver.EvaluateResidual(0.0);
      }
//    m_Solver.PrintDisplacements();

      IterativeSolve(m_Solver);

      ThisScaleEnergy=m_Solver.EvaluateResidual(0.0);
      std::cout << " E " << ThisScaleEnergy << std::endl;

      GetVectorField(m_Solver);
      if ( i == m_MaxLevel-1) 
      { 
      //  m_RefImg=Tcaster2->GetOutput(); // for testing
        WarpImage();     
      } 
      
    }// end mesh resolution loop
  }// end image resolution loop

  //LinearSystemSolverType* temp=
  //    dynamic_cast<LinearSystemSolverType*>(mySolver.GetLinearSystemWrapper());
  //delete temp;

  return;
  
}
/*


void ImageRegLMEx<TReference,TTarget>::CreateLinearSystemSolver()
{
    // experiment with these values and choices for solver
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


*/

}} // end namespace itk::fem


// Below, we have typedefs that instantiate all necessary classes.
// Here, we instantiate the image type, load type and 
// explicitly template the load implementation type.
typedef itk::Image< unsigned char, 2 >                     ImageType;
typedef itk::fem::ImageMetricLoad<ImageType,ImageType>     ImageLoadType;
template class itk::fem::ImageMetricLoadImplementation<ImageLoadType>;

// We now declare an element type and load implementation pointer for the visitor class.
typedef itk::fem::Element2DC0LinearQuadrilateralMembrane   ElementType;
typedef ElementType::LoadImplementationFunctionPointer     LoadImpFP;
typedef ElementType::LoadType                              ElementLoadType;
typedef itk::fem::VisitorDispatcher<ElementType,ElementLoadType, LoadImpFP>   
                                                           DispatcherType;
 
int main() 
{ 
  
// Register the correct load implementation with the element typed visitor dispatcher. 
  DispatcherType::RegisterVisitor((ImageLoadType*)0, 
          &(itk::fem::ImageMetricLoadImplementation<ImageLoadType>::ImplementImageMetricLoad));

  itk::fem::ImageRegLMEx<ImageType,ImageType> X; // Declare the registration class

  X.SetConfigFileName("U://itk//Insight//Examples//FEM//FEMregLMparams.txt");
  //X.SetConfigFileName("c:\\itk\\Insight\\Examples\\FEM\\FEMregLMparams.txt");
  if (!X.ReadConfigFile(X.GetConfigFileName(),*X.GetSolver())) { return -1; }
 
  // Choose the similarity metric

  typedef itk::MeanSquaresImageToImageMetric<ImageType,ImageType> MetricType0;
  typedef itk::NormalizedCorrelationImageToImageMetric<ImageType,ImageType> MetricType1;
  typedef itk::PatternIntensityImageToImageMetric<ImageType,ImageType> MetricType2;
  typedef itk::MutualInformationImageToImageMetric<ImageType,ImageType> MetricType3;
  typedef MetricType0 MetricType;
  MetricType::Pointer msqp=MetricType::New();
  msqp->SetScaleGradient(1.0); // this is the default(?)
  X.SetMetric(msqp.GetPointer());


  // Choose the material properties
  itk::fem::MaterialLinearElasticity::Pointer m;
  m=itk::fem::MaterialLinearElasticity::New();
  m->GN=0;       // Global number of the material ///
  m->E=X.GetElasticity();  // Young modulus -- used in the membrane ///
  m->A=1.0;     // Crossection area ///
  m->h=1.0;     // Crossection area ///
  m->I=1.0;    // Momemt of inertia ///
  m->nu=0.; //.0;    // poissons -- DONT CHOOSE 1.0!!///
  m->RhoC=1.0;
  
  // Create the element type 
  ElementType::Pointer e1=ElementType::New();
  e1->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>( m );
  X.SetElement(e1);

  // Register the images
  X.RunRegistration();

  // Get outputs -- in image or vector field form
  X.WriteWarpedImage(X.GetResultsFileName());

  X.SetWriteDisplacements(false);
  if (X.GetWriteDisplacements()) {
    X.WriteDisplacementField(0);
    X.WriteDisplacementField(1);
  }

  return 0;
}


