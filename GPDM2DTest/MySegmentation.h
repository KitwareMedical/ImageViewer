#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <math.h>
#include <iostream>
#include <time.h>

#include <itkImage.h>
#include <itkImageRegionIteratorWithIndex.h>

// Deformable model and its supporting classes
#include <itkDeformableMesh3DFilter.h>
#include <itkMesh.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkCovariantVector.h>
#include <itkGradientImageFilter.h>
#include <itkGradientToMagnitudeImageFilter.h>
#include <itkDerivativeImageFilter.h>
#include <itkGradientVectorFlowImageFilter.h>
#include <itkLaplacianImageFilter.h>
#include "itkImageRegionIterator.h"
#include "itkShrinkImageFilter.h"
#include "itkBinaryMask3DMeshSource.h"
#include "itkSphereMeshSource.h"
#include "itkBalloonForceFilter.h"

// Gibbs Filter and its supporting classes
#include "itkRGBGibbsPriorFilter.h"

#include "itkImageClassifierBase.h"
#include "itkImageGaussianModelEstimator.h"
#include "itkMahalanobisDistanceMembershipFunction.h"
#include "itkMinimumDecisionRule.h"

#include "itkImage.h"
#include "itkVector.h"
#include "itkMesh.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhood.h"

//FLTK classes
#include <FL/fl_file_chooser.H>
#include "fltkImageViewer.h"
#include "GLSliceView.h"

class MySegmentation
{

public:
  typedef itk::Image<unsigned short, 3> ImageType3D;
  typedef itk::Image<unsigned short, 2> ImageType2D;
  typedef itk::Image<float, 2> FloatImage;
  typedef itk::Image<float, 3> FloatImage3D;
  typedef ImageType2D::IndexType IndexType2D;
  typedef ImageType3D::IndexType IndexType3D;
  typedef GLSliceView<float,float>     ViewerType;
  typedef itk::Image<itk::Vector<unsigned short, 1>, 3> VecImageType; 
  typedef VecImageType::PixelType VecImagePixelType;
  typedef itk::Vector<double, 2>  double2Vector;
  typedef itk::Vector<int, 2>     int2Vector;
  typedef itk::CovariantVector<float, 2> myGradientType;
  typedef itk::Image<myGradientType, 2>   myGradientImageType;

  //----------------------------------------------------------------------
  //Set membership function (Using the statistics objects)
  //----------------------------------------------------------------------

//  namespace stat = itk::Statistics;

  typedef itk::Statistics::MahalanobisDistanceMembershipFunction< VecImagePixelType > 
    MembershipFunctionType ;
  typedef MembershipFunctionType::Pointer MembershipFunctionPointer ;

  typedef std::vector< MembershipFunctionPointer > 
    MembershipFunctionPointerVector;  

  //----------------------------------------------------------------------
  // Set the image model estimator (train the class models)
  //----------------------------------------------------------------------

  typedef itk::ImageGaussianModelEstimator<VecImageType,
    MembershipFunctionType, ImageType3D> 
    ImageGaussianModelEstimatorType;

  //----------------------------------------------------------------------
  //Set the decision rule 
  //----------------------------------------------------------------------  
  typedef itk::DecisionRuleBase::Pointer DecisionRuleBasePointer;

  typedef itk::MinimumDecisionRule DecisionRuleType;

  //----------------------------------------------------------------------
  // Set the classifier to be used and assigne the parameters for the 
  // supervised classifier algorithm except the input image which is 
  // grabbed from the MRF application pipeline.
  //----------------------------------------------------------------------
  //---------------------------------------------------------------------
  typedef VecImagePixelType MeasurementVectorType;

  typedef itk::ImageClassifierBase< VecImageType,
    ImageType3D > ClassifierType;

  typedef itk::ClassifierBase<VecImageType>::Pointer 
    ClassifierBasePointer;

  typedef ClassifierType::Pointer ClassifierPointer;

  //Set the Gibbs Prior labeller
  typedef itk::RGBGibbsPriorFilter<VecImageType, ImageType3D> GibbsFilterType;

  typedef itk::SphereMeshSource< itk::Mesh<float, 3> >  MeshSourceType;

  typedef itk::BalloonForceFilter< itk::Mesh<float, 3>, itk::Mesh<float, 3> > BFFilterType;

  typedef itk::LaplacianImageFilter<FloatImage, FloatImage> myLaplacianFilterType;

  typedef itk::GradientImageFilter<FloatImage, float, float>
                                              myGFilterType;

  typedef itk::GradientToMagnitudeImageFilter<myGradientImageType, FloatImage>
                                              myGToMFilterType;

  typedef itk::ImageRegionIteratorWithIndex<FloatImage>    FloatIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<myGradientImageType> myGradientIteratorType;

  typedef itk::GradientRecursiveGaussianImageFilter<
                                            FloatImage,
                                            myGradientImageType
                                            >  GRGFilterType;
            
  MySegmentation();
  ~MySegmentation();

protected:
  virtual void LoadImage (void);
  virtual int Load (const char * inputFileName);
  void LoadNextImage( unsigned int nextslice, const char * filename);
  void ReadImage(const char * inputFileName);  
  void ShowImage(void);
  void ComputeStats(void); 
  void PickSeed(){};
  void PickDeformableSeed();
  void LoadPotential();
  void GenerateData();
  void ShowResult();
  void ClearClicked();
  void SaveResult();
  void DeformableFit();

  unsigned int cols;
  unsigned int rows;
  unsigned int slices;
  unsigned int cslices;
  unsigned short lens;
  unsigned int m_Region;
  unsigned short m_ClassNum;
  unsigned int m_Gradient;
  unsigned int m_Radius;
  unsigned int m_Step;
  unsigned int offset;
  double       m_TimeStep;
  double       m_DistanceForGradient;
  double       m_DistanceToStop;
  double2Vector m_Stiffness;
  unsigned int m_Resolution;
  char datatype;
  char m_FileName[100];
  char str_slice[10];
  bool AskScene;
  bool showdm;
  unsigned char *buffer;

  IndexType2D m_Center;
  myGradientImageType::Pointer gdimg;
  VecImageType::Pointer m_DataImage;
  ImageType2D::Pointer m_SegImage;
  ImageType2D::Pointer m_PtImage;
  ImageType3D::Pointer m_ClassImage;
  ViewerType *m_Viewer;
  ViewerType *m_ViewerResult;
  unsigned int m_SampleRegion;
  FloatImage3D::Pointer m_ViewImage;
  FloatImage3D::Pointer m_ViewSegImage;
  FloatImage::Pointer   m_GradImage;
  GibbsFilterType::Pointer m_GibbsFilter;
  MeshSourceType::Pointer m_meshsource[10];
  BFFilterType::Pointer m_bffilter[10];
  myGFilterType::Pointer gfilter;
  GRGFilterType::Pointer grfilter;
  myGToMFilterType::Pointer gtomfilter;
  myLaplacianFilterType::Pointer m_LFilter;

  ImageGaussianModelEstimatorType::Pointer 
    applyEstimateModel;
  MembershipFunctionPointerVector membershipFunctions;

  DecisionRuleType::Pointer  
    myDecisionRule;

  ClassifierPointer myClassifier;

  FloatImage3D::RegionType region;
  FloatImage::RegionType region5;
  ImageType3D::RegionType region2;
  ImageType2D::RegionType region4;
  VecImageType::RegionType region3;
  VecImagePixelType myvec;
};

