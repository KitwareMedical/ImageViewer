/*===============================================================
Summer 2001

  This file contains the definitions of the callback 
  functions for the fluid generated TripleTKGUI structure.  
  Callbacks are defined in fluid under the format 
  'name'_CB without parameter brackets and then are defined 
  here under the format 'name'_CB('type of fluid member'*o, void*).  
  Also existing in this file are supplementary functions 
  to the callbacks.

  Author: Andrew MacKelfresh
  CADDLAB
  UNC-CH
================================================================*/
#include "TripleTKGUI.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%SUPPLEMENTARY FUNCTIONS%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 


/*
*	  View the spefified slice (num), update the slider to
*	  show the corresponding slice number, and update the 
*   specified slice in 3D.  Is called by the callback for 
*   the slider and the up/down slice movement buttons.
*/
void selectSlice(unsigned int num)
  {
  sliceNumberSlider->value(num);
  tk2D->sliceNum((int)sliceNumberSlider->value());
  if (coronal2D->value() == 1) tk3D->sliceCoronal(num);
  else if (axial2D->value() == 1)tk3D->sliceAxial(num);
  else if (sagittal2D->value() == 1)tk3D->sliceSaggital(num);
  tk3D->update();
  tk2D->update();
  }


/*
*	  Zooms out of the 2D window.
*/
void zoomOut(void)
  {
  tk2D->winZoom(tk2D->winZoom()/2.0f);
  tk2D->update();
  }


/*
*	  Zooms into the 2D window.
*/
void zoomIn(void)
  {
  tk2D->winZoom(tk2D->winZoom()*2.0f);
  tk2D->update();
  }


/*
*	  Shift the image in the 2D window up.
*/
void shiftUp(void)
  {
  tk2D->winShift(-1,0);
  tk2D->update();
  }


/*
*	  Shift the image in the 2D window down.
*/
void shiftDown(void)
  {
  tk2D->winShift(1,0);
  tk2D->update();
  }


/*
*	  Spefifies the sagittal, axial or coronal view of the 2D image.
*   The unsigned interger parameter specifies these and 
*   corresponds to each as follows:
*	
*   coronal:    0
*   sagittal:	  1	
*   axial:		  2
*/
void Orientation(unsigned int value)
  {  
  tk2D->orientation(value);
  sliceNumberSlider->range(0.0f, tk2D->numSlices());
  tk2D->update();
  }


/*
*   Adjusts level intensity in 2D.  Is called by arrow keys when
*   radio button Level/Win is activated
*/
void setLevelIntensity(float val)
  {
  levelOutput->value(val);
  tk2D->iwMin(levelOutput->value());  
  tk2D->update();
  }


/*
*   Adjusts window intensity in 2D.  Is called by arrow keys when
*   radio button Level/Win is activated
*/
void setWindowIntensity(float val)
  {
  windowOutput->value(val);
  tk2D->iwMax(windowOutput->value());  
  tk2D->update();
  }


/*
*   Reset the GUI to its original settings.  Functionality 
*   dependent on whether in cut or view mode.
*/
void reset(void)
  {
  Orientation(0);                       //2D Image
  tk2D->winCenter();
  Orientation(1);                  
  tk2D->winCenter();
  Orientation(2);                  
  tk2D->winCenter();   //need for slices but also takes care of movement
  selectSlice(tk2D->numSlices()/2);
  
  coronal2D->value(0);                  //2D View Buttons
  axial2D->value(1);
  sagittal2D->value(0);
  
  float distance = tk2D->winZoom();     //zoom         
  tk2D->winZoom(-1*distance);
  
  setLevelIntensity(tk2D->getOriginalLevel());  //level/window
  setWindowIntensity(tk2D->getOriginalWindow());

  levelWindowButton->value(0);          //2D Radio Buttons
  move2DButton->value(0);
  zoom2DButton->value(0);
  sliceViewButton->value(1);
  
  coronal3D->value(0);                  //3D View Buttons
  axial3D->value(1);
  sagittal3D->value(0);
  allViewButton->value(0);
  outlineButton->value(0);

  tk3D->centerPlanes();                 //3D Planes
  
  tk3D->viewAxial(true);                //3D Image
  tk3D->viewCoronal(false);
  tk3D->viewSaggital(false);
  tk3D->viewOutline(false);
  
  tk2D->update();
  tk3D->update();
  }


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%GUI CALLBACKS%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


/*
*	  Changes slice being viewed and updates slice number
*	  displayed correspondingly; both by calling function
*   selectSlice(unsigned int blah) where blah is the
*   changed value of the slider.
*/
void sliceNumberSlider_CB(Fl_Value_Slider*o, void*)
  {
  selectSlice((unsigned int)o->value());
  } 


/*
* 	When this radio button is clicked, only its value in its 
*   group is one; therefore, the up/down arrow buttons function 
*   as is specified by the button.
*/
void zoom2D_CB(Fl_Check_Button*, void*)
  {
  } 


/*
*	  When this radio button is clicked, only its value in its 
*   group is one; therefore, the up/down arrow buttons function 
*   as is specified by the button.
*/
void move2D_CB(Fl_Check_Button*, void*)
  {
  }


/*
*	  When this radio button is clicked, only its value in its 
*   group is one; therefore, the up/down arrow buttons function 
*   as is specified by the button.
*/
void sliceScroll_CB(Fl_Check_Button*, void*)
  {
  } 


/*
*	  Sets the arrow buttons so that they adjust the "level"
*   and "window" properties of the 2D window.  Level is 
*   adjusted using the up and down buttons; window is 
*   adjusted using the right and left buttons.
*/
void levelWindow_CB(Fl_Check_Button*, void*)
  {
  } 


/*
*	  Change the 2D view to coronal by using the Orientation function.
*/
void coronal2D_CB(Fl_Check_Button*, void*)
  {
  Orientation(0);
  selectSlice(tk2D->sliceNum());
  } 


/*
*	  Change the 2D view to sagittal using the Orientation function.
*/
void sagittal2D_CB(Fl_Check_Button*, void*)
  {
  Orientation(1);
  selectSlice(tk2D->sliceNum());
  } 


/*
*	  Change the 2D view to axial using the Orientation function.
*	  This view is default in the beginning.
*/
void axial2D_CB(Fl_Check_Button*, void*)
  {
  Orientation(2);
  selectSlice(tk2D->sliceNum());
  } 


/*
*	  Change the 3D view to coronal.
*/
void coronal3D_CB(Fl_Check_Button*, void*)
  {
  allViewButton->value(false);
  tk3D->viewAxial(false);
  tk3D->viewCoronal(true);
  tk3D->viewSaggital(false);
  tk3D->update();
  } 


/*
*	  Change the 3D view to sagittal.
*/
void sagittal3D_CB(Fl_Check_Button*, void*)
  {
  allViewButton->value(false);
  tk3D->viewAxial(false);
  tk3D->viewCoronal(false);
  tk3D->viewSaggital(true);
  tk3D->update();
  } 


/*
* 	Change the 3D view to axial.
*	  This view is default in the beginning.
*/
void axial3D_CB(Fl_Check_Button*, void*)
  {
  allViewButton->value(false);
  tk3D->viewAxial(true);
  tk3D->viewCoronal(false);
  tk3D->viewSaggital(false);
  tk3D->update();
  } 


/*
*	  Performs either a slice scroll, movement up in 2D, or zoom into
*	  in 2D.  The functionality of the button is determined by 
*	  the value of the radio buttons, Slice Scroll, Move 2D, and Zoom 2D,
*	  all whose purpose is to determine this functionality. 
*/
void upButton_CB(Fl_Button*, void*)
  {
  if(sliceViewButton->value() == 1)
    {
    int slices = sliceNumberSlider->value();
    if (slices > 0) selectSlice(slices-1);
    } 
  else if (move2DButton->value() == 1) shiftDown();
  else if (zoom2DButton->value() == 1) zoomOut();
  else if (levelWindowButton->value() == 1) setLevelIntensity((float)levelOutput->value()+10);
  else std::cout << "Error: Scroll Radio Buttons not responding or none are initialized with value 1 (true)" << std::endl;
  } 


/*
*	  Performs either a slice scroll, movement down in 2D, or zoom out of
*	  in 2D.  The functionality of the button is determined by 
*	  the value of the radio buttons, Slice Scroll, Move 2D, and Zoom 2D,
*	  all whose purpose is to determine this functionality. 
*/
void downButton_CB(Fl_Button*, void*)
  {
  if(sliceViewButton->value() == 1)
    {
    int slices = sliceNumberSlider->value();
    if (slices < tk2D->numSlices()) selectSlice(slices+1);
    } 
  else if (move2DButton->value() == 1) shiftUp();
  else if (zoom2DButton->value() == 1) zoomIn();
  else if (levelWindowButton->value() == 1) setLevelIntensity((float)levelOutput->value()-10);
  else std::cout << "Error: Scroll Radio Buttons not responding or none are initialized with value 1 (true)" << std::endl;
  } 


/*
*	  Pan left in 2D.
*/
void leftButton_CB(Fl_Button*, void*)
  {
  if (move2DButton->value() == 1) tk2D->winShift(0,1);
  else if (levelWindowButton->value() == 1) setWindowIntensity((float)windowOutput->value()+10);
  tk2D->update();   
  } 


/*
*	  Pan right in 2D.
*/
void rightButton_CB(Fl_Button*, void*)
  {
  if (move2DButton->value() == 1) tk2D->winShift(0,-1);
  else if (levelWindowButton->value() == 1) setWindowIntensity((float)windowOutput->value()-10);
  tk2D->update();      
  } 


/*
*	  Toggles between displaying and hiding the outline
*   box in 3D.
*/
void outline_CB(Fl_Light_Button* o, void*)
  {
  tk3D->viewOutline(o->value());
  tk3D->update();
  } 


/*
*	  Toggles between displaying all of or only viewing one 
*   of the views (axial, sagittal, or coronal) in 3D.
*/
void allViews_CB(Fl_Light_Button*o, void*)
  {
  if (o->value() == 1)  
    {
    tk3D->viewAxial(1);
    tk3D->viewCoronal(1);
    tk3D->viewSaggital(1);
    }
  else
    {
    if (axial3D->value() == 1){tk3D->viewCoronal(0);tk3D->viewSaggital(0);}
    else if (coronal3D->value() == 1){tk3D->viewAxial(0);tk3D->viewSaggital(0);}
    else {tk3D->viewCoronal(0);tk3D->viewAxial(0);}
    }
  tk3D->update();
  } 


/*
*   Restores the program to its original state.	
*/
void resetAll_CB(Fl_Button*, void*)
  {
  reset();
  } 


/*
*	  Closes the program.
*/
void fileExit_CB(Fl_Menu_*, void*)
  {
  exit(0);   
  } 


/*
* 	Closes the program.
*/
void buttonExit_CB(Fl_Button*, void*)
  {
  exit(0);   
  } 
