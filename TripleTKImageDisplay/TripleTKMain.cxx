/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    TripleTKMain.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "TripleTKMain.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%TripleTKMain Member functions%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


//------------------------------------------------------------
TripleTKMain ::TripleTKMain()
  {
  window = make_window();
  }


//------------------------------------------------------------
TripleTKMain ::~TripleTKMain()
  {
  }


//------------------------------------------------------------
void TripleTKMain::synchronize2D(void) 
  {
  float iwDiff  = tk2D->iwMax() - tk2D->iwMin();
  float b       = (float)((int)log10(iwDiff)-2);
  double iwMin  = ((int)(tk2D->iwMin()*pow(10, -b)))/pow(10,-b);
  double iwMax  = ((int)(tk2D->iwMax()*pow(10, -b)))/pow(10,-b);
  double iwStep = (iwMax-iwMin)/100.0;
  sliceNumberSlider->range(0.0f, tk2D->numSlices());
  sliceNumberSlider->value((float)((int)tk2D->numSlices()/2));
  levelOutput->range(iwMin-iwStep,iwMax+iwStep);
  windowOutput->range(iwMin-iwStep,iwMax+iwStep);
  levelOutput->step(iwStep);
  windowOutput->step(iwStep);
  levelOutput->value(iwMin);
  windowOutput->value(iwMax);
  tk2D->flipY(true); 
  tk2D->setOriginalLevel(iwMin);
  tk2D->setOriginalWindow(iwMax);
  }


//-------------------------------------------------------------
void TripleTKMain::synchronize3D(void) 
  {
  tk3D->viewAxial(true);
  tk3D->viewCoronal(false);
  tk3D->viewSaggital(false);
  tk3D->viewOutline(false);
  tk3D->viewVolRend(false);
  tk3D->opacity(1);
  tk3D->show();
  tk3D->update();
  }


//-------------------------------------------------------------
void TripleTKMain::SetImage(ImageType::Pointer img)
  { 
  //Set a spacing
  double *spacing = (double*)img->GetSpacing();
  spacing[0] = 1;
  spacing[1] = 1;
  spacing[2] = 1;
  img->SetSpacing(spacing);
  
  //Pass image data to TripleTK2D/3D
  tk2D->SetInputImage(img);
  tk3D->SetInputImage(img);
  
  //Clean everything up
  synchronize2D();
  synchronize3D();
  }


//-------------------------------------------------------------
void TripleTKMain::show(void)
  {
  tk2D->show();
  tk3D->show();
  }


//-------------------------------------------------------------
void TripleTKMain::update(void)
  {
  tk2D->update();
  tk3D->update();
  }


//-------------------------------------------------------------
void TripleTKMain::redraw(void) 
  {
  tk2D->redraw();
  tk3D->redraw();
  }


/*
*   Helper function to set slice num callback
*/
void selectSliceCB(void)
  {
  sliceNumberSlider->value(tk2D->sliceNum());
  if (coronal2D->value() == 1) tk3D->sliceCoronal(tk2D->sliceNum());
  else if (axial2D->value() == 1)tk3D->sliceAxial(tk2D->sliceNum());
  else if (sagittal2D->value() == 1)tk3D->sliceSaggital(tk2D->sliceNum());
  tk3D->update();
  tk2D->update();
  }


//-------------------------------------------------------------
void TripleTKMain::setCallBacks(void)
  {
  tk2D->sliceNumCallBack(selectSliceCB);
  }


//-------------------------------------------------------------
Fl_Window * TripleTKMain::getWindow()
  {
  return window;
  }


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%SHINY BACKGROUND%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


static uchar color[8][3] = 
  {
  {128,136,149},
  {63,54,21},
  {128,136,146},
  {223,223,223},
  {121,128,128},
  {192,206,220},
  {137,143,145},
  {99,81,64}
  };

int which = 0;

static Fl_Color pcolor;

static uchar C[8][3]; // actual colors for current button

static void calc_color(Fl_Color c) 
  {
  uchar r[3];
  pcolor = c;
  Fl::get_color(c,r[0],r[1],r[2]);
  for (int x = 0; x<8; x++) for (int y=0; y<3; y++) 
    {
    int i = r[y]-166+color[x][y];
    if (i<0) i = 0; else if (i>255) i = 255;
    C[x][y] = i;
    }
  }


//will be the box in the background of the window
void shiny_flat_box(int x, int y1, int w, int h, Fl_Color c) 
  {
  if (c != pcolor) calc_color(c);
  int y = Fl_Window::current()->h()-(y1+h);
  gl_start();
  glBegin(GL_POLYGON);
  glColor3ub(C[4][0],C[4][1],C[4][2]);
  glVertex2i(x,y);
  glColor3ub(C[5][0],C[5][1],C[5][2]);
  glVertex2i(x+w,y);
  glColor3ub(C[6][0],C[6][1],C[6][2]);
  glVertex2i(x+w,y+h);
  glColor3ub(C[7][0],C[7][1],C[7][2]);
  glVertex2i(x,y+h);
  glEnd();

  gl_finish();
  Fl::flush();
  }

//define box for shiny background
#define SHINY_BOX (Fl_Boxtype)30


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%MAIN%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int main()
  {     
  Fl::gl_visual(FL_DOUBLE | FL_RGB | FL_ALPHA | FL_DEPTH);
  Fl::visual(FL_DOUBLE | FL_RGB | FL_ALPHA | FL_DEPTH);
  Fl::set_boxtype(SHINY_BOX,shiny_flat_box,0,0,0,0);//shiny background
  
  //Specify Images to load
  char *metaImageFileName;
  metaImageFileName = fl_file_chooser("Pick a MetaImage file", "*.mh*", "/images");
  if(metaImageFileName == NULL)  return 0;

  std::cout << "Creating window..." << std::endl;

  //TripleTKMain viewer type
  typedef TripleTKMain ImageViewerType;
  ImageViewerType* viewer = new ImageViewerType;

  viewer->getWindow()->box(SHINY_BOX);
  viewer->getWindow()->show();
  viewer->setCallBacks();
 
  std::cout << "loading MetaImage..." << std::endl; 

  //Define the Image type
  typedef itk::Image<short,3>  ImageType;
  ImageType::Pointer input_image;

  //Load the image
  typedef FileIOToImageFilter<ImageType> LoaderType;
  ObjectFactoryBase::RegisterFactory(new FileIOMetaImageFactory);
  LoaderType::Pointer loader = LoaderType::New();

  //set mh* file
  loader->SetFileName(metaImageFileName); 
  loader->Update();
  input_image  = loader->GetOutput(); 

  std::cout << "Showing Images..." << std::endl;

  viewer->SetImage(input_image);
  viewer->show();
  viewer->getWindow()->redraw();

  return Fl::run();
  }
