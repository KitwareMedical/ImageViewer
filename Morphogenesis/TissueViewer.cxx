#include "TissueViewer.h"
#include "Bacteria.h"


namespace bio {



TissueViewer::TissueViewer()
{
  m_Stop = true;
  
  Bacteria * first = new Bacteria;

  m_Tissue.SetCells( first->GetAggregate() );

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
  unsigned int iteration = 0;
	m_Stop = false;
	while( !m_Stop )
	{
     timeValueOutput->value( iteration );
     cellsValueOutput->value( m_Tissue.GetNumberOfCells() );
     m_Tissue.Grow();
     m_Tissue.Spread();
	   tissueView->redraw();
	   Fl::check();
     iteration++;
	}
}



void 
TissueViewer
::Stop(void)
{
	m_Stop = true;
}




void TissueViewer::Restart(void)
{
	m_Stop = true;
  m_Tissue.Restart();
  timeValueOutput->value( 0 );
  cellsValueOutput->value( 1 );
  tissueView->redraw();
  Fl::check();
}


}; // end namespace bio


