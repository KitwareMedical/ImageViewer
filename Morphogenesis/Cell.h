#ifndef Cell_H
#define Cell_H

#include "itkPoint.h"
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

  typedef enum 
    {
    M,          // Mitosis
    G1,         // Growth Gap 1
    S,          // DNA Synthesis = DNA Replication
    G2,         // Growth
    Arrest,     // Cell Cycle arrest
    Apoptosis   // Programmed cell death
    }             CellCycleStateType;

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
  
  // This is the additive
  static void SetGrowthRadiusLimit( double );
  static void SetGrowthRadiusIncrement( double );
  static void SetEnergySelfRepairLevel( double );
  static void SetNutrientSelfRepairLevel( double );

protected:

   PointType          m_Position;
   VectorType         m_Force;
   Fl_Color           m_Color;
   double             m_Radius;
   double             m_EnergyReserve;
   double             m_NutrientsReserve;

   CellCycleStateType     m_CellCycleState;


   static     double      DefaultRadius;
   static     Fl_Color    DefaultColor;
   static     PointType   DefaultPosition;

   static     double      GrowthRadiusLimit;
   static     double      GrowthRadiusIncrement;

   static     double      EnergySelfRepairLevel;
   static     double      NutrientSelfRepairLevel;

   static     double      DefaultEnergyIntake;
   static     double      DefaultNutrientsIntake;

   static     CellsListType   m_Aggregate;

   static     CellCycleStateType TransitionTable[100][100];

};


}; // end namespace bio

#endif
