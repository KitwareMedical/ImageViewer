/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKFilterLib.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef ITK_FILTER_LIB
#define ITK_FILTER_LIB


/** 
 * Since we don't want to expose ITK and since we want to create a .lib, 
 *   templates have to be fully instatiated and therefore only one data type 
 *   can be supported by this class. The supported data type is specified below.   
 * Caution global variable!   If you want to support more than one data type, 
 *   you've got to create multiple instances of this file and rename this 
 *   variable and class.
 * CHANGE HERE: Change this one line to produce a .lib file that supports a 
 *   different data type.
 **/
typedef float g_ItkFilterLibDataType;


/** 
 * This class provides a demonstration of how to create a .lib that contains
 *   multiple itk filters that can be easily called from outside of itk by 
 *   including this .h and linking with its .lib
 * Since we don't want to expost ITK to ensure maximum compatibility, the 
 *   itk filters that this class actually runs are specified within its .cxx
 *   file.   The line(s) to change in that file (and in this file) are marked 
 *   with the phrase "CHANGE HERE".
 **/
class itkFilterLib
  {

  public:
  
    /** 
     *  The dimension size, origin, and spacing of the input image is assumed to match
     *    that of the output image. The images are, of course, assumed to be 3D.   All
     *    of these facts can be changed within the .cxx file.
     **/
    itkFilterLib(int * imageDimSize, float * imageOrigin, 
                 float * imageSpacing);
    
    /** 
     *  This member function must be called first.   It receives a pointer to
     *    the chunk of memory that contains the input image's data.
     **/
    void SetInputData(g_ItkFilterLibDataType * inImageData);

    /** 
     *  Call this after the SetInputData member function is called.   Pass a 
     *   pointer to allocated memory into which the results will be stored.   
     *   The size of the allocated space must equal the size of the input 
     *   image data space.
     **/
    void SetOutputData(g_ItkFilterLibDataType * outImageData);

    /** 
     *  CHANGE HERE: Specify member functions to set the parameters of the 
     *    filters.
     *  See http://www.itk.org examples, documentation, and doxygen pages for 
     *    details on the filters. These are the parameters of the anisotropic 
     *    diffusion filter.
     **/
    void SetIterationsOfAnisotropicDiffusionFilter(int iterations);
    void SetTimeStepOfAnisotropicDiffusionFilter(float timeStep);
    void SetConductanceOfAnisotropicDiffusionFilter(float conductance);
    void SetFixedAverageGradientMagnitudeOfAnisotropicDiffusionFilter(float fixedAverageGradientMagnitude);
    void ExecuteAnisotropicDiffusionFilter(void);

    /** 
     *  CHANGE HERE: Specify member functions to set the parameters of the 
     *    filters.
     *  See http://www.itk.org examples, documentation, and doxygen pages for 
     *    details on the filters. These are the parameters of the Gaussian 
     *    filter.
     **/
    void SetSigmaOfGaussianBlurFilter(float sigma);
    void ExecuteGaussianBlurFilter(void);

  private:
    
    int m_DataQuantity;

  };



#endif
