#ifndef Cell_H
#define Cell_H

#include "Point.h"
#include <Fl/fl_draw.H>
#include <list>


class Cell 
{


public:
  Cell();
  ~Cell();
  void Draw(void) const;

private:

   Point      m_Position;
   Fl_Color   m_Color;
   double     m_Radius;

   static     double DefaultRadius;
   static     Fl_Color DefaultColor;
   static     Point    DefaultPosition;
};



#endif
