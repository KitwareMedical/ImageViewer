/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IntensityHistogram3D.cxx
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
#include <vector>
#include <map>

#include <itkImage.h>
#include <itkReadMetaImage.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "mydefs.h"
#include "imageutils.h"
#include "OptionList.h"

typedef std::map<int, int> Histogram ;

void print_usage()
{
  print_line("MRIBiasCorrection 1.0 (21.June.2001)");

  print_line("usage: IntensityHistogram3D --file file" ) ;
  print_line("       [--mask] file --bin-size int") ;

  print_line("");

  print_line("--file file") ;
  print_line("        image file [meta image format]" );
  print_line("--mask file") ;
  print_line("        mask image. If mask is specified, this program") ;
  print_line("        uses the pixels greater than zero only in the mask file") ;
  print_line("--bin-size") ;
  print_line("        intensity interval") ;

  print_line("") ;

  print_line("example: IntensityHistogram3D --fiel sampl.mhd") ;
  print_line("         --mask sample.mask.mhd --bin-size 100") ;
}

void addNewInstanceToHistogram(Histogram& bins,
                               double binSize,
                               ImageType::PixelType intensity)
{
  int key = (int) (intensity / binSize) ;
  if (bins.find(key) == bins.end())
    bins[key] = 1 ;
  else
    bins[key] += 1 ;
}

void printHistogram(Histogram& bins, double binSize, int sliceNo)
{
  double middle = binSize / 2.0 ;

  Histogram::iterator m_iter = bins.begin() ;
  while (m_iter != bins.end())
    {
      std::cout << sliceNo << "\t" 
                << m_iter->first * binSize + middle << "\t" 
                << m_iter->second << std::endl ;
      m_iter++ ;
    }
}


int main(int argc, char* argv[])
{

  if (argc <= 1)
    {
      print_usage() ;
      exit(0) ;
    }

  OptionList options(argc, argv) ;

  ImagePointer image = ImageType::New() ;
  MaskPointer mask = MaskType::New() ;

  std::string fileName ;
  std::string maskFileName = "";
  bool maskAvailable = false ;
  double binSize ;
  int ret ;

  try
    {
      options.GetStringOption("file", &fileName, true) ;
      options.GetStringOption("mask", &maskFileName, false) ;
      binSize = options.GetDoubleOption("bin-size", true) ;
    }
  catch(OptionList::RequiredOptionMissing e)
    {
      std::cout << "Error: The '" << e.OptionTag 
                << "' option is required but missing." 
                << std::endl ;
      exit(0) ;
    }
  
  try
    {
      loadImage(fileName, image) ;
      if (maskFileName != "")
        {
          loadMask(maskFileName, mask) ;
          maskAvailable = true ;
        }
    }
  catch (ImageIOError e)
    {
      std::cout << "Error: " << e.Operation << " file name:" 
                << e.FileName << std::endl ;
      exit(0) ;
    }



  //  double min = options.GetDoubleOption("min", true) ;
  // double max = options.GetDoubleOption("max", true) ;

  Histogram bins ;

  itk::ImageRegionIteratorWithIndex<ImageType> 
    iter(image, image->GetLargestPossibleRegion()) ;
  itk::ImageRegionIteratorWithIndex<MaskType>
    mask_iter(mask, mask->GetLargestPossibleRegion()) ;
  
  ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize() ;
  int sliceSize = size[0] * size[1] ;
  int pixelCount = 0 ;
  int sliceNo = 0 ;
  ImageType::PixelType pixelValue ;
  double middle = binSize / 2.0 ;

  std::cout << "Slice\tIntensity\tFrequency" << std::endl ;

  while (!iter.IsAtEnd())
    {
      if (pixelCount < sliceSize)
        {
          ++pixelCount ;
          pixelValue = iter.Get() ;
          if (maskAvailable)
            {
              if (mask_iter.Get() > 0)
                {
                  addNewInstanceToHistogram(bins, binSize, pixelValue) ;
                }
            }
          else
            {
              addNewInstanceToHistogram(bins, binSize, pixelValue) ;
            }
        }
      else
        {
          printHistogram(bins, binSize, sliceNo) ;
          bins.clear() ;
          sliceNo++ ;
          pixelCount = 0 ;
        }

      ++iter ;
      ++mask_iter ;
    }


  printHistogram(bins, binSize, sliceNo) ;

  return 0 ;
}
