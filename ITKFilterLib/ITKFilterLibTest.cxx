/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKFilterLibTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ITKFilterLib.h"

#include <iostream>

/**
 * This test demonstrates how to use the .lib created in 
 *   Applications/ITKFilterLib.  That .lib wraps multiple itk filters
 *   so that they can be easily called from outside of itk (even
 *   from ANIS C).
 **/
int main(int argc, char ** argv)
  {

  //
  // Create a sample input image...
  //
  
  int inputDimSize[3];
  inputDimSize[0] = 5;
  inputDimSize[1] = 5;
  inputDimSize[2] = 5;
  
  float inputOrigin[3];
  inputOrigin[0] = 0;
  inputOrigin[1] = 0;
  inputOrigin[2] = 0;

  float inputSpacing[3];
  inputSpacing[0] = 1;
  inputSpacing[1] = 1;
  inputSpacing[2] = 1;

  g_ItkFilterLibDataType * inputImageData = 
      new g_ItkFilterLibDataType [ inputDimSize[0]
                                   * inputDimSize[1]
                                   * inputDimSize[2]
                                   * sizeof(g_ItkFilterLibDataType)];

  // Fill the sample input image with bogus data
  int i,j,k,cnt;
  cnt = 0;
  for(k=0; k<inputDimSize[2]; k++)
    {
    for(j=0; j<inputDimSize[1]; j++)
      {
      for(i=0; i< inputDimSize[0]; i++)
        {
        inputImageData[cnt] = cnt/100000.0;
        cnt++;
        }
      }
    }

  // put in the piece of noise and a piece of signal
  //   - noise is similar to background
  //   - signal is different from background
  int signalPos = 63;
  inputImageData[signalPos] = 1.0;
  std::cout << "Start:" << std::endl;
  std::cout << "  Signal value = " 
            << inputImageData[signalPos] << std::endl;
  float startSignalMeanDiff = inputImageData[signalPos] 
          - (inputImageData[signalPos+1] + inputImageData[signalPos-1])/2;
  std::cout << "    Signal mean difference from background = " 
            << startSignalMeanDiff << std::endl;
  int noisePos = 83;
  inputImageData[noisePos] *= (float)1.02;
  std::cout << "  Noise value = " 
            << inputImageData[noisePos] << std::endl;
  float startNoiseMeanDiff = inputImageData[noisePos] 
          - (inputImageData[noisePos+1] + inputImageData[noisePos-1])/2;
  std::cout << "    Noise mean difference from background = " 
            << startNoiseMeanDiff << std::endl;
  std::cout << std::endl;

  //
  //
  // Below here is where the real stuff happens....
  //
  //

  // Allocate space for storing the results image,
  //   i.e., the moving image after alignment with the fixed image
  g_ItkFilterLibDataType * outputImageData = 
      new g_ItkFilterLibDataType [ inputDimSize[0]
                                   * inputDimSize[1]
                                   * inputDimSize[2]
                                   * sizeof(g_ItkFilterLibDataType) ];

  // Create an instance of the filter
  itkFilterLib filter(inputDimSize, inputOrigin, inputSpacing);

  // Provide the filter with the input image info and voxels
  filter.SetInputData(inputImageData);

  // Provide the filter with allocated memory into which the results 
  //   will be stored
  filter.SetOutputData(outputImageData);

  // Set any of the necessary parameters of the filter
  filter.SetIterationsOfAnisotropicDiffusionFilter(100);

  // After this statement is executed, the memory allocated for the output 
  //   image contains the results.
  filter.ExecuteAnisotropicDiffusionFilter();

  //
  // See what the aniostropic filter did to the noise and signal points....
  //
  std::cout << "After Anisotropic Filter:" << std::endl;
  std::cout << "  Signal value = " 
            << outputImageData[signalPos] << std::endl;
  float finalSignalMeanDiff = outputImageData[signalPos] 
          - (outputImageData[signalPos+1] + outputImageData[signalPos-1])/2;
  std::cout << "    Signal mean difference = " 
            << finalSignalMeanDiff << std::endl;
  std::cout << "  Noise value = " 
            << outputImageData[noisePos] << std::endl;
  float finalNoiseMeanDiff = outputImageData[noisePos] 
          - (outputImageData[noisePos+1] + outputImageData[noisePos-1])/2;
  std::cout << "    Noise mean difference = " 
            << finalNoiseMeanDiff << std::endl;
  std::cout << std::endl;


  // Feed the output of the aniotropic filter as input to the Gaussian
  //   filter...instead of creating another image for output, swap pointers
  //   and execute.
  g_ItkFilterLibDataType * tmpPnt = outputImageData;
  outputImageData = inputImageData;
  inputImageData = tmpPnt;
  filter.SetInputData(inputImageData);
  filter.SetOutputData(outputImageData);

  filter.SetSigmaOfGaussianBlurFilter(3);
  
  filter.ExecuteGaussianBlurFilter();


  //
  // See what the Gaussian filter did to the noise and signal points....
  //
  std::cout << "After Anisotropic+Gaussian Filter:" << std::endl;
  std::cout << "  Signal value = " 
            << outputImageData[signalPos] << std::endl;
  finalSignalMeanDiff = outputImageData[signalPos] 
            - (outputImageData[signalPos+1] + outputImageData[signalPos-1])/2;
  std::cout << "    Signal mean difference = " 
            << finalSignalMeanDiff << std::endl;  
  std::cout << "  Noise value = " 
            << outputImageData[noisePos] << std::endl;
  finalNoiseMeanDiff = outputImageData[noisePos] 
            - (outputImageData[noisePos+1] + outputImageData[noisePos-1])/2;
  std::cout << "    Noise mean difference = " 
            << finalNoiseMeanDiff << std::endl;
  std::cout << std::endl;

  return true;
  }
