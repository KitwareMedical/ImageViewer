/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BiasField.h
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
#ifndef __BIASFIELD_H
#define __BIASFIELD_H

#include <vnl/vnl_vector.h>
#include "itkIndex.h"


#define  DEFAULT_DEGREE (int) 3 

/** \class BiasField
 * \brief bias field estimate which is represented by Legendre ploynomials 
 *
 * 
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

class BiasField
{
public:
  /**
   * Legendre polynomial coefficients type
   */
  typedef vnl_vector<double> CoefficientVector ;

  /**
   * same as CoefficientVector
   * this type definition will be used by EnergyFunction object
   */
  typedef CoefficientVector ParametersType ;

  /**
   * internal coefficient storage type
   */
  typedef double* CoefficientArray ;


  /**
   * constructor:
   * calls Initialize() function
   */
  BiasField(int dimension, int degree, itk::Size<3> size) ;

  ~BiasField() ;

  /**
   * constructor calls this function. So there is no need to
   * call separately. However, if you want to set parameters to
   * zeros, call this.
   * allocates memory for the internal coefficients array 
   * and caches.
   */
  void Initialize() ;

  /**
   * gets bias field dimension
   */
  int GetDimension() { return m_Dimension ; }

  /**
   * gets bias field degree (the degree of Legendre polynomials)
   */ 
  int GetDegree() { return m_Degree ; } 

  /**
   * sets each dimension's size
   */
  void SetSize(itk::Size<3> size) ;

  /**
   * gets each dimesion's size
   */
  itk::Size<3> GetSize() { return m_Size ; }

  /**
   * if the bias field is multiplicative, it returns true.
   * Otherwise, it returns false.
   */
  bool IsMultiplicative() { return m_MultiplicativeBias ; }

  /**
   * sets the type of bias field (multiplicative or additive)
   */ 
  void IsMultiplicative(bool flag) { m_MultiplicativeBias = flag ; }

  /**
   * exception object
   */
  class CoefficientVectorSizeMismatch 
  {
  public:
    CoefficientVectorSizeMismatch(int given, int required)
    {
      Required = required ;
      Given = given ;
    }
    
    int Required;
    int Given ;
  } ;

  /**
   * sets Legendre polynomials' parameters
   */
  void SetCoefficients(CoefficientVector coef) 
    throw (CoefficientVectorSizeMismatch) ;

  /**
   * gets coefficients
   */
  CoefficientVector& GetCoefficients() ;
 
  /**
   * In the case which the bias field is 2D, it returns bias value at
   * the point which is specified by the index
   */
  double GetBias(itk::Index<2> index) ;
  
  /**
   * In the case which the bias field is 3D, it returns bias value at
   * the point which is specified by the index
   */
  double GetBias(itk::Index<3> index) ;

  /**
   * gets the number of coefficients
   */
  int GetNoOfCoefficients() ;


  class SimpleForwardIterator ;
  friend SimpleForwardIterator ;

  /**
   * iterator which only supports forward iteration and Begin(), IsAtEnd()
   * , and Get() method which work just like as SimpleImageRegionIterator.
   */
  class SimpleForwardIterator
  {
  public:

    SimpleForwardIterator (BiasField* biasField) 
    {
      m_BiasField = biasField ;
      m_Dimension = m_BiasField->GetDimension() ;
      m_Size = m_BiasField->GetSize() ;
    }
    
    void Begin() 
    {
      m_Index[0] = 0 ;
      m_Index[1] = 0 ;
      m_Index[2] = 0 ;
    }
    
    bool IsAtEnd()
    {
      if (m_Index[2] == m_Size[2])
        return true ;
      else 
        return false ;
    }
    
    SimpleForwardIterator& operator++()
    {
      if (m_Index[0] < m_Size[0] - 1)
        {
          m_Index[0] += 1 ;
          return *this ;
        }
      
      m_Index[0] = 0 ;
      
      if (m_Index[1] < m_Size[1] - 1)
        {
          m_Index[1] += 1 ;
          return *this ;
        }
      
      m_Index[1] = 0 ;
      m_Index[2] += 1 ;
      return *this ;
    }
    
    double Get()
    {

      if (m_Dimension == 3)
        {
//            std::cout << "### (" << m_Index[0] << ", "
//                  << m_Index[1] << ", "
//                  << m_Index[2] << ")" << std::endl ;
          return m_BiasField->GetBias(m_Index) ;
        }
      else if (m_Dimension == 2)
        {
          m_Index2D[0] = m_Index[0] ;
          m_Index2D[1] = m_Index[1] ;
          return m_BiasField->GetBias(m_Index2D) ;
        }
    }
    
  private:
    BiasField* m_BiasField ;
    int m_Dimension ;
    itk::Size<3> m_Size ;
    itk::Index<3> m_Index ;
    itk::Index<2> m_Index2D ;
  } ; // end of class Iterator 
  
protected:
  double LegendreSum(const double x, int n, double* coef) ; 
  void CalculateXCoef(double norm_y, double* coef) ;
  void CalculateYCoef(double norm_z, double* coef) ;
  void DeleteArrays() ;

private:
  itk::Size<3> m_Size ;
  int m_Dimension ;
  int m_Degree ;
  int m_NoOfCoefficients ;
  bool m_MultiplicativeBias; 
  
  CoefficientVector m_CoefficientVector ;
  CoefficientArray m_CoefficientArray ;
  CoefficientArray m_CachedXCoef ;
  CoefficientArray m_CachedYCoef ;
  CoefficientArray m_CachedZCoef ;
  double m_NormFactor[3] ;
  long m_PrevY ;
  long m_PrevZ ;
} ; // end of class


#endif
