#include "MySegmentation.h"
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Int_Input.H>
//#include <fstream>

MySegmentation::
MySegmentation()
{
  m_ViewImage=FloatImage3D::New();// 2D image for original image view
  m_ViewSegImage=FloatImage3D::New(); // 2D image for segmentation results review
  m_DataImage=FloatImage3D::New(); // for 3D gradient map construction
  m_SegImage=ImageType2D::New(); // segmentation result
  m_GPImage = VecImageType::New(); // gibbs prior model input
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
  if(this->Load( filename )) {
    cslices = 0;
    gpmaskcreated = false;

    FloatImage3D::SizeType size={{cols,rows,1}};
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
 
    ImageType3D::SizeType size2={{cols,rows,1}};
    ImageType3D::IndexType index2={{0,0,0}};
    region2.SetSize(size2);
    region2.SetIndex(index2);

    VecImageType::SizeType size3={{cols,rows,1}};
    VecImageType::IndexType index3={{0,0,0}};
    region3.SetSize(size3);
    region3.SetIndex(index3);

    m_GPImage->SetLargestPossibleRegion(region3);
    m_GPImage->SetBufferedRegion(region3);
    m_GPImage->SetRequestedRegion(region3);
    m_GPImage->Allocate();

    ReadImage(filename);
    LoadNextImage(cslices);
  }
}

void 
MySegmentation::LoadNextImage(unsigned int nc)
{
  FloatImage3D::SizeType size={{cols,rows,1}};
  FloatImage3D::IndexType index={{0,0,nc}};
  FloatImage3D::IndexType index1={{0,0,0}};
  region.SetSize(size);
  region.SetIndex(index1);
  region1.SetSize(size);
  region1.SetIndex(index);

  itk::ImageRegionIteratorWithIndex<FloatImage3D> it(m_DataImage, region1);
  itk::ImageRegionIteratorWithIndex<VecImageType> itg(m_GPImage, region3); 
  itk::ImageRegionIteratorWithIndex<FloatImage3D> itv(m_ViewImage, region); 

  while ( !it.IsAtEnd() ) {
    myvec = (unsigned short)(it.Get());
    itg.Set(myvec);
    itv.Set(it.Get()); 
    ++it;
    ++itg;
    ++itv;
  }

  if(!m_Viewer)
  m_Viewer=new ViewerType(100,100,cols,rows,"");
  if(!m_ViewerResult)
  m_ViewerResult=new ViewerType(100,500,cols,rows,"");
  this->ShowImage();
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
  m_DataImage->SetLargestPossibleRegion(region);
  m_DataImage->SetBufferedRegion(region);
  m_DataImage->SetRequestedRegion(region);
  m_DataImage->Allocate();

  unsigned long rsize=cols*rows;
  unsigned long iter;

  FILE *fin=fopen(filename,"rb"); 
  itk::ImageRegionIteratorWithIndex<FloatImage3D> it(m_DataImage, region);

  char *offread=new char[offset];
  fread(offread,1,offset,fin);

  switch (datatype){
    case 0: 
      {
        unsigned char *reader1=new unsigned char[rsize];
        for (int slice = 0; slice < slices; slice++) {
          fread(reader1,1,rsize,fin); 
          for (iter=0;iter<rsize;iter++) {
            it.Set((float)(reader1[iter]));
            ++it;
          }
        }
      }
      break;
    case 1:
      {
        unsigned short *reader2=new unsigned short[rsize];
        for (int slice = 0; slice < slices; slice++) {
          fread(reader2,2,rsize,fin);
          for(iter=0;iter<rsize;iter++){
            it.Set((float)(reader2[iter]));
            ++it;
          }
        }
      }
      break;
    default:
      break; 
  }
//  m_GibbsFilter->SetInput(m_DataImage);

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

}

void 
MySegmentation::
ShowResult()
{
  itk::ImageRegionIteratorWithIndex<FloatImage3D> iit(m_ViewSegImage, region);
  itk::ImageRegionIteratorWithIndex<ImageType3D> iit2(m_GibbsFilter->GetOutput(), region2);
  while (!iit.IsAtEnd()){
     iit.Set(iit2.Get());
     ++iit;++iit2;  
  }  

//  free(buffer);

  m_ViewerResult->SetInputImage( m_ViewSegImage );
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
DeformableFit()
{  
//  m_dfilter->Update();
}

void 
MySegmentation::
SetGradient()
{
  if (!grfilter) grfilter = GRGFilterType::New();
  if (!gtomfilter) gtomfilter = myGToMFilterType::New();
/*
  // Connect the input images
  grfilter->SetInput( m_ViewImage ); 

  // Set sigma
  grfilter->SetSigma( 2.0 );

//  if (!m_LFilter) m_LFilter = myLaplacianFilterType::New();

  gtomfilter->SetInput(grfilter->GetOutput());
*/
  if (!gfilter) gfilter = myGFilterType::New();
/*
  gfilter->SetInput(gtomfilter->GetOutput());
  gfilter->Update();

//  itk::ShrinkImageFilter< ImageType3D, ImageType3D >::Pointer shrink;
//  shrink = itk::ShrinkImageFilter< ImageType3D, ImageType3D >::New();
*/
  if ( !gpmaskcreated ) {
    const char * filename = fl_file_chooser("Image Filename","*.*","");
    if( !filename )
    {
      return;
    }
    if(this->Load( filename ))
    {
      m_ClassImage = ImageType3D::New();
      ImageType3D::SizeType size={{cols,rows,slices}};
      ImageType3D::IndexType index={{0,0,0}};
      region2.SetSize(size);
      region2.SetIndex(index);

      m_ClassImage->SetLargestPossibleRegion(region2);
      m_ClassImage->SetBufferedRegion(region2);
      m_ClassImage->SetRequestedRegion(region2);
      m_ClassImage->Allocate();

      unsigned long rsize=cols*rows;
      unsigned long iter;

      FILE *fin=fopen(filename,"rb"); 
      itk::ImageRegionIteratorWithIndex<ImageType3D> it(m_ClassImage, region2);
      char *offread=new char[offset];
      fread(offread,1,offset,fin);

      switch (datatype){
        case 0: 
          {
            unsigned char *reader=new unsigned char[rsize];
            for ( int i=0; i<slices; i++) {
              fread(reader,1,rsize,fin);  
              for(iter=0;iter<rsize;iter++){
                it.Set((unsigned short)(reader[iter]));
                ++it;
              }
            }
          }
          break;
        case 1:
          {
            unsigned short *reader2=new unsigned short[rsize];
            fread(reader2,2,rsize,fin);  
            for ( int i=0; i<slices; i++) {
              fread(reader2,1,rsize,fin);  
              for(iter=0;iter<rsize;iter++){
                it.Set(reader2[iter]);
                ++it;
              }
            }
          }
          break;
        default:
          break; 
      }

    } 
  }
/*
  shrink->SetInput( m_ClassImage );
  shrink->SetNumberOfThreads(4);

  unsigned int factors[3] = { 1, 1, 1 };
  shrink->SetShrinkFactors(factors);
  shrink->UpdateLargestPossibleRegion();

  binaryImageType::RegionType requestedRegion;
  requestedRegion = shrink->GetOutput()->GetRequestedRegion();
*/
  /*
  myMeshSource::Pointer m_meshsource = myMeshSource::New();

  myDFilterType::Pointer m_dfilter = myDFilterType::New();
  m_dfilter->SetInput(m_meshsource->GetOutput());
//  m_dfilter->SetGradient(m_GVFFilter->GetOutput());
  m_dfilter->SetGradient(gfilter->GetOutput());

//  m_meshsource->SetBinaryImage( shrink->GetOutput() );
  m_meshsource->SetBinaryImage( m_ClassImage );
  m_meshsource->SetObjectValue( 255 );

  m_dfilter->SetStiffness(m_Stiffness);
  m_dfilter->SetGradientMagnitude(m_GradientMagnitude);
  m_dfilter->SetTimeStep(m_TimeStep);
  m_dfilter->SetStepThreshold(m_StepThreshold);
  m_dfilter->SetScale(m_Scale);
  m_dfilter->Update();*/
//  m_meshsource->Update();
}
