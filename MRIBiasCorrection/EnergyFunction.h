/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    EnergyFunction.h
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
#ifndef __ENERGYFUNCTION_H_
#define __ENERGYFUNCTION_H_

#include <vector>
#include <vnl/vnl_vector.h>

#include <itkImage.h>
#include <itkSimpleImageRegionIterator.h>

#include "EnergyTable.h"
#include "BiasField.h"

/** \class EnergyFunction
 * \brief a cost function for optimization 
 *
 * The EnergyTable object will do almost all the calculation jobs. This object
 * simply wraps the data (input image and mask image) and the EnergyTable 
 * object
 *
 * All softwares and methods for the bias correction in this directory
 * was initially developed and implemented 
 * by Martin Styner, Univ. of North Carolina at Chapel Hill, and his
 * colleagues.
 *
 * For more details. refer to the following articles.
 * "Parametric estimate of intensity inhomogeneities applied to MRI" 
 * Martin Styner, G. Gerig, Christian Brechbuehler, Gabor Szekely,  
 * IEEE TRANSACTIONS ON MEDICAL IMAGING; 19(3), pp. 153-165, 2000, 
 * (http://www.cs.unc.edu/~styner/docs/tmi00.pdf)
 *
 * "Evaluation of 2D/3D bias correction with 1+1ES-optimization" 
 * Martin Styner, Prof. Dr. G. Gerig (IKT, BIWI, ETH Zuerich), TR-197
 * (http://www.cs.unc.edu/~styner/docs/StynerTR97.pdf)
 */

template<class TImage, class TImageMask>
class EnergyFunction
{
public:
  /**
   * not used, but expected by SingleValuedNonLinearOptimizer class
   */
  typedef double DerivativeType;

  /**
   * cost value type
   */
  typedef double MeasureType ;

  /**
   * parameters type for optimizier
   */
  typedef BiasField::ParametersType ParametersType ;

  /**
   * not used, but expected by SingleValuedNonLinearOptimizer class
   */
  enum { SpaceDimension = 3 };

  /**
   * Image related type definitions
   */
  typedef TImage ImageType ;
  typedef TImageMask MaskType ;
  typedef typename ImageType::Pointer ImagePointer ;
  typedef typename MaskType::Pointer MaskPointer ;
  typedef typename ImageType::PixelType ImageElementType ;
  typedef typename MaskType::PixelType MaskElementType ;
  typedef typename ImageType::IndexType ImageIndexType ;


  /**
   * constructor:
   * this creates EnergyTable object.
   */
  EnergyFunction(std::vector<double> classMeans, 
                 std::vector<double> classSigmas) ;

  virtual ~EnergyFunction() ;

  /**
   * sets input image
   */
  void SetImage(ImagePointer image) ;
  
  /**
   * sets input mask image
   */
  void SetMask(MaskPointer mask) ;

  /**
   * sets slice number.
   * if slice number is greater than -1, GetValue function sums the
   * energy values only using the pixels and bias values in the slice 
   * which is specifed by this function.
   */
  void SetSliceNo(int sliceNo) ;

  /**
   * sets BiasField object
   */
  void SetBiasField(BiasField* biasField) ;

  /**
   * gets an energy value for the intensity difference between a pixel
   * and its corresponding bias
   */
  double GetEnergy0(double diff) { return m_EnergyTable->GetEnergy0(diff) ; } 

  /**
   * gets the total energy value of an image or a slice.
   */
  MeasureType GetValue(ParametersType parameters, 
                       MeasureType& ret) ;
protected:

private:
  BiasField* m_BiasField ;
  ImagePointer m_Image ;
  MaskPointer m_Mask ;
  int m_SliceNo ;
  EnergyTable* m_EnergyTable ;
} ; // end of class

#include "EnergyFunction.txx"

#endif
