/** ImageViewer
 *  Provides a basic 2D and 3D image viewing application
 *  Demonstrates the use and capabilities of GLSliceViewer and how to
 *  derive that class
 *  Also demonstrates loading images using the MetaImage functions
 *  \author Stephen R. Aylward
 *  \date   11/21/99
 */

// FLTK Includes
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/filename.H>
#include <FL/fl_file_chooser.H>

// Image manipulation functions
#include <itkImage.h>

// Multifunction slice-based display class
#include <IViewGUIBase.h>

//include <ImageIOLib/FileSaveGUI.h>
#include <deque>

//FileIO
#include <IViewUtils.h>


typedef Image<float,3> ImageType;
typedef Image<unsigned char,3> OverlayType;

typedef std::deque<ImageType::Pointer> DequeOfImages;

typedef std::deque<OverlayType::Pointer> DequeOfOverlays;



IViewGUIBase *ivGUI;    // Pointer to base FLTK window


unsigned short int currentImageNum = 0;

// Pointer to current image being shown
ImageType::Pointer currentImage = (ImageType::Pointer)NULL;
//Pointer to current overlay
OverlayType::Pointer overlayImage = (OverlayType::Pointer)NULL;


DequeOfImages imageList;
DequeOfOverlays overlayList;


void UpdateDisplayWindow()
{
  // resize window to be proportional to image size - scale image to fit screen
  unsigned long winSize[2];


  ImageType::RegionType region = currentImage->GetLargestPossibleRegion();
  ImageType::SizeType   size   = region.GetSize();


  winSize[0] = size[0];
  winSize[1] = size[1];

  while(winSize[0]>800 || winSize[1]>800) 
    {
    winSize[0] /= 2;
    winSize[1] /= 2;
    }

  while(winSize[0]<200 || winSize[1]<200) 
    {
    winSize[0] *= 2;
    winSize[1] *= 2;
    }

  winSize[0] += ivGUI->iviewWindow->w() - ivGUI->sliceView->w();
  winSize[1] += ivGUI->iviewWindow->h() - ivGUI->sliceView->h();
  ivGUI->iviewWindow->resize(ivGUI->iviewWindow->x(), ivGUI->iviewWindow->y(), winSize[0], winSize[1]);

  ivGUI->iviewWindow->redraw();
}


void loadIm(char *fName)
{
 
  currentImage = loadImage<float>(fName);


  overlayImage = (OverlayType::Pointer)NULL;


  imageList.push_back(currentImage);
  overlayList.push_back((OverlayType::Pointer)NULL);//placeholder in overlay
						    //image list
  
  ivGUI->setImage(currentImage);
  ivGUI->iviewWindow->label(fName);
  ivGUI->sliceView->ViewOverlayData(false);
 
  UpdateDisplayWindow();

  ivGUI->imageNumberSlider->maximum(imageList.size() - 1);
  ivGUI->imageNumberSlider->value(imageList.size() - 1);
  ivGUI->sliceNumberSlider->value(0);
}

void loadOverlay(char *fName)
{
  overlayImage = loadImage<unsigned char>(fName);

  overlayList[currentImageNum] = overlayImage;

  ivGUI->setOverlay(overlayImage);
  UpdateDisplayWindow();
}

void tkOpenImageCB(void)
{
    char *fName = (char *)fl_file_chooser("Pick a meta file","*.mh*", "");

    if(fName == NULL)
        return;

    loadIm(fName);
}

void tkOpenOverlayCB(void)
{
    char *fName = (char *)fl_file_chooser("Pick a meta file","*.mh*", "");

    if(fName == NULL)
        return;

    loadOverlay(fName);
}



void tkRemoveCB(void)
{
  DequeOfImages::iterator i = imageList.begin();
  DequeOfOverlays::iterator j = overlayList.begin();

  if(imageList.size() > 1)
    {
    //delete imageList[currentImageNum];	no need to delete
    //delete overlayList[currentImageNum];	I think - smart pointers

    imageList.erase(i+currentImageNum);
    overlayList.erase(j+currentImageNum);

    //image 0 is displayed
    ivGUI->imageNumberSlider->maximum(imageList.size() - 1);
    currentImageNum = 0;
    currentImage = imageList[currentImageNum];
    overlayImage = overlayList[currentImageNum];
    ivGUI->setImage(currentImage);
    ivGUI->imageNumberSlider->value(currentImageNum);
    UpdateDisplayWindow();
    }
  else
    std::cerr << "Error: can't delete only image in set" << std::endl;
}

void tkSaveCB(void)
{
  //FileSaveGUI* fileSaveDialog;
  //fileSaveDialog = new FileSaveGUI(currentImage, currentIO->IO()->FileName(), ivGUI->sliceNumberSlider->value());
  //fileSaveDialog->show();
}

void tkImageNumSliderCB(unsigned short int imageNum)
{
  currentImageNum = imageNum;
  currentImage = imageList[currentImageNum];
  overlayImage = overlayList[currentImageNum];
  ivGUI->setImage(currentImage);

  if(overlayImage == (OverlayType::Pointer)NULL)
    ivGUI->sliceView->ViewOverlayData(false);
  else
    ivGUI->setOverlay(overlayImage);

  UpdateDisplayWindow();
  ivGUI->sliceNumberSlider->value(0);
}


int main(int argc, char **argv)
{
  char *fName1, *fName2, *filename1 , *filename2;

  if(argc >= 3)
    {
    fName1 = argv[1];
    fName2 = argv[2];
    }
  else if(argc == 2)
    {
    fName1 = argv[1];
    fName2 = NULL;
    }
  else
    {
      filename1 = (char *)fl_file_chooser("Pick the image file","*.mh*", "");
      if(filename1 == NULL)
      return 0;
      fName1=new char[255];
      strcpy (fName1,filename1);
      filename2 = (char *)fl_file_chooser("Pick the overlay file","*.mh*", "");
      if (filename2 == NULL)
      {
        fName2 = NULL;
      }
      else
      {
        fName2=new char[255];
        strcpy (fName2, filename2 );
      }
  }

  // create the FLTK window
  ivGUI = new IViewGUIBase;
  ivGUI->imageNumberSlider->show();
  ivGUI->imageNumberSlider->activate();
  ivGUI->setImageNumSliderCB(tkImageNumSliderCB);
  ivGUI->OpenAdd->show();
  ivGUI->OpenAdd->activate();
  ivGUI->AddOverlay->show();
  ivGUI->AddOverlay->activate();
  ivGUI->setOpenImageCB(tkOpenImageCB);
  ivGUI->setOpenOverlayCB(tkOpenOverlayCB);
  ivGUI->Remove->show();
  ivGUI->Remove->activate();
  ivGUI->setRemoveCB(tkRemoveCB);
  ivGUI->setSaveCB(tkSaveCB);

  // set the appropriate FLTK visuals
  Fl::gl_visual(FL_DOUBLE | FL_RGB | FL_ALPHA | FL_DEPTH);
  Fl::visual(FL_ALPHA | FL_RGB | FL_DOUBLE);

  // indicate that currentIO and currentImage haven't been loaded previously
  currentImage = (ImageType::Pointer)NULL;
  overlayImage = (OverlayType::Pointer)NULL;

  // load the initial image
  loadIm(fName1);

  // if there is an overlay image, load it
  if(fName2 != NULL)
    loadOverlay(fName2);

  // display the windows and images
  ivGUI->show();

  // run
  Fl::run();//while(IdleFunc());

  delete fName1;
  delete fName2;
  return 1;
}
