#include "TissueViewer.h"


namespace bio {



TissueViewer::TissueViewer()
{
  m_Stop = true;
  tissueView->SetTissue( & m_Tissue );
}






TissueViewer::~TissueViewer()
{
	Hide();

}



void TissueViewer::Quit(void)
{
	Hide();

}



void TissueViewer::Show(void)
{
	tissueWindow->show();
	controlWindow->show();
}




void TissueViewer::Hide(void)
{
	tissueWindow->hide();
	controlWindow->hide();
}


void
TissueViewer
::Run(void)
{
	m_Stop = false;
	while( !m_Stop )
	{
       m_Tissue.Grow();
	   tissueView->redraw();
	   Fl::check();
	}
}



void 
TissueViewer
::Stop(void)
{
	m_Stop = true;
}



}; // end namespace bio


