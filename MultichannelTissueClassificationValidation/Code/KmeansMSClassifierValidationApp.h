/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    KmeansMSClassifierValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _KmeansMSClassifierValidationApp_h
#define _KmeansMSClassifierValidationApp_h

#include <string>
#include "MSClassifierApplicationBase.h"
#include "MSClassifierValidationInputParser.h"
#include "KmeansImageMSClassifierApp.h"
#include "MSClassifierValidationOutput.h"
#include "itkArray.h"

namespace itk
{

/** \class KmeansMSClassifierValidationApp
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
class KmeansMSClassifierValidationApp : 
public MSClassifierApplicationBase<
  MSClassifierValidationInputParser <TImage,TMaskImage>,
  KmeansImageMSClassifierApp<TImage,TMaskImage>,
  MSClassifierValidationOutput<TMaskImage> >
{
public:

  /** Standard class typedefs. */
  typedef KmeansMSClassifierValidationApp Self;

  typedef MSClassifierApplicationBase <
    MSClassifierValidationInputParser<TImage,TMaskImage>,
    KmeansImageMSClassifierApp<TImage,TMaskImage>,
    MSClassifierValidationOutput<TMaskImage> >   Superclass;

  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(KmeansMSClassifierValidationApp, MSClassifierApplicationBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Image types. */
  typedef TImage                                 ImageType;
  typedef TMaskImage                             MaskImageType;
  typedef TMaskImage                             TruthImageType;
  typedef Array<unsigned int>                    IntegerArrayType;
  typedef std::string                            StringType; 
  typedef std::vector< StringType >              StringVectorType; 

  /** Set the input image directory path. */
  itkSetStringMacro( ImageDirectoryName );

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

  /** Set input parameter file */
  itkSetStringMacro( ParameterFileName );

  /** Set output filename. */
  itkSetStringMacro( OutputFileName );

  /** Set/Get the tissue labels in the truth data set */
  itkSetMacro( TruthLabels, IntegerArrayType );
  itkGetConstReferenceMacro( TruthLabels, IntegerArrayType );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

  /** Get the file extension vector */
  StringVectorType GetFileExtensions()
    {
    return m_FileExtensions;
    }

  /** Add the file extensions */
  void AddFileExtension(const char* buffer)
    {
    m_FileExtensions.push_back( buffer );
    }   

protected:
  KmeansMSClassifierValidationApp();
  virtual ~KmeansMSClassifierValidationApp(){};

  /** Initialize the input parser. */
  virtual void InitializeParser();

  /*** Initialize the preprocessor */
  virtual void InitializeMSClassifier();

  /*** Initialize the output generator. */
  virtual void InitializeOutputGenerator();

private:

  std::string                   m_ImageDirectoryName;
  std::string                   m_BrainSegmentationDirectoryName;
  std::string                   m_PatientID;
  signed long                   m_StartSliceNumber;
  unsigned long                 m_NumberOfSlices;
  signed long                   m_StartSegSliceNumber;
  unsigned long                 m_NumberOfSegSlices;
  unsigned long                 m_NumberOfChannels;
  unsigned long                 m_NumberOfClasses;
  IntegerArrayType              m_TruthLabels;

  std::string                   m_ParameterFileName;
  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

  StringVectorType              m_FileExtensions;
  std::string                   m_tempstring;


};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "KmeansMSClassifierValidationApp.txx"
#endif

#endif

