#ifndef TissueViewer_H
#define TissueViewer_H

#include "Tissue.h"
#include "TissueViewerGUI.h"


class TissueViewer : public TissueViewerGUI 
{


public:
	TissueViewer();

	virtual ~TissueViewer();

	virtual void Show(void);
	virtual void Hide(void);
	virtual void Quit(void);
	virtual void Run(void);
	virtual void Stop(void);

private:
	Tissue   m_Tissue;
	bool     m_Stop;
};


#endif
