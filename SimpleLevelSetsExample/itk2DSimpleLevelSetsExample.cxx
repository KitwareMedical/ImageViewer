/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itk2DSimpleLevelSetsExample.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <fstream>
#include "itkImageRegionIterator.h"
#include "itkLevelSetFunction.h"
#include "itkRawImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSparseFieldLevelSetImageFilter.h"

extern "C" {
#include "stdio.h"
}

/**
 * This test morphs a square to a circle using the level-set surface
 * modeling framework.  Speed function input to the level-set equation
 * is a distance transform between the two shapes.
 */
const unsigned int HEIGHT = (256);
const unsigned int WIDTH  = (256);

#define RADIUS (vnl_math_min(HEIGHT, WIDTH)/4)

float circle(unsigned x, unsigned y)
{
  float dis;
  dis = (x - (float)WIDTH/2.0)*(x - (float)WIDTH/2.0)
    + (y - (float)HEIGHT/2.0)*(y - (float)HEIGHT/2.0);
  dis = RADIUS - sqrt(dis);
  return(dis);
}

float square(unsigned x, unsigned y)
{
  int rad = RADIUS;
  
  float X, Y;
  X = ::fabs(x - (float)WIDTH/2.0);
  Y = ::fabs(y - (float)HEIGHT/2.0);
  float dis;
  if (!((X > rad)&&(Y > rad)))
    {  dis = rad - vnl_math_max(X, Y); }
  else
    { dis = -sqrt((X - rad)*(X - rad) +  (Y - rad)*(Y - rad)); }
  return(dis);
}

void evaluate_function(itk::Image<float, 2> *im,
                       float (*f)(unsigned int, unsigned int) )
  
{
  itk::Image<float, 2>::IndexType idx;
  for (unsigned int x = 0; x < WIDTH; ++x)
    {
      idx[0] = x;
      for (unsigned int y = 0; y < HEIGHT; ++y)
        {
          idx[1] = y;
          im->SetPixel(idx, f(x, y) );
        }
    }
}

namespace itk {

class MorphFunction : public LevelSetFunction< Image<float, 2> >
{
public:
  void SetDistanceTransform (Image<float, 2> *d)
    { m_DistanceTransform = d; }
  
  typedef MorphFunction Self;

  typedef LevelSetFunction< Image<float, 2> > Superclass;
  typedef Superclass::RadiusType RadiusType;
  
   /** 
   * Smart pointer support for this class.
   */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro( MorphFunction, LevelSetFunction );
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

protected:
  ~MorphFunction() {}

  MorphFunction()
    {
      RadiusType r;
      r[0] = r[1] = 1;
      Superclass::Initialize(r);
    }

private:
  Image<float, 2>::Pointer m_DistanceTransform;
 
  virtual ScalarValueType PropagationSpeed(
                            const NeighborhoodType& neighborhood,
                            const FloatOffsetType
                          ) const
    {
      Index<2> idx = neighborhood.GetIndex();

      return m_DistanceTransform->GetPixel(idx);
    }

  virtual ScalarValueType PropagationSpeed(const BoundaryNeighborhoodType
                               &neighborhood, const FloatOffsetType &
                                           ) const
  {
    Index<2> idx = neighborhood.GetIndex();
    return m_DistanceTransform->GetPixel(idx);
  }
  
};


class MorphFilter : public
SparseFieldLevelSetImageFilter< Image<float, 2>, Image<float, 2> >
{
public:
  typedef MorphFilter Self;
  
  /** 
   * Smart pointer support for this class.
   */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro( MorphFilter, SparseFieldLevelSetImageFilter );
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  itkSetMacro(Iterations, unsigned int);

  void SetDistanceTransform(Image<float, 2> *im)
    {
      ((MorphFunction *)(this->GetDifferenceFunction().GetPointer()))
        ->SetDistanceTransform(im);
    }


protected:
  ~MorphFilter() {}
  MorphFilter()
    {
      MorphFunction::Pointer p = MorphFunction::New();
      p->SetPropagationWeight(-1.0);
      p->SetAdvectionWeight(0.0);
      p->SetCurvatureWeight(0.1);
      this->SetDifferenceFunction(p);

    }
  MorphFilter(const Self &) {}
  
private:
  unsigned int m_Iterations; 
  
  virtual bool Halt()
    {
      if (this->GetElapsedIterations() == m_Iterations) return true;
      else return false;
    }  
};

} // end namespace itk
int main(int argc, char *argv[])
{
  typedef itk::Image<float, 2> ImageType;
    
  try {

  itk::RawImageIO<float, 2>::Pointer output_io = itk::RawImageIO<float, 2>::New();
   output_io->SetByteOrderToLittleEndian();
   output_io->SetFileTypeToBinary();
   output_io->SetFileDimensionality(2);

  itk::ImageFileWriter<ImageType>::Pointer writer
     = itk::ImageFileWriter<ImageType>::New(); 
  writer->SetImageIO(output_io);
   
  int n;
  if (argc < 2)
    {
      std::cout << "Usage: LevelSetFunctionTest no_of_iterations\n" << std::endl;
      exit(-1);
    }
  sscanf(argv[1],"%d",&n);
  
  ImageType::Pointer im_init = ImageType::New();
  ImageType::Pointer im_target = ImageType::New();
  
  ImageType::RegionType r;
  ImageType::SizeType   sz;
  sz[0] = HEIGHT;
  sz[1] = WIDTH;
  ImageType::IndexType  idx;
  idx[0] = 0;
  idx[1] = 0;
  r.SetSize(sz);
  r.SetIndex(idx);

  im_init->SetLargestPossibleRegion(r);
  im_init->SetBufferedRegion(r);
  im_init->SetRequestedRegion(r);

  im_target->SetLargestPossibleRegion(r);
  im_target->SetBufferedRegion(r);
  im_target->SetRequestedRegion(r);

  im_init->Allocate();
  im_target->Allocate();

  evaluate_function(im_init, square);
  evaluate_function(im_target, circle);

  itk::ImageRegionIterator<ImageType> itr(im_target,
                                          im_target->GetRequestedRegion());

  // Squash level sets everywhere but near the zero set.
  for (itr = itr.Begin(); ! itr.IsAtEnd(); ++itr)
  {
    itr.Value() = itr.Value() /vnl_math_sqrt((5.0f +vnl_math_sqr(itr.Value())));
  
  }
  
  writer->SetFileName("distance_transform_2D.raw");
  writer->SetInput(im_target);
  writer->Write();
  
  writer->SetFileName("initial_image_2D.raw");
  writer->SetInput(im_init);
  writer->Write();
  
  itk::MorphFilter::Pointer mf = itk::MorphFilter::New();
  mf->SetDistanceTransform(im_target);
  mf->SetIterations(n);
  mf->SetInput(im_init);
  mf->SetNumberOfLayers(2);
  
  writer->SetFileName("final_image_2D.raw");
  writer->SetInput(mf->GetOutput());
  writer->Write();
  
  }
  catch (itk::ExceptionObject &e)
    {
      std::cout << e << std::endl;
    }
  
  return 0;
}
