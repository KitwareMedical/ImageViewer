//--------------------------------------------
//
//		Project Registrator
//
//		Author:  Luis Ibanez
//	
//		Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------

#ifndef liSAMPLEGENERATORBASE
#define liSAMPLEGENERATORBASE

#include <liSample.h>
#include <vector>
#include <itkPointSet.h>



class liSampleGeneratorBase {
public:
  typedef  std::vector<liSample>             SamplesType;
  typedef  liSample::PointType               PointType;
  typedef  liSample::VectorType              VectorType;
  typedef  itk::DefaultStaticMeshTraits<double,
                      3,3,double,double>     MeshDefaultType;
  typedef  itk::PointSet<double,3,MeshDefaultType>  PointSetType;

  liSampleGeneratorBase();
  virtual ~liSampleGeneratorBase();
  virtual void GenerateSamples( SamplesType & samples );
  PointSetType::Pointer  GetPointSet(void);
    
  void Test(void) {
    m_PointSet->GetPoints();
  }


protected:
  void TransferSamplesToPointSet( const SamplesType & samples );
  void NormalizeSetOfSamples( SamplesType & samples );

private:  
  PointSetType::Pointer       m_PointSet;

};



#endif
