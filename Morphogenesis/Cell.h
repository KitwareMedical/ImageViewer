#ifndef Cell_H
#define Cell_H

#include "itkRGBPixel.h"
#include <FL/fl_draw.H>
#include "itkVector.h"
#include "itkPoint.h"
#include "fltkSphere3D.h"


namespace bio {


class CellularAggregate;


/**
 * \class Cell
 * \brief This class implement the minimal behavior 
 * of a biological cell.
 * The basic behavior of a cell is related with the
 * cell cycle. Geometrical concepts like size and shape
 * are also managed by this abstract cell.
 * 
 */
class Cell  
{

public:

  enum { Dimension = 2 };
  enum { PointDimension = Dimension };

  typedef   itk::Vector<double,PointDimension>  VectorType;
  typedef   itk::Point<double,PointDimension>   PointType;
  typedef   itk::RGBPixel<float>                ColorType;
  typedef   unsigned long                       IdentifierType;


public:

  Cell();
  virtual ~Cell();
  virtual void Draw( const PointType & ) const;
  virtual void ClearForce(void);
  virtual void AddForce(const VectorType & force);
  virtual void AdvanceTimeStep(void);

  virtual void SetCellularAggregate( CellularAggregate * );

  virtual       CellularAggregate * GetCellularAggregate( void );
  virtual const CellularAggregate * GetCellularAggregate( void ) const;
  static  const char * GetSpeciesName(void) 
                              { return "Primitive Cell"; }
  static  Cell * CreateEgg(void);

  bool MarkedForRemoval(void) const;

protected:
  
  virtual void Grow(void);
  virtual void Divide(void);
  virtual void Apoptosis(void);
  
  virtual void EnergyIntake(void);
  virtual void NutrientsIntake(void);
  virtual void ReceptorsReading(void);
  
  virtual bool CheckPointGrowth(void);
  virtual bool CheckPointDivision(void);
  virtual bool CheckPointApoptosis(void);

  virtual Cell * CreateNew(void);

  void MarkForRemoval(void);

  typedef enum {
                  M = 1,
                  Gap1,
                  S,
                  Gap2,
                  Gap0,
                  Apop,
                        } CellCycleState;

public:

  virtual const VectorType & GetForce(void) const;

  virtual ColorType GetColor(void) const;
  
  double GetRadius(void) const;
  
  IdentifierType GetSelfIdentifier(void) const;
  IdentifierType GetParentIdentifier(void) const;
  
  static void SetGrowthRadiusLimit( double );
  static void SetGrowthRadiusIncrement( double );
  static void SetEnergySelfRepairLevel( double );
  static void SetNutrientSelfRepairLevel( double );
  static void SetDefaultColor( const ColorType & color );

  static double GetGrowthRadiusLimit( void );
  static void SetMaximumGenerationLimit( unsigned long );
  
  static void ResetCounter(void);


protected:

   VectorType           m_Force;
   ColorType            m_Color;
   
   double               m_Radius;
   double               m_EnergyReserveLevel;
   double               m_NutrientsReserveLevel;

   IdentifierType       m_ParentIdentifier;
   IdentifierType       m_SelfIdentifier;

   unsigned long        m_Generation;

   CellCycleState       m_CycleState;

   CellularAggregate  * m_Aggregate;


   // Static Members
   static     double      DefaultRadius;
   static     ColorType   DefaultColor;

   static     double      GrowthRadiusLimit;
   static     double      GrowthRadiusIncrement;
   static unsigned long   MaximumGenerationLimit;

   static     double      EnergySelfRepairLevel;
   static     double      NutrientSelfRepairLevel;

   static     double      DefaultEnergyIntake;
   static     double      DefaultNutrientsIntake;

   static     unsigned long  Counter;

   static     int         DisplayList;
     
   static fltk::Sphere3D::Pointer    SphereShape;


private:

   void DrawCircle(void) const;
   void DrawSphere(void) const;
   void DrawIcosaedron(void) const;

private:


   bool                       m_MarkedForRemoval;


};


} // end namespace bio

#endif
