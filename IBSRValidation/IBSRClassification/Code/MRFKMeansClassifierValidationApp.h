/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MRFKMeansClassifierValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MRFKMeansClassifierValidationApp_h
#define _MRFKMeansClassifierValidationApp_h

#include <string>
#include "ClassifierApplicationBase.h"
#include "ClassifierValidationInputParser.h"
#include "MRFKMeansImageClassifierApp.h"
#include "ClassifierValidationOutput.h"
#include "itkArray.h"

namespace itk
{

/** \class MRFKMeansClassifierValidationApp
 * 
 * This class is an application that 
 * [1] reads in user specified images from the IBSR directory, 
 * [2] performs gaussian classifier based labelling of two brain volumes 
 * [4] compute the similarity between the classified tissue types and
 * ground truth, and
 * [5] write similarity index to file.
 *
 * This class is activatived by method Execute().
 *
 * Inputs:
 * - path to IBSR "20Normals_T1" directory
 * - path to IBSR "20Nomrals_T1_brain" directory
 * - path to IBSR "20Normals_T1_seg" directory
 * - the patient ID
 * - the patient starting slice
 * - the number of slices in the patient image/segmentation
 * - the algorithm parameter filename
 * - the output filename
 * - the number of channels/bands 
 * - flag to indicate whether or not to append to output file
 *
 */

template < typename TImage,
           typename TMaskImage >
class MRFKMeansClassifierValidationApp : 
public ClassifierApplicationBase<
  ClassifierValidationInputParser <TImage,TMaskImage>,
  MRFKMeansImageClassifierApp<TImage,TMaskImage>,
  ClassifierValidationOutput<TMaskImage> >
{
public:

  /** Standard class typedefs. */
  typedef MRFKMeansClassifierValidationApp Self;

  typedef ClassifierApplicationBase <
    ClassifierValidationInputParser<TImage,TMaskImage>,
    MRFKMeansImageClassifierApp<TImage,TMaskImage>,
    ClassifierValidationOutput<TMaskImage> >   Superclass;

  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MRFKMeansClassifierValidationApp, ClassifierApplicationBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image dimension. */
  enum{  ImageDimension = TImage::ImageDimension };

  /** Image types. */
  typedef TImage               ImageType;
  typedef TMaskImage           MaskImageType;
  typedef TMaskImage           TruthImageType;
  typedef Array<unsigned int>  IntegerArrayType;

  /** Set the IBSR "20Normals_T1" directory path. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set the IBSR "20Nomrals_T1_brain" directory path. */
  itkSetStringMacro( BrainMaskDirectoryName );

  /** Set the IBSR "20Nomrals_T1_brain" directory path. */
  itkSetStringMacro( BrainSegmentationDirectoryName );

  /** Set the patient ID. */
  itkSetStringMacro( PatientID );
  itkGetStringMacro( PatientID );

  /** Set the starting slice number in the input image. */
  itkSetMacro( StartSliceNumber, signed long );
  itkGetMacro( StartSliceNumber, signed long );

  /** Set the number of slices in the input image. */
  itkSetMacro( NumberOfSlices, unsigned long );
  itkGetMacro( NumberOfSlices, unsigned long );

  /** Set the starting slice number of the segmentation file. */
  itkSetMacro( StartSegSliceNumber, signed long );
  itkGetMacro( StartSegSliceNumber, signed long );

  /** Set the number of slices in the segmentation image. */
  itkSetMacro( NumberOfSegSlices, unsigned long );
  itkGetMacro( NumberOfSegSlices, unsigned long );

  /** Set the number of channels in the input image. */
  itkSetMacro( NumberOfChannels, unsigned long );
  itkGetMacro( NumberOfChannels, unsigned long );

  /** Set the number of classes in the input image. */
  itkSetMacro( NumberOfClasses, unsigned long );
  itkGetMacro( NumberOfClasses, unsigned long );

  /** Set/Get the number of iteration of the Iterated Conditional Mode
   * (ICM) algorithm. A default value is set at 50 iterations. */
  itkSetMacro(MaximumNumberOfIterations, unsigned int);
  itkGetMacro(MaximumNumberOfIterations, unsigned int);

  /** Set/Get the error tollerance level which is used as a threshold
   * to quit the iterations */
  itkSetMacro(ErrorTolerance, double);
  itkGetMacro(ErrorTolerance, double);

  /** Set/Get the degree of smoothing desired 
   * */
  itkSetMacro(SmoothingFactor, double);
  itkGetMacro(SmoothingFactor, double);

  /** Sets the radius for the neighborhood, for the MRF filter */
  itkSetMacro( NeighborhoodRadius, unsigned long );
  itkGetMacro( NeighborhoodRadius, unsigned long );

  /** Set input parameter file */
  itkSetStringMacro( ParameterFileName );

  /** Set output transformation filename. */
  itkSetStringMacro( OutputFileName );

  /** Set/Get the tissue labels in the truth data set */
  itkSetMacro( TruthLabels, IntegerArrayType );
  itkGetConstReferenceMacro( TruthLabels, IntegerArrayType );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

protected:
  MRFKMeansClassifierValidationApp();
  virtual ~MRFKMeansClassifierValidationApp(){};

  /** Initialize the input parser. */
  virtual void InitializeParser();

  /*** Initialize the preprocessor */
  virtual void InitializeClassifier();

  /*** Initialize the output generator. */
  virtual void InitializeOutputGenerator();

private:

  std::string                   m_ImageDirectoryName;
  std::string                   m_BrainMaskDirectoryName;
  std::string                   m_BrainSegmentationDirectoryName;
  std::string                   m_PatientID;

  signed long                   m_StartSliceNumber;
  unsigned long                 m_NumberOfSlices;
  signed long                   m_StartSegSliceNumber;
  unsigned long                 m_NumberOfSegSlices;
  unsigned long                 m_NumberOfChannels;
  unsigned long                 m_NumberOfClasses;
  IntegerArrayType              m_TruthLabels;

  unsigned int                  m_MaximumNumberOfIterations;
  unsigned int                  m_NeighborhoodRadius;
  double                        m_ErrorTolerance;
  double                        m_SmoothingFactor;

  std::string                   m_ParameterFileName;
  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "MRFKMeansClassifierValidationApp.txx"
#endif

#endif

