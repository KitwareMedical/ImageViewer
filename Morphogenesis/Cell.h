#ifndef Cell_H
#define Cell_H

#include "itkPoint.h"
#include <Fl/fl_draw.H>
#include <list>


namespace bio {


class Cell 
{

  typedef   itk::Point<double,2>  PointType;

public:
  Cell();
  ~Cell();
  void Draw(void) const;

private:

   PointType      m_Position;
   Fl_Color       m_Color;
   double         m_Radius;

   static     double      DefaultRadius;
   static     Fl_Color    DefaultColor;
   static     PointType   DefaultPosition;
};


}; // end namespace bio

#endif
