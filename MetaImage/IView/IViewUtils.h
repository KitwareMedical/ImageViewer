// Utility file - definition of loadImage
// Templated over the Pixel Type


#include<MetaImageLib.h>
#include<itkImage.h>
#include<itkProcessObject.h>
#include<itkSimpleImageRegionIterator.h>


template <class T>
Image<T, 3>::Pointer loadImage(char *fname)
{
 MetaImage *imIO = new MetaImage();
 imIO->OpenMetaFile(fname);
 imIO->PrintMetaInfo();

 itk::Image<T,3>::Pointer image = itk::Image<T,3>::New();

 itk::Image<T,3>::SizeType size;

 double spacing[3];

 size[0] = imIO->DimSize(0);
 size[1] = imIO->DimSize(1);
 size[2] = imIO->DimSize(2);

 spacing[0] = imIO->ElemSpacing(0);
 spacing[1] = imIO->ElemSpacing(1);
 spacing[2] = imIO->ElemSpacing(2);

 if (spacing[0] == 0)
    spacing[0] = 1;
 if (spacing[1] == 0)
    spacing[1] = 1;
 if (spacing[2] == 0)
    spacing[2] = 1;

 Image<T,3>::RegionType region;
 region.SetSize(size);
 region.SetIndex( itk::Index<3>::ZeroIndex );
 image->SetLargestPossibleRegion(region);
 image->SetBufferedRegion(region);
 image->SetRequestedRegion(region);
 image->SetSpacing(spacing);
 image->Allocate();


 itk::SimpleImageRegionIterator< itk::Image<T,3> > it(image, region);
 int i;
 for(i = 0; !it.IsAtEnd(); i++, ++it)
   it.Set(imIO->Get(i));


 return image;
}

