/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MRFKMeansImageClassifierApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _MRFKMeansImageClassifierApp_h
#define _MRFKMeansImageClassifierApp_h

#include "itkObject.h"

#include "itkMRFImageFilter.h"
#include "itkImageKmeansModelEstimator.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkMinimumDecisionRule.h"
#include "itkImageClassifierBase.h"
#include "itkMaskImageFilter.h"

namespace itk
{
// class to support progress feeback
class ShowProgressObject
{
public:
  ShowProgressObject(LightProcessObject * o)
    {m_Process = o;}
  void ShowProgress()
    {std::cout << "Progress " << m_Process->GetProgress() << std::endl;}
  LightProcessObject::Pointer m_Process;
};

/** \class MRFKMeansImageClassifierApp
 *
 * This component pre-processes the moving and fixed image before
 * registration.
 *
 * This particular, this class first normalizes both images to have
 * intensity between zero and one.
 *
 * The moving image is then normalize to the intensity of the fixed
 * image by using histogram matching. 
 *
 * The preprocessing is activatived by method Execute().
 *
 * Inputs:
 *    - pointer to original fixed image
 *    - pointer original moving image
 *    - number of histogram levels
 *    - number of match points
 *
 * Outputs:
 *    - pointer to processed fixed image
 *    - pointer to processed moving image
 *    - the minimum value of original fixed image
 *    - the minimum value of original moving image
 *
 */
template <typename TVectorInputImage, typename TMaskImage>
class ITK_EXPORT MRFKMeansImageClassifierApp : public Object
{
public:

  /** Standard class typedefs. */
  typedef MRFKMeansImageClassifierApp Self;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MRFKMeansImageClassifierApp, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Integer Matrix type. */
  typedef vnl_matrix<unsigned int> IntegerMatrixType;

  /** Double Matrix type. */
  typedef vnl_matrix<double> DoubleMatrixType;

  /** Array of double matrices */
  typedef std::vector<DoubleMatrixType> DoubleMatrixArrayType; 

  /** Input Image Type. */
  typedef TVectorInputImage VectorInputImageType;

  /** Input image pixel type. */
  typedef typename VectorInputImageType::PixelType VectorInputPixelType;

  /** Mask Image Type. */
  typedef TMaskImage MaskImageType;

  /** Classifierd Image Type */
  typedef TMaskImage ClassifiedImageType;

  /** Classified Image pixel type */
  typedef typename ClassifiedImageType::PixelType ClassifiedImagePixelType;

  /** Image dimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int, TVectorInputImage::ImageDimension);

  /** Set the input fixed image. */
  itkSetObjectMacro( VectorInputImage, VectorInputImageType );
  
  /** Set the input moving image. */
  itkSetObjectMacro( MaskInputImage, MaskImageType );

  /** Set the number of classes to use. */
  itkSetMacro( NumberOfClasses, unsigned long );

  /** Set the number of channels to use. */
  itkSetMacro( NumberOfChannels, unsigned long );

  /** Set the class means */
  void SetClassMeans( const DoubleMatrixType & classMeans)
    {
    m_ClassMeans = classMeans;
    }

  /** Set the class number of samples */
  void SetClassNumberOfSamples( const IntegerMatrixType & classNumberOfSamples)
    {
    m_ClassNumberOfSamples = classNumberOfSamples;
    }

  /** Set the class covariances */
  void SetClassCovariances( const DoubleMatrixArrayType & classCovariances )
    {
    m_ClassCovariances = classCovariances;
    } 

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

  /** Method to execute the preprocessing. */
  virtual void Execute();

  /** Get the output fixed image. */
  itkGetObjectMacro( ClassifiedImage, ClassifiedImageType );


protected:
  MRFKMeansImageClassifierApp();
  ~MRFKMeansImageClassifierApp(){};

  /** Set the input moving image. */
  itkSetObjectMacro( ClassifiedImage, ClassifiedImageType );

private:
  MRFKMeansImageClassifierApp( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename VectorInputImageType::Pointer       m_VectorInputImage;
  typename MaskImageType::Pointer              m_MaskInputImage;
  typename ClassifiedImageType::Pointer        m_ClassifiedImage;

  unsigned long                                m_NumberOfClasses;
  unsigned long                                m_NumberOfChannels;

  DoubleMatrixType                             m_ClassMeans;
  IntegerMatrixType                            m_ClassNumberOfSamples;
  DoubleMatrixArrayType                        m_ClassCovariances;  

  unsigned int                                 m_MaximumNumberOfIterations;
  unsigned int                                 m_NeighborhoodRadius;
  double                                       m_ErrorTolerance;   
  double                                       m_SmoothingFactor; 

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "MRFKMeansImageClassifierApp.txx"
#endif

#endif
