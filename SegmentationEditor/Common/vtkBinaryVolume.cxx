/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkBinaryVolume.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkBinaryVolume.h"
#include "vtkObjectFactory.h"
#include <fstream>

vtkBinaryVolume::vtkBinaryVolume()
{
  this->paint_radius = 0;
  this->SetNumberOfScalarComponents(1);
  this->SetScalarType(VTK_UNSIGNED_CHAR);
}

vtkBinaryVolume* vtkBinaryVolume::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkBinaryVolume");
  if(ret)
    {
    return (vtkBinaryVolume*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkBinaryVolume;
}

void vtkBinaryVolume::Clear()
{
  int x0, x1, y0, y1, z0, z1;
  this->GetExtent(x0, x1, y0, y1, z0, z1);
  
  unsigned long sz  = ((x1-x0)+1) * ((y1-y0)+1) * ((z1-z0)+1);
  
  unsigned char *ptr = (unsigned char *)(this->GetScalarPointer());
  unsigned char *end = ptr + sz;

  while (ptr != end)
    {
      *ptr = 0;
      ptr++; 
    }
  this->Modified();
  
}

int vtkBinaryVolume::WriteToDisk(const char *fn)
{
  // Dump the contents of the volume to disk.
  const int *e = this->GetExtent();

  unsigned volume_size_in_bytes =
    (e[1] +1  - e[0]) * (e[3] + 1 - e[2]) * (e[5] + 1 - e[4]) * sizeof(unsigned char);

  // HEADER is the 6 integer extent x0, x1, y0, y1, z0, z1
  std::ofstream out;
  out.open(fn);
  if (! out ) return -1;
  out.write((char *)e, sizeof(int) * 6);
  out.write((char *)this->GetScalarPointer(), volume_size_in_bytes);
  out.close();

  //  std::cout << "vtkBinaryVolume::WriteToDisk: header size is "
  //              << sizeof(int) * 6 << std::endl;
  //  std::cout << "vtkBinaryVolume::WriteToDisk: volume size in bytes is "
  //            << volume_size_in_bytes << std::endl;
  //  std::cout << "vtkBinaryVolume::WriteToDisk: volume dimensions are: "
  //            << (e[1] + 1 - e[0]) << "x" << (e[3] +1 - e[2]) << "x" << (e[5] +1 - e[4])
  //            << std::endl;
  return 0; // all OK  
}

int vtkBinaryVolume::ReadFromDisk(const char *fn)
{
  // Dump the contents of the file to the data container.
  // But only if they are of the same size. Otherwise complain and return.  We
  // aren't in the business here of resizing and allocating new memory.

  // ERRORS:
  // -1 Could not open file
  // -2 Bad dimensions.
  // -3 Unexpected read error.
  
  int *e = new int[6];
  const int *c = this->GetExtent();

  unsigned volume_size_in_bytes =
    (c[1] - c[0]) * (c[3] - c[2]) * (c[5] - c[4]) * sizeof(unsigned char);
  
  // HEADER is the 6 integer extent x0, x1, y0, y1, z0, z1
  std::ifstream in;
  in.open(fn);
  if (! in ) return -1;
  in.read((char *)e, sizeof(int) * 6);
  if ( in.gcount() != sizeof(int) * 6) return -3;

  // check size
  for (unsigned i = 0; i < 6; ++i) { if (e[i] != c[i]) return -2; }
  
  in.read((char *)this->GetScalarPointer(), volume_size_in_bytes);

  if (in.gcount() != volume_size_in_bytes) return -3;

  in.close();
  delete [] e;
  
  this->Modified();
  return 0; // all OK  
}

void vtkBinaryVolume::SetWithRadius(int x, int y, int z)
{
  const int *c = this->GetExtent();
  int xpos, ypos, xx, yy, x_tem, y_tem, r, r2;

  if (paint_radius == 0)    { this->Set(x,y,z); }
 
  // only set in the 2d plane

  // SQUARE
  //  for (ypos = y - paint_radius; ypos < y + paint_radius; ++ypos)
  //    for (xpos = x - paint_radius; xpos < x + paint_radius; ++xpos)
  //      {
  //        if (xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3])
  //          {        this->Set(xpos, ypos, z); }
  //      }
  
  // CIRCLE
  r2 = paint_radius * paint_radius;
  for (yy = -paint_radius + 1; yy < paint_radius; yy++)
    {
      ypos = y + yy;
      xx = (int) (::floor( ::sqrt(static_cast<double>(r2 - (yy * yy))) ) );
      for (x_tem = xx; x_tem >= -xx ; x_tem --)
        {
          xpos = x + x_tem;
          if ( xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3] )
            {        this->Set(xpos, ypos, z); }
        }
    }

  // fix the funny shape due to rounding at values of x close to 0
  for ( xx = -paint_radius + 1; xx < paint_radius; xx++)
    {
      xpos = x + xx;
      yy = (int) ( ::floor(::sqrt( static_cast<double>(r2 - (xx * xx)))) );
      for (y_tem = yy; y_tem >= -yy ; y_tem -= yy)
        {
          ypos = y + y_tem;
          if ( xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3] )
            {        this->Set(xpos, ypos, z); }
        }
    }
}


void vtkBinaryVolume::UnsetWithRadius(int x, int y, int z)
{
  const int *c = this->GetExtent();
  int xpos, ypos, xx, yy, x_tem, y_tem, r, r2;

  if (paint_radius == 0)    { this->Unset(x,y,z); }

  // SQUARE
  // only set in the 2d plane
  //  for (ypos = y - paint_radius; ypos < y + paint_radius; ++ypos)
  //    for (xpos = x - paint_radius; xpos < x + paint_radius; ++xpos)
  //      {
  //        if (xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3])
  //         {        this->Unset(xpos, ypos, z); }
  //      }
    
  // CIRCLE
  r2 = paint_radius * paint_radius;
  for (yy = -paint_radius + 1; yy < paint_radius; yy++)
    {
      ypos = y + yy;
      xx = (int) (::floor( ::sqrt(static_cast<double>(r2 - (yy * yy))) ) );
      for (x_tem = xx; x_tem >= -xx ; x_tem --)
        {
          xpos = x + x_tem;
          if ( xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3] )
            {        this->Unset(xpos, ypos, z); }
        }
    }

  // fix the funny shape due to rounding at values of x close to 0
  for ( xx = -paint_radius + 1; xx < paint_radius; xx++)
    {
      xpos = x + xx;
      yy = (int) ( ::floor(::sqrt( static_cast<double>(r2 - (xx * xx)))) );
      for (y_tem = yy; y_tem >= -yy ; y_tem -= yy)
        {
          ypos = y + y_tem;
          if ( xpos >= c[0] && xpos <= c[1] && ypos >= c[2] && ypos <= c[3] )
            {        this->Unset(xpos, ypos, z); }
        }
    }
}
