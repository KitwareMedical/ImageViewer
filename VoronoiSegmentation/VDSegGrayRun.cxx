/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VDSegGrayRun.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "VDSegGrayRun.h"
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Int_Input.H>
#include <fstream>
#include <iostream>

VDSegGrayRun::
VDSegGrayRun()
{
  m_ViewResultImage=FloatImage::New();
  m_DataImage=FloatImage::New();
  m_WorkingImage=FloatImage2D::New();

  m_Viewer=0;
  m_ViewerResult=0;

  m_UsePrior=1;
  m_OutputBnd=0;
  m_InitSeeds=200;
  m_MinRegion=20;
  m_DisplaySlice=1;

}

VDSegGrayRun::
~VDSegGrayRun()
{
}

bool 
VDSegGrayRun::LoadImage()
{
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return 0;
    }
  if(this->Load( filename )){
    ReadImage(filename);
    return 1;
  }
  return 0;
}

int
VDSegGrayRun::
Load( const char * filename )
{
  Fl_Window window(200,280);
  Fl_Int_Input inputcols(80, 10, 100, 25, "Columns:");
  Fl_Int_Input inputrows(80, 50, 100, 25, "Rows:");
  Fl_Int_Input inputslices(80, 90, 100, 25, "Slices:");
  inputcols.value("256");
  inputrows.value("256");
  inputslices.value("1");
  Fl_Int_Input inputheader(80, 170, 100, 25, "Header:");
  inputheader.value("0");
  Fl_Button cancel(40, 240, 60, 25, "cancel");
  Fl_Return_Button ok(120, 240, 60, 25, "OK");
  Fl_Menu_Item pulldown[] = {
    {"Unsigned Char"},
    {"Unsigned Short"},
    {"Float"},
    {0}
  };
  Fl_Choice ch(45,140,140,25,"Data:"); ch.menu(pulldown);

  window.hotspot(&cancel); // you must position modal windows
  window.end();
  window.set_modal();
  window.show();
  for (;;) {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue())) {
      if (o == &ok) {
        m_Cols=atoi(inputcols.value());
        m_Rows=atoi(inputrows.value());
        m_Pages=atoi(inputslices.value());
        m_Offset=atoi(inputheader.value());
        m_Datatype=ch.value();
        return 1;
      } 
      else if (o == &cancel || o == &window) {
        return 0;
      }
    }
  }
}

void
VDSegGrayRun::
ReadImage( const char * filename )
{
  FloatImage::SizeType size={{m_Cols,m_Rows,m_Pages}};
  FloatImage::IndexType index={{0,0,0}};
  m_ImageRegion.SetSize(size);
  m_ImageRegion.SetIndex(index);
  m_ViewResultImage->SetLargestPossibleRegion(m_ImageRegion);
  m_ViewResultImage->SetBufferedRegion(m_ImageRegion);
  m_ViewResultImage->SetRequestedRegion(m_ImageRegion);
  m_ViewResultImage->Allocate();
  m_DataImage->SetLargestPossibleRegion(m_ImageRegion);
  m_DataImage->SetBufferedRegion(m_ImageRegion);
  m_DataImage->SetRequestedRegion(m_ImageRegion);
  m_DataImage->Allocate();

  FloatImage2D::SizeType size2={{m_Cols,m_Rows}};
  FloatImage2D::IndexType index2={{0,0}};
  m_ImageRegion2D.SetSize(size2);
  m_ImageRegion2D.SetIndex(index2);
  m_WorkingImage->SetLargestPossibleRegion(m_ImageRegion2D);
  m_WorkingImage->SetBufferedRegion(m_ImageRegion2D);
  m_WorkingImage->SetRequestedRegion(m_ImageRegion2D);
  m_WorkingImage->Allocate();
  m_WorkingImage->SetLargestPossibleRegion(m_ImageRegion2D);
  m_WorkingImage->SetBufferedRegion(m_ImageRegion2D);
  m_WorkingImage->SetRequestedRegion(m_ImageRegion2D);
  m_WorkingImage->Allocate();

  m_ImageSize=m_Cols*m_Rows*m_Pages;
  unsigned long iter;
  std::ifstream fin(filename,std::ios::binary);
  itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_DataImage, m_ImageRegion);
  char *offread=new char[m_Offset];
  fin.read(offread,m_Offset);
  delete[] offread;

  switch (m_Datatype){
    case 0: 
      {
      unsigned char *reader1=new unsigned char[m_ImageSize];
      fin.read((char*)reader1,m_ImageSize);
      for(iter=0;iter<m_ImageSize;iter++){
        iit.Set((float)(reader1[iter]));
        ++iit;
        }
      }
      break;
    case 1:
      {
      unsigned short *reader2=new unsigned short[m_ImageSize];
      fin.read((char*)reader2,m_ImageSize+m_ImageSize);
      for(iter=0;iter<m_ImageSize;iter++){
        iit.Set((float)(reader2[iter]));
        ++iit;
        }
      }
      break;
    case 2:
      {
      float *reader3=new float[m_ImageSize];
      fin.read((char*)reader3,4*m_ImageSize);
      for(iter=0;iter<m_ImageSize;iter++){
        iit.Set(reader3[iter]);
        ++iit;
        }
      }
      break;
    default:
      break; 
  }


  m_Viewer=new ViewerType(100,100,m_Cols,m_Rows,"");
  m_Viewer->SetInputImage(m_DataImage);
  m_Viewer->show();
  m_Viewer->update();
  m_Viewer->viewDetails(false);
  m_Viewer->viewValue(true);
  m_Viewer->viewCrosshairs(false);
  Fl::check();
  m_Viewer->redraw();
  Fl::check();

  m_ViewerResult=new ViewerType(200,200,m_Cols,m_Rows,"");
}


bool 
VDSegGrayRun::LoadPrior()
{
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return 0;
    }
  strcpy(m_PriorFileName,filename);
  return 1;
}

void 
VDSegGrayRun::
ClearClicked(void)
{
  m_Viewer->clearClickedPointsStored();
}


void 
VDSegGrayRun::
ComputeStats(void)
{
  unsigned int numPoints=m_Viewer->numClickedPointsStored();
  unsigned int i,j,k,l;  
  int x,y,z,ex,bx,ey,by,ez,bz;
  unsigned int halfr=m_SampleRegion/2;
  unsigned int totalp=0;
  float totalv=0;
  float totalvv=0;
  float currp=0;
  IndexType idx;
  for(i=0;i<numPoints;i++){
    ClickPoint *point=m_Viewer->getClickedPoint(i);
    x=(int)(point->x);
    y=(int)(point->y);
    z=(int)(point->z);
    bx=x-halfr;ex=bx+m_SampleRegion;
    by=y-halfr;ey=by+m_SampleRegion;
    bz=z-halfr;ez=bz+m_SampleRegion;
    if(bx<0) bx=0;
    if(by<0) by=0;
    if(bz<0) bz=0;
    if(ex>m_Cols) ex=m_Cols;
    if(ey>m_Rows) ey=m_Rows;
    if(ez>m_Pages) ez=m_Pages;
    for(l=bz;l<ez;l++){
      for(j=by;j<ey;j++){
        for(k=bx;k<ex;k++){
          totalp++;
          idx[0]=k;idx[1]=j;idx[2]=l;
          currp= m_DataImage->GetPixel(idx);
          totalv+=currp;
          totalvv+=currp*currp;      
        }
      }
    }
  }
  m_Mean=totalv/(totalp-1);
  m_STD=(totalvv-totalv*totalv/totalp)/(totalp-1);
  m_STD=sqrt(m_STD);
}


void 
VDSegGrayRun::
GenerateData()
{
  if(m_UsePrior){
    GenerateDataPrior();
  }
  else{
    GenerateDataStandard();
  }
  ShowResult();
}

void 
VDSegGrayRun::
GenerateDataPrior()
{
  unsigned int i;
  BinImage2D::Pointer prior=BinImage2D::New();
  prior->SetLargestPossibleRegion(m_ImageRegion2D);
  prior->SetBufferedRegion(m_ImageRegion2D);
  prior->SetRequestedRegion(m_ImageRegion2D);
  prior->Allocate();

  itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_DataImage, m_ImageRegion);
  itk::ImageRegionIteratorWithIndex<FloatImage> oit(m_ViewResultImage, m_ImageRegion);
  itk::ImageRegionIteratorWithIndex<FloatImage2D> iit2(m_WorkingImage, m_ImageRegion2D);
  itk::ImageRegionIteratorWithIndex<BinImage2D> pit(prior, m_ImageRegion2D);

  std::ifstream fpp(m_PriorFileName,std::ios::binary);
  unsigned long ssize=m_Cols*m_Rows;
  unsigned char *reader=new unsigned char[ssize];
  unsigned long j;  

  for(i=0;i<m_Pages;i++){
    pit.GoToBegin();
    iit2.GoToBegin();
    VDFilter::Pointer vdseger=VDFilter::New();
    while(!iit2.IsAtEnd()){
      iit2.Set(iit.Get());
      ++iit2;++iit;
    }
    vdseger->SetMean(m_Mean);
    vdseger->SetSTD(m_STD);
    vdseger->SetMeanPercentError(m_MeanRatio);
    vdseger->SetSTDPercentError(m_STDRatio);
    
    j=0;
    fpp.read((char*)reader,ssize);
    while(!pit.IsAtEnd()){
      pit.Set((reader[j])!=0);
      ++j;++pit;
    }

    vdseger->SetInput(m_WorkingImage);
    vdseger->TakeAPrior(prior);
    vdseger->SetSteps(0);
    vdseger->SetOutputBoundary(m_OutputBnd);
    vdseger->SetNumberOfSeeds(m_InitSeeds); 
    vdseger->SetMinRegion(m_MinRegion);

    vdseger->Update();

    itk::ImageRegionIteratorWithIndex<BinImage2D> oit2(vdseger->GetOutput(), m_ImageRegion2D);
    while(!oit2.IsAtEnd()){
      oit.Set((float)(oit2.Get()));
      ++oit2;++oit;
    }
  }  
}

void 
VDSegGrayRun::
GenerateDataStandard()
{
  unsigned int i;
  itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_DataImage, m_ImageRegion);
  itk::ImageRegionIteratorWithIndex<FloatImage> oit(m_ViewResultImage, m_ImageRegion);
  itk::ImageRegionIteratorWithIndex<FloatImage2D> iit2(m_WorkingImage, m_ImageRegion2D);

  for(i=0;i<m_Pages;i++){
    iit2.GoToBegin();
    VDFilter::Pointer vdseger=VDFilter::New();
    while(!iit2.IsAtEnd()){
      iit2.Set(iit.Get());
      ++iit2;++iit;
    }
    vdseger->SetMean(m_Mean);
    vdseger->SetSTD(m_STD);
    vdseger->SetMeanPercentError(m_MeanRatio);
    vdseger->SetSTDPercentError(m_STDRatio);
    vdseger->SetSteps(0);
    vdseger->SetOutputBoundary(m_OutputBnd);
    vdseger->SetNumberOfSeeds(m_InitSeeds); 
    vdseger->SetMinRegion(m_MinRegion);

    vdseger->SetInput(m_WorkingImage);
    vdseger->Update();
    itk::ImageRegionIteratorWithIndex<BinImage2D> oit2(vdseger->GetOutput(), m_ImageRegion2D);
    while(!oit2.IsAtEnd()){
      oit.Set((float)(oit2.Get()));
      ++oit2;++oit;
    }
  }  
}

void 
VDSegGrayRun::
ShowResult()
{
  m_ViewerResult->SetInputImage(m_ViewResultImage);
  m_ViewerResult->show();
  m_ViewerResult->update();
  m_ViewerResult->viewDetails(false);
  m_ViewerResult->viewValue(false);
  m_ViewerResult->viewCrosshairs(false);
  Fl::check();
  m_ViewerResult->redraw();
  Fl::check();
  m_ViewerResult->sliceNum(m_DisplaySlice);
  m_ViewerResult->update();

}

void
VDSegGrayRun::
Save(void)
{
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return;
    }
   Save(filename);
}

void
VDSegGrayRun::
Save(const char *filename)
{
  std::ofstream fout(filename,std::ios::binary);

  itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_ViewResultImage, m_ImageRegion);
  unsigned char *writer=new unsigned char [m_ImageSize];
  unsigned long i=0;
  while (!iit.IsAtEnd()){
     writer[i]=(unsigned char)(255*(iit.Get()));
     ++iit;i++;
  }

  fout.write((char *)(writer),m_ImageSize);
}


