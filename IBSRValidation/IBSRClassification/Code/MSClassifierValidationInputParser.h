/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MSClassifierValidationInputParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MSClassifierValidationInputParser_h
#define _MSClassifierValidationInputParser_h

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkArray.h"

#include "itkImage.h"
#include "itkImageRegionIterator.h"

namespace itk
{

/** \class MSClassifierValidationInputParser
 *
 * This component parses an input parameter file for a classifier
 * that reads a MS phantom data provided with the toolkit.
 *
 * This class is activated by method Execute(). This class reads the 
 * multiple input images and packs it into a vector image needed for the 
 * multichannel classification.
 *
 * Inputs:
 * - path to MS data directory (filename syntax: patientID + extension)
 * - extensions for the 1st channel 
 * - extension for the 2nd channel
 * - output result file
 * - the patient ID
 * - the number of slices in the patient image/segmentation
 * - the algorithm parameter filename (see syntax for the parameter file)
 * 
 * Outputs:
 *  - pointer to the classified image
 *
 */
template <typename TVectorInputImage, typename TMaskImage>
class ITK_EXPORT MSClassifierValidationInputParser : public Object
{
public:

  /** Standard class typedefs. */
  typedef MSClassifierValidationInputParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MSClassifierValidationInputParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TVectorInputImage VectorInputImageType;

  typedef TMaskImage MaskImageType;

  typedef TMaskImage TruthImageType;

  /** Image dimension enumeration. */
  enum { ImageDimension = TVectorInputImage::ImageDimension };

  /** Storage type for individual image type */
  typedef typename TVectorInputImage::PixelType VectorInputImageVectorType;
  typedef typename VectorInputImageVectorType::ValueType 
    VectorInputImagePixelType;

  /** Integer Matrix type. */
  typedef vnl_matrix<unsigned int> IntegerMatrixType;

  /** Double Matrix type. */
  typedef vnl_matrix<double> DoubleMatrixType;

  /** Array of double matrices */
  typedef std::vector<DoubleMatrixType> DoubleMatrixArrayType; 

  /** Array of strings */
  typedef typename std::string                   StringType; 
  typedef typename std::vector< StringType >     StringVectorType; 

  /** Set IBSR images directory. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set IBSR brain ground truth directory. */
  itkSetStringMacro( BrainMaskDirectoryName );

  /** Set IBSR brain ground truth directory. */
  itkSetStringMacro( BrainSegmentationDirectoryName );

  /** Set the patientID. */
  itkSetStringMacro( PatientID );

  /** Set the input image starting slice number. */
  itkSetMacro( StartSliceNumber, signed long );

  /** Set the number of atlas patient slices. */
  itkSetMacro( NumberOfSlices, unsigned long );

  /** Set the starting slice number of the segmentation file. */
  itkSetMacro( StartSegSliceNumber, signed long );

  /** Set the number of slices in the segmentation image. */
  itkSetMacro( NumberOfSegSlices, unsigned long );

  /** Set the number of channels.*/
  itkSetMacro( NumberOfChannels, unsigned long )

  /** Set input parameter file name. */
  itkSetStringMacro( ParameterFileName );

  /** Parse the input file. */
  void Execute();

  /** Get pointer to the input image. */
  itkGetObjectMacro( VectorInputImage, VectorInputImageType );

  /** Get pointer to the mask image. */
  itkGetObjectMacro( MaskImage, MaskImageType );

  /** Get pointer to the truthimage. */
  itkGetObjectMacro( TruthImage, TruthImageType );

  /** Get the number of classes bins. */
  itkGetConstMacro( NumberOfClasses, unsigned long );

  /** Set the input image starting slice number. */
  itkGetConstMacro( StartSliceNumber, signed long );

  /** Set the number of atlas patient slices. */
  itkGetConstMacro( NumberOfSlices, unsigned long );

  /** Set the starting slice number of the segmentation file. */
  itkGetConstMacro( StartSegSliceNumber, signed long );

  /** Set the number of slices in the segmentation image. */
  itkGetConstMacro( NumberOfSegSlices, unsigned long );

  /** Get the number of classes bins. */
  itkGetConstMacro( NumberOfChannels, unsigned long );

  /** Get the class means  */
  itkGetConstReferenceMacro( ClassMeans, DoubleMatrixType );

  /** Get the number of samples in each class */
  itkGetConstReferenceMacro( ClassNumberOfSamples, IntegerMatrixType );

  /** Get the covariance matrices in each class */
  const DoubleMatrixArrayType & GetClassCovariances( )
    {
    return  m_ClassCovariances;
    } 
  /** Set the fileextensions */
  void SetFileExtensions( StringVectorType stringVector )
    {
    m_FileExtensions = stringVector;
    }
  
protected:
  MSClassifierValidationInputParser();
  ~MSClassifierValidationInputParser(){};

  typedef Image<VectorInputImagePixelType, ImageDimension> TempImageType;

  typedef typename TempImageType::PixelType TempImagePixelType;

  typedef typename TempImageType::Pointer TempImagePointer;
  typedef typename MaskImageType::Pointer MaskImagePointer;
  typedef typename MaskImageType::Pointer TruthImagePointer;

  void ReadInputImage( const char *, unsigned long, unsigned long, TempImagePointer& );
  void ReadMaskImage( const char *, unsigned long, MaskImagePointer& );
  void ReadTruthImage( const char *, unsigned long, TruthImagePointer& );

private:
  MSClassifierValidationInputParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string                      m_ImageDirectoryName;
  StringVectorType                 m_FileExtensions;
  std::string                      m_BrainMaskDirectoryName;
  std::string                      m_BrainSegmentationDirectoryName;
  std::string                      m_PatientID;
  signed long                      m_StartSliceNumber;
  unsigned long                    m_NumberOfSlices;
  signed long                      m_StartSegSliceNumber;
  unsigned long                    m_NumberOfSegSlices;

  std::string                      m_ParameterFileName;

  typename VectorInputImageType::Pointer m_VectorInputImage;
  typename MaskImageType::Pointer  m_MaskImage;
  typename TruthImageType::Pointer m_TruthImage;


  TempImagePointer                 m_tempImage;
  std::vector<TempImagePointer>    m_tempImagePointerVector;

  unsigned long                    m_NumberOfClasses;
  unsigned long                    m_NumberOfChannels;
  DoubleMatrixType                 m_ClassMeans;
  IntegerMatrixType                m_ClassNumberOfSamples;
  DoubleMatrixArrayType            m_ClassCovariances;              


};


} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "MSClassifierValidationInputParser.txx"
#endif

#endif
