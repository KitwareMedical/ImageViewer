/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorPedicle.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//    Project Registrator
//
//    Author:  Luis Ibanez
//  
//    Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------


#include <liSampleGeneratorPedicle.h>




//----------------------------------------------
//
//    Constructor
//
//----------------------------------------------
liSampleGeneratorPedicle::liSampleGeneratorPedicle() 
{
  const double radius        =   20.0f;
  const double width         =    4.0f;
  const double falloff       =    2.0f;
  const double threshold     = 1000.0f;
  
  const double xSize         =   80.0f;
  const double ySize         =    5.0f;
  const double zSize         =  100.0f;

  const double xPosition     =  -10.0f;
  const double yPosition     =    7.0f;
  const double zPosition     =    8.0f;

  const double xScale        =    0.12f;
  const double yScale        =    0.30f;
  const double zScale        =    0.20f;

  radiusInput->value( radius );
  widthInput->value( width );
  falloffInput->value( falloff );
  thresholdInput->value( threshold );

  xSizeCounter->value( xSize );
  ySizeCounter->value( ySize );
  zSizeCounter->value( zSize );

  xPositionInput->value( xPosition );
  yPositionInput->value( yPosition );
  zPositionInput->value( zPosition );

  xScaleInput->value( xScale );
  yScaleInput->value( yScale );
  zScaleInput->value( zScale );

}


//----------------------------------------------
//
//    Destructor
//
//----------------------------------------------
liSampleGeneratorPedicle::~liSampleGeneratorPedicle() 
{
}





//----------------------------------------------
//
//    Hide
//
//----------------------------------------------
void 
liSampleGeneratorPedicle::Hide( void ) 
{
  parametersWindow->hide();
  liSampleGenerator::Hide();
}





//----------------------------------------------
//
//    Show Parameters
//
//----------------------------------------------
void 
liSampleGeneratorPedicle::ShowParameters( void ) 
{
  parametersWindow->show();
}





//----------------------------------------------
//
//    Show Parameters
//
//----------------------------------------------
void 
liSampleGeneratorPedicle::GenerateSamples( SamplesType & samples ) 
{

  samples.clear();

  VectorType translation;
  translation[0] = xPositionInput->value();
  translation[1] = yPositionInput->value();
  translation[2] = zPositionInput->value();

  unsigned int size[3];
  size[0] =  (unsigned int)xSizeCounter->value();
  size[1] =  (unsigned int)ySizeCounter->value();
  size[2] =  (unsigned int)zSizeCounter->value();

  double spacing[3];
  spacing[0] = xSpacingInput->value();
  spacing[1] = ySpacingInput->value();
  spacing[2] = zSpacingInput->value();
  
  double scale[3];
  scale[0] = xScaleInput->value();
  scale[1] = yScaleInput->value();
  scale[2] = zScaleInput->value();
  
  const double width          =   widthInput->value();
  const double radius         =   radiusInput->value();
  const double falloff        =   falloffInput->value();
  const double threshold      =   thresholdInput->value();

  const float w2 = width/2.0;
  const float factor = 10000.0f;
  const float pi2 = atan(1.0)*2.0;


  for( unsigned int iz = 0; iz < size[2]; iz++ )
  {
    for( unsigned int iy = 0; iy < size[1]; iy++ )
    {
      for( unsigned int ix = 0; ix < size[0]; ix++ )
      {

        double x = (ix - size[0]/2.0 + ((double)rand())/RAND_MAX -0.5 ) * spacing[0];
        double y = (iy - size[1]/2.0 + ((double)rand())/RAND_MAX -0.5 ) * spacing[1];
        double z = (iz - size[2]/2.0 + ((double)rand())/RAND_MAX -0.5 ) * spacing[2];

        const double d = (sqrt(x*x + z*z) - radius) / falloff;
        const double v = 1.0 - atan(fabs(d) - w2)/pi2;
        const double value = factor * v / 2.0;

        if( value < threshold ) 
        {
          continue;
        }

        x *= scale[0];
        y *= scale[1];
        z *= scale[2];

        PointType point;
        point = x, y, z;

        liSample sample;
        sample.value = value;
        sample.point = point + translation;
        
        samples.push_back( sample );

      }
    }
  }


  NormalizeSetOfSamples( samples );
  TransferSamplesToPointSet( samples );

}




