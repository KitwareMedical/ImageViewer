#include "TissueViewerWindow.h"

#include <Fl/fl_draw.H>


namespace bio  {


TissueViewerWindow
::TissueViewerWindow(int x,int y,int w,int h, const char *l):
Fl_Box(x,y,w,h,l)
{
	m_Tissue = 0;
}

TissueViewerWindow
::~TissueViewerWindow()
{
	m_Tissue = 0;
}


void 
TissueViewerWindow
::SetTissue(Tissue * tissue )
{
	m_Tissue = tissue;
}


void
TissueViewerWindow
::draw(void)
{
	Fl_Box::draw();
    fl_clip(0,0,w(),h());
	fl_pop_clip();
  fl_push_matrix();
  fl_translate( w()/2.0, h()/2.0 );
	if( m_Tissue )
	{
		m_Tissue->Draw();
	}
  fl_pop_matrix();
	
}



};  // end namespace bio
