//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#ifndef liVERTEBRALSAMPLEGENERATORCLASS
#define liVERTEBRALSAMPLEGENERATORCLASS

#include "liVertebralSegment.h"
#include "liSampleGeneratorCollection.h"
#include "liSampleGeneratorPedicle.h"
#include "liSampleGeneratorBodyPlane.h"


namespace li {


/**
 *  This class generate samples for the vertebral segment
 */
class VertebralSampleGenerator 
{

public:
  /** 
   * Container Type for Sets of Samples
   */
  typedef liSampleGeneratorCollection::SetsOfSamplesType 
                                        SetOfSamplesContainerType;

  /** 
   * Container Type for Samples
   */
  typedef liSampleGeneratorCollection::SamplesType 
                                        SamplesContainerType;


  /** 
   * Point Set Type
   */
  typedef liSampleGeneratorBase::PointSetType   PointSetType;


  /**
   *  Destructor
   */
	~VertebralSampleGenerator();


  /**
   *  Show the Sampler Generator GUI
   */
  void Show(void);


  /**
   *  Generate Samples
   */
  void GenerateSamples(void);


  /**
   *  Update the parameters of the Sample Generator
   *  using geometric data from the vertebral model
   */
  void UpdateParameters(void);

  /**
   *  Return the Samples
   */
  const SetOfSamplesContainerType & GetSamples( void ) const;

  /**
   *  Sets the Vertebral Model
   */
  void SetVertebralSegment( VertebralSegment * segment );

  /**
   *  Constructor
   */
	VertebralSampleGenerator();


  /**
   *  Hide the interface
   */
  void Hide(void);
  

  /**
   *  Transfer the sample to a Point Set
   */ 
  void TransferSamplesToPointSet(void);


  /**
   *  Get the set of samples in a PointSet structure
   */ 
  PointSetType::ConstPointer GetPointSet( void ) const;


private:

  /**
   *  Model of the Vertebral Segment
   */
  VertebralSegment::Pointer        m_VertebralSegment;

  /**
   *  Sample Generators 
   */
  liSampleGeneratorCollection           m_SampleGenerator;
  liSampleGeneratorPedicle      *       m_LeftPedicleGenerator;
  liSampleGeneratorPedicle      *       m_RightPedicleGenerator;
  liSampleGeneratorBodyPlane    *       m_TopBodyGenerator;
  liSampleGeneratorBodyPlane    *       m_BottomBodyGenerator;

  /** 
   * Hierarchical set of samples
   */
  SetOfSamplesContainerType             m_Samples;


  /** 
   * Set of samples in a PointSet structure
   */
  PointSetType::Pointer                 m_PointSet;




  };


} // end namespace li

#endif




