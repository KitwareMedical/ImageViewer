/*===============================================================
Summer 2001

  This class is an abstraction of the class GLSliceView.  It is
  derived of this class for the purposes of being used with 
  the TripleTKGUI structure.  This class will allow the viewing of
  image data in 2D through its GLSliceView inheritance and allows
  for any necessary additions to GLSliceView without actually 
  having to manipulate GLSliceView.

  Author: Andrew MacKelfresh
  CADDLab
  UNC-CH
===============================================================*/
#ifndef _TRIPLETK2D_H
#define _TRIPLETK2D_H

#include <GLSliceView.h>


/*
*	  Class TripleTK2D derived from class GLSliceView
*/
class TripleTK2D : public GLSliceView<short, unsigned char>
  {

  public:

    TripleTK2D(int x, int y, int w, int h, const char *l);
    ~TripleTK2D();
    float getOriginalLevel(void);
    float getOriginalWindow(void);
    void setOriginalLevel(float level);
    void setOriginalWindow(float window);
    bool getOverlayBoolean(void);
   
  protected:

    float originalLevelValue;
    float originalWindowValue;
          
  };


#endif
