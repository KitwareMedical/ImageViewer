/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ClassifierValidationInputParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ClassifierValidationInputParser_h
#define _ClassifierValidationInputParser_h

#include "itkObjectFactory.h"
#include "itkObject.h"
#include "itkFixedArray.h"
#include "itkArray.h"

#include "itkImage.h"
#include "itkImageRegionIterator.h"

namespace itk
{

/** \class ClassifierValidationInputParser
 *
 * This component parse an input parameter file for a simple 
 * atlas based segmentation application using the IBSR 
 * dataset.
 *
 * This class is activated by method Execute().
 *
 * Inputs:
 *  - path to the IBSR "20Normals_T1" directory
 *  - path to the IBSR "20Normals_T1_brain" directory
 *  - path to the IBSR "20Normals_T1_seg" directory
 *  - patient ID
 *  - starting slice number
 *  - number of patient slices
 *  - the algorithm parameter filename
 * 
 * Outputs:
 *  - pointer to the input image
 *  - pointer to the mask image
 *  - pointer to the truth image
 *
 *  - the number of histogram levels to use
 *  - the number of histogram match points to use
 *  
 *  - the number of levels
 *  - the number of iterations at each level
 *
 *  - the fixed image starting shrink factors
 *  - the moving image starting shrink factors
 *
 */
template <typename TVectorInputImage, typename TMaskImage>
class ITK_EXPORT ClassifierValidationInputParser : public Object
{
public:

  /** Standard class typedefs. */
  typedef ClassifierValidationInputParser Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClassifierValidationInputParser, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Image Type. */
  typedef TVectorInputImage VectorInputImageType;

  typedef TMaskImage MaskImageType;

  typedef TMaskImage TruthImageType;

  /** Image dimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int, TVectorInputImage::ImageDimension);

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

protected:
  ClassifierValidationInputParser();
  ~ClassifierValidationInputParser(){};

  typedef Image<VectorInputImagePixelType, itkGetStaticConstMacro(ImageDimension)> TempImageType;

  typedef typename TempImageType::PixelType TempImagePixelType;

  typedef typename TempImageType::Pointer TempImagePointer;
  typedef typename MaskImageType::Pointer MaskImagePointer;
  typedef typename MaskImageType::Pointer TruthImagePointer;

  void ReadInputImage( const char *, signed long, unsigned long, TempImagePointer& );
  void ReadMaskImage( const char *, unsigned long, MaskImagePointer& );
  void ReadTruthImage( const char *, unsigned long, TruthImagePointer& );

private:
  ClassifierValidationInputParser( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  std::string                      m_ImageDirectoryName;
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

  unsigned long                    m_NumberOfClasses;
  unsigned long                    m_NumberOfChannels;
  DoubleMatrixType                 m_ClassMeans;
  IntegerMatrixType                m_ClassNumberOfSamples;
  DoubleMatrixArrayType            m_ClassCovariances;              


};


} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "ClassifierValidationInputParser.txx"
#endif

#endif
