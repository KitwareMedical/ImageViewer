/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itk3DSimpleLevelSetsExample.cxx
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
#include "itkSparseFieldLevelSetImageFilter.h"
#include "itkRawImageIO.h"
#include "itkImageFileWriter.h"

extern "C" {
#include "stdio.h"
}

const unsigned int V_WIDTH  = 64;
const unsigned int V_HEIGHT = 64;
const unsigned int V_DEPTH  = 64;

#define T_RADIUS (V_WIDTH/10.0)
#define XY_RADIUS (V_WIDTH/3.0)

#define T_CENTER_X (0.0)
#define T_CENTER_Y (0.0)
#define T_CENTER_Z (0.0)

#define SPH_R_X (0.2f*V_WIDTH)
#define SPH_R_Y (0.2f*V_HEIGHT)
#define SPH_R_Z (0.2f*V_DEPTH)

float torus_full_signed(unsigned x, unsigned y, unsigned z)
{
  float t_dis, dis;
  t_dis = (x - (float)V_WIDTH/2.0)*(x - (float)V_WIDTH/2.0) +
    (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0);
  t_dis = vnl_math_sqrt(t_dis);
  dis = vnl_math_sqrt((t_dis - XY_RADIUS)*(t_dis - XY_RADIUS) +
    (z - (float)V_DEPTH/2.0)*(z - (float)V_DEPTH/2.0));
  return(0.0001f*(T_RADIUS - dis));
}

float sphere(unsigned x, unsigned y, unsigned z)
{
  float dis;
  dis = (x - (float)V_WIDTH/2.0)*(x - (float)V_WIDTH/2.0)
    /(SPH_R_X*SPH_R_X) + (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0)
    /(SPH_R_Y*SPH_R_Y) + (z - (float)V_DEPTH/2.0)*(z - (float)V_DEPTH/2.0)
    /(SPH_R_Z*SPH_R_Z);
  
  return(1.0f-dis);
}

float two_spheres(unsigned x, unsigned y, unsigned z)
{
  float dis;
  if (x < V_WIDTH/2)
    {
      dis = (x - (float)V_WIDTH/4.0)*(x - (float)V_WIDTH/4.0)
        /(SPH_R_X*SPH_R_X) + (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0)
        /(SPH_R_Y*SPH_R_Y) + (z - (float)V_DEPTH/2.0)*(z - (float)V_DEPTH/2.0)
        /(SPH_R_Z*SPH_R_Z);
      return(1.0f-dis);
    }
  else
    {
      dis = (x - (float)3*V_WIDTH/4.0)*(x - (float)3*V_WIDTH/4.0)
        /(SPH_R_X*SPH_R_X) + (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0)
        /(SPH_R_Y*SPH_R_Y) + (z - (float)V_DEPTH/2.0)*(z - (float)V_DEPTH/2.0)
        /(SPH_R_Z*SPH_R_Z);
      
      return(1.0f-dis);
    } 
}

float cylinder(unsigned x, unsigned y, unsigned z)
{
  float dis;
  dis = (x - (float)V_WIDTH/2.0)*(x - (float)V_WIDTH/2.0)
    /(SPH_R_X*SPH_R_X) + (y - (float)V_HEIGHT/2.0)*(y - (float)V_HEIGHT/2.0)
    /(SPH_R_Y*SPH_R_Y);
  if ( (z > V_DEPTH/4.0) && (z < 3*V_DEPTH/4.0) )
    return(1.0f - dis);
  else if (z<=V_DEPTH/4.0)
    return(z-V_DEPTH/4.0);
  else
    return((3*V_DEPTH/4.0) - z);
}

float the_torus(unsigned x, unsigned y, unsigned z)
{
  float t_dis, dis;
  t_dis = (x - T_CENTER_X)*(x - T_CENTER_X) + (y - T_CENTER_Y)*(y - T_CENTER_Y);
  t_dis = vnl_math_sqrt(t_dis);
  dis = vnl_math_sqrt((t_dis - XY_RADIUS)*(t_dis - XY_RADIUS)
                      + (z - T_CENTER_Z)*(z - T_CENTER_Z));
  return(T_RADIUS - dis);
}

float the_torus(float x, float y, float z)
{
  float t_dis, dis;
  t_dis = (x - T_CENTER_X)*(x - T_CENTER_X) + (y - T_CENTER_Y)*(y - T_CENTER_Y);
  t_dis = vnl_math_sqrt(t_dis);
  dis = vnl_math_sqrt((t_dis - XY_RADIUS)*(t_dis - XY_RADIUS)
                      + (z - T_CENTER_Z)*(z - T_CENTER_Z));
  return(T_RADIUS - dis);
}

float box(unsigned x, unsigned y, unsigned z)
{
  float X, Y,Z;
  X = vnl_math_abs(x - (float)V_WIDTH/2.0);
  Y = vnl_math_abs(y - (float)V_HEIGHT/2.0);
  Z = vnl_math_abs(z - (float)V_DEPTH/2.0);
  float dis;
  if ((X <= XY_RADIUS) || (Y <= XY_RADIUS) || (Z <= XY_RADIUS) )
    {
      dis = XY_RADIUS - vnl_math_max(X, vnl_math_max( Y, Z));
    }
  else
    {
      dis = -vnl_math_sqrt((X - XY_RADIUS)*(X - XY_RADIUS)
                           +(Y - XY_RADIUS)*(Y - XY_RADIUS)
                           + (Z - XY_RADIUS)*(Z - XY_RADIUS));
    }
  return(dis);
}

float ellipse(unsigned int x, unsigned int y, unsigned int z)
{
  float dis;
  float cx = V_WIDTH/2, cy = V_HEIGHT/2, cz=V_DEPTH/2;
  float r1 = V_WIDTH/3;  //r2 = V_HEIGHT/3, r3=V_DEPTH/4;
  
  dis=r1-sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)+(z-cz)*(z-cz));
  
  return(dis);
}

void evaluate_function(itk::Image<float, 3> *im,
                       float (*f)(unsigned int, unsigned int, unsigned int) )
  
{
  itk::Image<float, 3>::IndexType idx;
  
  for(unsigned int z = 0; z < V_DEPTH; ++z)
    {
      idx[2] = z;
      for (unsigned int y = 0; y < V_HEIGHT; ++y)
        {
          idx[1] = y;
          for (unsigned int x = 0; x < V_WIDTH; ++x)
            {
              idx[0] = x;
              im->SetPixel(idx, f(x, y, z) );
            }
        }
      
    }
}


namespace itk {

class MorphFunction : public LevelSetFunction< Image<float, 3> >
{
public:
  void SetDistanceTransform (Image<float, 3> *d)
    { m_DistanceTransform = d; }
  
  typedef MorphFunction Self;

  typedef LevelSetFunction< Image<float, 3> > Superclass;
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
      r[0] = r[1] = r[2]= 1;
      Superclass::Initialize(r);
  }
  
private:
  Image<float, 3>::Pointer m_DistanceTransform;

  virtual ScalarValueType PropagationSpeed(
                    const NeighborhoodType& neighborhood,
                         const FloatOffsetType &
                                ) const
  {
    return m_DistanceTransform->GetPixel( neighborhood.GetIndex() );
  }
  
};

class MorphFilter : public
SparseFieldLevelSetImageFilter< Image<float, 3>, Image<float, 3> >
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
  itkTypeMacro( MorphFunction, LevelSetFunction );
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
  
  itkSetMacro(Iterations, unsigned int);
  
  void SetDistanceTransform(Image<float, 3> *im)
  {
    ((MorphFunction *)(this->GetDifferenceFunction().GetPointer()))
      ->SetDistanceTransform(im);
  }
  
  void SetCurvatureWeight(float w)
  {
    (dynamic_cast<MorphFunction *>( this->GetDifferenceFunction().GetPointer() ) )->SetCurvatureWeight(w);
  }
  
protected:
  ~MorphFilter() {}
  MorphFilter()
  {
    MorphFunction::Pointer p = MorphFunction::New();
    p->SetPropagationWeight(-1.0);
    p->SetAdvectionWeight(0.0);
    p->SetCurvatureWeight(1.0);
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

} //end of itk namespace

int main(int argc, char** argv)
{
  typedef itk::Image<float,3> ImageType;
  
  try {
    int n;
    
    itk::RawImageIO<float, 3>::Pointer output_io
      = itk::RawImageIO<float, 3>::New();
    output_io->SetByteOrderToLittleEndian();
    output_io->SetFileTypeToBinary();
    output_io->SetFileDimensionality(3);
    
    itk::ImageFileWriter<ImageType>::Pointer writer
      = itk::ImageFileWriter<ImageType>::New(); 
    writer->SetImageIO(output_io);
    
    // Initialize the initial and target volumes.
    ImageType::Pointer vol_init = ImageType::New();
    ImageType::Pointer vol_target = ImageType::New();
    
    ImageType::SizeType sz;
    sz[0] = V_WIDTH;
    sz[1] = V_HEIGHT;
    sz[2] = V_DEPTH;
    
    ImageType::IndexType idx;
    idx[0] = 0;
    idx[1] = 0;
    idx[2] = 0;
    
    ImageType::RegionType reg;  
    reg.SetSize(sz);
    reg.SetIndex(idx);
    
    vol_init->SetRegions(reg);
    vol_init->Allocate();
    
    vol_target->SetRegions(reg);
    vol_target->Allocate();
    
    if (argc < 4)
      {
        std::cout << "Usage:"<<argv[0]
                  <<" number_of_iterations initial_shape target_shape"
                  << std::endl;
        exit(-1);
      }
    
    n = (int)atoi(argv[1]);
    int init_no   = (int)atoi(argv[2]);
    int target_no = (int)atoi(argv[3]);
    
    switch(init_no)
      {
      case 1:
        evaluate_function(vol_init, torus_full_signed);
        break;
      case 2:
        evaluate_function(vol_init, sphere);
        break;
      case 3:
        evaluate_function(vol_init, two_spheres);
        break;
      case 4:
        evaluate_function(vol_init, cylinder);
        break;
      case 5:
        evaluate_function(vol_init, box);
        break;
      default:
        evaluate_function(vol_init, sphere);
      }
    
    switch (target_no)
      {
      case 1:
        evaluate_function(vol_target, torus_full_signed);
        break;
      case 2:
        evaluate_function(vol_target, sphere);
        break;
      case 3:
        evaluate_function(vol_target, two_spheres);
        break;
      case 4:
        evaluate_function(vol_target, cylinder);
        break;
      case 5:
        evaluate_function(vol_target, box);
        break;
      default:
        evaluate_function(vol_target, box);
      }
    
    itk::ImageRegionIterator<ImageType> itr(vol_target,
                             vol_target->GetRequestedRegion());
    
    // Squash level sets everywhere but near the zero set.
    for (itr = itr.Begin(); ! itr.IsAtEnd(); ++itr)
      {
        itr.Value() = itr.Value() /vnl_math_sqrt((5.0f +vnl_math_sqr(itr.Value())));
        
      }
    
    writer->SetFileName("initial_image_3D.raw");
    writer->SetInput(vol_init);
    writer->Write();
    
    writer->SetFileName("distance_transform_3D.raw");
    writer->SetInput(vol_target);
    writer->Write();
    
    itk::MorphFilter::Pointer mf = itk::MorphFilter::New();
    mf->SetDistanceTransform(vol_target);
    mf->SetNumberOfIterations(n);
    mf->SetInput(vol_init);
    mf->SetNumberOfLayers(3);
    mf->SetCurvatureWeight(0.1);
    mf->Update();
    
    writer->SetFileName("final_image_3D.raw");
    writer->SetInput(mf->GetOutput());
    writer->Write();
  }  catch (itk::ExceptionObject *e)
    {
      std::cout << e << std::endl;
    }
  

  return 0;
}

