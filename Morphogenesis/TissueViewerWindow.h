#ifndef TissueViewerWindow_H
#define TissueViewerWindow_H

#include "Tissue.h"
#include <Fl/Fl_Box.H>


namespace bio {
  

class TissueViewerWindow : public Fl_Box
{


public:
  TissueViewerWindow(int x,int y, int w,int h, const char *l=0);
  ~TissueViewerWindow();
  void SetTissue( Tissue * tissue );
  void draw(void);

private:

	Tissue * m_Tissue;


};

}; // end namespace bio


#endif
