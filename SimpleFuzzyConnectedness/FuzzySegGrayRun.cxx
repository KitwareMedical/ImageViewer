#include "FuzzySegGrayRun.h"
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Int_Input.H>
#include <fstream>
#include <iostream>

FuzzySegGrayRun::
FuzzySegGrayRun()
{
  m_ViewResultImage=FloatImage::New();
  m_DataImage=FloatImage::New();
  m_OutputObject=OutputImage::New();
  m_Segmenter=FuzzyFilter::New();

  m_Viewer=0;
  m_ViewerResult=0;


  m_Threshold=0.5;
  m_SampleRegion=5;
  m_AskScene=0;

  m_DisplaySlice=1;

}

FuzzySegGrayRun::
~FuzzySegGrayRun()
{
}

bool 
FuzzySegGrayRun::LoadImage()
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
FuzzySegGrayRun::
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
FuzzySegGrayRun::
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
  m_OutputObject->SetLargestPossibleRegion(m_ImageRegion);
  m_OutputObject->SetBufferedRegion(m_ImageRegion);
  m_OutputObject->SetRequestedRegion(m_ImageRegion);
  m_OutputObject->Allocate();

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

  m_Segmenter->SetInput(m_DataImage);

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

void 
FuzzySegGrayRun::
ClearClicked(void)
{
  m_Viewer->clearClickedPointsStored();
}


void 
FuzzySegGrayRun::
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
  m_Variance=(totalvv-totalv*totalv/totalp)/(totalp-1);
}


void 
FuzzySegGrayRun::
GenerateData()
{
  m_Segmenter->SetMean(m_Mean);
  m_Segmenter->SetVariance(m_Variance);
  m_Segmenter->SetThreshold(m_Threshold);
  unsigned int numPoints=m_Viewer->numClickedPointsStored();
  IndexType idx;

  ClickPoint *point=m_Viewer->getClickedPoint(0);
  idx[0]=(int)(point->x);  
  idx[1]=(int)(point->y);  
  idx[2]=(int)(point->z);  
  m_Segmenter->SetObjectsSeed(idx);
  m_Segmenter->Update(); 
  ShowResult();
}

void 
FuzzySegGrayRun::
ShowResult()
{
  if(m_AskScene){
    itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_ViewResultImage, m_ImageRegion);
    itk::ImageRegionIteratorWithIndex<FuzzySceneImage> iit2(m_Segmenter->GetFuzzyScene(), m_ImageRegion);
    while (!iit.IsAtEnd()){
       iit.Set(iit2.Get());
       ++iit;++iit2;  
    }  
  }
  else{
    itk::ImageRegionIteratorWithIndex<FloatImage> iit(m_ViewResultImage, m_ImageRegion);
    itk::ImageRegionIteratorWithIndex<OutputImage> iit2(m_Segmenter->GetOutput(), m_ImageRegion);
    while (!iit.IsAtEnd()){
       iit.Set(iit2.Get());
       ++iit;++iit2;  
    }  
  }


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
FuzzySegGrayRun::
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
FuzzySegGrayRun::
Save(const char *filename)
{
  std::ofstream fout(filename,std::ios::binary);

  itk::ImageRegionIteratorWithIndex<OutputImage> iit(m_Segmenter->GetOutput(), m_ImageRegion);
  unsigned char *writer=new unsigned char [m_ImageSize];
  unsigned long i=0;
  while (!iit.IsAtEnd()){
     writer[i]=(unsigned char)(255*(iit.Get()));
     ++iit;i++;
  }

  fout.write((char *)(writer),m_ImageSize);
}


