/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BrainStripValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _BrainStripValidationApp_h
#define _BrainStripValidationApp_h

#include <string>
#include "ApplicationBase.h"
#include "ValidationInputParser.h"
#include "DemonsPreprocessor.h"
#include "DemonsRegistrator.h"
#include "AtlasLabeler.h"
#include "ValidationOutput.h"

namespace itk
{

/** \class BrainStripValidationApp
 * 
 * This class is an application that 
 * [1] reads in user specified images from the IBSR directory, 
 * [2] performs deformable registration between the two images, 
 * [3] use the output deformation field to warp an atlas label image,
 * [4] compute the similarity between the warped atlas label image and
 * ground truth, and
 * [5] write similarity index to file.
 *
 * This class is activatived by method Execute().
 *
 * Inputs:
 * - path to IBSR "20Normals_T1" directory
 * - path to IBSR "20Nomrals_T1_brain" directory
 * - the atlas patient ID
 * - the atlas patient starting slice
 * - the number of slices in the atlas patient image/segmentation
 * - the subject patient ID
 * - the subject patient starting slice
 * - the number of slices in the subject patint image/segmentation
 * - the algorithm parameter filename
 * - the output filename
 * - flag to indicate whether or not to append to output file
 *
 */
template < typename TImage,
           typename TLabelImage,
           typename TRealImage >
class BrainStripValidationApp : 
public ApplicationBase <
  ValidationInputParser<TImage,TLabelImage>,
  DemonsPreprocessor<TImage,TRealImage>,
  DemonsRegistrator<TRealImage,TRealImage>,
  AtlasLabeler<TLabelImage, ITK_TYPENAME DemonsRegistrator<TRealImage,TRealImage>::DeformationFieldType >,
  ValidationOutput<TLabelImage> >
{
public:

  /** Standard class typedefs. */
  typedef BrainStripValidationApp Self;
  typedef ApplicationBase <
    ValidationInputParser<TImage,TLabelImage>,
    DemonsPreprocessor<TImage,TRealImage>,
    DemonsRegistrator<TRealImage,TRealImage>,
    AtlasLabeler<TLabelImage, 
      ITK_TYPENAME DemonsRegistrator<TRealImage,TRealImage>::DeformationFieldType >,
    ValidationOutput<TLabelImage> >   Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BrainStripValidationApp, ApplicationBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image dimension. */
  enum{  ImageDimension = TImage::ImageDimension };

  /** Image types. */
  typedef TImage               ImageType;
  typedef TLabelImage          LabelImageType;
  typedef TRealImage           RealImageType;

  /** Set the IBSR "20Normals_T1" directory path. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set the IBSR "20Nomrals_T1_brain" directory path. */
  itkSetStringMacro( BrainSegmentationDirectoryName );

  /** Set the atlas patient ID. */
  itkSetStringMacro( AtlasPatientID );
  itkGetStringMacro( AtlasPatientID );

  /** Set the atlas starting slice number. */
  itkSetMacro( AtlasStartSliceNumber, signed long );

  /** Set the number of atlas slices. */
  itkSetMacro( AtlasNumberOfSlices, unsigned long );

  /** Set the subject patient ID. */
  itkSetStringMacro( SubjectPatientID );
  itkGetStringMacro( SubjectPatientID );

  /** Set the subject starting slice number. */
  itkSetMacro( SubjectStartSliceNumber, signed long );

  /** Set the number of subject slices. */
  itkSetMacro( SubjectNumberOfSlices, unsigned long );

  /** Set input parameter file */
  itkSetStringMacro( ParameterFileName );

  /** Set output transformation filename. */
  itkSetStringMacro( OutputFileName );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

protected:

  BrainStripValidationApp();
  virtual ~BrainStripValidationApp(){};

  /** Initialize the input parser. */
  virtual void InitializeParser();

  /*** Initialize the preprocessor */
  virtual void InitializePreprocessor();

  /*** Initialize the registrator  */
  virtual void InitializeRegistrator();

  /*** Initialize the labeler */
  virtual void InitializeLabeler();

  /*** Initialize the output generator. */
  virtual void InitializeGenerator();

private:

  std::string                   m_ImageDirectoryName;
  std::string                   m_BrainSegmentationDirectoryName;
  std::string                   m_AtlasPatientID;
  std::string                   m_SubjectPatientID;
  signed long                   m_AtlasStartSliceNumber;
  signed long                   m_SubjectStartSliceNumber;
  unsigned long                 m_AtlasNumberOfSlices;
  unsigned long                 m_SubjectNumberOfSlices;

  std::string                   m_ParameterFileName;
  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "BrainStripValidationApp.txx"
#endif

#endif

