#ifndef Cell_H
#define Cell_H

#include "itkPoint.h"
#include <Fl/fl_draw.H>
#include <list>


namespace bio {


class Cell 
{

  typedef   itk::Point<double,2>   PointType;
  typedef   itk::Vector<double,2>  VectorType;
  typedef   std::list< Cell * >    CellsListType;

public:
  Cell();
  virtual ~Cell();
  virtual void Draw(void) const;
  virtual void Grow(void);
  virtual void ClearForce(void);
  virtual void AddForce(const VectorType & force);
  virtual void UpdatePosition(void);
  
  const PointType & GetPosition(void) const;
  CellsListType   * GetAggregate(void);
  

protected:

   PointType      m_Position;
   VectorType     m_Force;
   Fl_Color       m_Color;
   double         m_Radius;

   static     double      DefaultRadius;
   static     Fl_Color    DefaultColor;
   static     PointType   DefaultPosition;

   static     double      RadiusIncrement;
   static     double      RadiusLimit;

   static     CellsListType   m_Aggregate;

};


}; // end namespace bio

#endif
