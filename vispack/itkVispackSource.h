/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackSource.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkVispackSource_h
#define __itkVispackSource_h

#include "itkVispackImageContainer.h"
#include "itkImageSource.h"
#include "itkExceptionObject.h"

namespace itk
{

/** \class VispackImageSource
 * \brief Imports a vispack VISImage or VISVolume.
 *
 * VispackImageSource imports the reference counted data structure from a
 * VISImage or VISVolume into an itk::Image templated on the
 * VispackImageContainer structure.
 */
template <class TPixel, unsigned int VImageDimension>
class ITK_EXPORT VispackSource
  : public ImageSource<Image<TPixel,
                            VImageDimension, VispackImageContainer<TPixel> > >
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VispackSource        Self;

  /**
   *
   */
  typedef Image<TPixel, VImageDimension, VispackImageContainer<TPixel> >
  OutputImageType;
  
  /**
   * Standard "Superclass" typedef.
   */
   typedef ImageSource<OutputImageType> Superclass;


  typedef ImageRegion<VImageDimension> RegionType;
  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Vispack container type typedef support.
   */
  typedef VISImRep<TPixel> VispackType;
  
  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VispackSource,ImageSource);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 /**
   * Set the region object that defines the size and starting index
   * for the imported image. This will serve as the LargestPossibleRegion,
   * the BufferedRegion, and the RequestedRegion.
   * \sa ImageRegion
   */
  void SetRegion(const RegionType &region)
    { if (m_Region != region) {m_Region = region; this->Modified();} };
  
  /**
   * Get the region object that defines the size and starting index
   * for the imported image. This will serve as the LargestPossibleRegion,
   * the BufferedRegion, and the RequestedRegion.
   * \sa ImageRegion
   */
  const RegionType& GetRegion() const
    { return m_Region;};
  
  /** 
   * Set the spacing (size of a pixel) of the image.
   * \sa GetSpacing()
   */
  itkSetVectorMacro(Spacing, const double, VImageDimension);

  /** 
   * Get the spacing (size of a pixel) of the image.
   * \sa SetSpacing()
   */
  itkGetVectorMacro(Spacing, const double, VImageDimension);
  
  /** 
   * Set the origin of the image.
   * \sa GetOrigin()
   */
  itkSetVectorMacro(Origin, const double, VImageDimension);

  /** 
   * Get the origin of the image.
   * \sa SetOrigin()
   */
  itkGetVectorMacro(Origin, const double, VImageDimension);

  void SetInput(const VispackType &);
  
protected:
  VispackSource();
  ~VispackSource() {};
  VispackSource(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent);

  void EnlargeOutputRequestedRegion(DataObject *);
  void GenerateOutputInformation();
  void GenerateData();

private:
  RegionType m_Region;
  double m_Spacing[VImageDimension];
  double m_Origin[VImageDimension];
  

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVispackSource.txx"
#endif

#endif
