/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ITKRegistrationLibTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "ITKRegistrationLib.h"

#include <iostream>

int main(int argc, char ** argv)
  {
  // Create the fixed image
  int fixedDimSize[ 3 ];
  fixedDimSize[ 0 ] = 100;
  fixedDimSize[ 1 ] = 100;
  fixedDimSize[ 2 ] = 100;
  
  float fixedOrigin[ 3 ];
  fixedOrigin[ 0 ] = 0;
  fixedOrigin[ 1 ] = 0;
  fixedOrigin[ 2 ] = 0;

  float fixedSpacing[ 3 ];
  fixedSpacing[ 0 ] = 1;
  fixedSpacing[ 1 ] = 1;
  fixedSpacing[ 2 ] = 1;

  unsigned short * fixedImageData = 
        new unsigned short [fixedDimSize[0]
                            * fixedDimSize[1]
                            * fixedDimSize[2]
                            * sizeof(unsigned short)];

  int i,j,k,cnt;
  cnt = 0;
  for(k=0; k<fixedDimSize[ 2 ]; k++)
    {
    for(j=0; j<fixedDimSize[ 1 ]; j++)
      {
      for(i=0; i< fixedDimSize[ 0 ]; i++)
        {
        fixedImageData[ cnt ] = ( ( k / 10 + j / 10 ) % 2 ) * 2 
                                + ( ( i / 10 ) % 2 );
        cnt++;
        }
      }
    }

  // Create the image to be registered to the fixed image, 
  //   i.e., the moving image
  int movingDimSize[ 3 ];
  movingDimSize[ 0 ] = 100;
  movingDimSize[ 1 ] = 100;
  movingDimSize[ 2 ] = 100;
  
  float movingOrigin[ 3 ];
  movingOrigin[ 0 ] = 0;
  movingOrigin[ 1 ] = 0;
  movingOrigin[ 2 ] = 0;

  float movingSpacing[ 3 ];
  movingSpacing[ 0 ] = 1;
  movingSpacing[ 1 ] = 1;
  movingSpacing[ 2 ] = 1;

  unsigned short * movingImageData = 
      new unsigned short [movingDimSize[ 0 ]
                          * movingDimSize[ 1 ]
                          * movingDimSize[ 2 ]
                          * sizeof(unsigned short)];

  cnt = 0;
  for(k=0; k<movingDimSize[ 2 ]; k++)
    {
    for(j=0; j<movingDimSize[ 1 ]; j++)
      {
      for(i=0; i< movingDimSize[ 0 ]; i++)
        {
        movingImageData[ cnt ] = ( ( k / 10 + j / 10 ) % 2 ) * 2 
                                 + ( ( ( i + 3 ) / 10 ) % 2 );
        cnt++;
        }
      }
    }

  //
  // Allocate space for storing the results image, 
  //   i.e., the moving image after alignment with the fixed image
  //
  unsigned short * resultImageData = 
      new unsigned short[fixedDimSize[0]
                         * fixedDimSize[1]
                         * fixedDimSize[2]
                         * sizeof(unsigned short)];

  ITKRegistrationLib(fixedDimSize, fixedOrigin, fixedSpacing, 
                     fixedImageData, 
                     movingDimSize, movingOrigin, movingSpacing, 
                     movingImageData,
                     resultImageData);
  
  return true;

  }
