/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVispackSource.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
