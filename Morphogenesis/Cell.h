#ifndef Cell_H
#define Cell_H

#include "itkPoint.h"
#include "itkRGBPixel.h"
#include <FL/fl_draw.H>
#include <list>


namespace bio {


/**
 * \class Cell
 * \brief This class implement the minimal behavior 
 * of a biological cell.
 * The basic behavior of a cell is related with the
 * cell cycle. Geometrical concepts like position
 * and size are also managed by this abstract cell.
 * 
 */
class Cell 
{

public:

  typedef   itk::Point<double,2>   PointType;
  typedef   itk::Vector<double,2>  VectorType;
  typedef   std::list< Cell * >    CellsListType;
  typedef   itk::RGBPixel<float>   ColorType;


public:

  Cell();
  virtual ~Cell();
  virtual void Draw(void) const;
  virtual void Grow(void);
  virtual void ClearForce(void);
  virtual void AddForce(const VectorType & force);
  virtual void UpdatePosition(void);
  virtual void AdvanceTimeStep(void);
  virtual void EnergyIntake(void);
  virtual void NutrientsIntake(void);
  
  const PointType & GetPosition(void) const;
  CellsListType   * GetAggregate(void);
  
  static void SetGrowthRadiusLimit( double );
  static void SetGrowthRadiusIncrement( double );
  static void SetEnergySelfRepairLevel( double );
  static void SetNutrientSelfRepairLevel( double );
  static void SetDefaultColor( const ColorType & color );

protected:

   PointType          m_Position;
   VectorType         m_Force;
   ColorType          m_Color;
   
   double             m_Radius;
   double             m_EnergyReserveLevel;
   double             m_NutrientsReserveLevel;


   static     double      DefaultRadius;
   static     ColorType   DefaultColor;
   static     PointType   DefaultPosition;

   static     double      GrowthRadiusLimit;
   static     double      GrowthRadiusIncrement;

   static     double      EnergySelfRepairLevel;
   static     double      NutrientSelfRepairLevel;

   static     double      DefaultEnergyIntake;
   static     double      DefaultNutrientsIntake;

   static     CellsListType   m_Aggregate;

};


}; // end namespace bio

#endif
