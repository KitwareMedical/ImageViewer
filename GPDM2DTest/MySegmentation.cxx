#include "MySegmentation.h"
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Int_Input.H>
//#include <fstream>

MySegmentation::
MySegmentation()
{
  m_ViewImage=FloatImage3D::New();
  m_ViewSegImage=FloatImage3D::New();
  m_DataImage=VecImageType::New();
  m_SegImage=ImageType2D::New();
  m_Viewer=0;
  m_ViewerResult=0;
  m_SampleRegion=5;
  showdm=FALSE;
}

MySegmentation::
~MySegmentation()
{
}

void 
MySegmentation::LoadImage()
{
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return;
    }
  if(this->Load( filename )){
    if (slices > 1) { 
      strcpy(m_FileName, filename);
      unsigned char i=1;
      unsigned int tmp;
      tmp = slices-1;
      while (tmp > 9) { 
        tmp = tmp/10; 
        i++;
      }
      cslices = 0;
      lens = i;
      LoadNextImage(cslices, filename);
    } else ReadImage(filename);
  }
}

void 
MySegmentation::LoadNextImage(unsigned int nc, const char * fname)
{
  const char * zero = "0";
  char * tmp="";
  char tmp1[10];
  itoa(nc, tmp1, 10);
  strcat(tmp1, ".raw");
  const char * nslices = tmp1;
  int i = 0;
  strcpy(tmp, fname);
  while (strlen(nslices)+i<lens+4) {
    strcat(tmp, zero);
    i++;
  }
  strcat(tmp, nslices);
  ReadImage(tmp);
}

int
MySegmentation::
Load( const char * filename )
{
  Fl_Window window(200,280);
  Fl_Int_Input inputcols(80, 10, 100, 25, "Columns:");
  Fl_Int_Input inputrows(80, 50, 100, 25, "Rows:");
  Fl_Int_Input inputslices(80, 90, 100, 25, "Slices:");
  inputcols.value("256");
  inputrows.value("256");
  inputslices.value("1");
  Fl_Int_Input inputheader(80, 130, 100, 25, "Header:");
  inputheader.value("0");
  Fl_Button cancel(40, 240, 60, 25, "cancel");
  Fl_Return_Button ok(120, 240, 60, 25, "OK");
  Fl_Menu_Item pulldown[] = {
    {"Unsigned Char"},
    {"Unsigned Short"},
    {0}
  };
  Fl_Choice ch(45,180,140,25,"Data:"); ch.menu(pulldown);

  window.hotspot(&cancel); // you must position modal windows
  window.end();
  window.set_modal();
  window.show();
  for (;;) {
    Fl::wait();
    Fl_Widget *o;
    while ((o = Fl::readqueue())) {
      if (o == &ok) {
        cols=atoi(inputcols.value());
        rows=atoi(inputrows.value());
        slices=atoi(inputslices.value());
        offset=atoi(inputheader.value());
        datatype=ch.value();
        return 1;
      } 
      else if (o == &cancel || o == &window) {
        return 0;
      }
    }
  }
}

void
MySegmentation::
ReadImage( const char * filename )
{
  unsigned int slice=1;
  FloatImage3D::SizeType size={{cols,rows,slices}};
  FloatImage3D::IndexType index={{0,0,0}};
  region.SetSize(size);
  region.SetIndex(index);
  m_ViewImage->SetLargestPossibleRegion(region);
  m_ViewImage->SetBufferedRegion(region);
  m_ViewImage->SetRequestedRegion(region);
  m_ViewImage->Allocate();
  m_ViewSegImage->SetLargestPossibleRegion(region);
  m_ViewSegImage->SetBufferedRegion(region);
  m_ViewSegImage->SetRequestedRegion(region);
  m_ViewSegImage->Allocate();

  ImageType3D::SizeType size2={{cols,rows,slice}};
  ImageType3D::IndexType index2={{0,0,0}};
  region2.SetSize(size2);
  region2.SetIndex(index2);

  ImageType2D::SizeType size4={{cols,rows}};
  ImageType2D::IndexType index4={{0,0}};
  region4.SetSize(size4);
  region4.SetIndex(index4);

  VecImageType::SizeType size3={{cols,rows,slice}};
  VecImageType::IndexType index3={{0,0,0}};
  region3.SetSize(size3);
  region3.SetIndex(index3);

  m_DataImage->SetLargestPossibleRegion(region3);
  m_DataImage->SetBufferedRegion(region3);
  m_DataImage->SetRequestedRegion(region3);
  m_DataImage->Allocate();

  m_SegImage->SetLargestPossibleRegion(region4);
  m_SegImage->SetBufferedRegion(region4);
  m_SegImage->SetRequestedRegion(region4);
  m_SegImage->Allocate();
/*
  m_OutputObject->SetLargestPossibleRegion(region2);
  m_OutputObject->SetBufferedRegion(region2);
  m_OutputObject->SetRequestedRegion(region2);
  m_OutputObject->Allocate();
*/
  unsigned long rsize=cols*rows;
  unsigned long iter;
//  std::ifstream fin(filename,std::ios::binary);
  FILE *fin=fopen(filename,"rb"); 
  itk::ImageRegionIteratorWithIndex<FloatImage3D> iit(m_ViewImage, region);
  itk::ImageRegionIteratorWithIndex<VecImageType> iit2(m_DataImage, region3);
  char *offread=new char[offset];
  fread(offread,1,offset,fin);

  switch (datatype){
    case 0: 
      {
      unsigned char *reader1=new unsigned char[rsize];
      fread(reader1,1,rsize,fin);  
//      fin.read((char*)reader1,rsize);
      for(iter=0;iter<rsize;iter++){
        iit.Set((float)(reader1[iter]));
        myvec = (unsigned short)(reader1[iter]);
        iit2.Set(myvec);
        ++iit;++iit2;
        }
      }
      break;
    case 1:
      {
      unsigned short *reader2=new unsigned short[rsize];
      fread(reader2,2,rsize,fin);  
//      fin.read((char*)reader2,rsize+rsize);
      for(iter=0;iter<rsize;iter++){
        iit.Set((float)(reader2[iter]));
        myvec = reader2[iter];
        iit2.Set(myvec);
        ++iit;++iit2;
        }
      }
      break;
    default:
      break; 
  }
//  m_GibbsFilter->SetInput(m_DataImage);
  if(!m_Viewer)
  m_Viewer=new ViewerType(100,100,cols,rows,"");
  if(!m_ViewerResult)
  m_ViewerResult=new ViewerType(100,500,cols,rows,"");
  this->ShowImage();
}

void
MySegmentation::
ShowImage(void)
{
  m_Viewer->SetInputImage( m_ViewImage);
  m_Viewer->show();
  m_Viewer->update();
  Fl::check();
  m_Viewer->redraw();
  Fl::check();
}

void 
MySegmentation::
ClearClicked(void)
{
  m_Viewer->clearClickedPointsStored();
}


void 
MySegmentation::
ComputeStats(void)
{
////////////////////////////////////////////////////////////////////
// we use this function to recalculate the parameters for the
// Gibbs Prior models. but not done yet since we have not decided 
// to apply 2D deformable model only or 2D/3D deformable model,
// one other reason is that for different region-based method, the 
// requirement for the parameters can be different, so we left it
// open here
/*
  unsigned int numPoints=m_Viewer->numClickedPointsStored();
  unsigned int i,j,k;  
  unsigned int x,y,ex,bx,ey,by;
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
    bx=x-halfr;ex=bx+m_SampleRegion;
    by=y-halfr;ey=by+m_SampleRegion;
    if(bx<0) bx=0;
    if(by<0) by=0;
    if(ex>cols-1) ex=cols-1;
    if(ey>rows-1) ey=rows-1;
    for(j=by;j<ey;j++){
      for(k=bx;k<ex;k++){
        totalp++;
        idx[0]=k;idx[1]=j;
        currp= m_DataImage->GetPixel(idx);
        totalv+=currp;
        totalvv+=currp*currp;      
      }
    }
  }
  m_Mean=totalv/(totalp-1);
  m_Variance=(totalvv-totalv*totalv/totalp)/(totalp-1);
*/
}

void 
MySegmentation::
ShowResult()
{
  if(showdm){
  itk::ImageRegionIteratorWithIndex<FloatImage3D> iit(m_ViewSegImage, region);

  unsigned int numModels=m_Viewer->numClickedPointsStored();
  itk::Mesh<float, 3>::PointsContainerPointer     points;
  itk::Mesh<float, 3>::PointsContainer::Iterator  pointsit;
  itk::Mesh<float, 3>::PointType                  node;

  int k=0;
  buffer = (unsigned char *) malloc(sizeof(unsigned char)*rows*cols);
  while (k<cols*rows) {
    buffer[k] = 0;
    k++;
  }

  for (int j=0; j<numModels; j++) {
    points = m_bffilter[j]->GetOutput()->GetPoints();
    pointsit = points->Begin();

    while ( pointsit != points->End() ) {
      node = pointsit.Value();
      k = ((int)node[1])*cols+((int)node[0]);
      buffer[k] = 255;
      ++pointsit;
    }
  }

  k = 0;
  while (k<cols*rows) {
    iit.Set((float)buffer[k]);
    ++iit;
    ++k;
  }    
  }
  else
  {
  itk::ImageRegionIteratorWithIndex<FloatImage3D> iit(m_ViewSegImage, region);
  itk::ImageRegionIteratorWithIndex<ImageType3D> iit2(m_GibbsFilter->GetOutput(), region2);
  while (!iit.IsAtEnd()){
     iit.Set(iit2.Get());
     ++iit;++iit2;  
  }  
  }

//  free(buffer);

  m_ViewerResult->SetInputImage( m_ViewSegImage);
  m_ViewerResult->show();
  m_ViewerResult->update();
  Fl::check();
  m_ViewerResult->redraw();
  Fl::check();
}

void 
MySegmentation::
SaveResult()
{
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return;
    }

  unsigned long wsize=cols*rows;
  unsigned long iter;

  FILE *fout=fopen(filename,"wb"); 
  itk::ImageRegionIteratorWithIndex<ImageType3D> oit(m_GibbsFilter->GetOutput(), region2);
      
  unsigned char *writer=new unsigned char[wsize]; 
  for(iter=0; iter<wsize; iter++){
    writer[iter] = oit.Get();
    ++oit;
  }
  fwrite(writer,1,wsize,fout); 

}

void 
MySegmentation::
PickDeformableSeed()
{  

  itk::ImageRegionIteratorWithIndex<ImageType3D> it3D(m_GibbsFilter->GetOutput(), region2);
  itk::ImageRegionIteratorWithIndex<ImageType2D> it2D(m_SegImage, region4);
  
  buffer = (unsigned char *) malloc(sizeof(unsigned char)*rows*cols);
  int k = 0;
  while ( k<rows*cols ) {
    buffer[k] = (unsigned char)it3D.Get();
    ++it3D;
    k++;
  }
  k = 0;
  while ( !it2D.IsAtEnd() ) {
    it2D.Set( (unsigned short)buffer[k] );
    ++it2D;
    ++k;
  }

//  free(buffer);
}

void 
MySegmentation::
DeformableFit()
{  
  unsigned int numPoints=m_Viewer->numClickedPointsStored();

  for ( int j = 0; j < numPoints; j++ ) {
    m_bffilter[j]->Update();
  }
}

void 
MySegmentation::
LoadPotential()
{
/*
  const char * filename = fl_file_chooser("Image Filename","*.*","");
  if( !filename )
    {
      return;
    }
  if(this->Load( filename )){
    strcpy(m_FileName, filename);
    ReadImage(filename);
  }
*/
  unsigned int numPoints=this->m_Viewer->numClickedPointsStored();
  ClickPoint *point;
  itk::Point<float, 3> meshcenter;
  itk::Point<float, 3> meshscale;

  if (!grfilter) grfilter = GRGFilterType::New();
  if (!gtomfilter) gtomfilter = myGToMFilterType::New();

  if (!m_GradImage) {
    m_GradImage = FloatImage::New();
    FloatImage::SizeType size={{cols,rows}};
    FloatImage::IndexType index={{0,0}};
    region5.SetSize(size);
    region5.SetIndex(index);
    m_GradImage->SetLargestPossibleRegion(region5);
    m_GradImage->SetBufferedRegion(region5);
    m_GradImage->SetRequestedRegion(region5);
    m_GradImage->Allocate();
  }

  if (!m_PtImage) {
    m_PtImage = ImageType2D::New();
    ImageType2D::SizeType ptsize = {{cols, rows}};
    ImageType2D::IndexType ptindex = {{0, 0}};
    region4.SetSize(ptsize);
    region4.SetIndex(ptindex);
    m_PtImage->SetLargestPossibleRegion(region4);
    m_PtImage->SetBufferedRegion(region4);
    m_PtImage->SetRequestedRegion(region4);
    m_PtImage->Allocate();
  }

  itk::ImageRegionIteratorWithIndex<FloatImage3D> it3D(m_ViewImage, region);
  itk::ImageRegionIteratorWithIndex<FloatImage> it2D(m_GradImage, region5);
  it3D.GoToBegin();
  it2D.GoToBegin();

  while ( !it2D.IsAtEnd() ) {
    it2D.Set( it3D.Get() );
    ++it2D;
    ++it3D;
  }

  // Connect the input images
  grfilter->SetInput( m_GradImage ); 

  // Set sigma
  grfilter->SetSigma( 2.0 );

  if (!m_LFilter) m_LFilter = myLaplacianFilterType::New();

  gtomfilter->SetInput(grfilter->GetOutput());

  if (!gfilter) gfilter = myGFilterType::New();
  gfilter->SetInput(gtomfilter->GetOutput());
  gfilter->Update();

  for ( int j = 0; j < numPoints; j++ ) {
    point=m_Viewer->getClickedPoint(j);
    m_Center[0] = point->x;
    m_Center[1] = point->y;
    meshcenter[0] = point->x;
    meshcenter[1] = point->y;
    meshcenter[2] = 0;
    meshscale[0] = 10;
    meshscale[1] = 10;
    meshscale[2] = 1;
    m_meshsource[j] = MeshSourceType::New();
    m_meshsource[j]->SetCenter(meshcenter);
    m_meshsource[j]->SetScale(meshscale);
    m_meshsource[j]->SetResolutionY(m_Resolution);
    m_meshsource[j]->SetResolutionX(1);

    m_bffilter[j] = BFFilterType::New();
    m_bffilter[j]->SetInput(m_meshsource[j]->GetOutput());
    m_bffilter[j]->SetCenter(m_Center);
//  m_bffilter[j]->SetGradient(gfilter->GetOutput());
    m_bffilter[j]->SetStiffness(m_Stiffness);
    m_bffilter[j]->SetTimeStep(m_TimeStep);
    m_bffilter[j]->SetDistanceForGradient(m_DistanceForGradient);
    m_bffilter[j]->SetDistanceToStop(m_DistanceToStop);
    m_bffilter[j]->SetResolution(m_Resolution);
    m_bffilter[j]->SetPotential(m_SegImage);
    m_bffilter[j]->SetGradient(gfilter->GetOutput());
  }

}
