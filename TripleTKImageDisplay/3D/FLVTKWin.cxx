#include "FLVTKWin.h"

//windows specific functions
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/gl.h>


//-----------------------------------------
FLVTKWin::FLVTKWin(int x, int y, int w, int h, const char* l)
: Fl_Box(x,y,w,h,l)
  {
  mRenWin = vtkRenderWindow::New();

	mRenWin->BordersOff();						

	//make vtk window the same size as the fl_box. 
	mRenWin->SetSize(w, h);
	mFirstdraw = true;
  }

//-----------------------------------------
FLVTKWin::~FLVTKWin()
  {	
  mRenWin->Delete();
  }

//-----------------------------------------
vtkRenderWindow * FLVTKWin::win()
  {
  return mRenWin;
  }

//-----------------------------------------
void FLVTKWin::draw()
  {
  gl_start();
	if(mFirstdraw)
		if(mInitializePosition())
		    mFirstdraw = false;

  mRenWin->Render();
  gl_finish();
  Fl::flush();
  }

//-----------------------------------------
//If the box is set to resizeable, make the vtk window resize
//along with it. You can prevent resizing by making the fltk box
//unresizable.
void FLVTKWin::size(int w, int h)
  {
	this->Fl_Box::size(w, h);
	mRenWin->SetSize(w, h);
  redraw();
  }

//-----------------------------------------
void FLVTKWin::resize(int x, int y, int w, int h)
  {
  this->Fl_Box::resize(x,y,w,h);
	mRenWin->SetPosition(x, y);
	mRenWin->SetSize(w, h);
  redraw();
  }

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		//PROTECTED FUNCTIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//get the offset into the root window and the root window windowID
bool FLVTKWin::mInitializePosition(void)
  {
	mRenWin->SetParentId((void *)fl_xid(this->window()));
	mRenWin->SetPosition(this->x(),this->y());
	mRenWin->SetSize(this->w(), this->h());
	return true;
  }
