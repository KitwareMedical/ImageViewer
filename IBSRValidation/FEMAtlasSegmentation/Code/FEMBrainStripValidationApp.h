/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FEMBrainStripValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _FEMBrainStripValidationApp_h
#define _FEMBrainStripValidationApp_h

#include "itkFEM.h"

#include "../../AtlasSegmentation/Code/ApplicationBase.h"
#include "itkResampleImageFilter.h"
#include "FEMValidationInputParser.h"
#include "FEMPreprocessor.h"
#include "FEMRegistrator.h"
#include "FEMAtlasLabeler.h"
#include "../../AtlasSegmentation/Code/ValidationOutput.h"

#include <string>

namespace itk
{

/** \class FEMBrainStripValidationApp
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
class FEMBrainStripValidationApp : 
public ApplicationBase <
  FEMValidationInputParser<TImage,TLabelImage>,
  FEMPreprocessor<TImage,TRealImage>,
  FEMRegistrator<TRealImage,TRealImage>,
  FEMAtlasLabeler<TLabelImage, ITK_TYPENAME FEMRegistrator<TRealImage,TRealImage>::DeformationFieldType >,
  ValidationOutput<TLabelImage> >
{
public:

  /** Standard class typedefs. */
  typedef FEMBrainStripValidationApp Self;
  typedef ApplicationBase <
    FEMValidationInputParser<TImage,TLabelImage>,
    FEMPreprocessor<TImage,TRealImage>,
    FEMRegistrator<TRealImage,TRealImage>,
    FEMAtlasLabeler<TLabelImage, 
      ITK_TYPENAME FEMRegistrator<TRealImage,TRealImage>::DeformationFieldType >,
    ValidationOutput<TLabelImage> >   Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FEMBrainStripValidationApp, ApplicationBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Image types. */
  typedef TImage               ImageType;
  typedef TLabelImage          LabelImageType;
  typedef TRealImage           RealImageType;

  /** Set the IBSR "20Normals_T1" directory path. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set the IBSR "20Nomrals_T1_brain" directory path. */
  itkSetStringMacro( BrainSegmentationDirectoryName );

  /** Set the number of histogram levels to use. */
  itkSetMacro( NumberOfHistogramLevels, unsigned long );

  /** Set the number of match points to use. */
  itkSetMacro( NumberOfMatchPoints, unsigned long );

  /** Get the number of histogram levels to use. */
  itkGetMacro( NumberOfHistogramLevels, unsigned long );

  /** Get the number of match points to use. */
  itkGetMacro( NumberOfMatchPoints, unsigned long );

  /** Set the atlas patient ID. */
  itkSetStringMacro( AtlasPatientID );
  itkGetStringMacro( AtlasPatientID );


  /** Set the size of the x-dimension of the image */
  itkSetMacro( ImageXSize, unsigned int );
  itkGetMacro( ImageXSize, unsigned int );

  /** Set the size of the y-dimension of the image */
  itkSetMacro( ImageYSize, unsigned int );
  itkGetMacro( ImageYSize, unsigned int );

  /** Set the atlas starting slice number. */
  itkSetMacro( AtlasStartSliceNumber, signed long );

  /** Set the number of atlas slices. */
  itkSetMacro( AtlasNumberOfSlices, unsigned long );

  itkGetMacro( EdgeFilter, int );
  itkSetMacro( EdgeFilter, int );

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

  /** Set FEM registration configuratrion filename. */
  itkSetStringMacro( FEMConfigurationFileName );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

protected:

  FEMBrainStripValidationApp();
  virtual ~FEMBrainStripValidationApp(){};

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
  std::string                   m_FEMConfigurationFileName;
  unsigned int                  m_ImageXSize;
  unsigned int                  m_ImageYSize;
  signed long                   m_AtlasStartSliceNumber;
  signed long                   m_SubjectStartSliceNumber;
  unsigned long                 m_AtlasNumberOfSlices;
  unsigned long                 m_SubjectNumberOfSlices;
  int                           m_EdgeFilter;

  unsigned long                          m_NumberOfHistogramLevels;
  unsigned long                          m_NumberOfMatchPoints;
  std::string                   m_ParameterFileName;
  std::string                   m_OutputFileName;
  bool                          m_AppendOutputFile;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "FEMBrainStripValidationApp.txx"
#endif

#endif

