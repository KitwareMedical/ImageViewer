/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IViewUtils.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// Utility file - definition of loadImage
// Templated over the Pixel Type


#include<MetaImageLib.h>
#include<itkImage.h>
#include<itkProcessObject.h>
#include<itkSimpleImageRegionIterator.h>


template <class T>
typename itk::Image<T, 3>::Pointer 
loadImage(char *fname)
{
 MetaImage *imIO = new MetaImage();
 imIO->OpenMetaFile(fname);
 imIO->PrintMetaInfo();

 typedef itk::Image<T,3>  ImageType;

 typedef typename ImageType::Pointer     ImagePointer;
 typedef typename ImageType::SizeType    SizeType;
 typedef typename ImageType::RegionType  RegionType;

 ImagePointer image = ImageType::New();

 SizeType size;

 double spacing[3];

 size[0] = imIO->DimSize(0);
 size[1] = imIO->DimSize(1);
 size[2] = imIO->DimSize(2);

 spacing[0] = imIO->ElemSpacing(0);
 spacing[1] = imIO->ElemSpacing(1);
 spacing[2] = imIO->ElemSpacing(2);

 if (spacing[0] == 0)
   {
   spacing[0] = 1;
   }
 if (spacing[1] == 0)
   {
   spacing[1] = 1;
   }
 if (spacing[2] == 0)
   {
   spacing[2] = 1;
   }

 RegionType region;
 region.SetSize(size);
 region.SetIndex( itk::Index<3>::ZeroIndex );
 image->SetLargestPossibleRegion(region);
 image->SetBufferedRegion(region);
 image->SetRequestedRegion(region);
 image->SetSpacing(spacing);
 image->Allocate();


 itk::SimpleImageRegionIterator< ImageType > it(image, region);
 int i;
 for(i = 0; !it.IsAtEnd(); i++, ++it)
   it.Set(imIO->Get(i));


 return image;
}

